#include "MyApp.h"

#include <spdlog/spdlog.h>

void MyApp::init() {
  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0) throw MyException("SDL_Init FAILED", SDL_GetError());
}
void MyApp::createWindow(int width, int height, const char* szTitle) {
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
      // update surface
      SDL_Surface* pSurface = nullptr;
      pSurface = SDL_GetWindowSurface(mMainWindow);
      if (pSurface) {
        _render(pSurface);
        SDL_UpdateWindowSurface(mMainWindow);
      } else {
        spdlog::error("SDL_Surface is nullptr.");
      }
    }
  }  // end of while
}

void MyApp::shutdown() {
  if (mMainWindow) {
    SDL_DestroyWindow(mMainWindow);
    mMainWindow = nullptr;
  }
  SDL_Quit();
}

void MyApp::_render(SDL_Surface* pSurface) {
  int ret = SDL_LockSurface(pSurface);
  if (ret < 0) {
    spdlog::error("SDL_LockSurface FAILED.");
    return;
  }

  uint32_t clearColor = SDL_MapRGBA(pSurface->format, 128, 128, 128, 255);
  SDL_memset(pSurface->pixels, clearColor, pSurface->h * pSurface->pitch);

  SDL_UnlockSurface(pSurface);
}