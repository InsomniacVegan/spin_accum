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
//  File: main.cpp
//
//  Main execution/logic code.
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

// Standard headers
#include <iostream>

// Unit testing
#include "catch.hpp"

// Personal modules
#include "catch.hpp"
#include "io.hpp"
#include "system.hpp"

int main(int argc, char *argv[]) {

  // IO initialization
  // ----------------------------------------------------
  // Output program header
  // Check command flags are given
  // Get command line flag to determine mode of operation
  io::io_init(argc, argv);

  // Mode of operation select
  switch(io::io_flag) {

  // Input file mode
  case 0:
    {
      // TESTING

      // READ FILE HERE
      io::io_read_file(argc, argv);

      // System intialization




      // EVOLVE

    break;
    }

  // Test mode
  case 1:


    break;
  }





  return 0;
}
