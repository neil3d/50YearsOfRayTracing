#pragma once
#include "MySceneLoaderBase.h"

namespace RTKit2 {

/**
 * load scene from a custom json config file
 */
class MySceneLoader : public MySceneLoaderBase {
 public:
  virtual void loadScene(MyScene::Ptr scene, const MyAppSettings& appSettings,
                         const std::string& szFileName) override;
};
}  // namespace RTKit2