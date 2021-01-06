#include <iostream>

#include "AnalyticDirectIntegrator.h"
#include "framework/MyApp.h"
#include "framework/MySceneLoader.h"
using namespace RTKit2;

const char* const APP_NAME = "Analytic Direct Lighting - Arvo 1995";
const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;

int main(int argc, char* argv[]) {
  MyApp app;
  MyRenderer::Ptr renderer;
  MyCamera::Ptr camera;
  MyScene::Ptr scene;

  MyAppSettings settings = {APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT};

  try {
    // create app & window
    app.init(settings);

    // create my renderer
    renderer = app.createRenderer<AnalyticDirectIntegrator>();

    // load scene
    {
      MySceneLoader loader;
      scene = app.createScene(&loader, "content/dragon.scene");
      camera = loader.getCamera();
    }

    // here we go
    renderer->renderScene(scene, camera, glm::vec4(0, 0, 0, 1));
    app.mainLoop();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  app.shutdown();
  return 0;
}
