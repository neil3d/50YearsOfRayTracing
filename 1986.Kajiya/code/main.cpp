#include <iostream>

#include "CornellBoxScene.h"
#include "DemoScene.h"
#include "MonteCarloPathTracer.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"

using namespace RayTracingHistory;

// const char* const APP_NAME = "Path Tracing -  Kajiya 1986";
const char* const APP_NAME = "Monte Carlo Direct Lighting";

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;

int main(int argc, char* argv[]) {
  MyApp app;
  try {
    app.init();
    app.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
    auto renderer = app.createRenderer<MonteCarloPathTracer>();

#if 1
    auto scene = std::make_shared<CornellBoxScene>();
    scene->init();

    glm::vec3 eyePos(278, 278, -656);
    glm::vec3 lookAt(278, 278, 0);
#else
    auto scene = std::make_shared<DemoScene>();
    scene->init();

    glm::vec3 eyePos(0, 250, -850);
    glm::vec3 lookAt(0, 250, 0);
#endif

    auto camera = std::make_shared<PinholeCamera>();
    camera->setZNear(0.1f)
        .setAspect((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)
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