#include "MyApp.h"

#include <spdlog/spdlog.h>

void MyApp::init() {
  mLastFrameTime = std::chrono::steady_clock::now();

  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0) throw MyException("SDL_Init FAILED", SDL_GetError());
}
void MyApp::createWindow(int width, int height, const char* szTitle) {
  mWindowTitle = szTitle;
  mMainWindow =
      SDL_CreateWindow(szTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_UTILITY);
  if (!mMainWindow)
    throw MyException("SDL_CreateWindow FAILED.", SDL_GetError());
}

void MyApp::mainLoop() {
  SDL_Event event = {};
  while (event.type != SDL_QUIT) {
    if (!SDL_PollEvent(&event)) {
      _tick();
    }
  }  // end of while
}

void MyApp::shutdown() {
  if (mRenderer) {
    mRenderer->_shutdown();
    mRenderer.reset();
  }

  if (mMainWindow) {
    SDL_DestroyWindow(mMainWindow);
    mMainWindow = nullptr;
  }
  SDL_Quit();
}

void MyApp::_tick() {
  if (!mRenderer) return;

  auto nowTime = std::chrono::steady_clock::now();
  auto durationTime = std::chrono::duration_cast<std::chrono::milliseconds>(
      nowTime - mLastFrameTime);
  if (durationTime.count() < mPresentInterval) return;

  // show rendering result
  mRenderer->_present();
  SDL_UpdateWindowSurface(mMainWindow);

  // show progress
  float progress = mRenderer->getProgress();
  std::string szTitle = mWindowTitle;
  szTitle.append(" - ");
  szTitle.append(std::to_string(progress * 100.0f));
  szTitle.append("%");
  SDL_SetWindowTitle(mMainWindow, szTitle.c_str());

  mLastFrameTime = nowTime;
}