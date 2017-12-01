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
#include <cmath>
#include <fstream>
#include <string>

// Personal modules
#include "io.h"
#include "term.h"
#include "func.h"
#include "material.h"
#include "physics.h"


int main(int argc, char *argv[]) {

  // IO initialization
  // Check command flags are given
  // Get command flag to determine mode of operation
  //io::io_init(argc, argv); // Totally pointless atm but call with -f or -t to stop an exit

  std::vector<mat::material> material(5);

  // Material setup

  // Non-magnet (left-side)
  material[0].id = 0;
  material[0].lower_bound=0.0;
  material[0].upper_bound=45.0e-9;
  material[0].mag = {0.0, 0.0, 0.0};
  material[0].spin_accum_inf = 0.0;
  material[0].spin_polar_con = 0.0;
  material[0].spin_polar_diff = 0.0;
  material[0].diff = 0.005;
  material[0].precession_len = 1.0e-20;
  material[0].dephasing_len = 1.0e-20;
  material[0].spin_flip_len = 2.0e-08;
  material[0].diff_len = 0.0;

  // FM (left-side)
  material[1].id = 1;
  material[1].lower_bound=45.01e-9;
  material[1].upper_bound=75.0e-9;
  material[1].mag = {0.0, 0.0, 1.0};
  material[1].spin_accum_inf = 4.0e7;
  material[1].spin_polar_con = 0.5;
  material[1].spin_polar_diff = 0.7;
  material[1].diff = 0.003;
  material[1].precession_len = 4.0e-9;
  material[1].dephasing_len = 4.0e-9;
  material[1].spin_flip_len = 5.0e-9;
  material[1].diff_len = 9.0e-9;

  // Non-magnet (centre)
  material[2].id = 2;
  material[2].lower_bound=75.01e-9;
  material[2].upper_bound=78.0e-9;
  material[2].mag = {0.0, 0.0, 0.0};
  material[2].spin_accum_inf = 0.0;
  material[2].spin_polar_con = 0.0;
  material[2].spin_polar_diff = 0.0;
  material[2].diff = 0.005;
  material[2].precession_len = 1.0e-20;
  material[2].dephasing_len = 1.0e-20;
  material[2].spin_flip_len = 2.0e-08;
  material[2].diff_len = 0.0;

  // FM (right-side)
  material[3].id = 3;
  material[3].lower_bound=78.01e-9;
  material[3].upper_bound=93.0e-9;
  material[3].mag = {0.0, 0.0, 0.0};
  material[3].spin_accum_inf = 4.0e7;
  material[3].spin_polar_con = 0.5;
  material[3].spin_polar_diff = 0.7;
  material[3].diff = 0.003;
  material[3].precession_len = 4.0e-9;
  material[3].dephasing_len = 4.0e-9;
  material[3].spin_flip_len = 5.0e-09;
  material[3].diff_len = 5.0e-09;

  // Non-magnet (right)
  material[4].id = 4;
  material[4].lower_bound=93.01e-9;
  material[4].upper_bound=150.0e-9;
  material[4].mag = {0.0, 0.0, 0.0};
  material[4].spin_accum_inf = 0.0;
  material[4].spin_polar_con = 0.0;
  material[4].spin_polar_diff = 0.0;
  material[4].diff = 0.005;
  material[4].precession_len = 1.0e-20;
  material[4].dephasing_len = 1.0e-20;
  material[4].spin_flip_len = 2.0e-08;
  material[4].diff_len = 0.0;

  // Simulation parameters
  double dx = 1.0e-11;
  double dt = 6.0e-21;
  double T = 600.0e-21;
  double out_time = 1.0e-17;
  int system_len = ceil(material[4].upper_bound/dx);

  double electric_curr = 1e15;

  // Overall system properties
  std::vector<std::vector <double> > system_mag(system_len, std::vector<double>(3));
  std::vector<double> system_spin_accum_inf(system_len);
  std::vector<double> system_spin_polar_con(system_len);
  std::vector<double> system_spin_polar_diff(system_len);
  std::vector<double> system_diff(system_len);
  std::vector<double> system_precession_len(system_len);
  std::vector<double> system_dephasing_len(system_len);
  std::vector<double> system_spin_flip_len(system_len);


  // Spin vectors
  std::vector<std::vector <double> > spin_accum(system_len, std::vector<double>(3));
  std::vector<std::vector <double> > spin_curr(system_len, std::vector<double>(3));

  // Empty arrays
  std::fill(spin_accum.begin(), spin_accum.end(), std::vector<double> {0.0, 0.0, 0.0});
  std::fill(spin_curr.begin(), spin_curr.end(), std::vector<double> {0.0, 0.0, 0.0});

  // Initialise system properties for materials
  for(int i=0; i<material.size(); i++) {
    std::cout << i << ' ' << floor(material[i].lower_bound/dx) << ' ' << ceil(material[i].upper_bound/dx)+1 << std::endl;

    // Magnetization vector
    std::fill(system_mag.begin()+floor(material[i].lower_bound/dx),
              system_mag.begin()+ceil(material[i].upper_bound/dx)+1,
              material[i].mag);

    // Equilibrium spin accumulation
    std::fill(system_spin_accum_inf.begin()+floor(material[i].lower_bound/dx),
              system_spin_accum_inf.begin()+ceil(material[i].upper_bound/dx)+1,
              material[i].spin_accum_inf);

    // Spin polarisation for the conductivity (B)
    std::fill(system_spin_polar_con.begin()+floor(material[i].lower_bound/dx),
              system_spin_polar_con.begin()+ceil(material[i].upper_bound/dx)+1,
              material[i].spin_polar_con);

    // Spin polarisation for the diffusion (B')
    std::fill(system_spin_polar_diff.begin()+floor(material[i].lower_bound/dx),
              system_spin_polar_diff.begin()+ceil(material[i].upper_bound/dx)+1,
              material[i].spin_polar_diff);

    // Diffusion constant (D0)
    std::fill(system_diff.begin()+floor(material[i].lower_bound/dx),
              system_diff.begin()+ceil(material[i].upper_bound/dx)+1,
              material[i].diff);

    // Spin precession length (L_j)
    std::fill(system_precession_len.begin()+floor(material[i].lower_bound/dx),
              system_precession_len.begin()+ceil(material[i].upper_bound/dx)+1,
              material[i].precession_len);

    // Spin dephasing length (L_phi)
    std::fill(system_dephasing_len.begin()+floor(material[i].lower_bound/dx),
              system_dephasing_len.begin()+ceil(material[i].upper_bound/dx)+1,
              material[i].dephasing_len);

    // Spin flip length (L_sf)
    std::fill(system_spin_flip_len.begin()+floor(material[i].lower_bound/dx),
              system_spin_flip_len.begin()+ceil(material[i].upper_bound/dx)+1,
              material[i].spin_flip_len);
  }


  // // Fragile implementation, careful with this!
  // for(int i=0; i<material.size(); i++) {
  //   for(int j=0; j<ceil(material[i].diff_len/dx); j++) {
  //     double dj = (j+1)/ceil(material[i].diff_len/dx);

  //     // for(int k=0; k<material[i].mag.size(); k++){
  //     //   system_mag[floor(material[i].lower_bound/dx)+j][k] *= dj;
  //     //   system_mag[ceil(material[i].upper_bound/dx)+1-j][k] *= dj;
  //     // }

  //     // Equilibrium spin accumulation
  //     system_spin_accum_inf[floor(material[i].lower_bound/dx)+j] *= dj;
  //     system_spin_accum_inf[ceil(material[i].upper_bound/dx)+1-j] *= dj;

  //     // Spin polarisation for the conductivity (B)
  //     system_spin_polar_con[floor(material[i].lower_bound/dx)+j] *= dj;
  //     system_spin_polar_con[ceil(material[i].upper_bound/dx)+1-j] *= dj;

  //     // Spin polarisation for the diffusion (B')
  //     system_spin_polar_diff[floor(material[i].lower_bound/dx)+j] *= dj;
  //     system_spin_polar_diff[ceil(material[i].upper_bound/dx)+1-j] *= dj;

  //     // Diffusion constant (D0)
  //     system_diff[floor(material[i].lower_bound/dx)+j] *= dj;
  //     system_diff[ceil(material[i].upper_bound/dx)+1-j] *= dj;

  //     // Spin precession length (L_j)
  //     system_precession_len[floor(material[i].lower_bound/dx)+j] *= dj;
  //     system_precession_len[ceil(material[i].upper_bound/dx)+1-j] *= dj;

  //     // Spin dephasing length (L_phi)
  //     system_dephasing_len[floor(material[i].lower_bound/dx)+j] *= dj;
  //     system_dephasing_len[ceil(material[i].upper_bound/dx)+1-j] *= dj;

  //     // Spin flip length (L_sf)
  //     system_spin_flip_len[floor(material[i].lower_bound/dx)+j] *= dj;
  //     system_spin_flip_len[ceil(material[i].upper_bound/dx)+1-j] *= dj;

  //   }
  // }

  // Output system
  std::ofstream system_file;
  system_file.open("system.dat");
  for(int i=0; i<system_len; i++) {
    system_file << i*dx << ' ' << system_spin_accum_inf[i] << ' ' << system_spin_polar_con[i] << ' ' << system_diff[i] << ' ' << system_precession_len[i] << ' ' << system_dephasing_len[i] << ' ' << system_spin_flip_len[i] << std::endl;
  }

  // Main simulation loop
  for(int i=0; i<ceil(T/dt); i++) {
    // Calculate spin current across the system
    std::vector<std::vector<double> > spin_accum_grad = func::gradient(spin_accum, dx);

    //Output spin_accum_grad
    std::ofstream spin_accum_grad_file;
    spin_accum_grad_file.open("spin_accum_grad.dat");
    for(int j=0; j<spin_accum_grad.size(); j++){
      spin_accum_grad_file << j*dx << ' ' << spin_accum_grad[j][0] << ' ' << spin_accum_grad[j][1] << ' ' << spin_accum_grad[j][2] << std::endl;
    }

    spin_curr = physics::spin_curr(spin_accum_grad,
                                   system_mag,
                                   system_spin_polar_con,
                                   system_spin_polar_diff,
                                   system_diff,
                                   electric_curr);

    //Output spin_curr
    std::ofstream spin_curr_file;
    spin_curr_file.open("spin_curr.dat");
    for(int j=0; j<spin_curr.size(); j++){
      spin_curr_file << j*dx << ' ' << spin_curr[j][0] << ' ' << spin_curr[j][1] << ' ' << spin_curr[j][2] << std::endl;
    }

    // Evolve one time step
    std::vector<std::vector<double> > spin_curr_grad = func::gradient(spin_curr, dx);

    //Output spin_curr_grad
    std::ofstream spin_curr_grad_file;
    spin_curr_grad_file.open("spin_curr_grad.dat");
    for(int j=0; j<spin_curr_grad.size(); j++){
      spin_curr_grad_file << j*dx << ' ' << spin_curr_grad[j][0] << ' ' << spin_curr_grad[j][1] << ' ' << spin_curr_grad[j][2] << std::endl;
    }

    std::vector<std::vector<double> > dm_dt = physics::dm_dt(spin_accum,
                                                               system_mag,
                                                               spin_curr_grad,
                                                               system_precession_len,
                                                               system_dephasing_len,
                                                               system_spin_flip_len,
                                                               system_spin_accum_inf);

    //Output spin_curr_grad
    std::ofstream dm_dt_file;
    dm_dt_file.open("dm_dt.dat");
    for(int j=0; j<dm_dt.size(); j++){
      dm_dt_file << j*dx << ' ' << dm_dt[j][0] << ' ' << dm_dt[j][1] << ' ' << dm_dt[j][2] << std::endl;
    }

      // Euler integration
      #pragma omp parallel for
      for(int k=0; k<dm_dt.size(); k++) {
        for(int l=0; l<dm_dt[k].size(); l++) {
          dm_dt[k][l] *= dt;
          spin_accum[k][l] += dm_dt[k][l];
        }
      }

    // Output data here
    std::string filename = std::to_string(i)+ ".dat";
    std::ofstream myfile;
    myfile.open(filename);
    for(int k=0; k<spin_accum.size(); k++) {
      myfile << k*dx << ' ';
      for(int l=0; l<spin_accum[k].size(); l++) {
        myfile << spin_accum[k][l] << ' ';
      }
      myfile << std::endl;
    }
    myfile.close();
  }

return 0;
}
