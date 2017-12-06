// =======================================================
//  Dynamic Spin Accumulation Calculation
//
//  Author: Luke Elliott
//
//  Test code for simulating the spin accumulation across
//  a given system, assumed to be a spin valve/tunnel ju-
//  nction.
//
//  File: io.hpp <HEADER>
//
//  Initial code output for user, argument checking, inp-
//  ut file reading and data output.
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

#ifndef IO_HPP
#define IO_HPP

namespace io {
  // IO flag for file/test mode
  extern unsigned int io_flag;

  // Initialization wrapper (io_header, io_check, io_get_flag)
  extern void io_init(int argc, char *argv[]);

  // Read input file and return as vector to main
  extern  void io_read_file(int argc, char *argv[]);

}

#endif /* IO_HPP */
