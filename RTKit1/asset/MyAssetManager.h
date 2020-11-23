/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <filesystem>
#include <map>
#include <memory>
#include <vector>

#include "../framework/MyException.h"
#include "MyAssetObject.h"

namespace RTKit1 {
class MyAssetManager {
 public:
  MyAssetManager() {
    mSearchPath.emplace_back("./");               // vs code start path
    mSearchPath.emplace_back("../../../"); 
    mSearchPath.emplace_back("../../../../");  // visual studio start path
    mSearchPath.emplace_back("../../../../../");     // visual studio start path
  }

  static MyAssetManager& instance() {
    static MyAssetManager gInstance;
    return gInstance;
  }

  std::string makeFullPath(const std::string& szPath) const {
    for (const auto& szBasePath : mSearchPath) {
      std::filesystem::path fullPath(szBasePath);
      fullPath.append(szPath);

      if (std::filesystem::exists(fullPath)) {
        return fullPath.string();
      }
    }  // end of for
    return std::string();
  }

  template <typename T>
  std::shared_ptr<T> loadAssetObject(const std::string& szPath) {
    // find existing
    auto iter = mAssetDict.find(szPath);
    if (iter != mAssetDict.end())
      return std::dynamic_pointer_cast<T>(iter->second);

    std::string szFullPath = makeFullPath(szPath);

    if (szFullPath.empty()) {
      std::string szError("Asset file NOT exist: ");
      szError.append(szPath);
      throw MyException(szError);
      return std::shared_ptr<T>();
    }

    // load new one
    auto assetObj = std::make_shared<T>(szPath);
    assetObj->loadFromFile(szFullPath);
    mAssetDict.emplace(szPath, assetObj);
    return assetObj;
  }

 private:
  std::vector<std::string> mSearchPath;
  std::map<std::string, std::shared_ptr<MyAssetObject>> mAssetDict;
};
}  // namespace RTKit1