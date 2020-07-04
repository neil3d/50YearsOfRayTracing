#include "MonteCarloPathTracer.h"

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "MaterialBase.h"
#include "framework/PinholeCamera.h"
#include "sampling//UniformSampling.h"
#include "sampling/JitteringSampling.h"

namespace RayTracingHistory {

#define RUSSIAN_ROULETTE true

constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr uint32_t SPP_ROOT = 32;
constexpr uint32_t RUSSIAN_ROULETTE_MIN_BOUNCES = 5;
constexpr uint32_t MAX_BOUNCES = 1024;

void MonteCarloPathTracer::_init(SDL_Window* pWnd) {
  TiledRenderer::_init(pWnd);

  int SPP = SPP_ROOT * SPP_ROOT;
  mInfo = std::string(" - SPP: ");
  mInfo.append(std::to_string(SPP));

  if (MAX_BOUNCES == 0)
    mInfo.append(", Light Source ");
  else if (MAX_BOUNCES == 1)
    mInfo.append(", Direct Lighting ");
  else if (MAX_BOUNCES > 1) {
    mInfo.append(", Bounces: ");
    mInfo.append(std::to_string(RUSSIAN_ROULETTE_MIN_BOUNCES));
    mInfo.append("+");
  }
}

std::string MonteCarloPathTracer::getInfo() const { return mInfo; }

float MonteCarloPathTracer::getProgress() const {
  auto SPP = SPP_ROOT * SPP_ROOT;
  auto total = mFrameWidth * mFrameHeight * SPP;
  if (mPixelCount == total)
    return 1.0f;
  else
    return (float)mPixelCount / total;
}

void MonteCarloPathTracer::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MySceneWithLight* pScene = dynamic_cast<MySceneWithLight*>(scene.get());

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  float W = static_cast<float>(mFrameWidth);
  float H = static_cast<float>(mFrameHeight);
  int MAX_SPP = SPP_ROOT * SPP_ROOT;

  int SPP = 0;
  std::vector<glm::vec3> tileBuffer;

  int tileW = tile.right - tile.left;
  int tileH = tile.bottom - tile.top;
  tileBuffer.resize(tileW * tileH, glm::vec3(0));

#if 1
  auto xi1 = JitteringSampling::generateSamples(SPP_ROOT, false);
  auto xi2 = JitteringSampling::generateSamples(SPP_ROOT, true);
#else
  auto xi1 = UniformSampling::generateSamples(SPP_ROOT);
  auto xi2 = UniformSampling::generateSamples(SPP_ROOT);
#endif

  while (SPP < MAX_SPP) {
    int index = 0;
    float scale = 1.0f / (SPP + 1);

    for (int y = tile.top; y < tile.bottom; y++)
      for (int x = tile.left; x < tile.right; x++) {
        if (!mRuning) break;

        const glm::vec2& xi = xi1[SPP];
        Ray primaryRay =
            pCamera->generateViewingRay((x + xi.x) / W, (y + xi.y) / H);

        auto& buf = tileBuffer[index++];
        buf += _traceRay(primaryRay, pScene, xi2[SPP], 1.0f, 0);

        _writePixel(x, y, glm::vec4(buf / float(SPP + 1), 1), 1);
        mPixelCount++;
      }

    SPP++;
  }  // end of while

  spdlog::info("Tile[{0}] rendering finished, max depth = {1}.", tile.id,
               (int)mMaxDepth);
  _onTileFinished();
}

glm::vec3 MonteCarloPathTracer::_traceRay(const Ray& wo,
                                          MySceneWithLight* pScene,
                                          const glm::vec2& xi, float weight,
                                          int depth) {
  const glm::vec3 bgColor(0.f, 0.f, 0.f);

  // NEE == Next Event Estimation
  if (depth > mMaxDepth) mMaxDepth = depth;

  if (depth >= MAX_BOUNCES) return bgColor;

  // trace the ray
  HitRecord hitRec;
  bool bHit = pScene->closestHit(wo, 0.001f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  MaterialBase* pMtl = static_cast<MaterialBase*>(hitRec.mtl);

  // error check
  if (!pMtl) return glm::abs(hitRec.normal);

  const AreaLight* pLight = pScene->getMainLight();

  // avoid double counting the light source
  if (depth == 0) {
    // hit light
    if (pMtl->isLight()) return glm::vec3(pLight->getIntensity());
  }

  // bounces == 0: light source
  if (MAX_BOUNCES == 0) return bgColor;

  constexpr float NEE_Pr = 0.5f;
  constexpr float NEE_PDF_LIGHT = 1.0f / NEE_Pr;
  constexpr float NEE_PDF_REFLECT = 1.0f / (1 - NEE_Pr);

  bool sampleLight = glm::linearRand(0.0f, 1.0f) < NEE_Pr;
  const float sysUnit = pScene->systemUnit();

  //----- begin of direct lighting -----------------------------------------
  glm::vec3 directLighting(0);
  {
    // visibility between the shading point and the light
    float visibilityTerm = 1.0f;

    auto shadowRet = pLight->generateShadowRay(hitRec.p, hitRec.normal, xi);
    Ray shadowRay = std::get<0>(shadowRet);
    shadowRay.applayBiasOffset(hitRec.normal, 0.0001f, 0.00001f);

    float lightDistance = std::get<1>(shadowRet);
    glm::vec3 lightNormal = std::get<2>(shadowRet);

    auto stopWithAnyHit = [](const HitRecord&) { return true; };
    bool bShadow = pScene->anyHit(shadowRay, 0, lightDistance, stopWithAnyHit);
    if (bShadow) visibilityTerm = 0.f;

    // geometry term
    const glm::vec3 lightDir = shadowRay.direction;
    const float R = lightDistance / sysUnit;
    const float attenuation = glm::min(1.0f, 1.0f / (R * R));
    const float geometryTerm =
        glm::max(0.0f, glm::dot(lightDir, hitRec.normal)) *
        glm::max(0.0f, glm::dot(lightDir, lightNormal)) * attenuation;

    glm::vec3 color = pMtl->getBaseColor(hitRec.uv, hitRec.p);
    const float brdf = pMtl->evaluate(lightDir, -wo.direction, hitRec.normal);
    const float A = pLight->getArea() / sysUnit / sysUnit;
    const float Li = pLight->getIntensity();

    // uniform sampling the light source, PDF = 1/A
    directLighting = Li * A * brdf * visibilityTerm * geometryTerm * color;
  }
  if (sampleLight) return directLighting / NEE_PDF_LIGHT;
  //----- end of direct lighting -----------------------------------------

  // bounces==1: direct lighting, bounces>1: indirect lighting
  glm::vec3 indirectLighting(0);
  if (MAX_BOUNCES > 1 && weight > glm::epsilon<float>()) {
    glm::vec3 p = hitRec.p;
    auto sampleRet = pMtl->sample(-wo.direction, hitRec.normal);
    glm::vec3 d = sampleRet.scattered;
    float pdf = sampleRet.pdf;
    float brdf = pMtl->evaluate(d, -wo.direction, hitRec.normal);
    float reflectance = weight * glm::max(0.0f, glm::dot(d, hitRec.normal));

    if (reflectance < glm::epsilon<float>()) return glm::vec3(0);

    // Russian Roulette termination, only applied in indirect lighting
    float RR_Pr = glm::pow(0.95f, depth);

    // minimal bounces
    if (depth < RUSSIAN_ROULETTE_MIN_BOUNCES) RR_Pr = 1.0f;

    if (glm::linearRand(0.0f, 1.0f) < RR_Pr) {
      float RR_Boost = 1 / RR_Pr;

      Ray secondaryRay(p, d);
      secondaryRay.applayBiasOffset(hitRec.normal, 0.001f);
      indirectLighting =
          _traceRay(secondaryRay, pScene, xi, reflectance, depth + 1);
      indirectLighting =
          indirectLighting * (weight * brdf * RR_Boost / pdf / NEE_PDF_REFLECT);
    }
  }

  return indirectLighting;
}

}  // namespace RayTracingHistory
