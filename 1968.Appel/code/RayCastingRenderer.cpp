#include "RayCastingRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void RayCastingRenderer::_renderThread(MyScene::Ptr scene,
                                       MyCamera::Ptr camera) {
  mEyePos = camera->getEyePos();
  camera->getNearPlane(mNearPlaneH, mNearPlaneV, mNearPlaneLeftBottom);

  mLightDir = glm::normalize(glm::vec3(1, -1, 0));

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
             mNearPlaneLeftBottom + s * mNearPlaneH + t * mNearPlaneV - origin);
}

Ray RayCastingRenderer::_generateShadowRay(const glm::vec3& point) {
  return Ray(point, mLightDir * -1.0f);
}

void RayCastingRenderer::_drawSinglePixel(int x, int y, MyScene* pScene) {
  Ray eyeRay = _generateEyeRay(x, y);
  HitRecord hitRec;
  bool bHit =
      pScene->hit(eyeRay, 0.0, std::numeric_limits<float>::max(), hitRec);

  glm::vec4 color;
  if (bHit) {
    float c = std::max(0.0f, glm::dot(hitRec.normal, mLightDir));
    color = glm::vec4(c, c, c, 1);
  } else {
    color = glm::vec4(1, 1, 1, 1);
  }

  _writePixel(x, y, color);
}