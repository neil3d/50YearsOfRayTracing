#include "DirectLightingRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "framework/ThinLensCamera.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int TARGET_SPP = 4;

bool DirectLightingRenderer::isDone() const {
  if (mSPP < TARGET_SPP) return false;
  return DirectRenderer::isDone();
}

float DirectLightingRenderer::getProgress() const {
  return DirectRenderer::getProgress();
}

std::string DirectLightingRenderer::getInfo() const {
  return std::string(" - SPP: ") + std::to_string(mSPP);
}

void DirectLightingRenderer::_renderThread(MyScene::Ptr scene,
                                           MyCamera::Ptr camera) {
  ThinLensCamera* pCamera = static_cast<ThinLensCamera*>(camera.get());
  MyScene* pScene = scene.get();

  // alloc float back buffer
  mBackBuffer.resize(mFrameWidth * mFrameHeight);

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  int W = mFrameWidth;
  int H = mFrameHeight;

  while (mSPP < TARGET_SPP) {
    float average = 1.0f / (mSPP + 1);
    mPixelCount = 0;
    mPresentLine = 0;

    // one path for every single pixel
    for (int y = 0; y < H; y++)
      for (int x = 0; x < W; x++) {
        if (!mRuning) break;

        glm::vec2 xiS(uniformDist(stdRand), uniformDist(stdRand));
        glm::vec2 xiR(uniformDist(stdRand), uniformDist(stdRand));

        Ray primaryRay =
            pCamera->jitteredViewingRay((x + xiS.x) / W, (y + xiS.y) / H, xiR);

        glm::vec3 color = _traceRay(primaryRay, scene.get(), 0);

        int index = y * mFrameWidth + x;
        glm::vec3& backbuffer = mBackBuffer[index];
        backbuffer += color;

        _writePixel(x, y, glm::vec4(average * backbuffer, 1), 1.0f);
        mPixelCount++;

      }  // end of for(x)

    mSPP++;
  }  // end of while(spp)
}

glm::vec3 DirectLightingRenderer::_traceRay(Ray ray, MyScene* pScene,
                                            int depth) {
  const glm::vec3 bgColor(0.f, 0.f, 0.f);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  return hitRec.normal * glm::vec3(1, 1, -1);
}

}  // namespace RayTracingHistory
