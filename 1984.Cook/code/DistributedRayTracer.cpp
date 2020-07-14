#include "DistributedRayTracer.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "Material.h"
#include "MySceneWithLight.h"
#include "framework/ThinLensCamera.h"
#include "geometry/ONB.h"
#include "sampling/JitteringSampling.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int MAX_BOUNCES = 32;
constexpr int SPP_N = 16;
constexpr float GAMA = 1.5f;

std::string DistributedRayTracer::getInfo() const {
  return std::string(" - SPP: ") + std::to_string(SPP_N * SPP_N);
}

void DistributedRayTracer::_tileRenderThread(MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  ThinLensCamera* pCamera = static_cast<ThinLensCamera*>(camera.get());
  MySceneWithLight* pScene = dynamic_cast<MySceneWithLight*>(scene.get());

  constexpr int n = SPP_N;
  float invSPP = 1.0f / (n * n);

  std::vector<glm::vec2> xiR, xiS;
  Tile tile;

  while (mRuning && _popTile(tile)) {
    for (int y = tile.top; y < tile.bottom; y++)
      for (int x = tile.left; x < tile.right; x++) {
        if (!mRuning) break;

        xiR = JitteringSampling::generateSamples(n, false);
        xiS = JitteringSampling::generateSamples(n, true);

        glm::vec3 color(0);
        for (int i = 0; i < n * n; i++) {
          const glm::vec2& pixelXi = xiR[i];
          const glm::vec2& sampleXi = xiS[i];

          Ray viewingRay = pCamera->jitteredViewingRay(
              (x + pixelXi.x) / mFrameWidth, (y + pixelXi.y) / mFrameHeight,
              sampleXi);

          viewingRay.time = glm::linearRand(0.0f, 1.0f);
          color += _traceRay(viewingRay, pScene, 0, sampleXi, 1.0f);
        }

        _writePixel(x, y, glm::vec4(invSPP * color, 1.0f), GAMA);
        mPixelCount++;
      }  // end of for(x)

    _onTileFinished();
  }  // end of while
}

glm::vec3 DistributedRayTracer::_traceRay(const Ray& ray,
                                          MySceneWithLight* pScene, int depth,
                                          const glm::vec2 xi, float weight) {
  const glm::vec3 bgColor(0.25f, 0.25f, 0.25f);

  if (depth > MAX_BOUNCES) return glm::vec3(0);

  // intersections
  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  // error check
  Material* mtl = dynamic_cast<Material*>(hitRec.mtl);
  if (!mtl) return glm::abs(hitRec.normal);

  glm::vec3 baseColor(1);
  if (mtl) baseColor = mtl->sampleBaseColor(hitRec.uv, hitRec.p);

  auto shading = _shade(-ray.direction, hitRec, pScene, xi, ray.time);
  float Ka = std::get<0>(shading);
  float Kd = std::get<1>(shading);

  // reflection
  glm::vec3 reflectionColor(0);
  float Kr = mtl->Kr;
  if (Kr > 0) {
    Ray rRay = _jitteredReflectionRay(ray.direction, hitRec.p, hitRec.normal,
                                      xi, mtl->gloss);
    rRay.time = ray.time;
    reflectionColor = Kr * _traceRay(rRay, pScene, depth + 1, xi, Kr * weight);
  }

  return Ka * baseColor + (Kd * baseColor + reflectionColor) * (weight);
}

std::tuple<float, float> DistributedRayTracer::_shade(
    const glm::vec3& wo, const HitRecord& shadingPoint,
    MySceneWithLight* pScene, const glm::vec2 xi, float time) {
  const auto& light = pScene->getMainLight();
  Material* mtl = dynamic_cast<Material*>(shadingPoint.mtl);

  auto shadowRet = light.jitteredShadowRay(shadingPoint.p, xi);
  Ray shadowRay = std::get<0>(shadowRet);
  float lightDistance = std::get<1>(shadowRet);

  shadowRay.applayBiasOffset(shadingPoint.normal, 0.001f);
  shadowRay.time = time;
  HitRecord hitRecS;

  float ambient, diffuse;

  auto stopWithAnyHit = [](const HitRecord&) { return true; };
  bool bShadow = pScene->anyHit(shadowRay, 0, lightDistance, stopWithAnyHit);
  if (bShadow) {
    ambient = light.ambient;
    diffuse = 0;
  } else {
    diffuse = light.lighting(shadingPoint.p, shadingPoint.normal, wo, xi,
                             mtl->shininess);
    ambient = light.ambient;
  }
  return std::make_tuple(ambient, diffuse);
}

Ray DistributedRayTracer::_jitteredReflectionRay(const glm::vec3& dir,
                                                 const glm::vec3& point,
                                                 const glm::vec3& normal,
                                                 const glm::vec2 xi,
                                                 float glossy) {
  ONB onb(normal);

  float u = -glossy / 2 + xi.x * glossy;
  float v = -glossy / 2 + xi.y * glossy;
  glm::vec3 U = glossy * onb.U;
  glm::vec3 V = glossy * onb.V;

  // jittered direction
  glm::vec3 R = glm::reflect(dir, normal);
  glm::vec3 Rp = R + u * U + v * V;

  return Ray(point + normal * 0.001f, Rp);
}

}  // namespace RayTracingHistory
