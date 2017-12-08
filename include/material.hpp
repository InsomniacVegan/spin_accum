// =======================================================
//  Dynamic Spin Accumulation Calculation
//
//  Author: Luke Elliott
//  Email: lee506@york.ac.uk
//
//  Test code for simulating the spin accumulation across
//  a given magnetic system, assumed to be a spin valve/tunnel ju-
//  nction.
//
//  File: material.hpp
//
//  Material data structure and initialization function.
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <vector>

namespace mat {
  struct material {
    double lower_bound;
    double upper_bound;
    std::vector<double> mag;
    double len_diff;

    // Scalar properties
    // Properties indexing
    // --------------------------------------------------
    // [0] Equilibrium spin accumulation          (m_inf)
    // [1] Spin polarization of the conductivity  (B)
    // [2] Spin polarization of the diffusivity   (B')
    // [3] Diffusivity                            (D_0)
    // [4] Precession length                      (L_j)
    // [5] Dephasing length                       (L_phi)
    // [6] Spin-flip length                       (L_sf)
    std::vector<double> scal_prop;

  };


  extern std::vector<std::string> scal_prop_s;
}

#endif /* MATERIAL_HPP */
