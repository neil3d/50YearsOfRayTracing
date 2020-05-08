#pragma once
#include <string>
#include <memory>

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
