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

#include "../material/MyMaterial.h"
namespace RayTracingHistory {

class MaterialImporter {
 public:
  MaterialImporter() = default;
  virtual ~MaterialImporter() = default;

  virtual MyMaterial::Ptr importObj(const std::string& szName) = 0;
};
}  // namespace RayTracingHistory