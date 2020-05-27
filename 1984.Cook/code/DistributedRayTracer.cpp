#include "DistributedRayTracer.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "BilliardScene.h"
#include "framework/PinholeCamera.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int SPP_N = 2;

void DistributedRayTracer::_renderThread(MyScene::Ptr scene,
                                         MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  BilliardScene* pScene = dynamic_cast<BilliardScene*>(scene.get());

  constexpr int n = SPP_N;
  float invN = 1.0f / n;
  float invSPP = 1.0f / (n * n);

  std::array<glm::vec2, n * n> jitteredPointsR, jitteredPointsS;
  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());

  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
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

        Ray viewingRay = pCamera->generateViewingRay(
            (x + pixelXi.x) / mFrameWidth, (y + pixelXi.y) / mFrameHeight);

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
    return hitRec.normal * glm::vec3(1, 1, -1);
  }

  const glm::vec3 topColor(0.85f, 0.85f, 0.95f);
  return topColor;
}

}  // namespace RayTracingHistory
