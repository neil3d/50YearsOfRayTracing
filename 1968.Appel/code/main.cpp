#include <iostream>

#include "RayCastingRenderer.h"
#include "framework/MyApp.h"
#include "scene/SpheresScene.h"

const char* const APP_NAME = "Ray Casting - Arthur Appel 1968";

int main(void) {
  MyApp app;
  try {
    app.init();
    app.createWindow(640, 400, APP_NAME);
    auto renderer = app.createRenderer<RayCastingRenderer>(2000);
    auto scene = std::make_shared<SpheresScene>();
    renderer->renderScene(scene, glm::vec4(1, 1, 1, 1));

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  app.shutdown();
  return 0;
}
