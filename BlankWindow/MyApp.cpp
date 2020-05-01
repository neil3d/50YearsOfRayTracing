#include "MyApp.h"

#include <spdlog/spdlog.h>

void MyApp::init() {
  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0)
    throw MyException(std::string("SDL_Init FAILED:") + SDL_GetError());
}
void MyApp::createWindow(int width, int height, const char* szTitle) {
  mMainWindow =
      SDL_CreateWindow(szTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_UTILITY);
  if (!mMainWindow)
    throw MyException(std::string("SDL_CreateWindow FAILED.") + SDL_GetError());
}

void MyApp::mainLoop() {
  SDL_Event event = {};
  while (event.type != SDL_QUIT) {
    if (!SDL_PollEvent(&event)) {
      // update frame
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