#include "MonteCarloPathTracer.h"

#include <glm/glm.hpp>
#include <random>

#include "MaterialBase.h"
#include "framework/PinholeCamera.h"

namespace RayTracingHistory {

constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int SIMPLES = 66;
constexpr int MAX_DEPTH = 4;

std::string MonteCarloPathTracer::getInfo() const {
  int total = mFrameWidth * mFrameHeight;
  int spp = mPixelCount / total;
  return std::string(" - SPP: ") + std::to_string(spp);
}

float MonteCarloPathTracer::getProgress() const {
  float p = TiledRenderer::getProgress();
  return p / SIMPLES;
}

void MonteCarloPathTracer::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  int W = mFrameWidth;
  int H = mFrameHeight;

  int SPP = 0;
  std::vector<glm::vec3> tileBuffer;

  int tileW = tile.right - tile.left;
  int tileH = tile.bottom - tile.top;
  tileBuffer.resize(tileW * tileH);

  while (SPP < SIMPLES) {
    int index = 0;
    float scale = 1.0f / (SPP + 1);

    for (int y = tile.top; y < tile.bottom; y++)
      for (int x = tile.left; x < tile.right; x++) {
        if (!mRuning) break;

        glm::vec3& buf = tileBuffer[index++];

        float xi = uniformDist(stdRand);
        float yi = uniformDist(stdRand);
        buf += _rayGeneration(pCamera, (x + xi) / W, (y + yi) / H, pScene);

        glm::vec3 color = scale * buf;
        _writePixel(x, y, glm::vec4(color, 1), 0.68f);
        mPixelCount++;
      }

    SPP++;
  }  // end of while
}

glm::vec3 MonteCarloPathTracer::_rayGeneration(PinholeCamera* pCamera,
                                               float pixelX, float pixelY,
                                               MyScene* pScene) {
  const glm::vec3 bgColor(0.f, 0.f, 0.f);

  Ray primaryRay = pCamera->generateViewingRay(pixelX, pixelY);

  glm::vec3 color = bgColor;
  HitRecord hitRec;
  if (pScene->closestHit(primaryRay, 0.01f, FLOAT_MAX, hitRec)) {
    MaterialBase* pMtl = static_cast<MaterialBase*>(hitRec.mtl);
    if (pMtl->getEmission() > 0.1f)
      color = pMtl->getEmission() * pMtl->getBaseColor(hitRec.uv, hitRec.p);
    else
      color = _shade(Ray(hitRec.p, -primaryRay.direction), hitRec, pScene, 0);
  }
  return color;
}

glm::vec3 MonteCarloPathTracer::_shade(const Ray& wo,
                                       const HitRecord& shadingPoint,
                                       MyScene* pScene, int depth) {
  if (depth > MAX_DEPTH) return glm::vec3(0);

  MaterialBase* pMtl = static_cast<MaterialBase*>(shadingPoint.mtl);

  // error check
  if (!pMtl) return glm::vec3(1, 0, 0);
  glm::vec3 baseColor = pMtl->getBaseColor(shadingPoint.uv, shadingPoint.p);

  // Monte Carlo Estimating
  glm::vec3 color(0);
  float Kd = powf(0.25f, depth * depth);

  glm::vec3 wi = pMtl->scatter(shadingPoint.normal);
  float pdf = pMtl->pdf(wi, shadingPoint.normal);
  float cosine = glm::dot(wi, shadingPoint.normal);

  Ray ray(shadingPoint.p, wi);
  HitRecord hitRec;
  if (!pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec)) return glm::vec3(0);

  MaterialBase* pHitMtl = static_cast<MaterialBase*>(hitRec.mtl);
  if (pHitMtl->getEmission() > 0.1f) {
    // hit a light
    glm::vec3 lightColor = pHitMtl->getBaseColor(hitRec.uv, hitRec.p);
    glm::vec3 CC = pHitMtl->getEmission() * Kd * cosine * lightColor / pdf;
    color += CC * baseColor;
  } else {
    glm::vec3 CC = _shade(Ray(hitRec.p, -wi), hitRec, pScene, depth + 1);
    color += baseColor * CC * Kd * cosine / pdf;
  }

  return color;
}

}  // namespace RayTracingHistory
