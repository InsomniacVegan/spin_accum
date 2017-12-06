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
#include <vector>
#include <string>

#include "material.hpp"

namespace mat {
  std::vector<std::string> scal_prop_s = {"equil_spin_accum",
                                          "spin_polar_con",
                                          "spin_polar_dif",
                                          "diffusivity",
                                          "len_precess",
                                          "len_dephase",
                                          "len_spin_flip"};
}
