#include <cmath>
#include <vector>

namespace solvers{

  std::vector<double> rk4(std::vector<double> state, const double stepsize, std::vector<double>(*f)(std::vector<double>)) {
    std::vector<double> k_1 = f(state);
    std::vector<double> k_2 = f(state);

  }

  std::vector<double> rk4_coup(std::vector<double> state, const double stepsize, double (*f1)(std::vector<double>), double (*f2)(std::vector<double>)) {
    double k_1 = stepsize*f1(state);
    double k_2 = stepsize*f2(state);

    std::vector<double> state_inter = {state[0]+(0.5*stepsize), state[1]+(0.5*k_1), state[2]+(0.5*k_2)};

    double k_3 = stepsize*f1(state_inter);
    double k_4 = stepsize*f2(state_inter);

    state_inter = {state[0]+(0.5*stepsize), state[1]+(0.5*k_3), state[2]+(0.5*k_4)};

    double k_5 = stepsize*f1(state_inter);
    double k_6 = stepsize*f2(state_inter);

    state_inter = {state[0]+stepsize, state[1]+k_5, state[2]+k_6};

    double k_7 = stepsize*f1(state_inter);
    double k_8 = stepsize*f2(state_inter);

    state[0] += stepsize;
    state[1] += ((k_1+k_7)/6.0) + ((k_3+k_5)/3.0);
    state[2] += ((k_2+k_8)/6.0) + ((k_4+k_6)/3.0);

    return state;

  }


}

