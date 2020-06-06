#include "MyApp.h"

#include <spdlog/spdlog.h>

namespace RayTracingHistory {

void MyApp::init() {
  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0) throw MyException("SDL_Init FAILED", SDL_GetError());

  spdlog::info("Base path = {0} .", SDL_GetBasePath());
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
  if (mRenderer->isDone()) {
    _showProgress();
    SDL_Delay(10);
    return;
  }

  if (mRenderer->nextPresentReady()) {
    // show rendering result
    mRenderer->_present();
    SDL_UpdateWindowSurface(mMainWindow);

    // show progress
    _showProgress();
  }
}

void MyApp::_showProgress() {
  float progress = mRenderer->getProgress();
  std::string szTitle = mWindowTitle;
  szTitle.append(mRenderer->getInfo());
  szTitle.append(" - ");
  szTitle.append(std::to_string(progress * 100.0f));
  szTitle.append("%");
  SDL_SetWindowTitle(mMainWindow, szTitle.c_str());
}

}  // namespace RayTracingHistory
