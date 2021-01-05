/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <map>
#include <memory>
#include <vector>

#include "../framework/MyException.h"
#include "MyAssetObject.h"

namespace RTKit2 {
class MyAssetManager {
 public:
  MyAssetManager() {
    mSearchPath.emplace_back("./");            // vs code start path
    mSearchPath.emplace_back("../../");        // vs code start path
    mSearchPath.emplace_back("../../../");     // visual studio start path
    mSearchPath.emplace_back("../../../../");  // visual studio start path
  }

  static MyAssetManager& instance() {
    static MyAssetManager gInstance;
    return gInstance;
  }

  std::string makeFullPath(const std::string& szPath) const;

  MyAssetObject::Ptr add(const std::string& szKey, const std::string& szPath);

  template <typename T>
  std::shared_ptr<T> get(const std::string& szKey) {
    auto iter = mAssetDict.find(szKey);
    if (iter == mAssetDict.end())
      return std::shared_ptr<T>();
    else
      return std::dynamic_pointer_cast<T>(iter->second);
  }

 private:
  template <typename T>
  std::shared_ptr<T> load(const std::string& szKey, const std::string& szPath) {
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
    auto assetObj = std::make_shared<T>(szKey);
    assetObj->loadFromFile(szFullPath);
    mAssetDict.emplace(szKey, assetObj);
    return assetObj;
  }

 private:
  std::vector<std::string> mSearchPath;
  std::map<std::string, std::shared_ptr<MyAssetObject>> mAssetDict;
};
}  // namespace RTKit2