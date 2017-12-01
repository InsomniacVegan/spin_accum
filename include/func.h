#ifndef FUNC_H
#define FUNC_H


#include <vector>

namespace func {
  std::vector<double> cross(std::vector<double> v1, std::vector<double> v2);
  double dot(std::vector<double> v1, std::vector<double> v2);
  std::vector<std::vector<double> > gradient(std::vector<std::vector<double> > vec, double stepsize);
  double dy_dt(std::vector<double> state);
  double dz_dt(std::vector<double> state);
}

#endif /* FUNC_H */
