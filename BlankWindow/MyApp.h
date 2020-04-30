#pragma once
#include "MyRenderer.h"

class MyApp final {
 public:
  MyApp() = default;
  ~MyApp() = default;

  void init();
  void createWindow(int width, int height);
  void mainLoop();
  void shutdown();

  template <typename T>
  MyRenderer::Ptr createRenderer() {
      mRenderer = std::make_shared<T>();
      return mRenderer;
  }

 private:
  MyRenderer::Ptr mRenderer;

 public:
  MyApp(const MyApp&) = delete;
  MyApp& operator=(const MyApp&) = delete;
};