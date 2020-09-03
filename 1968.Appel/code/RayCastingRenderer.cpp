#include "RayCastingRenderer.h"

#include <algorithm>
#include <glm/glm.hpp>

#include "framework/PinholeCamera.h"

namespace RayTracingHistory {

constexpr float F_Max = std::numeric_limits<float>::max();
constexpr float SIGN_SIZE = 9;

void RayCastingRenderer::_renderThread(MyScene::Ptr scene,
                                       MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  // Ray casting
  int W = mFrameWidth;
  int H = mFrameHeight;

  mPixelCount = 0;
  for (int y = 0; y < H; y += SIGN_SIZE)
    for (int x = 0; x < W; x += SIGN_SIZE) {
      if (!mRuning) break;

      float darkness = _castRay((x + SIGN_SIZE * 0.5f) / W,
                                (y + SIGN_SIZE * 0.5f) / H, pScene, pCamera);

      if (darkness > 0.0f) _drawDrakSign(x, y, darkness);

      mPixelCount += SIGN_SIZE * SIGN_SIZE;
    }  // end of for(x)

  // just for fun
  if (SIGN_SIZE > 1) _drawWireframe(pScene, pCamera);

  // done
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

  if (SIGN_SIZE == 1) {
    constexpr float AMBIENT = 0.25f;
    float G = glm::clamp(1 - glm::pow(darkness, 0.25f) + 0.25f, 0.0f, 1.0f);
    _writePixel(x, y, glm::vec4(G, G, G, 1), GAMA);
    return;
  }

  glm::vec3 color(1 - glm::pow(darkness, 0.8f));
  glm::vec4 CC(color, 1.0f);
  int lineWidth = glm::max(2.0f, glm::ceil(darkness * Hs));

  for (int i = BORDER; i < SIGN_SIZE - BORDER; i++) {
    int offset = (SIGN_SIZE - lineWidth) / 2 + BORDER;
    for (int p = 0; p < lineWidth; p++) {
      // draw h line
      _writePixel(x + i, y + offset, CC, GAMA);
      // draw v line
      _writePixel(x + offset, y + i, CC, GAMA);
    }
  }
}

void RayCastingRenderer::_drawWireframe(MyScene* pScene,
                                        PinholeCamera* camera) {
  int W = mFrameWidth;
  int H = mFrameHeight;

  const glm::vec4 CC(0, 0, 0, 1);
  const float ANGLE_THRESHOLD = glm::cos(glm::radians(15.0f));

  bool bHit = false, bLastHit = false;
  HitRecord hit, lastHit;
  Ray viewRay;
  float u, v;

  float count = 0;
  const int SPP = 3;

  for (int y = 1; y < H - 1; y++) {
    for (int x = 1; x < W - 1; x++) {
      if (!mRuning) break;

      count = 0;
      for (int j = 0; j < SPP; j++) {
        for (int i = 0; i < SPP; i++) {
          u = (x + i - SPP * 0.5f + 0.5f) / W;
          v = (y + j - SPP * 0.5f + 0.5f) / H;

          viewRay = camera->generateViewingRay(u, v);
          bHit = pScene->closestHit(viewRay, 0, F_Max, hit);
          if (bHit != bLastHit || hit.obj != lastHit.obj ||
              glm::dot(hit.normal, lastHit.normal) < ANGLE_THRESHOLD) {
            bLastHit = bHit;
            lastHit = hit;
            count += 1;
          }
        }
      }  // end of for

      if (count > 0) {
        float c = 1 - count / (SPP * SPP);
        c = glm::pow(c, 3);
        _writePixel(x, y, glm::vec4(c, c, c, 1), 1);
      }
      mPixelCount++;
    }
  }  // end of for
}

}  // namespace RayTracingHistory
