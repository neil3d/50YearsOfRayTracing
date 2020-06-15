#include "TriangleMesh.h"

#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>

#include <filesystem>
#include <limits>

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
  std::filesystem::path filePath(szFileName);
  std::filesystem::path basePath = filePath.remove_filename();
  std::string szBasePath = basePath.string();

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              szFileName.c_str(), szBasePath.c_str(), true);

  if (!warn.empty()) spdlog::warn(warn);
  if (!err.empty()) spdlog::error(err);

  if (!ret)
    throw MyException(std::string("Failed to load/parse: ") + szFileName);

  // copy materials
  for (const auto& mtl : materials) {
    MtlDesc desc = {mtl.name};
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

  std::vector<int> faceList(mFaces.size());
  int i = 0;
  std::generate(faceList.begin(), faceList.end(), [&i] { return i++; });

  _buildBVH(&mBVHRoot, std::move(faceList));
}

TriangleMesh::Intersection TriangleMesh::intersect(const Ray& ray, float tMin,
                                                   float tMax) {
  if (!mBoundingBox.intersect(ray, tMin, tMax)) return Intersection();

#if 0
  return _perFaceIntersect(ray, tMin, tMax);
#else
  return _accelIntersect(&mBVHRoot, ray, tMin, tMax);
#endif
}

TriangleMesh::Intersection TriangleMesh::_accelIntersect(const BVHNode* pNode,
                                                         const Ray& ray,
                                                         float tMin,
                                                         float tMax) {
  Intersection result;

  if (!pNode) return result;

  bool bHitNode = pNode->boudingBox.intersect(ray, tMin, tMax);
  if (!bHitNode) return result;

  // left node
  if (!(pNode->faceList.empty())) {
    float closestSoFar = tMax;

    for (int faceIndex : pNode->faceList) {
      const auto& face = mFaces[faceIndex];
      auto hit = _faceIntersect(face, ray, tMin, closestSoFar);

      if (hit.hit) {
        float tnear = hit.t;
        if (tnear < closestSoFar) {
          result = hit;
          closestSoFar = tnear;
        }
      }
    }  // end of for

    return result;
  }  // end of if

  // recursive
  auto leftResult = _accelIntersect(pNode->leftChild.get(), ray, tMin, tMax);
  auto rightResult = _accelIntersect(pNode->rightChild.get(), ray, tMin, tMax);

  if (leftResult.hit && rightResult.hit) {
    float leftT = leftResult.t;
    float rightT = rightResult.t;
    if (leftT < rightT)
      return leftResult;
    else
      return rightResult;
  } else {
    if (leftResult.hit) return leftResult;
    if (rightResult.hit) return rightResult;
  }

  return Intersection();
}

TriangleMesh::Intersection TriangleMesh::_faceIntersect(const Face& face,
                                                        const Ray& ray,
                                                        float tMin,
                                                        float tMax) {
  Intersection result;

  const auto& v0 = mVertices[face.vertexIndex[0]];
  const auto& v1 = mVertices[face.vertexIndex[1]];
  const auto& v2 = mVertices[face.vertexIndex[2]];

  auto check = Triangle::intersect(ray, v0, v1, v2);
  bool bHit = std::get<0>(check);
  float t = std::get<1>(check);
  glm::vec3 uvw = std::get<2>(check);
  if (bHit && t > tMin && t < tMax) {
    result.hit = true;
    result.t = t;
    result.mtlID = face.materialID;

    if (!mNormals.empty() && face.normalIndex[0] != -1 &&
        face.normalIndex[1] != -1 && face.normalIndex[2] != -1) {
      const auto& n0 = mNormals[face.normalIndex[0]];
      const auto& n1 = mNormals[face.normalIndex[1]];
      const auto& n2 = mNormals[face.normalIndex[2]];

      result.normal =
          glm::normalize(Triangle::barycentricInterpolation(uvw, n0, n1, n2));
    } else {
      result.normal = face.normal;
    }

    if (!mTexcoords.empty() && face.texcoordIndex[0] != -1 &&
        face.texcoordIndex[1] != -1 && face.texcoordIndex[2] != -1) {
      const auto& uv0 = mTexcoords[face.texcoordIndex[0]];
      const auto& uv1 = mTexcoords[face.texcoordIndex[1]];
      const auto& uv2 = mTexcoords[face.texcoordIndex[2]];
      result.uv = Triangle::barycentricInterpolation(uvw, uv0, uv1, uv2);
    }
  }

  return result;
}

TriangleMesh::Intersection TriangleMesh::_perFaceIntersect(const Ray& ray,
                                                           float tMin,
                                                           float tMax) {
  Intersection result;
  float closestSoFar = tMax;

  for (auto& face : mFaces) {
    auto hit = _faceIntersect(face, ray, tMin, closestSoFar);
    if (hit.hit) {
      float tnear = hit.t;
      if (tnear < closestSoFar) {
        result = hit;
        closestSoFar = tnear;
      }
    }
  }  // end of for

  return result;
}
std::vector<MyMaterial::Ptr> TriangleMesh::importMaterial(
    MaterialImporter* pImporter) {
  std::vector<MyMaterial::Ptr> result;
  for (const auto& desc : mMaterials) {
    result.emplace_back(pImporter->importObj(desc.name));
  }

  return result;
}
void TriangleMesh::_generateFaceNormal() {
  for (auto& face : mFaces) {
    const auto& v0 = mVertices[face.vertexIndex[0]];
    const auto& v1 = mVertices[face.vertexIndex[1]];
    const auto& v2 = mVertices[face.vertexIndex[2]];

    face.normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
  }  // end of for
}

void TriangleMesh::_buildBoundingBox() {
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

void TriangleMesh::_buildBVH(BVHNode* pNode, std::vector<int> faceList) {
  constexpr size_t LEAF_FACE_COUNT = 5;

  // build bounding box for this node
  pNode->boudingBox = _buildBoundingBox(faceList);

  // is a leaf node
  if (faceList.size() <= LEAF_FACE_COUNT) {
    pNode->faceList = std::move(faceList);
    return;
  }

  // split face list to children nodes
  pNode->leftChild = std::make_unique<BVHNode>();
  pNode->rightChild = std::make_unique<BVHNode>();

  auto split = faceList.size() / 2;
  std::vector<int> leftArray(faceList.begin(), faceList.begin() + split);
  std::vector<int> rightArray(faceList.begin() + split, faceList.end());

  _buildBVH(pNode->leftChild.get(), std::move(leftArray));
  _buildBVH(pNode->rightChild.get(), std::move(rightArray));
}

AABBox TriangleMesh::_buildBoundingBox(const std::vector<int>& faceList) {
  constexpr float S = std::numeric_limits<float>::min();
  constexpr float B = std::numeric_limits<float>::max();

  glm::vec3 max(S, S, S);
  glm::vec3 min(B, B, B);

  for (const auto& i : faceList) {
    const Face& face = mFaces[i];
    for (int j = 0; j < 3; j++) {
      const auto& v = mVertices[face.vertexIndex[j]];
      if (v.x > max.x) max.x = v.x;
      if (v.y > max.y) max.y = v.y;
      if (v.z > max.z) max.z = v.z;

      if (v.x < min.x) min.x = v.x;
      if (v.y < min.y) min.y = v.y;
      if (v.z < min.z) min.z = v.z;
    }
  }  // end of for

  glm::vec3 e(0.01f, 0.01f, 0.01f);
  return {min - e, max + e};
}

}  // namespace RayTracingHistory