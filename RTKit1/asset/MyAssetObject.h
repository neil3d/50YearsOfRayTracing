/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>
#include <string>

namespace RTKit1 {
class MyAssetObject {
 private:
  std::string mName;

 public:
  MyAssetObject(const std::string& szName) : mName(szName) {}
  virtual ~MyAssetObject() {}

  virtual void loadFromFile(const std::string& szPath) = 0;

 public:
  MyAssetObject(const MyAssetObject&) = delete;
  MyAssetObject& operator=(const MyAssetObject&) = delete;
};
}  // namespace RTKit1
