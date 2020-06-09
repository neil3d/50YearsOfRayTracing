#include "MonteCarloPathTracer.h"

#include <glm/glm.hpp>
#include <random>

#include "MaterialBase.h"
#include "framework/PinholeCamera.h"
#include "sampling/JitteringSampling.h"

namespace RayTracingHistory {

constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int SPP_ROOT = 10;
constexpr int MAX_BOUNCES = 5;

void MonteCarloPathTracer::_init(SDL_Window* pWnd) {
  TiledRenderer::_init(pWnd);

  int SPP = SPP_ROOT * SPP_ROOT;
  mInfo = std::string(" - SPP: ");
  mInfo.append(std::to_string(SPP));
  mInfo.append(", Bounces: ");
  mInfo.append(std::to_string(MAX_BOUNCES));
}

std::string MonteCarloPathTracer::getInfo() const { return mInfo; }

float MonteCarloPathTracer::getProgress() const {
  int SPP = SPP_ROOT * SPP_ROOT;
  int total = mFrameWidth * mFrameHeight * SPP;
  if (mPixelCount == total)
    return 1.0f;
  else
    return (float)mPixelCount / total;
}

bool MonteCarloPathTracer::isDone() const {
  int SPP = SPP_ROOT * SPP_ROOT;
  int total = mFrameWidth * mFrameHeight * SPP;
  return mPixelCount >= total;
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
  int MAX_SPP = SPP_ROOT * SPP_ROOT;

  int SPP = 0;
  std::vector<glm::vec4> tileBuffer;

  int tileW = tile.right - tile.left;
  int tileH = tile.bottom - tile.top;
  tileBuffer.resize(tileW * tileH, glm::vec4(0));

  while (SPP < MAX_SPP) {
    int index = 0;
    float scale = 1.0f / (SPP + 1);
    auto jit = jitteredPoints(SPP_ROOT, true);

    for (int y = tile.top; y < tile.bottom; y++)
      for (int x = tile.left; x < tile.right; x++) {
        if (!mRuning) break;

        glm::vec4& buf = tileBuffer[index++];

        const glm::vec2& xi = jit[SPP];
        auto result =
            _rayGeneration(pCamera, (x + xi.x) / W, (y + xi.y) / H, pScene);
        if (std::get<0>(result)) {
          buf.w += 1.0f;

          const glm::vec3& color = std::get<1>(result);
          buf.x += color.x;
          buf.y += color.y;
          buf.z += color.z;
        }

        glm::vec3 color = glm::vec3(buf.x, buf.y, buf.z) / buf.w;
        _writePixel(x, y, glm::vec4(color, 1), 1);
        mPixelCount++;
      }

    SPP++;
  }  // end of while
}

std::tuple<bool, glm::vec3> MonteCarloPathTracer::_rayGeneration(
    PinholeCamera* pCamera, float pixelX, float pixelY, MyScene* pScene) {
  const glm::vec3 bgColor(0.f, 0.f, 0.f);

  Ray primaryRay = pCamera->generateViewingRay(pixelX, pixelY);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(primaryRay, 0.01f, FLOAT_MAX, hitRec);
  if (!bHit) return std::make_tuple(false, bgColor);

  glm::vec3 color;
  MaterialBase* pMtl = static_cast<MaterialBase*>(hitRec.mtl);

  // error check
  if (!pMtl) return std::make_tuple(true, glm::abs(hitRec.normal));

  if (pMtl->isLight())
    color = pMtl->getEmission() * pMtl->getBaseColor(hitRec.uv, hitRec.p);
  else
    color = _shade(Ray(hitRec.p, -primaryRay.direction), hitRec, pScene, 0);
  return std::make_tuple(true, color);
}

glm::vec3 MonteCarloPathTracer::_shade(const Ray& wo,
                                       const HitRecord& shadingPoint,
                                       MyScene* pScene, int depth) {
  const glm::vec3 bgColor(0);
  if (depth > MAX_BOUNCES) return bgColor;

  MaterialBase* pMtl = static_cast<MaterialBase*>(shadingPoint.mtl);

  // error check
  if (!pMtl) return glm::vec3(1, 0, 0);
  glm::vec3 baseColor = pMtl->getBaseColor(shadingPoint.uv, shadingPoint.p);

  // Monte Carlo Estimating
  glm::vec3 wi = pMtl->scatter(shadingPoint.normal);
  float pdf = pMtl->pdf(wi, shadingPoint.normal);
  float cosine = std::max(0.0f, glm::dot(wi, shadingPoint.normal));
  float fr = pMtl->evaluate(wo.direction, wi, shadingPoint.normal);

  if (pdf == 0.0f || fr <= 0.0f) return bgColor;

  Ray ray(shadingPoint.p, wi);
  HitRecord hitRec;
  if (!pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec)) return glm::vec3(0);

  MaterialBase* pHitMtl = static_cast<MaterialBase*>(hitRec.mtl);
  if (!pHitMtl) return glm::vec3(1, 0, 0);

  float d = glm::distance(shadingPoint.p, hitRec.p) / 50;
  float falloff = 1.0f;
  if (d > 1) falloff = 1.0f / (d * d);

  glm::vec3 color(0);
  if (pHitMtl->isLight()) {
    // hit a light
    glm::vec3 lightColor = pHitMtl->getBaseColor(hitRec.uv, hitRec.p);
    glm::vec3 CC = pHitMtl->getEmission() * cosine * lightColor / pdf;
    color = falloff * CC * baseColor;
  } else {
    glm::vec3 CC = _shade(Ray(hitRec.p, -wi), hitRec, pScene, depth + 1);
    glm::vec3 Le = pHitMtl->getEmission() * baseColor;
    color = Le + baseColor * CC * fr * cosine / pdf * falloff;
  }

  return color;
}

}  // namespace RayTracingHistory
