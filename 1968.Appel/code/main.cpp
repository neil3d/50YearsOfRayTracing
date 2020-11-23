#include <iostream>

#include "DemoScene.h"
#include "RayCastingRenderer.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"

using namespace RTKit1;

const char* const APP_NAME = "Ray Casting - Arthur Appel 1968";
const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;

int main(int argc, char* argv[]) {
  MyApp app;
  try {
    app.init();
    app.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
    auto renderer = app.createRenderer<RayCastingRenderer>();
    auto scene = std::make_shared<DemoScene>();
    scene->init();

    glm::vec3 eyePos(-5, 5, 10);
    glm::vec3 lookAt(0, 1, 0);
    auto camera = std::make_shared<PinholeCamera>();
    camera->setAspect((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)
        .setView(eyePos, lookAt, glm::vec3(0, 1, 0))
        .setFOV(45);
    camera->init();

    renderer->renderScene(scene, camera, glm::vec4(1, 1, 1, 1));

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  app.shutdown();
  return 0;
}
