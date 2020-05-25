#include "DistributedRayTracer.h"

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>

namespace RayTracingHistory {

void DistributedRayTracer::_renderThread(MyScene::Ptr scene,
                                         MyCamera::Ptr camera) {
  int n = mSPPn;
  float invSPP = 1.0f / (n * n);

  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (!mRuning) break;

      glm::vec4 color(0);
      // jittering/stratified sampling
      for (int p = 0; p < n; p++) {
        for (int q = 0; q < n; q++) {
          float xp = x + (p + rand()) / n;
          float yp = y + (q + rand()) / n;

          color += _sampleColor(xp / mFrameWidth, yp / mFrameHeight);
        }  // end of for(q)
      }    // end of for(p)

      _writePixel(x, y, invSPP * color);

      mPixelCount++;
    }  // end of for(x)
}

glm::vec4 DistributedRayTracer::_sampleColor(float s, float t) {
  const glm::vec3 topColor(0.85f, 0.85f, 0.95f);
  const glm::vec3 bottomColor(0.5f, 0.6f, 0.98f);
  glm::vec3 c = topColor * t + bottomColor * (1.0f - t);
  return glm::vec4(c, 1.0f);
}

}  // namespace RayTracingHistory
