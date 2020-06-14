/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <array>

#include "../scene/MyScene.h"
#include "MyCamera.h"
#include "MyRenderer.h"

namespace RayTracingHistory {
constexpr int NUM_TILE_X = 4;
constexpr int NUM_TILE_Y = 4;

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
    // stop current
    _shutdown();

    // start new threads
    mRuning = true;
    int tileW = mFrameWidth / NUM_TILE_X;
    int tileH = mFrameHeight / NUM_TILE_Y;
    for (int y = 0; y < NUM_TILE_Y; y++) {
      for (int x = 0; x < NUM_TILE_X; x++) {
        Tile tile;
        tile.id = x + y * NUM_TILE_X;
        tile.left = x * tileW;
        tile.top = y * tileH;

        if (x == NUM_TILE_X - 1)
          tile.right = mFrameWidth;
        else
          tile.right = tile.left + tileW;

        if (y == NUM_TILE_Y - 1)
          tile.bottom = mFrameHeight;
        else
          tile.bottom = tile.top + tileH;

        int index = y * NUM_TILE_X + x;
        mRenderingThreads[index] = std::thread([this, tile, scene, camera] {
          this->_tileRenderThread(tile, scene, camera);
        });
      }  // end of for(y)
    }    // end of for(y)
  }

  virtual void _shutdown() override {
    mRuning = false;
    for (auto& thread : mRenderingThreads) {
      if (thread.joinable()) thread.join();
    }
  }

  virtual void _tileRenderThread(Tile tile, MyScene::Ptr scene,
                                 MyCamera::Ptr camera) {
    glm::vec4 topColor(1.0f, 1, 1, 1);
    glm::vec4 bottomColor(0.5f, 0.7f, 1.0f, 1);

    for (int y = tile.top; y < tile.bottom; y++)
      for (int x = tile.left; x < tile.right; x++) {
        if (mRuning) {
          float r = (float)y / mFrameHeight;
          _writePixel(x, y, bottomColor * r + topColor * (1.0f - r), 1.0f);
        } else
          break;
      }  // end of for(x)
  }

 protected:
 private:
  std::array<std::thread, NUM_TILE_X * NUM_TILE_Y> mRenderingThreads;
};  // namespace RayTracingHistory
}  // namespace RayTracingHistory
