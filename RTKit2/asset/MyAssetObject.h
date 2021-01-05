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

namespace RTKit2 {
class MyAssetObject {
 public:
  typedef std::shared_ptr<MyAssetObject> Ptr;
  
  MyAssetObject(const std::string& szName) : mName(szName) {}
  virtual ~MyAssetObject() {}

  virtual void loadFromFile(const std::string& szPath) = 0;

 private:
  std::string mName;

 public:
  MyAssetObject(const MyAssetObject&) = delete;
  MyAssetObject& operator=(const MyAssetObject&) = delete;
};
}  // namespace RTKit2
