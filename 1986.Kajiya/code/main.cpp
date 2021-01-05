#include <iostream>
#include <tuple>

#include "framework/MyApp.h"
#include "framework/MySceneLoader.h"
#include "framework/PinholeCamera.h"

using namespace RTKit2;

const char* const APP_NAME = "Path Tracing -  Kajiya 1986";

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;

int main(int argc, char* argv[]) {
  MyApp app;
  MyRenderer::Ptr renderer;
  MyCamera::Ptr camera;
  MyScene::Ptr scene;

  try {
    // create app & window
    app.init();
    app.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);

    // create my renderer
    // auto renderer = app.createRenderer<MyPathTracer>();

    // load scene
    {
      MySceneLoader loader;
      app.createScene(&loader, "content/minimal.scene");
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
