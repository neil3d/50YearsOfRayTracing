#include "SimpleMonteCarloApproach.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "framework/PinholeCamera.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int TARGET_SPP = 4;

void SimpleMonteCarloApproach::_renderThread(MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  int W = mFrameWidth;
  int H = mFrameHeight;

  for (int y = 0; y < H; y++)
    for (int x = 0; x < W; x++) {
      if (!mRuning) break;

      Ray primaryRay =
          pCamera->generateViewingRay((x + 0.5f) / W, (y + 0.5f) / H);

      glm::vec3 color = _traceRay(primaryRay, scene.get(), 0);

      _writePixel(x, y, glm::vec4(color, 1), 1.0f);
      mPixelCount++;
    }  // end of for(x)
}

glm::vec3 SimpleMonteCarloApproach::_traceRay(Ray ray, MyScene* pScene,
                                              int depth) {
  const glm::vec3 bgColor(0.f, 0.f, 0.f);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  return hitRec.normal * glm::vec3(1, 1, -1);
}

}  // namespace RayTracingHistory
