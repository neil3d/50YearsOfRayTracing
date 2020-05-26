#include "DistributedRayTracer.h"

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "framework/PinholeCamera.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();

void DistributedRayTracer::_renderThread(MyScene::Ptr scene,
                                         MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  int n = mSPPn;
  float invN = 1.0f / n;
  float invSPP = 1.0f / (n * n);

  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (!mRuning) break;

      glm::vec4 color(0);
      // jittering/stratified sampling
      for (int p = 0; p < n; p++) {
        for (int q = 0; q < n; q++) {
          glm::vec2 rand = glm::diskRand(1.0f);
          float xp = x + (p + rand.x) * invN;
          float yp = y + (q + rand.y) * invN;

          color += _sampleColor(xp / mFrameWidth, yp / mFrameHeight, pScene,
                                pCamera);
        }  // end of for(q)
      }    // end of for(p)

      _writePixel(x, y, invSPP * color);

      mPixelCount++;
    }  // end of for(x)
}

glm::vec4 DistributedRayTracer::_sampleColor(float s, float t, MyScene* pScene,
                                             PinholeCamera* pCamera) {
  Ray viewingRay = pCamera->generateViewingRay(s, t);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(viewingRay, 0, FLOAT_MAX, hitRec);
  if (bHit) {
    return glm::vec4(hitRec.normal, 1);
  }

  const glm::vec3 topColor(0.85f, 0.85f, 0.95f);
  const glm::vec3 bottomColor(0.5f, 0.6f, 0.98f);
  glm::vec3 c = topColor * t + bottomColor * (1.0f - t);
  return glm::vec4(c, 1.0f);
}

}  // namespace RayTracingHistory
