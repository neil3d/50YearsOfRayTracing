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

  if (attrib.normals.size() > 0) {
    mNormals.resize(attrib.normals.size() / 3);
    std::memcpy(mNormals.data(), attrib.normals.data(),
                attrib.normals.size() * sizeof(float));
  }

  if (attrib.texcoords.size() > 0) {
    mTexcoords.resize(attrib.texcoords.size() / 2);
    std::memcpy(mTexcoords.data(), attrib.texcoords.data(),
                attrib.texcoords.size() * sizeof(float));
  }

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

  // generation
  _generateFaceNormal();
  _buildBoundingBox();

  // build BVH
}

std::tuple<bool, float, glm::vec3, glm::vec2> TriangleMesh::intersect(
    const Ray& ray, float tMin, float tMax) {
  bool hitAnyFace = false;
  glm::vec3 hitNormal(0, 1, 0);
  glm::vec2 hitUV(0, 0);
  float tnear = 0;
  float closestSoFar = tMax;

  for (auto& face : mFaces) {
    const auto& v0 = mVertices[face.vertexIndex[0]];
    const auto& v1 = mVertices[face.vertexIndex[1]];
    const auto& v2 = mVertices[face.vertexIndex[2]];

    auto check = Triangle::intersect(ray, v0, v1, v2);
    bool bHit = std::get<0>(check);
    float t = std::get<1>(check);
    if (bHit && t > tMin && t < closestSoFar) {
      hitAnyFace = true;
      tnear = t;
      closestSoFar = t;
      hitNormal = face.normal;
      // TODO: UV
    }
  }  // end of for

  return std::make_tuple(hitAnyFace, tnear, hitNormal, hitUV);
}
void TriangleMesh::_generateFaceNormal() {
  for (auto& face : mFaces) {
    const auto& v0 = mVertices[face.vertexIndex[0]];
    const auto& v1 = mVertices[face.vertexIndex[1]];
    const auto& v2 = mVertices[face.vertexIndex[2]];

    face.normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
  }  // end of for
}

void TriangleMesh::_buildBoundingBox() {}

}  // namespace RayTracingHistory