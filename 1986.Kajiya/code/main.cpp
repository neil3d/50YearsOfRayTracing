#include <iostream>
#include <tuple>

#include "CornellBoxScene.h"
#include "DemoScene.h"
#include "KajiyaScene.h"
#include "MonteCarloPathTracer.h"
#include "MoriKnobScene.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"

using namespace RayTracingHistory;

// const char* const APP_NAME = "Path Tracing -  Kajiya 1986";
const char* const APP_NAME = "Monte Carlo Path Tracing";

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;

enum class EMyTestScenes { MyDemo, CornellBox, MoriKnob, Kajiya };

std::tuple<MyScene::Ptr, glm::vec3, glm::vec3> _createDemoScene(
    EMyTestScenes sceneType);

int main(int argc, char* argv[]) {
  MyApp app;
  try {
    app.init();
    app.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
    auto renderer = app.createRenderer<MonteCarloPathTracer>();

    auto sceneRet = _createDemoScene(EMyTestScenes::CornellBox);
    MyScene::Ptr scene = std::get<0>(sceneRet);
    glm::vec3 eyePos = std::get<1>(sceneRet);
    glm::vec3 lookAt = std::get<2>(sceneRet);
    scene->init();

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

std::tuple<MyScene::Ptr, glm::vec3, glm::vec3> _createDemoScene(
    EMyTestScenes sceneType) {
  MyScene::Ptr scene;
  glm::vec3 eyePos;
  glm::vec3 lookAt;

  switch (sceneType) {
    case EMyTestScenes::CornellBox:
      scene = std::make_shared<CornellBoxScene>();
      eyePos = glm::vec3(278, 278, -656);
      lookAt = glm::vec3(278, 278, 0);
      break;
    case EMyTestScenes::MoriKnob:
      scene = std::make_shared<MoriKnobScene>();
      eyePos = glm::vec3(0, 1, -2);
      lookAt = glm::vec3(0, 0, 0);
      break;
    case EMyTestScenes::Kajiya:
      scene = std::make_shared<KajiyaScene>();
      eyePos = glm::vec3(-250, 250, -1400);
      lookAt = glm::vec3(0, 250, 0);
      break;
    case EMyTestScenes::MyDemo:
    default:
      scene = std::make_shared<DemoScene>();
      eyePos = glm::vec3(0, 250, -850);
      lookAt = glm::vec3(0, 250, 0);
      break;
  }  // end of scene

  return std::make_tuple(scene, eyePos, lookAt);
}