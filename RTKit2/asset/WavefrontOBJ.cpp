#include "WavefrontOBJ.h"

#include <tiny_obj_loader.h>

#include <filesystem>
#include <iostream>
#include <limits>

#include "../framework/MyException.h"

static_assert(sizeof(tinyobj::real_t) == sizeof(float));

namespace RTKit2 {

static glm::vec3 _getVec(const float c[3]) {
  return glm::vec3(c[0], c[1], c[2]);
}

void WavefrontOBJ::loadFromFile(const std::string& szPath) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string warn;
  std::string err;

  // load and trianglulation
  std::filesystem::path filePath(szPath);
  std::filesystem::path basePath = filePath.remove_filename();
  std::string szBasePath = basePath.string();
  std::string szMainFileName =
      std::filesystem::path(szPath).filename().string();

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              szPath.c_str(), szBasePath.c_str(), true);

  if (!warn.empty()) std::cerr << warn << std::endl;
  if (!err.empty()) std::cerr << err << std::endl;

  if (!ret) throw MyException(std::string("Failed to load/parse: ") + szPath);

  // copy materials
  for (const auto& mtl : materials) {
    MtlDesc desc;
    desc.name = mtl.name;
    desc.emission = _getVec(mtl.emission);
    desc.diffuse = _getVec(mtl.diffuse);
    desc.specular = _getVec(mtl.specular);
    desc.shininess = mtl.shininess;
    mMaterials.push_back(desc);
  }

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
  size_t totalFaceCount = 0;

  mSubMeshes.resize(shapes.size());
  for (size_t i = 0; i < shapes.size(); i++) {
    const auto& shape = shapes[i];
    auto& subMesh = mSubMeshes[i];

    size_t indexCount = shape.mesh.indices.size();
    size_t meshFaceIndex = 0;

    subMesh.faces.resize(indexCount / 3);
    totalFaceCount += subMesh.faces.size();

    for (size_t i = 0; i < indexCount; i += 3) {
      auto& face = subMesh.faces[meshFaceIndex];
      face.materialID = shape.mesh.material_ids[meshFaceIndex];
      meshFaceIndex++;

      for (size_t j = 0; j < 3; j++) {
        const tinyobj::index_t& tinyIndex = shape.mesh.indices[i + j];
        face.vertexIndex[j] = tinyIndex.vertex_index;
        face.normalIndex[j] = tinyIndex.normal_index;
        face.texcoordIndex[j] = tinyIndex.texcoord_index;

        subMesh.indices.push_back(tinyIndex.vertex_index);
      }
    }  // end of for each index
  }    // end of for each shape

  std::cout << "triangle mesh loaded: " << szMainFileName
            << "\n\tnum vertices = " << mVertices.size()
            << "\n\tnum faces = " << totalFaceCount
            << "\n\tnum sub meshes = " << mSubMeshes.size() << std::endl;

  // internal data
  _generateFaceNormal();
  _buildBoundingBox();
}

void WavefrontOBJ::_generateFaceNormal() {
  for (auto& subMesh : mSubMeshes) {
    for (auto& face : subMesh.faces) {
      const auto& v0 = mVertices[face.vertexIndex[0]];
      const auto& v1 = mVertices[face.vertexIndex[1]];
      const auto& v2 = mVertices[face.vertexIndex[2]];

      face.normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    }  // end of for each face
  }
}

void WavefrontOBJ::_buildBoundingBox() {
  constexpr float S = std::numeric_limits<float>::min();
  constexpr float B = std::numeric_limits<float>::max();

  glm::vec3 max(S, S, S);
  glm::vec3 min(B, B, B);

  for (const auto& v : mVertices) {
    if (v.x > max.x) max.x = v.x;
    if (v.y > max.y) max.y = v.y;
    if (v.z > max.z) max.z = v.z;

    if (v.x < min.x) min.x = v.x;
    if (v.y < min.y) min.y = v.y;
    if (v.z < min.z) min.z = v.z;
  }

  glm::vec3 e(0.01f, 0.01f, 0.01f);

  mBoundingBox.max = max + e;
  mBoundingBox.min = min - e;
}
template <typename T>
T _barycentricInterpolation(const glm::vec2& uv, const T& v0, const T& v1,
                            const T& v2) {
  return (1.0f - uv.x - uv.y) * v0 + uv.x * v1 + uv.y * v2;
}

glm::vec3 WavefrontOBJ::getNormal(const Face& face, const glm::vec2& uv) {
  if (face.normalIndex[0] == -1)
    return face.normal;
  else
    return _barycentricInterpolation(uv, mNormals[face.normalIndex[0]],
                                     mNormals[face.normalIndex[1]],
                                     mNormals[face.normalIndex[2]]);
}
}  // namespace RTKit2