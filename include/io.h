#ifndef IO_H
#define IO_H

namespace io {
  // IO flag for file/test mode
  extern unsigned int io_flag;

  // Initialization wrapper (io_header, io_check, io_get_flag)
  extern void io_init(int argc, char *argv[]);

  // Read input file and return as vector to main
  extern  void io_read_file(int argc, char *argv[]);

}

#endif /* IO_H */
