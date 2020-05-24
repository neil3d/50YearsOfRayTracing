/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>
#include <string>

namespace RayTracingHistory {
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
}  // namespace RayTracingHistory
