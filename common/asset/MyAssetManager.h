#pragma once
#include <map>
#include <memory>

#include "MyAssetObject.h"

class MyAssetManager {
 public:
  static MyAssetManager& instance() {
    static MyAssetManager gInstance;
    return gInstance;
  }

  template <typename T>
  std::shared_ptr<T> loadAssetObject(const std::string& szPath) {
    // find existing
    auto iter = mAssetDict.find(szPath);
    if (iter != mAssetDict.end())
      return std::dynamic_pointer_cast<T>(iter->second);

    // load new one
    auto assetObj = std::make_shared<T>(szPath);
    assetObj->loadFromFile(szPath);
    mAssetDict.emplace(szPath, assetObj);
    return assetObj;
  }

 private:
  std::map<std::string, std::shared_ptr<MyAssetObject>> mAssetDict;
};
