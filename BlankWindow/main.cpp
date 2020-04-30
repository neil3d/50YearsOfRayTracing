#include <spdlog/spdlog.h>

const char* const APP_NAME = "BlankWindow";

int main(void) {
  spdlog::info("{0}: app start...", APP_NAME);

  spdlog::info("{0}: app shutdown", APP_NAME);
  return 0;
}
