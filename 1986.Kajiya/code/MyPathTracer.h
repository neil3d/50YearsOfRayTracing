#pragma once
#include <vector>

#include "framework/PinholeCamera.h"
#include "framework/TiledRenderer.h"
#include "scene/QuadLight.h"

namespace RTKit2 {
class MyPathTracer : public TiledRenderer {
 public:
  void setOptions(int spp, int maxDepth, bool RR);

 private:
  virtual std::string getInfo() const override;

  virtual void _tileRenderThread(MyScene::Ptr scene,
                                 MyCamera::Ptr camera) override;

  void _renderPixel(int x, int y, MyScene* scene, PinholeCamera* camera);

  glm::vec3 _traceRay(MyRay ray, MyScene* scene, int depth);

  glm::vec3 _directLighting(glm::vec3 wo, const HitRecord& shadingPt,
                            QuadLight* lgt, MyScene* scene);

 private:
  int mMaxDepth = 5;
  int mSamplePerPixel = 9;
  int mEnableRR = true;
  std::string mInfo;
};
}  // namespace RTKit2
