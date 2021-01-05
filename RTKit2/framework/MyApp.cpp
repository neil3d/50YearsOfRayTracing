#include "MyApp.h"

#include <iostream>

namespace RTKit2 {

void MyApp::init(const MyAppSettings& settings) {
  mSettings = settings;
  // init SDL
  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0) throw MyException("SDL_Init FAILED", SDL_GetError());

  // init Embree
  mDevice = rtcNewDevice(nullptr);
  if (!mDevice) throw MyException("Embree device create FAILED");

  std::cout << "Embree ray mask supported: "
            << rtcGetDeviceProperty(mDevice,
                                    RTC_DEVICE_PROPERTY_RAY_MASK_SUPPORTED)
            << std::endl;

  // create window
  _createWindow(settings.width, settings.height, settings.name.c_str());

  // log
  std::cout << "Base path: " << SDL_GetBasePath() << std::endl;
}

void MyApp::_createWindow(int width, int height, const char* szTitle) {
  mWindowTitle = szTitle;
  mMainWindow =
      SDL_CreateWindow(szTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_UTILITY);
  if (!mMainWindow)
    throw MyException("SDL_CreateWindow FAILED.", SDL_GetError());
}

void MyApp::mainLoop() {
  SDL_Event evt = {};
  while (evt.type != SDL_QUIT) {
    if (SDL_PollEvent(&evt)) {
      _onSDLEvent(evt);
    } else {
      _tick();

      // write image
      if (!mOutputImagePath.empty() && mRenderer && mRenderer->isDone()) {
        mRenderer->screenshot(mOutputImagePath);
        mOutputImagePath.clear();
      }  // end of if
    }
  }  // end of while
}

void MyApp::shutdown() {
  if (mRenderer) {
    mRenderer->_shutdown();
    mRenderer.reset();
  }

  if (mMainWindow) {
    SDL_DestroyWindow(mMainWindow);
    mMainWindow = nullptr;
  }

  if (mDevice) {
    rtcReleaseDevice(mDevice);
    mDevice = nullptr;
  }

  SDL_Quit();
}

void MyApp::_tick() {
  if (!mRenderer) return;

  if (mRenderer->nextPresentReady()) {
    // show rendering result
    mRenderer->_present();
    SDL_UpdateWindowSurface(mMainWindow);

    // show progress
    _showProgress();
  }

  if (mRenderer->isDone()) {
    SDL_Delay(10);
  }
}

void MyApp::_showProgress() {
  float progress = mRenderer->getProgress();
  std::string szTitle = mWindowTitle;
  szTitle.append(mRenderer->getInfo());
  szTitle.append("    ");
  szTitle.append(std::to_string(progress * 100.0f));
  szTitle.append("%");
  SDL_SetWindowTitle(mMainWindow, szTitle.c_str());
}

MyScene::Ptr MyApp::createScene(MySceneLoaderBase* loader,
                                const std::string& szFileName) {
  MyScene::Ptr newScene = std::make_shared<MyScene>(rtcNewScene(mDevice));
  loader->loadScene(newScene, mSettings, szFileName);
  return newScene;
}

}  // namespace RTKit2
