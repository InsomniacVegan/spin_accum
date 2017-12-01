#include <vector>
#include <cmath>
#include <omp.h>
#include <iostream>

#include "func.h"

namespace physics{
  // Calculate the spin current across the system
  std::vector<std::vector<double> > spin_curr(std::vector<std::vector<double> > spin_accum_grad,
                                                 std::vector<std::vector<double> > mag,
                                                 std::vector<double> spin_polar_con,
                                                 std::vector<double> spin_polar_diff,
                                                 std::vector<double> diff,
                                                 double electric_curr) {

    std::vector<std::vector<double> > j_m(spin_accum_grad.size(), std::vector<double>(3));

    for(int i=0; i<spin_accum_grad.size(); i++) {
      for(int j=0; j<spin_accum_grad[i].size(); j++) {
        j_m[i][j] = spin_polar_con[i]*electric_curr*mag[i][j] - ((2.0*diff[i])*(spin_accum_grad[i][j]-(spin_polar_con[i]*spin_polar_diff[i]*mag[i][j]*(func::dot(mag[i],spin_accum_grad[i])))));
      }
    }

    return j_m;
  }

  // Equation of motion for spin accumulation
  std::vector<std::vector<double> > dm_dt(std::vector<std::vector<double> > spin_accum,
                                          std::vector<std::vector<double> > mag,
                                          std::vector<std::vector<double> > spin_curr_grad,
                                          std::vector<double> precession_len,
                                          std::vector<double> dephasing_len,
                                          std::vector<double> spin_flip_len,
                                          std::vector<double> spin_accum_inf) {

    std::vector<std::vector<double> > dm_dt(spin_accum.size(), std::vector<double>(3));



    for(int i=0; i<dm_dt.size(); i++) {
      // Numerators
      std::vector<double> spin_mag_cross = func::cross(spin_accum[i], mag[i]);
      std::vector<double> mag_spin_mag_cross = func::cross(mag[i], spin_mag_cross);
      std::vector<double> spin_accum_spin_inf = {spin_accum[i][0]-spin_accum_inf[i], spin_accum[i][1]-spin_accum_inf[i], spin_accum[i][2]-spin_accum_inf[i]};

      // Terms
      std::vector<double> term1 = {-1.0*spin_curr_grad[i][0], -1.0*spin_curr_grad[1][0], -1.0*spin_curr_grad[i][2]};

      std::vector<double> term2 = {(-1.0*spin_mag_cross[0])/(pow(pow(precession_len[i],2),2)), (-1.0*spin_mag_cross[1])/(pow(precession_len[i],2)),(-1.0*spin_mag_cross[2])/(pow(precession_len[i],2))};

      std::vector<double> term3 = {(-1.0*mag_spin_mag_cross[0])/(pow(dephasing_len[i],2)), (-1.0*mag_spin_mag_cross[1])/(pow(dephasing_len[i],2)), (-1.0*mag_spin_mag_cross[2])/(pow(dephasing_len[i],2))};

      std::vector<double> term4 = {(-1.0*spin_accum_spin_inf[0])/(pow(spin_flip_len[i],2)), (-1.0*spin_accum_spin_inf[1])/(pow(spin_flip_len[i],2)), (-1.0*spin_accum_spin_inf[2])/(pow(spin_flip_len[i],2))};

      // // Numerator out
      // std::cout << i << ' ' << spin_mag_cross[0] << ' ' << spin_mag_cross[1] << ' ' << spin_mag_cross[2] << std::endl;
      // std::cout << i << ' ' << mag_spin_mag_cross[0] << ' ' << mag_spin_mag_cross[1] << ' ' << mag_spin_mag_cross[2] << std::endl;
      // std::cout << i << ' ' << spin_accum_spin_inf[0] << ' ' << spin_accum_spin_inf[1] << ' ' << spin_accum_spin_inf[2] << std::endl;

      // // Denom out
      // std::cout << i << ' ' << precession_len[i] << ' ' << pow(precession_len[i],2) << std::endl;
      // std::cout << i << ' ' << dephasing_len[i] << ' ' << pow(dephasing_len[i], 2) << std::endl;
      // std::cout << i << ' ' << spin_flip_len[i] << ' ' << pow(spin_flip_len[i], 2) << std::endl;

      // // Term out
      // std::cout << i << ' ' << term1[0] << ' ' << term1[1] << ' ' << term1[2] << std::endl;
      // std::cout << i << ' ' << term2[0] << ' ' << term2[1] << ' ' << term2[2] << std::endl;
      // std::cout << i << ' ' << term3[0] << ' ' << term3[1] << ' ' << term3[2] << std::endl;
      // std::cout << i << ' ' << term4[0] << ' ' << term4[1] << ' ' << term4[2] << std::endl;
      // std::cout << std::endl;

      dm_dt[i] = {(term1[0]+term2[0]+term3[0]+term4[0]), (term1[1]+term2[1]+term3[1]+term4[1]), (term1[2]+term2[2]+term3[2]+term4[2])};
    }

    return dm_dt;

  }


}


