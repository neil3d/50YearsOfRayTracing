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

#include "../material/MyMaterial.h"
namespace RTKit1 {

class MaterialImporter {
 public:
  MaterialImporter() = default;
  virtual ~MaterialImporter() = default;

  virtual MyMaterial::Ptr importObj(const std::string& szName) = 0;
};
}  // namespace RTKit1