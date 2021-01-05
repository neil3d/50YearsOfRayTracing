#include "MyPathTracer.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>
#include <limits>
#include <tuple>

#include "material/PhongMaterial.h"
#include "sampling/JitteringSampling.h"
#include "sampling/UniformSampling.h"
#include "scene/QuadLight.h"
#include "utility/Phong.h"

namespace RTKit2 {

static std::tuple<bool, float> russianRoulette(const glm::vec3& weight) {
  float throughput =
      glm::min(glm::max(glm::max(weight.x, weight.y), weight.z), 1.0f);
  float q = 1 - throughput;
  float rand = glm::linearRand(0.0f, 1.0f);

  bool terminate = rand < q;
  float boost = 1.0f / (1.0f - q);

  return std::make_tuple(terminate, boost);
}

constexpr float BIAS = 0.001f;

std::string MyPathTracer::getInfo() const { return mInfo; }

void MyPathTracer::setOptions(int spp, int maxDepth, bool RR) {
  mSamplePerPixel = spp;
  mMaxDepth = maxDepth;
  mEnableRR = RR;

  mInfo = std::string(" - SPP: ");
  mInfo.append(std::to_string(mSamplePerPixel));
}

void MyPathTracer::_tileRenderThread(MyScene::Ptr scene, MyCamera::Ptr camera) {
  MyScene* pScene = scene.get();
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());

  Tile tile;
  while (mRuning && _popTile(tile)) {
    for (int y = tile.top; y < tile.bottom; y++)
      for (int x = tile.left; x < tile.right; x++) {
        if (mRuning) {
          _renderPixel(x, y, pScene, pCamera);
          mPixelCount++;
        } else
          break;
      }  // end of for(x)
    _onTileFinished();
  }  // end of while
}

void MyPathTracer::_renderPixel(int x, int y, MyScene* scene,
                                PinholeCamera* camera) {
  constexpr float PI = glm::pi<float>();
  const float W = static_cast<float>(mFrameWidth);
  const float H = static_cast<float>(mFrameHeight);

  auto samples = JitteringSampling::generateSamples(mSamplePerPixel, false);
  int N = samples.size();

  glm::vec3 color(0);
  for (int i = 0; i < N; i++) {
    if (!mRuning) break;

    const auto& sample = samples[i];
    MyRay eyeRay =
        camera->generateViewingRay((x + sample.x) / W, (y + sample.y) / H);
    color += _traceRay(eyeRay, scene, 1);
  }  // end of for

  color = color / (float)N;
  _writePixel(x, y, glm::vec4(color, 1.0f), 1.0f);
}

glm::vec3 MyPathTracer::_traceRay(MyRay ray, MyScene* scene, int depth) {
  constexpr float PI = glm::pi<float>();
  constexpr float FARMOST = 2000.0f;  // std::numeric_limits<float>::infinity();
  HitRecord hrc;
  bool hit = scene->intersect(ray, BIAS, FARMOST, hrc);
  if (!hit) return glm::vec3(0);

  ////// emission //////////////////////////////////
  if (hrc.isLight) {
    if (depth == 1) {
      QuadLight* lgt = dynamic_cast<QuadLight*>(hrc.obj);
      return lgt->getIntensity();
    } else
      return glm::vec3(0);
  }

  glm::vec3 wo = -ray.direction;

  // direct/indirect light separation referred as Next Event Estimation
  ////// direct lighting //////////////////////////////////
  glm::vec3 directLighting(0);
  const auto& lights = scene->getLights();
  for (const auto& lgt : lights) {
    directLighting += _directLighting(wo, hrc, lgt.get(), scene);
  }

  if (!mEnableRR && depth > mMaxDepth) return directLighting;

  ////// indirect lighting ////////////////////////////////
  glm::vec3 indirectLighting(0);
  glm::vec3 wi = UniformSampling::sampleHemisphere(hrc.normal);

  float cosine = glm::max(0.0f, glm::dot(wi, hrc.normal));

  glm::vec3 diffuse(1);
  glm::vec3 specular(0);
  float shininess = 1;
  PhongMaterial* mtl = dynamic_cast<PhongMaterial*>(hrc.mtl);
  if (mtl) {
    diffuse = mtl->diffuse;
    specular = mtl->specular;
    shininess = mtl->shininess;
  }

  glm::vec3 brdf =
      modifiedPhong(diffuse, specular, shininess, wo, hrc.normal, wi);

  constexpr float RCP_PDF = 2 * PI;
  glm::vec3 throughput = brdf * (cosine * RCP_PDF);

  float RR_boost = 1.0f;
  if (mEnableRR) {
    auto RR = russianRoulette(throughput);
    bool RR_terminate = std::get<0>(RR);
    if (RR_terminate) return directLighting;
    RR_boost = std::get<1>(RR);
  }

  MyRay secondaryRay(hrc.pt, wi);
  indirectLighting =
      _traceRay(secondaryRay, scene, depth + 1) * throughput * RR_boost;

  return indirectLighting + directLighting;
}

glm::vec3 MyPathTracer::_directLighting(glm::vec3 wo,
                                        const HitRecord& shadingPt,
                                        QuadLight* lgt, MyScene* scene) {
  constexpr float PI = glm::pi<float>();
  glm::vec3 x = shadingPt.pt;
  glm::vec3 normal = shadingPt.normal;
  glm::vec3 Kd(1);
  glm::vec3 Ks(0);
  float s = 1;

  PhongMaterial* mtl = dynamic_cast<PhongMaterial*>(shadingPt.mtl);
  if (mtl) {
    Kd = mtl->diffuse;
    Ks = mtl->specular;
    s = mtl->shininess;
  }

  glm::vec3 lighting(0);
  glm::vec3 Nl = lgt->getNormal();
  glm::vec3 N = normal;

  glm::vec2 lightSample(glm::linearRand(0.0f, 1.0f),
                        glm::linearRand(0.0f, 1.0f));
  glm::vec3 xp = lgt->sample(lightSample);
  float R = glm::length(xp - x);
  glm::vec3 wi = glm::normalize(xp - x);
  glm::vec3 brdf = modifiedPhong(Kd, Ks, s, wo, normal, wi);

  float G = glm::max(0.0f, glm::dot(N, wi)) * glm::max(0.0f, glm::dot(Nl, wi)) /
            (R * R);

  MyRay shadowRay(x, wi);
  bool occluded = scene->occluded(shadowRay, BIAS, R - BIAS);
  float V = occluded ? 0.0f : 1.0f;

  lighting = brdf * G * V;

  float A = lgt->getArea();
  glm::vec3 Li = lgt->getIntensity();

  const float RCP_PDF = A;
  return Li * lighting * RCP_PDF;
}

}  // namespace RTKit2