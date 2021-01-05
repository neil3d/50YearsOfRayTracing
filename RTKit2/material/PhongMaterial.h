#include "MyMaterial.h"

namespace RTKit2 {

class PhongMaterial : public MyMaterial {
 public:
  typedef std::shared_ptr<PhongMaterial> Ptr;

 public:
  glm::vec3 emission = {0, 0, 0};
  glm::vec3 diffuse = {1, 1, 1};
  glm::vec3 specular = {0, 0, 0};
  float shininess = 1;
};

}  // namespace RTKit2
