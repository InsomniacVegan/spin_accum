#ifndef IO_H
#define IO_H

namespace io {
  // IO flag for file/test mode
  extern unsigned int io_flag;

  // Header output
  extern void io_header();

  // Output help, exit after
  extern void io_help();

  // Check arguments are given. Exit if none found
  extern  void io_check(int argc);

  // Takes flag from argument and converts to int for control
  extern  void io_get_flag(char *argv[]);

  // Read input file and return as vector to main
  extern  void io_read_file(int argc, char *argv[]);

  // Converts string unit input to exponent
  extern  double io_unit(std::string io_unit_flag);
}

#endif /* IO_H */
