#pragma once

#include "MyAssetObject.h"

class Image : public MyAssetObject {
 public:
  typedef std::shared_ptr<Image> Ptr;
};
