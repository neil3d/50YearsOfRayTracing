#include "MonteCarloPathTracer.h"

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <random>

#include "MaterialBase.h"
#include "framework/PinholeCamera.h"
#include "sampling/JitteringSampling.h"

namespace RayTracingHistory {

constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr uint32_t SPP_ROOT = 3;
constexpr uint32_t MAX_BOUNCES = 1;

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
  auto SPP = SPP_ROOT * SPP_ROOT;
  auto total = mFrameWidth * mFrameHeight * SPP;
  if (mPixelCount == total)
    return 1.0f;
  else
    return (float)mPixelCount / total;
}

bool MonteCarloPathTracer::isDone() const {
  return mPresentLine == mFrameHeight && getProgress() == 1.0f;
}

void MonteCarloPathTracer::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                             MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MySceneWithLight* pScene = dynamic_cast<MySceneWithLight*>(scene.get());

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  float W = mFrameWidth;
  float H = mFrameHeight;
  int MAX_SPP = SPP_ROOT * SPP_ROOT;

  int SPP = 0;
  std::vector<glm::vec3> tileBuffer;

  int tileW = tile.right - tile.left;
  int tileH = tile.bottom - tile.top;
  tileBuffer.resize(tileW * tileH, glm::vec3(0));

  auto xi1 = jitteredPoints(SPP_ROOT, false);
  auto xi2 = jitteredPoints(SPP_ROOT, true);

  while (SPP < MAX_SPP) {
    int index = 0;
    float scale = 1.0f / (SPP + 1);

    for (int y = tile.top; y < tile.bottom; y++)
      for (int x = tile.left; x < tile.right; x++) {
        if (!mRuning) break;

        const glm::vec2& xi = xi1[SPP];
        Ray primaryRay =
            pCamera->generateViewingRay((x + xi.x) / W, (y + xi.y) / H);

        auto& buf = tileBuffer[index++];
        buf += _traceRay(primaryRay, pScene, xi2[SPP]);

        _writePixel(x, y, glm::vec4(buf / float(SPP + 1), 1), 1);
        mPixelCount++;
      }

    SPP++;
  }  // end of while
}

glm::vec3 MonteCarloPathTracer::_traceRay(const Ray& wo,
                                          MySceneWithLight* pScene,
                                          const glm::vec2& xi) {
  const glm::vec3 bgColor(0.f, 0.f, 0.f);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(wo, 0.001f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  MaterialBase* pMtl = static_cast<MaterialBase*>(hitRec.mtl);

  // error check
  if (!pMtl) return glm::abs(hitRec.normal);

  const AreaLight* pLight = pScene->getMainLight();

  // visibility between the shading point and the light
  float visibilityTerm = 1.0f;
  constexpr float SHADOW_E = 0.002f;

  auto shadowRet = pLight->generateShadowRay(hitRec.p, hitRec.normal, xi);
  Ray shadowRay = std::get<0>(shadowRet);
  float lightDistance = std::get<1>(shadowRet);

  auto stopWithAnyHit = [](const HitRecord&) { return true; };
  bool bShadow =
      pScene->anyHit(shadowRay, SHADOW_E, lightDistance, stopWithAnyHit);
  if (bShadow) visibilityTerm = 0.125f;

  glm::vec3 color = pMtl->getBaseColor(hitRec.uv, hitRec.p);

  // TODO: shading
  return glm::vec3(visibilityTerm) * color;
}

}  // namespace RayTracingHistory
