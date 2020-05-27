#include "RayCastingRenderer.h"

#include <glm/glm.hpp>

#include "framework/PinholeCamera.h"

namespace RayTracingHistory {

void RayCastingRenderer::_renderThread(MyScene::Ptr scene,
                                       MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  int W = mFrameWidth;
  int H = mFrameHeight;

  for (int y = 0; y < H; y++)
    for (int x = 0; x < W; x++) {
      if (!mRuning) break;

      glm::vec4 color = _castRay((x+0.5f)/W, (y+0.5f)/H, pScene, pCamera);
      mPixelCount++;

      _writePixel(x, y, color);

    }  // end of for(x)
}

Ray RayCastingRenderer::_generateShadowRay(const glm::vec3& point) {
  glm::vec3 L = glm::normalize(mLightPos - point);
  return Ray(point, L);
}

glm::vec4 RayCastingRenderer::_castRay(float u, float v, MyScene* pScene,
                                           PinholeCamera* camera) {
  constexpr float fMax = std::numeric_limits<float>::max();

  HitRecord hitRec;
  Ray viewRay = camera->generateViewingRay(u, v);
  bool bHit = pScene->closestHit(viewRay, 0, fMax, hitRec);

  glm::vec4 color(0,0,0,1);
  if (!bHit) return color;

  Ray shadowRay = _generateShadowRay(hitRec.p);
  HitRecord hitRecS;
  constexpr float SHADOW_E = 0.001f;

  auto stopWithAnyHit = [](const HitRecord&) { return true; };

  bool bShadow = pScene->anyHit(shadowRay, SHADOW_E, fMax, stopWithAnyHit);
  if (bShadow) {
    color = glm::vec4(0, 0, 0, 1);
  } else {
    glm::vec3 L = glm::normalize(mLightPos - hitRec.p);
    float c = std::max(0.0f, glm::dot(hitRec.normal, L));
    color = glm::vec4(c, c, c, 1);
  }

  return color;
}

}  // namespace RayTracingHistory
