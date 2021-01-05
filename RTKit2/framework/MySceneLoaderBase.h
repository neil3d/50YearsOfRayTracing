#pragma once
#include <embree3/rtcore.h>

#include <string>

#include "../scene/MyScene.h"
#include "MyCamera.h"

namespace RTKit2 {

struct MyAppSettings;

class MySceneLoaderBase {
 public:
  MySceneLoaderBase() = default;
  virtual ~MySceneLoaderBase() = default;

  virtual void loadScene(MyScene::Ptr scene, const MyAppSettings& appSettings,
                         const std::string& szFileName) = 0;

  MyCamera::Ptr getCamera() const { return mCamera; }

 protected:
  MyCamera::Ptr mCamera;

 public:
  MySceneLoaderBase(const MySceneLoaderBase&) = delete;
  MySceneLoaderBase& operator=(const MySceneLoaderBase&) = delete;
};
}  // namespace RTKit2