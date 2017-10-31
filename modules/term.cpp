// ANSII codes for terminal manipulation

#include <string>
#include <term.h>

namespace term {

  // Text effects
  const std::string reset      = "\033[0m";
  const std::string bold       = "\033[1m";
  const std::string italic     = "\033[3m";
  const std::string under      = "\033[4m";
  const std::string invert     = "\033[7m";
  const std::string strike     = "\033[9m";
  const std::string bold_off   = "\033[22m";
  const std::string italic_off = "\033[23m";
  const std::string under_off  = "\033[24m";
  const std::string invert_off = "\033[27m";
  const std::string strike_off = "\033[29m";

  // Foreground colors
  const std::string fg_black   = "\033[30m";
  const std::string fg_red     = "\033[31m";
  const std::string fg_green   = "\033[32m";
  const std::string fg_yellow  = "\033[33m";
  const std::string fg_blue    = "\033[34m";
  const std::string fg_magenta = "\033[35m";
  const std::string fg_cyan    = "\033[36m";
  const std::string fg_white   = "\033[37m";
  const std::string fg_default = "\033[39m";

  // Background colors
  const std::string bg_black   = "\033[40m";
  const std::string bg_red     = "\033[41m";
  const std::string bg_green   = "\033[42m";
  const std::string bg_yellow  = "\033[43m";
  const std::string bg_blue    = "\033[44m";
  const std::string bg_magenta = "\033[45m";
  const std::string bg_cyan    = "\033[46m";
  const std::string bg_white   = "\033[47m";
  const std::string bg_default = "\033[49m";

}
