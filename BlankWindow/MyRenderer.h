#pragma once
#include <memory>

class MyRenderer {
 public:
  typedef std::shared_ptr<MyRenderer> Ptr;
  MyRenderer() = default;
  virtual ~MyRenderer() = default;

 public:
  MyRenderer(const MyRenderer&) = delete;
  MyRenderer& operator=(const MyRenderer&) = delete;
};