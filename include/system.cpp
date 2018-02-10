// =======================================================
//  Dynamic Spin Accumulation Calculation
//
//  Author: Luke Elliott
//  Email: lee506@york.ac.uk
//
//  Test code for simulating the spin accumulation across
//  a given 1Dmagnetic system, assumed to be a spin valve/tunnel
//  junction.
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

// Own headers
#include "system.hpp"
#include "material.hpp"
#include "term.hpp"
#include "physics.hpp"

namespace sys{

  // Main system class instance
  system_t system;

  // System constructor
  system_t::system_t() {
    // DOUBLE system parameters, add string flag to track a new parameter
    params_d_s = {"dx", "dt", "T", "j_e", "t_ramp"};
    params_d.resize(params_d_s.size());

    // INTEGER system parameters, add string flag to track a new parameter
    params_i_s = {"mat_num", "iface", "t_fout"};
    params_i.resize(params_i_s.size());
  }

  // Takes a parameter name and value as strings and sets the value
  void system_t::set_param(std::string property_s, std::string value_s) {
    // Attempt to find location of property as string in DOUBLE and set value
    auto prop_id = std::find(params_d_s.begin(), params_d_s.end(), property_s);
    if (prop_id != params_d_s.end()){
      params_d[prop_id-params_d_s.begin()] = std::stod(value_s);
    }

    // Otherwise attempt to find location of property as string in INTEGER and set value
    else {
      prop_id = std::find(params_i_s.begin(), params_i_s.end(), property_s);
      // Check property string exists
      if (prop_id != params_i_s.end()){
        params_i[prop_id-params_i_s.begin()] = std::stoi(value_s);

        // Special handling for number of materials
        if (property_s == "mat_num") {
          materials.resize(std::stoi(value_s));
          for (int i=0; i<materials.size(); i++) materials[i].scal_prop.resize(mat::scal_prop_s.size());
        }
      }

      // Report if unknown property
      else std::cout << term::bold << term::fg_yellow << " Unknown system parameter: "
                     << term::reset << property_s << std::endl;
    }

  }

  // Sanitized number of materials
  int system_t::mat_num() {
    return materials.size();
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

  // Write all materials and properties to screen
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

  // Initialize system properties by resizing for system length, zeroing and then filling
  void system_t::prop_init () {

    // Calculate system length
    // Assume materials go left to right
    double min_x = materials.front().lower_bound;
    double max_x = materials.back().upper_bound;

    // Find limits of system
    for (int i=0; i<materials.size(); i++) {
      if (materials[i].lower_bound<min_x) min_x = materials[i].lower_bound;
      if (materials[i].upper_bound>max_x) max_x = materials[i].upper_bound;
    }

    int system_len = ceil((max_x-min_x)/params_d[0]);

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

    // Empty property vector
    for (int i=0; i<scal_prop.size(); i++){
      std::fill(scal_prop[i].begin(),
                scal_prop[i].end(),
                0.0);
    }

    // Electrical current
    double j_e = 0.0;

    // Intialise properties from materials
    for (int i=0; i<materials.size(); i++) {
      int lower_bound = floor(materials[i].lower_bound/params_d[0]);
      int upper_bound = floor(materials[i].upper_bound/params_d[0]);

      // Magnetization
      std::fill(mag.begin()+lower_bound,
                mag.begin()+upper_bound+1,
                materials[i].mag);

      // Scalar properties fill
      for (int j=0; j<materials[i].scal_prop.size(); j++) {
        std::fill(scal_prop[j].begin()+lower_bound,
                  scal_prop[j].begin()+upper_bound+1,
                  materials[i].scal_prop[j]);
      }
    }


  }

  // Apply interface conditions to the system
  void system_t::iface_init() {
    switch(params_i[1]){

    // Atomically smooth
    case 0:
      break;

    // Linear increase
    case 1:

      // Bulk of system
      for (int i=0; i<materials.size(); i++) {

        // Skip materials without diffuse interface length
        if (materials[i].len_diff <= 0) continue;

        // Number of steps in each interface
        int iface_steps = materials[i].len_diff/params_d[0];
        int lower_bound = floor(materials[i].lower_bound/params_d[0]);
        int upper_bound = floor(materials[i].upper_bound/params_d[0]);

        // Set mag
        for (int k=0; k<iface_steps; k++) {
          // Left
          mag[lower_bound-iface_steps+k] = materials[i].mag;
          // Right
          mag[upper_bound+iface_steps-k] = materials[i].mag;
        }

        // Forward declare
        double left_step;
        double right_step;

        // Apply interface to each material in turn
        for (int j=0; j<materials[i].scal_prop.size(); j++) {

          // Do not apply interface to left/right of system
          // Left
          if (i==0) {left_step = materials[i].scal_prop[j]/iface_steps;}
          else {left_step = (materials[i].scal_prop[j]-materials[i-1].scal_prop[j])/iface_steps;}

          // Right
          if (i==materials.size()-1) {right_step = materials[i].scal_prop[j]/iface_steps;}
          else {right_step = (materials[i+1].scal_prop[j]-materials[i].scal_prop[j])/iface_steps;}

          // Aply interface condition
          for (int k=0; k<iface_steps; k++) {
            // Left
            scal_prop[j][lower_bound-iface_steps+k] += (left_step*k);
            // Right
            scal_prop[j][upper_bound+iface_steps-k] -= (right_step*k);

          }

        }
      }

      break;

    // Fick's
    case 2:

      break;
    }

    // Set spin accumulation across system to equilibrium values scaled by magnetization
    for (int i=0; i<sa.size(); i++) {
      sa[i] = {scal_prop[0][i]*mag[i][0], scal_prop[0][i]*mag[i][1], scal_prop[0][i]*mag[i][2]};
    }
  }

  // Write the system to file
  void system_t::system_out(){

    // Open file stream
    std::ofstream out_file;
    out_file.open("system.dat");

    // Step over system
    for (int i=0; i<scal_prop.front().size(); i++){

      // Position
      out_file  << i*params_d[0] << ' ';

      // Step over properties
      for (int j=0; j<scal_prop.size(); j++) {
        out_file  << scal_prop[j][i] << ' ';
      }

      // Next line
      out_file << std::endl;
    }

    out_file.close();
  }

  // Main evolution loop
  void system_t::evolve(){

    // Time loop
    for (int i=0; i<ceil(params_d[2]/params_d[1]); i++) {

      // Ramp electric current
      double j_e;
      if ((i*params_d[1])<params_d[4]) j_e = (params_d[3]*i)/(params_d[4]/params_d[1]);
      else j_e = params_d[3];

      // Output every params_i[2] timesteps
      // TODO: Make more efficient using for loops instead of branching
      if (i%params_i[2]==0){
        // Output data here

        // Name file by timestep
        std::string filename = std::to_string(i)+ ".dat";
        std::ofstream myfile;
        myfile.open(filename);

        // Space loop
        for(int k=0; k<sa.size(); k++) {
          myfile << k*params_d[0] << ' ';

          // Spin current dimension loop
          for (int l=0; l<j_m[k].size(); l++){
            myfile << j_m[k][l] << ' ';
          }

          // Spin accumulation dimension loop
          for (int l=0; l<sa[k].size(); l++) {
            myfile << sa[k][l] << ' ';
          }
          // Next line
          myfile << std::endl;
        }
        myfile.close();
      }

      // Calculate spin current across the system
      j_m = physics::spin_curr(sa, mag, scal_prop[1], scal_prop[2], scal_prop[3], j_e, params_d[0]);

      // Calculate time derivative of the spin accumulation
      std::vector<std::vector<double> > dm_dt = physics::dm_dt(sa,
                                                               mag,
                                                               j_m,
                                                               scal_prop[4],
                                                               scal_prop[5],
                                                               scal_prop[6],
                                                               scal_prop[0],
                                                               params_d[0]);

      // Space loop
      for (int k=0; k<dm_dt.size(); k++) {
        // Dimension loop
        for (int l=0; l<dm_dt[k].size(); l++){
          // Basic Euler integration
          dm_dt[k][l] *= params_d[1];
          sa[k][l] += dm_dt[k][l];
        }
      }
    }
  }
}
