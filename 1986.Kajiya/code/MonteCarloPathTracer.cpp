#include "MonteCarloPathTracer.h"

#include <glm/glm.hpp>
#include <random>

#include "MaterialBase.h"
#include "framework/PinholeCamera.h"
#include "sampling/JitteringSampling.h"

namespace RayTracingHistory {

constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr uint32_t SPP_ROOT = 1;
constexpr uint32_t MAX_BOUNCES = 5;

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
  MyScene* pScene = scene.get();

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  int W = mFrameWidth;
  int H = mFrameHeight;
  int MAX_SPP = SPP_ROOT * SPP_ROOT;

  int SPP = 0;
  std::vector<glm::vec3> tileBuffer;

  int tileW = tile.right - tile.left;
  int tileH = tile.bottom - tile.top;
  tileBuffer.resize(tileW * tileH, glm::vec3(0));

  auto jit = jitteredPoints(SPP_ROOT, true);

  while (SPP < MAX_SPP) {
    int index = 0;
    float scale = 1.0f / (SPP + 1);

    for (int y = tile.top; y < tile.bottom; y++)
      for (int x = tile.left; x < tile.right; x++) {
        if (!mRuning) break;

        const glm::vec2& xi = jit[SPP];
        Ray primaryRay =
            pCamera->generateViewingRay((x + xi.x) / W, (y + xi.y) / H);

        auto& buf = tileBuffer[index++];
        buf += _traceRay(primaryRay, pScene);

        _writePixel(x, y, glm::vec4(buf / float(SPP + 1), 1), 1);
        mPixelCount++;
      }

    SPP++;
  }  // end of while
}

glm::vec3 MonteCarloPathTracer::_traceRay(const Ray& wo, MyScene* pScene) {
  const glm::vec3 bgColor(0.f, 0.f, 0.f);

  HitRecord hitRec;
  bool bHit = pScene->closestHit(wo, 0.01f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  MaterialBase* pMtl = static_cast<MaterialBase*>(hitRec.mtl);

  // error check
  if (!pMtl) return glm::abs(hitRec.normal);

  // hit light
  if (pMtl->isLight())
    return pMtl->getEmission() * pMtl->getBaseColor(hitRec.uv, hitRec.p);

  // TODO: shading
  return glm::vec3(0.25f, 0.55f, 0.85f) * glm::abs(hitRec.normal);
}

}  // namespace RayTracingHistory
