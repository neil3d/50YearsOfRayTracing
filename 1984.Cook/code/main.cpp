#include <iostream>

#include "BilliardScene.h"
#include "DistributedRayTracer.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"

const char* const APP_NAME =
    "Distributed Ray Tracer -  Robert Cook et al. 1984";
const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;

int main(void) {
  MyApp app;
  try {
    app.init();
    app.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
    auto renderer = app.createRenderer<DistributedRayTracer>();
    
    auto scene = std::make_shared<BilliardScene>();
    scene->init();


    // renderer->renderScene(scene, camera, glm::vec4(1, 1, 1, 1));

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  app.shutdown();
  return 0;
}