#include "RayCastingRenderer.h"

#include <algorithm>
#include <glm/glm.hpp>

#include "framework/PinholeCamera.h"

namespace RayTracingHistory {

constexpr float F_Max = std::numeric_limits<float>::max();
constexpr float SIGN_SIZE = 7;

void RayCastingRenderer::_renderThread(MyScene::Ptr scene,
                                       MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  int W = mFrameWidth;
  int H = mFrameHeight;

  for (int y = 0; y < H; y += SIGN_SIZE)
    for (int x = 0; x < W; x += SIGN_SIZE) {
      if (!mRuning) break;

      float darkness = _castRay((x + SIGN_SIZE * 0.5f) / W,
                                (y + SIGN_SIZE * 0.5f) / H, pScene, pCamera);

      if (darkness > 0.0f) _drawDrakSign(x, y, darkness);

      mPixelCount += SIGN_SIZE * SIGN_SIZE;
    }  // end of for(x)

  mPixelCount = W * H;
  _onRenderFinished();
}

Ray RayCastingRenderer::_generateShadowRay(const glm::vec3& point) {
  glm::vec3 L = glm::normalize(mLightPos - point);
  return Ray(point, L);
}

float RayCastingRenderer::_castRay(float u, float v, MyScene* pScene,
                                   PinholeCamera* camera) {
  HitRecord hitRec;
  Ray viewRay = camera->generateViewingRay(u, v);
  bool bHit = pScene->closestHit(viewRay, 0, F_Max, hitRec);

  if (!bHit) return 0.0f;  // white background

  Ray shadowRay = _generateShadowRay(hitRec.p);
  shadowRay.applayBiasOffset(hitRec.normal, 0.001f);
  const float lightDistance = glm::distance(hitRec.p, mLightPos);

  auto stopWithAnyHit = [](const HitRecord&) { return true; };
  bool bShadow = pScene->anyHit(shadowRay, 0, lightDistance, stopWithAnyHit);

  if (bShadow) return 1.0f;  // Hs for shadow : maximum darkness

  glm::vec3 L = glm::normalize(mLightPos - hitRec.p);
  return 1 - std::max(0.0f, glm::dot(hitRec.normal, L));
}

void RayCastingRenderer::_drawDrakSign(int x, int y, float darkness) {
  constexpr int Hs = SIGN_SIZE * 0.35f;
  constexpr int BORDER = 1;
  constexpr float GAMA = 1.0f;

  glm::vec3 color(1 - glm::pow(darkness, 0.25f));
  glm::vec4 CC(color, 1.0f);

  int lineWidth = glm::ceil(darkness * Hs);

  // draw h line
  for (int i = BORDER; i < SIGN_SIZE - BORDER; i++) {
    for (int p = 0; p < lineWidth; p++)
      _writePixel(x + i, y + SIGN_SIZE / 2 - lineWidth / 2 + p, CC, GAMA);
  }

  // draw v line
  for (int i = BORDER; i < SIGN_SIZE - BORDER; i++) {
    for (int p = 0; p < lineWidth; p++)
      _writePixel(x + SIGN_SIZE / 2 - lineWidth / 2 + p, y + i, CC, GAMA);
  }
}

}  // namespace RayTracingHistory
