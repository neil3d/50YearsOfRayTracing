#pragma once
/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <tuple>

namespace RayTracingHistory {
namespace Shading {

// [Beckmann 1963, "The scattering of electromagnetic waves from rough
// surfaces"]
float D_Beckmann(float roughness, float NdotH) {
  constexpr float PI = glm::pi<float>();
  float a2 = powf(roughness, 4);
  float NoH2 = NdotH * NdotH;
  return exp((NoH2 - 1) / (a2 * NoH2)) / (PI * a2 * NoH2 * NoH2);
}

// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough
// surfaces"]
float D_GGX(float roughness, float NdotH) {
  constexpr float PI = glm::pi<float>();
  float a2 = powf(roughness, 4);
  float d = (NdotH * a2 - NdotH) * NdotH + 1;
  return a2 / (PI * d * d);
}

float G_SmithJoint(float roughness, float NdotV, float NdotL) {
  float a2 = powf(roughness, 4);
  float G1 = NdotL * std::sqrtf(NdotV * (NdotV - NdotV * a2) + a2);
  float G2 = NdotV * std::sqrtf(NdotL * (NdotL - NdotL * a2) + a2);
  return 0.5f / (G1 + G2);
}

float G_SmithV_Predivided(float roughness, float NoV, const float NoL) {
  float a2 = powf(roughness, 4);
  const float G_SmithV = NoV + sqrt((NoV - NoV * a2) * NoV + a2);
  return 0.5f / (G_SmithV * NoL);
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float F_Schlick(float VdotH) {
  float f = powf(1 - VdotH, 5);
  return f + (1 - f);
}

float F_Fresnel() {
  // TODO
  return 1;
}

// [ Heitz 2018, "Sampling the GGX Distribution of Visible Normals" ]
// http://jcgt.org/published/0007/04/01/
std::tuple<glm::vec3, float> importanceSampleVisibleGGX(glm::vec2 DiskE,
                                                        float roughness,
                                                        glm::vec3 V) {
  constexpr float PI = glm::pi<float>();
  float a = roughness;
  float a2 = powf(roughness, 4);

  // stretch
  glm::vec3 Vh = glm::normalize(glm::vec3(a * V.x, a * V.y, V.z));

  // Orthonormal basis
  // Tangent0 is orthogonal to N.
  glm::vec3 Tangent0 = (Vh.z < 0.9999)
                           ? glm::normalize(glm::cross(glm::vec3(0, 0, 1), Vh))
                           : glm::vec3(1, 0, 0);
  glm::vec3 Tangent1 = glm::cross(Vh, Tangent0);

  glm::vec2 p = DiskE;
  float s = 0.5 + 0.5 * Vh.z;
  p.y = (1 - s) * sqrt(1 - p.x * p.x) + s * p.y;

  glm::vec3 H;
  H = p.x * Tangent0;
  H += p.y * Tangent1;
  H += sqrt(glm::clamp(1 - glm::dot(p, p), 0.0f, 1.0f)) * Vh;

  // unstretch
  H = glm::normalize(glm::vec3(a * H.x, a * H.y, glm::max(0.0f, H.z)));

  float NoV = V.z;
  float NoH = H.z;
  float VoH = glm::dot(V, H);

  float d = (NoH * a2 - NoH) * NoH + 1;
  float D = a2 / (PI * d * d);

  float G_SmithV = 2 * NoV / (NoV + sqrt(NoV * (NoV - NoV * a2) + a2));

  float PDF = G_SmithV * VoH * D / NoV;

  return std::make_tuple(H, PDF);
}

}  // namespace Shading
}  // namespace RayTracingHistory
