#include <iostream>
#include "MyApp.h"

const char* const APP_NAME = "BlankWindow";

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

  return 0;
}
