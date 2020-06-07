/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
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

namespace RayTracingHistory {
class MyAssetManager {
 public:
  MyAssetManager() {
    mSearchPath.emplace_back("./");               // vs code start path
    mSearchPath.emplace_back("../../../../../");  // visual studio start path
  }

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

    std::string szFullPath;
    for (const auto& szBasePath : mSearchPath) {
      std::filesystem::path fullPath(szBasePath);
      fullPath.append(szPath);

      if (std::filesystem::exists(fullPath)) {
        szFullPath = fullPath.string();
        break;
      }
    }  // end of for

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
}  // namespace RayTracingHistory