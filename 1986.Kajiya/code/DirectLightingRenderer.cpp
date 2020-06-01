#include "DirectLightingRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

namespace RayTracingHistory {
constexpr int TARGET_SPP = 3;

bool DirectLightingRenderer::isDone() const {
  if (mSPP < TARGET_SPP) return false;
  return DirectRenderer::isDone();
}

float DirectLightingRenderer::getProgress() const {
  return DirectRenderer::getProgress() / mSPP;
}

std::string DirectLightingRenderer::getInfo() const {
  return std::string(" - SPP: ") + std::to_string(mSPP);
}

void DirectLightingRenderer::_renderThread(MyScene::Ptr scene,
                                           MyCamera::Ptr camera) {
  // alloc float back buffer
  mBackBuffer.resize(mFrameWidth * mFrameHeight);

  while (mSPP < TARGET_SPP) {
    int W = mFrameWidth;
    int H = mFrameHeight;
    float average = 1.0f / (mSPP + 1);

    // one path for every single pixel
    for (int y = 0; y < H; y++)
      for (int x = 0; x < W; x++) {
        if (!mRuning) break;

        Ray primaryRay;
        glm::vec3 color = _traceRay(primaryRay, scene.get(), 0);

        int index = y * mFrameWidth + x;
        glm::vec3& backbuffer = mBackBuffer[index];
        backbuffer = average * (backbuffer + color);

        _writePixel(x, y, glm::vec4(backbuffer, 1), 1.0f);
        mPixelCount++;

      }  // end of for(x)

    mSPP++;
  }  // end of while(spp)
}

glm::vec3 DirectLightingRenderer::_traceRay(Ray ray, MyScene* pScene,
                                            int depth) {
  return glm::vec3(0.25f, 0.5f, 1.0f);
}

}  // namespace RayTracingHistory
