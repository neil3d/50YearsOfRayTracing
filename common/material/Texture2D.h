#pragma once
#include "../asset/Image.h"
#include "MyTexture.h"

class Texture2D : public MyTexture {
 private:
  Image::Ptr mImage;
};