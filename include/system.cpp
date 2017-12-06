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
//  File: system.cpp
//
//  System data structure and initialization function.
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

// Standard header
#include <iostream>
#include <vector>
#include <algorithm>

// Material struct
#include "system.hpp"
#include "material.hpp"
#include "term.hpp"

namespace sys{

  // Main system class instance
  system_t system;

  // System constructor
  system_t::system_t() {
    params_s = {"dx", "dt", "T", "j_e"};
    params.resize(params_s.size());
  }


  void system_t::set_param(std::string property_s, std::string value_s) {
    auto prop_id = std::find(params_s.begin(), params_s.end(), property_s);
    if (prop_id!= params_s.end()){
      params[prop_id-params_s.begin()] = std::stod(value_s);
    }
    else std::cout << term::bold << term::fg_yellow << " Unknown system parameter: "
                   << term::reset << property_s << std::endl;
  }

  void system_t::add_mat(int mat_id) {
    materials.push_back(mat::material ());
    materials.back().id = mat_id;
    materials[mat_id].scal_prop.resize(7);
  }

  // Takes a material ID and property name and sets the value of the property
  void system_t::set_mat_prop(int mat_id, std::string property_s, std::string value_s) {
    // Check if property is a scalar property and set
    auto prop_id = std::find(mat::scal_prop_s.begin(), mat::scal_prop_s.end(), property_s);
    if (prop_id!=mat::scal_prop_s.end()){
      materials[mat_id].scal_prop[prop_id-mat::scal_prop_s.begin()] = std::stod(value_s);
    }

    // Left side of material
    else if (property_s == "lower_bound" || property_s == "lower-bound") {
      materials[mat_id].lower_bound = std::stod(value_s);
    }

    // Right side of material
    else if(property_s == "upper_bound" || property_s == "upper-bound") {
      materials[mat_id].upper_bound = std::stod(value_s);
    }

    // Get length of diffusion if used
    else if(property_s == "len_diff" || property_s == "len-diff") {
      materials[mat_id].upper_bound = std::stod(value_s);
    }

    // Parse magnetization vector
    else if(property_s == "magnetization" || property_s == "mag") {
      std::vector<double> mag(3);
      auto delim_1 = value_s.find(",");
      auto delim_2 = value_s.find(",", delim_1+1);
      mag[0] = std::stod(value_s.substr(value_s.find("[")+1, delim_1-value_s.find("[")-1));
      mag[1] = std::stod(value_s.substr(delim_1+1, delim_2-delim_1-1));
      mag[2] = std::stod(value_s.substr(delim_2+1, value_s.find("]")-delim_2-1));
      materials[mat_id].mag = mag;
    }

    else {
      std::cout << term::bold << term::fg_yellow <<  " Unknown material property: "
                << term::reset << property_s << std::endl;
    }
  }

  void system_t::enumerate_mats() {
    for(int i=0; i<materials.size(); i++) {
      std::cout << std::endl;
      std::cout << " Material ID: " << materials[i].id << std::endl;
      std::cout << " ============================" << std::endl;
      std::cout << " Vector properties" << std::endl;
      std::cout << " ----------------------------" << std::endl;
      std::cout << " Magnetization: " << materials[i].mag[0] << " " << materials[i].mag[1] << " " << materials[i].mag[2] << std::endl;
      std::cout << std::endl;
      std::cout << " Scalar properties" << std::endl;
      std::cout << " ----------------------------" << std::endl;
      for(int j=0; j<mat::scal_prop_s.size(); j++) {
        std::cout << " " << mat::scal_prop_s[j] << ": " << materials[i].scal_prop[j] << std::endl;
      }
    }
  }

  // System constructor
  void system_t::prop_init (int system_len) {
    // Vector properties
    // -------------------------
    // Magnetization
    // Spin accumulation
    // Spin current

    // Resize to system length
    mag.resize(system_len);
    j_m.resize(system_len);
    sa.resize(system_len);

    // Initialize empty
    std::fill(mag.begin(), mag.end(), std::vector<double> {0.0, 0.0, 0.0});
    std::fill(j_m.begin(), j_m.end(), std::vector<double> {0.0, 0.0, 0.0});
    std::fill(sa.begin(), sa.end(), std::vector<double>   {0.0, 0.0, 0.0});


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
    std::vector<std::vector<double> >  scal_prop((7), std::vector<double>(system_len));

    // Electrical current
    double j_e = 0.0;
  }
}
