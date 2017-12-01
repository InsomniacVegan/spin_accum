#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

namespace mat {
  struct material {
    int id;
    double upper_bound;
    double lower_bound;
    std::vector<double> mag;
    double spin_accum_inf;
    double spin_polar_con;
    double spin_polar_diff;
    double diff;
    double precession_len;
    double dephasing_len;
    double spin_flip_len;
    double diff_len;
  };
}

#endif /* MATERIAL_H */
