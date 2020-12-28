/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <embree3/rtcore.h>

#include <iostream>

#include "MyCamera.h"
#include "MyRenderer.h"

namespace RTKit2 {
class DirectRenderer : public MyRenderer {
 public:
  virtual void renderScene(MyScene::Ptr scene, MyCamera::Ptr camera,
                           const glm::vec4& clearColor) override {
    // restart
    _shutdown();
    MyRenderer::renderScene(scene, camera, clearColor);

    // start new
    mRuning = true;
    mPixelCount = 0;
    mPresentLine = 0;
    _clearFrameBuffer(clearColor);

    std::cout << "start rendering ..." << std::endl;
    mRenderingThread = std::thread(
        [this, scene, camera] { this->_renderThread(scene, camera); });
  }

 private:
  virtual void _shutdown() override {
    mRuning = false;
    if (mRenderingThread.joinable()) mRenderingThread.join();
  }

  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) {
    glm::vec4 topColor(1.0f, 1, 1, 1);
    glm::vec4 bottomColor(0.5f, 0.7f, 1.0f, 1);

    for (int y = 0; y < mFrameHeight; y++)
      for (int x = 0; x < mFrameWidth; x++) {
        if (mRuning) {
          float r = (float)y / mFrameHeight;
          _writePixel(x, y, bottomColor * r + topColor * (1.0f - r), 1.0f);
          mPixelCount++;
        } else
          break;
      }  // end of for(x)
    mFinished = true;
  }

 private:
  std::thread mRenderingThread;
};
}  // namespace RTKit2
