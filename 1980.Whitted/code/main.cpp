#include <iostream>

#include "WhittedRayTracer.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"
#include "DemoScene.h"

using namespace RayTracingHistory;

const char* const APP_NAME = "Recursive Ray Tracing - Turner Whitted 1980";
const uint32_t WINDOW_WIDTH = 640;
const uint32_t WINDOW_HEIGHT = 360;

int main(void) {
  MyApp app;
  try {
    app.init();
    app.createWindow(1280, 720, APP_NAME);

    auto renderer = app.createRenderer<WhittedRayTracer>();
    auto scene = std::make_shared<DemoScene>();
    scene->init();

    glm::vec3 eyePos(0, 1.25f, -8);
    glm::vec3 lookAt(0, 1, 0);
    auto camera = std::make_shared<PinholeCamera>();
    camera->setAspect((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)
        .setView(eyePos, lookAt, glm::vec3(0, 1, 0))
        .setFOV(45)
        .setFocalLength(1.0f);

    renderer->renderScene(scene, camera, glm::vec4(0, 0, 0, 1));

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  app.shutdown();
  return 0;
}
