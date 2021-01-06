#include "TriangleMesh.h"

#include <tiny_obj_loader.h>

#include <filesystem>
#include <iostream>
#include <limits>

#include "../framework/MyException.h"
#include "../geometry/Triangle.h"

static_assert(sizeof(tinyobj::real_t) == sizeof(float));

namespace RTKit1 {

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
  std::string szMainFileName =
      std::filesystem::path(szFileName).filename().string();

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              szFileName.c_str(), szBasePath.c_str(), true);

  if (!warn.empty()) std::cerr << warn << std::endl;
  if (!err.empty()) std::cerr << err << std::endl;

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
      }
    }  // end of for each index
  }    // end of for each shape

  std::cout << "triangle mesh loaded: " << szMainFileName
            << "\n\tnum vertices = " << mVertices.size()
            << "\n\tnum faces = " << totalFaceCount
            << "\n\tnum sub meshes = " << mSubMeshes.size() << std::endl;

  // build internal states
  _postMeshCreated();
}  // namespace RTKit1

void TriangleMesh::_postMeshCreated() {
  // generation
  _generateFaceNormal();
  _buildBoundingBox();

  // build BVH for each sub mesh
  std::cout << "building BVH ..." << std::endl;
  for (auto& subMesh : mSubMeshes) {
    std::vector<int> faceList(subMesh.faces.size());
    int i = 0;
    std::generate(faceList.begin(), faceList.end(), [&i] { return i++; });

    _buildBVH(&subMesh, &subMesh.BVHRoot, std::move(faceList));

    subMesh.boundingBox = subMesh.BVHRoot.boudingBox;
  }
}

void TriangleMesh::createDynamic(const std::vector<glm::vec3>& vertices,
                                 const std::vector<int>& indices) {
  // copy vertices
  mVertices.resize(vertices.size());
  std::memcpy(mVertices.data(), vertices.data(),
              vertices.size() * sizeof(glm::vec3));

  // indices --> triangle faces
  mSubMeshes.resize(1);
  auto& subMesh = mSubMeshes[0];

  subMesh.faces.resize(indices.size() / 3);
  for (size_t i = 0; i < indices.size(); i += 3) {
    auto& face = subMesh.faces[i / 3];
    face.vertexIndex[0] = indices[i];
    face.vertexIndex[1] = indices[i + 1];
    face.vertexIndex[2] = indices[i + 2];
    face.materialID = -1;
  }

  // build internal states
  _postMeshCreated();
}

TriangleMesh::Intersection TriangleMesh::intersect(const Ray& ray, float tMin,
                                                   float tMax) {
  if (!mBoundingBox.intersect(ray, tMin, tMax)) return Intersection();

#if 0
  return _perFaceIntersect(ray, tMin, tMax);
#else
  Intersection result;
  float closestSoFar = tMax;

  for (auto& subMesh : mSubMeshes) {
    auto hit = _accelIntersect(&subMesh, &subMesh.BVHRoot, ray, tMin, tMax);
    if (hit.hit) {
      float tnear = hit.t;
      if (tnear < closestSoFar) {
        result = hit;
        closestSoFar = tnear;
      }
    }
  }
  return result;
#endif
}

TriangleMesh::Intersection TriangleMesh::_accelIntersect(const SubMesh* subMesh,
                                                         const BVHNode* pNode,
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
      const auto& face = subMesh->faces[faceIndex];
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
  auto leftResult =
      _accelIntersect(subMesh, pNode->leftChild.get(), ray, tMin, tMax);
  auto rightResult =
      _accelIntersect(subMesh, pNode->rightChild.get(), ray, tMin, tMax);

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

  for (auto& subMesh : mSubMeshes) {
    if (subMesh.boundingBox.intersect(ray, tMin, closestSoFar)) {
      for (auto& face : subMesh.faces) {
        auto hit = _faceIntersect(face, ray, tMin, closestSoFar);
        if (hit.hit) {
          float tnear = hit.t;
          if (tnear < closestSoFar) {
            result = hit;
            closestSoFar = tnear;
          }
        }
      }  // end of for each face
    }    // end of if
  }

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
  for (auto& subMesh : mSubMeshes) {
    for (auto& face : subMesh.faces) {
      const auto& v0 = mVertices[face.vertexIndex[0]];
      const auto& v1 = mVertices[face.vertexIndex[1]];
      const auto& v2 = mVertices[face.vertexIndex[2]];

      face.normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    }  // end of for each face
  }
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

void TriangleMesh::_buildBVH(SubMesh* subMesh, BVHNode* pNode,
                             std::vector<int> faceList) {
  constexpr size_t LEAF_FACE_COUNT = 5;

  // build bounding box for this node
  pNode->boudingBox = _buildBoundingBox(subMesh, faceList);

  // is a leaf node
  if (faceList.size() <= LEAF_FACE_COUNT) {
    pNode->faceList = std::move(faceList);
    return;
  }

  // sort the face list along the max axis extend
  char maxExt = pNode->boudingBox.maxExtent();
  switch (maxExt) {
    case 'x':
      std::sort(std::begin(faceList), std::end(faceList),
                [this, subMesh](int f1, int f2) {
                  const auto& face1 = subMesh->faces[f1];
                  const auto& face2 = subMesh->faces[f2];
                  float x1 = 0, x2 = 0;
                  for (int i = 0; i < 3; i++) {
                    x1 += this->mVertices[face1.vertexIndex[i]].x;
                    x2 += this->mVertices[face2.vertexIndex[i]].x;
                  }
                  return x1 < x2;
                });
      break;
    case 'y':
      std::sort(std::begin(faceList), std::end(faceList),
                [this, subMesh](int f1, int f2) {
                  const auto& face1 = subMesh->faces[f1];
                  const auto& face2 = subMesh->faces[f2];
                  float y1 = 0, y2 = 0;
                  for (int i = 0; i < 3; i++) {
                    y1 += this->mVertices[face1.vertexIndex[i]].y;
                    y2 += this->mVertices[face2.vertexIndex[i]].y;
                  }
                  return y1 < y2;
                });
      break;
    case 'z':
      std::sort(std::begin(faceList), std::end(faceList),
                [this, subMesh](int f1, int f2) {
                  const auto& face1 = subMesh->faces[f1];
                  const auto& face2 = subMesh->faces[f2];
                  float z1 = 0, z2 = 0;
                  for (int i = 0; i < 3; i++) {
                    z1 += this->mVertices[face1.vertexIndex[i]].z;
                    z2 += this->mVertices[face2.vertexIndex[i]].z;
                  }
                  return z1 < z2;
                });
      break;
    default:
      std::cerr << "max extent error" << std::endl;
      break;
  }

  // split face list to children nodes
  pNode->leftChild = std::make_unique<BVHNode>();
  pNode->rightChild = std::make_unique<BVHNode>();

  auto split = faceList.size() / 2;
  std::vector<int> leftArray(faceList.begin(), faceList.begin() + split);
  std::vector<int> rightArray(faceList.begin() + split, faceList.end());

  _buildBVH(subMesh, pNode->leftChild.get(), std::move(leftArray));
  _buildBVH(subMesh, pNode->rightChild.get(), std::move(rightArray));
}

AABBox TriangleMesh::_buildBoundingBox(SubMesh* subMesh,
                                       const std::vector<int>& faceList) {
  constexpr float S = std::numeric_limits<float>::min();
  constexpr float B = std::numeric_limits<float>::max();

  glm::vec3 max(S, S, S);
  glm::vec3 min(B, B, B);

  for (const auto& i : faceList) {
    const Face& face = subMesh->faces[i];
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

}  // namespace RTKit1