#ifndef PHYSICS_H
#define PHYSICS_H


#include <vector>

namespace physics{
  std::vector<std::vector<double> > spin_curr(std::vector<std::vector<double> > spin_accum_grad,
                                                 std::vector<std::vector<double> > mag,
                                                 std::vector<double> spin_polar_con,
                                                 std::vector<double> spin_polar_diff,
                                                 std::vector<double> diff,
                                                 double electric_curr);

  std::vector<std::vector<double> > dm_dt(std::vector<std::vector<double> > spin_accum,
                                          std::vector<std::vector<double> > mag,
                                          std::vector<std::vector<double> > spin_curr_grad,
                                          std::vector<double> precession_len,
                                          std::vector<double> dephasing_len,
                                          std::vector<double> spin_flip_len,
                                          std::vector<double> spin_accum_inf);
}

#endif /* PHYSICS_H */
