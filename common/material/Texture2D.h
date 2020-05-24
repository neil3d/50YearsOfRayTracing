/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "../asset/Image.h"
#include "MyTexture.h"

namespace RayTracingHistory {

class Texture2D : public MyTexture {
 private:
  Image::Ptr mImage;
};

}  // namespace RayTracingHistory
