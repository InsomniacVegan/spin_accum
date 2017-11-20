#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

namespace solvers{

  extern std::vector<double> rk4_coup(std::vector<double> state, const double stepsize, double (*f1)(std::vector<double>), double (*f2)(std::vector<double>));

}

#endif /* SOLVER_H */
