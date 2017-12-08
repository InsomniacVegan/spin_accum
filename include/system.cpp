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
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

// Material struct
#include "system.hpp"
#include "material.hpp"
#include "term.hpp"

namespace sys{

  // Main system class instance
  system_t system;

  // System constructor
  system_t::system_t() {
    params_s = {"dx", "dt", "T", "j_e", "mat_num"};
    params.resize(params_s.size());
  }


  void system_t::set_param(std::string property_s, std::string value_s) {
    auto prop_id = std::find(params_s.begin(), params_s.end(), property_s);
    if (prop_id != params_s.end()){
      params[prop_id-params_s.begin()] = std::stod(value_s);
      if (property_s == "mat_num") {
        materials.resize(std::stod(value_s));
        for (int i=0; i<materials.size(); i++) materials[i].scal_prop.resize(mat::scal_prop_s.size());
      }
    }
    else std::cout << term::bold << term::fg_yellow << " Unknown system parameter: "
                   << term::reset << property_s << std::endl;
  }

  int system_t::mat_num() {
    return materials.size();
  }

  // void system_t::add_mat(int mat_id) {
  //   materials.push_back(mat::material ());
  //   materials.back().id = mat_id;
  //   materials[mat_id].scal_prop.resize(7);
  // }

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
      materials[mat_id].len_diff = std::stod(value_s);
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
      std::cout << " Material ID: " << i << std::endl;
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
  void system_t::prop_init () {

    // Calculate system length
    // Assume materials go left to right to begin
    double min_x = materials.front().lower_bound;
    double max_x = materials.back().upper_bound;

    for (int i=0; i<materials.size(); i++) {
      if (materials[i].lower_bound<min_x) min_x = materials[i].lower_bound;
      if (materials[i].upper_bound>max_x) max_x = materials[i].upper_bound;
    }

    int system_len = ceil((max_x-min_x)/params[0]);

    // Empty out all values

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
    scal_prop.resize((mat::scal_prop_s.size()), std::vector<double>(system_len));
    for (int i=0; i<scal_prop.size(); i++){
      std::fill(scal_prop[i].begin(), scal_prop[i].end(), 0.0);
    }

    // Electrical current
    double j_e = 0.0;

    // Intialise properties from materials
    for (int i=0; i<materials.size(); i++) {
      // Magnetization & diffusion
      std::fill(mag.begin()+floor(materials[i].lower_bound/params[0]), mag.begin()+ceil(materials[i].upper_bound/params[0]), materials[i].mag);
      for (int j=0; j<materials[i].scal_prop.size(); j++) {
        std::fill(scal_prop[j].begin()+floor(materials[i].lower_bound/params[0]), scal_prop[j].begin()+ceil(materials[i].upper_bound/params[0]), materials[i].scal_prop[j]);
      }
    }
  }

  void system_t::system_out(){
    std::ofstream out_file;
    out_file.open("system.dat");
    for (int i=0; i<scal_prop.front().size(); i++){
      out_file  << i << ' ';
      for (int j=0; j<scal_prop.size(); j++) {
        out_file  << scal_prop[j][i] << ' ';
      }
      out_file << std::endl;
    }
    out_file.close();
  }
}
