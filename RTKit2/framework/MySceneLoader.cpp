#include "MySceneLoader.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "../asset/MyAssetManager.h"
#include "../external/nlohmann/json.hpp"
#include "../scene/TriangleMesh.h"
#include "MyTransform.h"

namespace RTKit2 {

enum ECameraType { PinholeCamera, ThinLensCamera };

NLOHMANN_JSON_SERIALIZE_ENUM(ECameraType, {{PinholeCamera, "pinhole"},
                                           {ThinLensCamera, "thinlens"}})

struct CameraSettings {
  ECameraType type = PinholeCamera;
  float eye[3] = {0, 0, -5};
  float lookAt[3] = {0, 0, 0};
  float up[3] = {0, 1, 0};
  float fov = 45;
};

struct TransformSettings {
  float position[3] = {0, 0, 0};
  float rotation[3] = {0, 0, 0};
  float scale[3] = {0, 0, 0};

  glm::vec3 getPosition() const {
    return glm::vec3(position[0], position[1], position[2]);
  }
  glm::quat getRotation() const {
    return glm::quat(glm::vec3(glm::radians(rotation[0]),
                               glm::radians(rotation[1]),
                               glm::radians(rotation[2])));
  }
  glm::vec3 getScale() const { return glm::vec3(scale[0], scale[1], scale[2]); }
};

struct QuadLightSettings {
  float intensity[3] = {1, 1, 1};
  float corner[3] = {0, 0, 0};
  float edge1[3] = {1, 0, 0};
  float edge2[3] = {0, 0, 1};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraSettings, type, eye, lookAt, up, fov)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransformSettings, position, rotation, scale)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(QuadLightSettings, corner, edge1, edge2)

static glm::vec3 _getVec(float c[3]) { return glm::vec3(c[0], c[1], c[2]); }

static bool _jsonExists(const nlohmann::json& js, const std::string& key) {
  return js.find(key) != js.end();
}

static void _loadTriangleMesh(MyScene::Ptr scene, std::string name,
                              const Transform& trans,
                              const nlohmann::json& jsonObj) {
  std::string asset = jsonObj.at("asset").get<std::string>();

  TriangleMesh::Ptr mesh = std::make_shared<TriangleMesh>(name, scene.get());
  mesh->createFromObj(asset, trans.getLocal2World());
  scene->attachGeometry(mesh);
}

static void _loadQuadLight(MyScene::Ptr scene, std::string name,
                           const nlohmann::json& jsonObj) {
  QuadLightSettings settings = jsonObj.get<QuadLightSettings>();

  QuadLight::Ptr lgt = std::make_shared<QuadLight>(name, scene.get());
  lgt->create(_getVec(settings.corner), _getVec(settings.edge1),
              _getVec(settings.edge2), _getVec(settings.intensity));
  scene->attachLight(lgt);
}

void MySceneLoader::loadScene(MyScene::Ptr scene,
                              const std::string& szFileName) {
  auto& assetManager = MyAssetManager::instance();

  // 1. load json document
  nlohmann::json doc;
  std::string szFullPath = MyAssetManager::instance().makeFullPath(szFileName);
  std::ifstream ifs(szFullPath, std::ifstream::in);
  ifs >> doc;

  // 2. load assets from file
  auto assets = doc.at("assets").get<std::map<std::string, std::string>>();
  for (auto elem : assets) {
    assetManager.add(elem.first, elem.second);
  }

  // 3. get camera settings
  auto cameraSettings = doc.at("camera").get<CameraSettings>();

  // 4. create scene objects
  auto jsonScene = doc.at("scene");
  for (auto iter = jsonScene.begin(); iter != jsonScene.end(); ++iter) {
    auto jsonObj = iter.value();
    std::string objName = iter.key();
    std::string objClass = jsonObj.at("class").get<std::string>();

    Transform trans;
    if (_jsonExists(jsonObj, "transform")) {
      auto settings = jsonObj.at("transform").get<TransformSettings>();
      trans.setPosition(settings.getPosition());
      trans.setRotation(settings.getRotation());
      trans.setScale(settings.getScale());
    }

    if (objClass == "TriangleMesh") {
      _loadTriangleMesh(scene, objName, trans, jsonObj);
    } else if (objClass == "QuadLight") {
      _loadQuadLight(scene, objName, jsonObj);
    } else {
      std::cerr << "UNKNOWN object class: " << objClass << std::endl;
    }
  }  // end of for
}

}  // namespace RTKit2