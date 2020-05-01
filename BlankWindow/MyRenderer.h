#pragma once
#include <memory>

class MyRenderer {
 public:
  typedef std::shared_ptr<MyRenderer> Ptr;
  MyRenderer() = default;
  virtual ~MyRenderer() = default;

 public:
  virtual void _init(int frameWidth, int frameHeight) {
    mFrameWidth = frameWidth;
    mFrameHeight = frameHeight;
  }

 private:
  int mFrameWidth = 0;
  int mFrameHeight = 0;

 public:
  MyRenderer(const MyRenderer&) = delete;
  MyRenderer& operator=(const MyRenderer&) = delete;
};