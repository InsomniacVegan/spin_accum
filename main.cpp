// =======================================================
//  Dynamic Spin Accumulation Calculation
//
//  Author: Luke Elliott
//
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

// Standard headers
#include <iostream>

// Personal modules
#include "io.h"
#include "term.h"
#include "solvers.h"
#include "func.h"

double dy_dt(std::vector<double> state){
  return state[2];
}

double dz_dt(std::vector<double> state){
  return -1*state[1];
}

int main(int argc, char *argv[]) {

  // IO initialization
  // Check command flags are given
  // Get command flag to determine mode of operation
  io::io_header();
  io::io_check(argc);
  io::io_get_flag(argv);


  std::vector<double> state = {0.0, 0.0, 1.0};

  std::vector<double> s1 = {1.0, 2.0, 3.0};
  std::vector<double> s2 = {3.0, 2.0, 1.0};

  std::vector<double> s3 = funcs::cross(s1,s2);
  std::cout << s3[0] << ' ' << s3[1] << ' ' << s3[2] << std::endl;


                                                        /* for(int i=0; i<1000; i++) {
    state = solvers::rk4_coup(state, 0.01, &dy_dt, &dz_dt);
    std::cout << state[0] << ' ' <<state[1] << ' ' << state[2] << std::endl;
    }*/

  return 0;
}
