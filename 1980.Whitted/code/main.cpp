#include <iostream>

#include "MyApp.h"

const char* const APP_NAME = "Recursive Ray Tracing - Turner Whitted 1980";

int main(void) {
  MyApp app;
  try {
    app.init();
    app.createWindow(1280, 720, APP_NAME);
    app.createRenderer<MyRenderer>(2000);

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  app.shutdown();
  return 0;
}
