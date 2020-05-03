#include "RayCastingRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void RayCastingRenderer::_renderThread(MyScene::Ptr scene,
                                       MyCamera::Ptr camera) {
  mEyePos = camera->getEyePos();
  camera->getNearPlane(mNearPlaneH, mNearPlaneV, mNearPlaneLeftTop);

  mLightDir = glm::normalize(glm::vec3(1, 1, 0));

  MyScene* pScene = scene.get();
  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (mRuning) {
        _drawSinglePixel(x, y, pScene);
      } else
        break;
    }  // end of for(x)
}

Ray RayCastingRenderer::_generateEyeRay(int x, int y) {
  float s = (float)x / (float)(mFrameWidth);
  float t = (float)y / (float)(mFrameHeight);

  glm::vec3 origin = mEyePos;
  return Ray(origin,
             mNearPlaneLeftTop + s * mNearPlaneH - t * mNearPlaneV - origin);
}

Ray RayCastingRenderer::_generateShadowRay(const glm::vec3& point) {
  return Ray(point, mLightDir);
}

void RayCastingRenderer::_drawSinglePixel(int x, int y, MyScene* pScene) {
  constexpr float fMax = std::numeric_limits<float>::max();

  HitRecord hitRec;
  Ray eyeRay = _generateEyeRay(x, y);
  bool bHit = pScene->hit(eyeRay, 0.0001f, fMax, hitRec);

  if (!bHit) return;

  glm::vec4 color;
  Ray shadowRay = _generateShadowRay(hitRec.p);
  HitRecord hitRec2;
  bool bShadow = pScene->hit(shadowRay, 0.0001f, fMax, hitRec2);
  if (bShadow) {
    color = glm::vec4(0, 0, 0, 1);
  } else {
    float c = std::max(0.0f, glm::dot(hitRec.normal, mLightDir));
    color = glm::vec4(c, c, c, 1);
  }

  _writePixel(x, y, color);
}