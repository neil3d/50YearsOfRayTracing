/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <spdlog/spdlog.h>

#include <algorithm>
#include <mutex>

#include "../scene/MyScene.h"
#include "MyCamera.h"
#include "MyRenderer.h"

namespace RayTracingHistory {

constexpr int TILE_SIZE = 32;  // in pixel

struct Tile {
  int id;
  int left, top;
  int right, bottom;
};

/**
 * render every tile in a single thread
 */
class TiledRenderer : public MyRenderer {
 public:
  virtual void renderScene(MyScene::Ptr scene, MyCamera::Ptr camera,
                           const glm::vec4& clearColor) override {
    // restart
    _shutdown();
    MyRenderer::renderScene(scene, camera, clearColor);

    // make tiles
    mFinishedTile = 0;
    mPresentTile = 0;
    mTileCursor = 0;

    int numTileX = std::ceilf((float)mFrameWidth / TILE_SIZE);
    int numTileY = std::ceilf((float)mFrameHeight / TILE_SIZE);

    for (int y = 0; y < numTileY; y++) {
      for (int x = 0; x < numTileX; x++) {
        Tile tile;
        tile.id = x + y * numTileX;
        tile.left = x * TILE_SIZE;
        tile.top = y * TILE_SIZE;

        if (x == numTileX - 1)
          tile.right = mFrameWidth;
        else
          tile.right = tile.left + TILE_SIZE;

        if (y == numTileY - 1)
          tile.bottom = mFrameHeight;
        else
          tile.bottom = tile.top + TILE_SIZE;

        mTileList.push_back(tile);
      }  // end of for(x)
    }    // end of for(y)

    int CX = mFrameWidth / 2;
    int CY = mFrameHeight / 2;
    std::sort(std::begin(mTileList), std::end(mTileList),
              [CX, CY](const Tile& t1, const Tile& t2) {
                int dx1 = t1.left - CX;
                int dy1 = t1.top - CY;
                int dx2 = t2.left - CX;
                int dy2 = t2.top - CY;
                return dx1 * dx1 + dy1 * dy1 < dx2 * dx2 + dy2 * dy2;
              });

    // start new threads
    mRuning = true;
    unsigned int threadsCount = std::min((unsigned int)mTileList.size(),
                                         std::thread::hardware_concurrency());
    spdlog::info("rendering thread count = {0}", threadsCount);

  spdlog::info("start rendering ...");
    for (unsigned int i = 0; i < threadsCount; i++) {
      mRenderingThreads.emplace_back(std::thread(
          [this, scene, camera] { this->_tileRenderThread(scene, camera); }));
    }
  }

  virtual void _shutdown() override {
    mRuning = false;
    for (auto& thread : mRenderingThreads) {
      if (thread.joinable()) thread.join();
    }
  }

  virtual void _tileRenderThread(MyScene::Ptr scene, MyCamera::Ptr camera) {
    glm::vec4 topColor(1.0f, 1, 1, 1);
    glm::vec4 bottomColor(0.5f, 0.7f, 1.0f, 1);

    Tile tile;
    while (mRuning && _popTile(tile)) {
      for (int y = tile.top; y < tile.bottom; y++)
        for (int x = tile.left; x < tile.right; x++) {
          if (mRuning) {
            float r = (float)y / mFrameHeight;
            _writePixel(x, y, bottomColor * r + topColor * (1.0f - r), 1.0f);
          } else
            break;
        }  // end of for(x)
      _onTileFinished();
    }  // end of while
  }

  virtual bool nextPresentReady() const {
    if (mFinishedTile != mPresentTile) {
      mPresentTile = (int)mFinishedTile;
      return true;
    } else {
      return false;
    }
  }

  virtual float getProgress() const override {
    if (mTileList.size() == mFinishedTile) return 1.0f;
    int f = mFinishedTile;
    return float(f) / mTileList.size();
  }

 protected:
  bool _popTile(Tile& outTile) {
    if (mTileCursor < mTileList.size()) {
      outTile = mTileList[mTileCursor];
      mTileCursor++;
      return true;
    } else {
      return false;
    }
  }

  void _onTileFinished() {
    mFinishedTile++;
    if (mFinishedTile == mTileList.size()) _onRenderFinished();
  }

 private:
  std::vector<std::thread> mRenderingThreads;
  std::atomic<int> mFinishedTile = {0};
  mutable std::atomic<int> mPresentTile = {0};

  std::vector<Tile> mTileList;
  std::atomic<int> mTileCursor = 0;
};
}  // namespace RayTracingHistory
