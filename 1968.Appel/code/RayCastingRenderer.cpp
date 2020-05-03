#include "RayCastingRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void RayCastingRenderer::_renderThread(MyScene::Ptr scene,
                                       MyCamera::Ptr camera) {
  mEyePos = camera->getEyePos();
  camera->getNearPlane(mNearPlaneH, mNearPlaneV, mNearPlaneLeftBottom);

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

void RayCastingRenderer::_drawSinglePixel(int x, int y, MyScene* pScene) {
  Ray eyeRay = _generateEyeRay(x, y);

  HitRecord hitRec;
  bool bHit =
      pScene->hit(eyeRay, 0.0, std::numeric_limits<float>::max(), hitRec);

  glm::vec4 color(0, 0, 0, 1);
  if (bHit) {
  } else {
  }

  _writePixel(x, y, color);
}