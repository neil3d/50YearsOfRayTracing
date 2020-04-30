#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

const char* const APP_NAME = "BlankWindow";

int main(void) {
  spdlog::info("{0}: app start...", APP_NAME);

  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0) {
    spdlog::critical("SDL_Init FAILED.");
    return -1;
  }

  SDL_Quit();
  spdlog::info("{0}: app shutdown", APP_NAME);
  return 0;
}
