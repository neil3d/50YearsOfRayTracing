#include "RayCastingRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void RayCastingRenderer::_renderThread(MyScene::Ptr scene,
                                       MyCamera::Ptr camera) {
  mEyePos = camera->getEyePos();
  camera->getFocalPlane(mFocalPlaneH, mFocalPlaneV, mFocalPlaneLeftTop);

  MyScene* pScene = scene.get();
  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (!mRuning) break;

      _drawSinglePixel(x, y, pScene);
      mPixelCount++;
    }  // end of for(x)
}

Ray RayCastingRenderer::_generateViewRay(int x, int y) {
  float s = (x + 0.5f) / (float)(mFrameWidth);
  float t = (y + 0.5f) / (float)(mFrameHeight);

  glm::vec3 origin = mEyePos;
  return Ray(origin,
             mFocalPlaneLeftTop + s * mFocalPlaneH - t * mFocalPlaneV - origin);
}

Ray RayCastingRenderer::_generateShadowRay(const glm::vec3& point) {
  glm::vec3 L = glm::normalize(mLightPos - point);
  return Ray(point, L);
}

void RayCastingRenderer::_drawSinglePixel(int x, int y, MyScene* pScene) {
  constexpr float fMax = std::numeric_limits<float>::max();

  HitRecord hitRec;
  Ray viewRay = _generateViewRay(x, y);
  bool bHit = pScene->hit(viewRay, 0, fMax, hitRec);

  if (!bHit) return;

  Ray shadowRay = _generateShadowRay(hitRec.p);
  HitRecord hitRecS;
  constexpr float SHADOW_E = 0.1f;
  bool bShadow = pScene->hit(shadowRay, SHADOW_E, fMax, hitRecS);

  // according to the paper:
  // H = size of plus sign
  // Hs = maximum size of the plus sign
  // h = a contrast factor, usually .8
  const float Hs = 1.0f;
  const float h = 0.8f;
  float Hj;

  if (bShadow) {
    Hj = Hs;
  } else {
    glm::vec3 L = glm::normalize(mLightPos - hitRec.p);
    float c = std::max(0.0f, glm::dot(hitRec.normal, L));
    Hj = h * (1 - c);
  }

  glm::vec4 color(1 - Hj, 1 - Hj, 1 - Hj, 1.0f);
  _writePixel(x, y, color);
}
