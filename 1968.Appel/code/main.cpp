#include <iostream>

#include "RayCastingRenderer.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"
#include "scene/SpheresScene.h"

const char* const APP_NAME = "Ray Casting - Arthur Appel 1968";
const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;

int main(void) {
  MyApp app;
  try {
    app.init();
    app.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
    auto renderer = app.createRenderer<RayCastingRenderer>();
    auto scene = std::make_shared<SpheresScene>();
    scene->init();

    glm::vec3 eyePos(0, 1.25f, 5);
    glm::vec3 lookAt(0, 0, 0);
    auto camera = std::make_shared<PinholeCamera>();
    camera->setAspect((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)
        .setView(eyePos, lookAt, glm::vec3(0, 1, 0))
        .setPerspective(45, 1, 1000);

    renderer->renderScene(scene, camera, glm::vec4(0, 0, 0, 1));

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  app.shutdown();
  return 0;
}
