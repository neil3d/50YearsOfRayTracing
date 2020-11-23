#include "ImageRGBA.h"

#include <fstream>

#include "./framework/MyException.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image.h>

namespace RTKit1 {

void ImageRGBA::loadFromFile(const std::string& szPath) {
  static_assert(sizeof(Pixel) == 4);

  int x, y, n;
  unsigned char* data = stbi_load(szPath.c_str(), &x, &y, &n, 4);
  if (data == nullptr || x <= 0 || y <= 0) {
    std::string szError("load image FAILED: ");
    szError.append(szPath);
    throw MyException(szError);
  }

  // copy to member
  mWidth = x;
  mHeight = y;

  mData.resize(x * y);
  std::memcpy(mData.data(), data, x * y * 4);

  // free stb memory
  stbi_image_free(data);
}

}  // namespace RTKit1