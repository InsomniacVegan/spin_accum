// =======================================================
//  Dynamic Spin Accumulation Calculation
//
//  Author: Luke Elliott
//
//  Test code for simulating the spin accumulation across
//  a given 1D system, assumed to be a spin valve/tunnel
//  junction.
//
//  File: func.cpp <CODE>
//
//  Mathematical functions:
//  - Cross product
//  - Dot product
//  - Gradient
//  - SHM
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

#include <vector>

namespace func {
  // Perform cross product of two vectors v1 & v2
  std::vector<double> cross(std::vector<double> v1, std::vector<double> v2) {
    std::vector<double> v3 = {((v1[1]*v2[2])-(v1[2]*v2[1])),((v1[2]*v2[0])-(v1[0]*v2[2])), ((v1[0]*v2[1])-(v1[1]*v2[0]))};
    return v3;
  }

  // Perform dot product of two vectors v1 & v2
  double dot(std::vector<double> v1, std::vector<double> v2) {
    return (v1[0]*v2[0])+(v1[1]*v2[1])+(v1[2]*v2[2]);
  }

  // Calculate the gradient of a 1D array of vectors
  std::vector<std::vector<double> > gradient(std::vector<std::vector<double> > vec, double stepsize) {
    // Empty gradient array
    std::vector<std::vector<double> > grad(vec.size(), std::vector<double>(vec[0].size()));
    std::fill(grad.begin(), grad.end(), std::vector<double> {0.0, 0.0, 0.0});
    // Calculate gradient at end points first
    for(int i=0; i<grad[0].size(); i++){
      grad[0][i] = (vec[1][i]-vec[0][i])/(stepsize);
      grad[grad.size()-1][i] = (vec[vec.size()-1][i]-vec[vec.size()-2][i])/(stepsize);
    }

    for(int i=1; i<vec.size()-1; i++){
      for(int j=0; j<vec[i].size(); j++){
        grad[i][j] = (vec[i+1][j]-vec[i-1][j])/(2*stepsize);
      }

    }

    return grad;
  }

  // Test functions for integration
  // Performs Simple Harmonic Motion (sin/cos solutions)
  double dy_dt(std::vector<double> state){
    return state[2];
  }

  double dz_dt(std::vector<double> state){
    return -1*state[1];
  }

}
