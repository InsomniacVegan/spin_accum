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
//  File: system.hpp
//
//  System data structure and initialization function.
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

#ifndef SYSTEM_HPP
#define SYSTEM_HPP


// Standard header
#include <vector>
#include <string>

// Material struct
#include "material.hpp"

namespace sys{

  class system_t {
  public:
    system_t();
    void set_param(std::string property_s, std::string value_s);
    int mat_num();
    void add_mat(int mat_id);
    void set_mat_prop(int mat_id, std::string property_s, std::string value_s);
    void enumerate_mats();
    void prop_init();
    void iface_init();
    void system_out();
  private:
    // Materials
    std::vector<mat::material> materials;

    // Vector properties
    // --------------------------------------------------
    // Magnetization
    // Spin accumulation
    // Spin current
    std::vector<std::vector<double> > mag;
    std::vector<std::vector<double> > j_m;
    std::vector<std::vector<double> > sa;

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
    std::vector<std::vector<double> >  scal_prop;

    // Direct system parameters
    // --------------------------------------------------
    // Integer
    // [0] Number of materials
    // [1] Interface condition
    // --------------------------------------------------
    // Double
    // [0] Space discretization
    // [1] Time discretization
    // [2] Target time
    // [3] Electrical current
    std::vector<int> params_i;
    std::vector<std::string> params_i_s;

    std::vector<double> params_d;
    std::vector<std::string> params_d_s;
  };

  extern system_t system;
}

#endif /* SYSTEM_HPP */
