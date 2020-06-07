#include "TriangleMesh.h"

#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>

#include "../framework/MyException.h"
#include "../geometry/Triangle.h"

static_assert(sizeof(tinyobj::real_t) == sizeof(float));

namespace RayTracingHistory {

void TriangleMesh::loadFromFile(const std::string& szFileName) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string warn;
  std::string err;
  
  // load and trianglulation
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              szFileName.c_str(), nullptr, true);

  if (!warn.empty()) spdlog::warn(warn);
  if (!err.empty()) spdlog::error(err);

  if (!ret)
    throw MyException(std::string("Failed to load/parse: ") + szFileName);

  // copy attributes
  mVertices.resize(attrib.vertices.size() / 3);
  std::memcpy(mVertices.data(), attrib.vertices.data(),
              attrib.vertices.size() * sizeof(float));

  mNormals.resize(attrib.normals.size() / 3);
  std::memcpy(mNormals.data(), attrib.normals.data(),
              attrib.normals.size() * sizeof(float));

  mTexcoords.resize(attrib.texcoords.size() / 2);
  std::memcpy(mTexcoords.data(), attrib.texcoords.data(),
              attrib.texcoords.size() * sizeof(float));

  // convert sub meshes
  size_t faceCount = 0, faceIndex = 0;
  for (const auto& shape : shapes) {
    size_t indexCount = shape.mesh.indices.size();
    faceCount += indexCount / 3;
  }
  mFaces.resize(faceCount);

  for (const auto& shape : shapes) {
    const auto& mesh = shape.mesh;
    size_t indexCount = mesh.indices.size();
    size_t meshFaceIndex = 0;
    for (size_t i = 0; i < indexCount; i += 3) {
      auto& face = mFaces[faceIndex++];
      face.materialID = mesh.material_ids[meshFaceIndex++];

      for (size_t j = 0; j < 3; j++) {
        const tinyobj::index_t& tinyIndex = mesh.indices[i + j];
        face.vertexIndex[j] = tinyIndex.vertex_index;
        face.normalIndex[j] = tinyIndex.normal_index;
        face.texcoordIndex[j] = tinyIndex.texcoord_index;
      }
    }  // end of for_each(index_t)
  }

  // build bounding box

  // build BVH
}
}  // namespace RayTracingHistory