#pragma once
#include <embree3/rtcore.h>

#include <string>

#include "../scene/MyScene.h"

namespace RTKit2 {

class MySceneLoaderBase {
 public:
  MySceneLoaderBase() = default;
  virtual ~MySceneLoaderBase() = default;

  virtual void loadScene(MyScene::Ptr scene, const std::string& szFileName) = 0;

 public:
  MySceneLoaderBase(const MySceneLoaderBase&) = delete;
  MySceneLoaderBase& operator=(const MySceneLoaderBase&) = delete;
};
}  // namespace RTKit2