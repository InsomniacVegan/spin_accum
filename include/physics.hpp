// =======================================================
//  Dynamic Spin Accumulation Calculation
//
//  Author: Luke Elliott
//
//  Test code for simulating the spin accumulation across
//  a given system, assumed to be a spin valve/tunnel ju-
//  nction.
//
//  File: physics.hpp <HEADER>
//
//  Implements calculation of spin current and equation
//  of motion for spin accumulation.
//
//  See:
//
//  Chureemart, P., D'Amico, I., & Chantrell, R. W. (2015). Model of spin
//  accumulation and spin torque in spatially varying magnetisation structures:
//  limitations of the micromagnetic approach. Journal of Physics: Condensed
//  Matter, 27(14), 146004. http://dx.doi.org/10.1088/0953-8984/27/14/146004
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <vector>

namespace physics{
  // Calculate the spin current across the system
  std::vector<std::vector<double> > spin_curr(std::vector<std::vector<double> > spin_accum,
                                              std::vector<std::vector<double> > mag,
                                              std::vector<double> spin_polar_con,
                                              std::vector<double> spin_polar_diff,
                                              std::vector<double> diff,
                                              double electric_curr,
                                              double stepsize);

  // Equation of motion for spin accumulation
  std::vector<std::vector<double> > dm_dt(std::vector<std::vector<double> > spin_accum,
                                          std::vector<std::vector<double> > mag,
                                          std::vector<std::vector<double> > spin_curr,
                                          std::vector<double> precession_len,
                                          std::vector<double> dephasing_len,
                                          std::vector<double> spin_flip_len,
                                          std::vector<double> spin_accum_inf,
                                          double stepsize);
}

#endif /* PHYSICS_HPP */
