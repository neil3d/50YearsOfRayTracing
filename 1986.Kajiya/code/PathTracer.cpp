#include "PathTracer.h"

#include <glm/glm.hpp>

namespace RayTracingHistory {

std::string PathTracer::getInfo() const { return std::string(" - SPP: 000"); }

void PathTracer::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                   MyCamera::Ptr camera) {}

}  // namespace RayTracingHistory
