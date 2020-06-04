#include "MonteCarloPathTracer.h"

#include <glm/glm.hpp>

namespace RayTracingHistory {

std::string MonteCarloPathTracer::getInfo() const { return std::string(" - SPP: 000"); }

void MonteCarloPathTracer::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                   MyCamera::Ptr camera) {}

}  // namespace RayTracingHistory
