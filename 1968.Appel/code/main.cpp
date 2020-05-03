#include <iostream>

#include "RayCastingRenderer.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"
#include "scene/SpheresScene.h"

const char* const APP_NAME = "Ray Casting - Arthur Appel 1968";
const uint32_t WINDOW_WIDTH = 640;
const uint32_t WINDOW_HEIGHT = 400;

int main(void) {
  MyApp app;
  try {
    app.init();
    app.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
    auto renderer = app.createRenderer<RayCastingRenderer>(2000);
    auto scene = std::make_shared<SpheresScene>();
    scene->init();

    auto camera = std::make_shared<PinholeCamera>();
    camera->setAspect((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)
        .setView(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))
        .setPerspective(45, 0.5, 10);

    renderer->renderScene(scene, camera, glm::vec4(1, 1, 1, 1));

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  app.shutdown();
  return 0;
}
