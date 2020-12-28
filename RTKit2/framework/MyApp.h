/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <SDL2/SDL.h>
#include <embree3/rtcore.h>

#include <memory>

#include "../scene/MyScene.h"
#include "MyException.h"
#include "MyRenderer.h"
#include "MySceneLoaderBase.h"

namespace RTKit2 {
class MyApp {
 public:
  MyApp() = default;
  ~MyApp() = default;

  void init();
  void createWindow(int width, int height, const char* szTitle);
  void mainLoop();
  void shutdown();

  void setOutputImage(const std::string& szPath) { mOutputImagePath = szPath; }

  template <typename T>
  auto createRenderer() {
    if (!mMainWindow)
      throw MyException("MyApp.createRenderer: create window first.");

    auto newRenderer = std::make_shared<T>();
    newRenderer->_init(mMainWindow);
    mRenderer = newRenderer;
    return newRenderer;
  }

  MyScene::Ptr createScene(MySceneLoaderBase* loader,
                           const std::string& szFileName);

 private:
  virtual void _onSDLEvent(const SDL_Event& evt) {}

  void _tick();
  void _showProgress();

 protected:
  std::string mWindowTitle;
  std::string mOutputImagePath;

  MyRenderer::Ptr mRenderer;
  SDL_Window* mMainWindow = nullptr;
  RTCDevice mDevice;

 public:
  MyApp(const MyApp&) = delete;
  MyApp& operator=(const MyApp&) = delete;
};
}  // namespace RTKit2
