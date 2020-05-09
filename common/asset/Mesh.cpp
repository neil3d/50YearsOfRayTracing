#include "Mesh.h"

#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>

#include "../framework/MyException.h"

static_assert(sizeof(tinyobj::real_t) == sizeof(float));

void Mesh::loadFromFile(const std::string& szFileName) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string warn;
  std::string err;
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
  for (const auto& shape : shapes) {
    auto& subMesh = mSubMeshes.emplace_back();
    subMesh.name = shape.name;

    size_t indexCount = shape.mesh.indices.size();
    subMesh.faces.resize(indexCount / 3);
    for (size_t i = 0; i < indexCount; i += 3) {
      auto& face = subMesh.faces[i / 3];
      for (size_t j = 0; j < 3; j++) {
        const tinyobj::index_t& tinyIndex = shape.mesh.indices[i + j];
        face.vertexIndex[j] = tinyIndex.vertex_index;
        face.normalIndex[j] = tinyIndex.normal_index;
        face.texcoordIndex[j] = tinyIndex.texcoord_index;
      }
    }  // end of for_each(index_t)
  }

  // build bounding box

  // build BVH

}