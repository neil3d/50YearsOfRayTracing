#include "DistributedRayTracer.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "BilliardScene.h"
#include "Material.h"
#include "framework/ThinLensCamera.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int SPP_N = 3;

std::string DistributedRayTracer::getInfo() const {
  return std::string(" - SPP: ") + std::to_string(SPP_N * SPP_N);
}

void DistributedRayTracer::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  ThinLensCamera* pCamera = static_cast<ThinLensCamera*>(camera.get());
  BilliardScene* pScene = dynamic_cast<BilliardScene*>(scene.get());

  constexpr int n = SPP_N;
  float invN = 1.0f / n;
  float invSPP = 1.0f / (n * n);

  std::array<glm::vec2, n * n> jitteredPointsR, jitteredPointsS;
  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());

  for (int y = tile.top; y < tile.bottom; y++)
    for (int x = tile.left; x < tile.right; x++) {
      if (!mRuning) break;

      // jittering/stratified sampling
      for (int p = 0; p < n; p++) {
        for (int q = 0; q < n; q++) {
          int index = p * n + q;
          jitteredPointsR[index] =
              invN * (glm::diskRand(1.0f) + glm::vec2(p, q));
          jitteredPointsS[index] =
              invN * (glm::diskRand(1.0f) + glm::vec2(p, q));
        }  // end of for(q)
      }    // end of for(p)
      std::shuffle(jitteredPointsS.begin(), jitteredPointsS.end(), stdRand);

      glm::vec3 color(0);
      for (int i = 0; i < n * n; i++) {
        const glm::vec2& pixelXi = jitteredPointsR[i];
        const glm::vec2& sampleXi = jitteredPointsS[i];

        Ray viewingRay = pCamera->jitteredViewingRay(
            (x + pixelXi.x) / mFrameWidth, (y + pixelXi.y) / mFrameHeight,
            sampleXi);

        color += _traceRay(viewingRay, pScene, 0, sampleXi);
      }

      _writePixel(x, y, glm::vec4(invSPP * color, 1.0f));
      mPixelCount++;
    }  // end of for(x)
}

glm::vec3 DistributedRayTracer::_traceRay(const Ray& ray, BilliardScene* pScene,
                                          int depth, const glm::vec2 xi) {
  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0, FLOAT_MAX, hitRec);
  if (bHit) {
    return _shade(ray.direction, hitRec, pScene, xi);
  }

  const glm::vec3 topColor(0.8f, 0.8f, 0.95f);
  return topColor;
}

glm::vec3 DistributedRayTracer::_shade(const glm::vec3& dir,
                                       const HitRecord& shadingPoint,
                                       BilliardScene* pScene,
                                       const glm::vec2 xi) {
  const auto& light = pScene->getMainLight();
  Material* mtl = dynamic_cast<Material*>(shadingPoint.mtl);

  Ray shadowRay = light.jitteredShadowRay(shadingPoint.p, xi);
  HitRecord hitRecS;
  constexpr float SHADOW_E = 0.001f;

  auto stopWithAnyHit = [](const HitRecord&) { return true; };

  glm::vec3 albedo(1);
  if (mtl) albedo = mtl->sampleAlbedo(shadingPoint.uv, shadingPoint.p);

  glm::vec3 color;
  bool bShadow = pScene->anyHit(shadowRay, SHADOW_E, FLOAT_MAX, stopWithAnyHit);
  if (bShadow) {
    float a = light.ambient;
    color = a * albedo;
  } else {
    glm::vec3 lgt = light.blinnPhongShading(shadingPoint.p, shadingPoint.normal,
                                            dir, 30, xi);
    float c = lgt.x + lgt.y + lgt.z;
    color = c * albedo;
  }
  return color;
}

}  // namespace RayTracingHistory
