#ifndef TERM_H
#define TERM_H

#include <string>

namespace term {
  extern const std::string reset;
  extern const std::string bold;
  extern const std::string italic;
  extern const std::string under;
  extern const std::string invert;
  extern const std::string strike;
  extern const std::string bold_off;
  extern const std::string italic_off;
  extern const std::string under_off;
  extern const std::string invert_off;
  extern const std::string strike_off;

  // Foreground colors
  extern const std::string fg_black;
  extern const std::string fg_red;
  extern const std::string fg_green;
  extern const std::string fg_yellow;
  extern const std::string fg_blue;
  extern const std::string fg_magenta;
  extern const std::string fg_cyan;
  extern const std::string fg_white;
  extern const std::string fg_default;

  // Background colors
  extern const std::string bg_black;
  extern const std::string bg_red;
  extern const std::string bg_green;
  extern const std::string bg_yellow;
  extern const std::string bg_blue;
  extern const std::string bg_magenta;
  extern const std::string bg_cyan;
  extern const std::string bg_white;
  extern const std::string bg_default;

}

#endif /*TERM_H*/
