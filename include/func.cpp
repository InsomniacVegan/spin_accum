#include <vector>

namespace funcs {
  std::vector<double> cross(std::vector<double> v1, std::vector<double> v2) {
    std::vector<double> v3 = {((v1[1]*v2[2])-(v1[2]*v2[1])),((v1[2]*v2[0])-(v1[0]*v2[2])), ((v1[0]*v2[1])-(v1[1]*v2[0]))};
    return v3;
  }
}
