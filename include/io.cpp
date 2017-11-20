#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <cmath>

// Access system parameters
#include "term.h"
#include "params.h"

namespace io {
  // IO flag for file/test mode
  unsigned int io_flag;

  // Header output
  void io_header() {
    std::cout << " ======================================================" << std::endl;
    std::cout << term::bold << term::fg_green << " Dynamic Spin Accumulation" << term::reset << std::endl << std::endl;
    std::cout << " Author: Luker Elliott" << std::endl;
    std::cout << " Email: " << term::fg_white << "lee506@york.ac.uk" << term::reset << std::endl << std::endl;
    std::cout << " Calculates the spin accumulation of a given magnetic" << std::endl;
    std::cout << " system over time." << std::endl << std::endl;
    std::cout << " GNU GPLv3. See LICENSE for details." << std::endl;
    std::cout << " ======================================================" << std::endl << std::endl;
  }
  // Output help, exit after
  void io_help() {
    std::cout << " Usage: "<< std::endl;
    std::cout << " ---------------------------" << std::endl;
    std::cout << " Input file: -f <filename>" << std::endl;
    std::cout << " Run test suite: -t" << std::endl << std::endl;
    exit(EXIT_FAILURE);
  }


  // Check arguments are given. Exit if none found
  void io_check(int argc) {

    // First argument (argv[0]) is call to program in C++
    if(argc < 2) {
      std::cerr << term::bold << term::fg_red << " Error: " << term::reset << "no arguments given" << std::endl << std::endl;
      io_help();
    }
  }

  // Takes flag from argument and converts to int for control
  void io_get_flag(char *argv[]) {
    // Get flag
    std::string io_flag_str = argv[1];

    // Convert str to int
    if(io_flag_str == "-f") { // File mode
      io_flag = 0;

      // Relay to user
      std::cout << term::bold << term::fg_green << " Input file mode" << term::reset << std::endl << std::endl;
    }
    else if(io_flag_str == "-t") { // Test mode
      io_flag = 1;

      // Relay to user
      std::cout << term::bold << term::fg_green << " Test suite mode" << term::reset << std::endl << std::endl;
    }
    else { // Unknown flag
      io_flag = 2;
    }
  }


  // Read input file and return as vector to main
  void io_read_file(int argc, char *argv[]) {
    // Check a filename is given, exit if not
    if(argc<3) {
      std::cerr << term::bold << term::fg_red << " Error: " << term::reset << "no input file specified" << std::endl << std::endl;
      io_help();
    }

    // Get filename
    std::string io_filename(argv[2]);

    // Input file stream
    std::ifstream io_ifstream;

    // Open file
    io_ifstream.open(io_filename.c_str(), std::ios::in);

    // Test file was found and opened, exit if not.
    if(!io_ifstream) {
      std::cerr << term::bold << term::fg_red << " Error: " << term::reset << "Cannot open input file " << term::bold << io_filename << term::reset << std::endl << std::endl;
      exit(EXIT_FAILURE);
    }

    // Relay to user
    std::cout << " Reading input file: " << term::bold <<  io_filename << term::reset << std::endl << std::endl;

    // Take major/minor axis and dipole spacing from input file
    // Use flags to check they are given
    double maj_ax, min_ax, dip_spc;
    bool maj_ax_flag = false;
    bool min_ax_flag = false;
    bool dip_spc_flag = false;

    // Read file line by line
    for(std::string line; std::getline(io_ifstream, line);) {
      std::istringstream in(line); // Create string stream for individual line
      std::string in_flag, in_unit_flag;

      in >> in_flag; // Take first string before whitespace, i.e. flag

      double io_unit (std::string io_unit_flag); // Forward declare

      // Assign variables based on flag
      if(in_flag == "major-axis") {
        in >> maj_ax; // Value
        in >> in_unit_flag; // Units
        maj_ax *= io_unit(in_unit_flag); // Apply units
        if(maj_ax <= 0) { // Check for physical values
          std::cout << "\033[1;31m Error: Major axis \033[1;39m" << maj_ax <<  "\033[1;31m <= 0\033[0;39m" << std::endl;
          exit(EXIT_FAILURE);
        }
        maj_ax_flag = true;
      }
    
      else {
        std::cout << term::bold << term::fg_red << " Error: Not enough system parameters given" << term::reset << std::endl;
                                                                                                                  exit(EXIT_FAILURE);
      }
    }
  }
    // Converts string unit input to exponent
    double io_unit(std::string io_unit_flag) {
      // Check variations for some quality of life
      if(io_unit_flag == "!NM" || io_unit_flag == "!nm" || io_unit_flag == "!Nm" || io_unit_flag == "!nM") {
        return pow(10.0, -9);
      }
      else if(io_unit_flag == "!A" || io_unit_flag == "!a") {
        return pow(10.0, -10);
      }
      else {
        std::cout << " Unknown unit: " << term::bold << io_unit_flag << term::reset << std::endl;
        std::cout << " Defaulting to " << term::bold <<  "!nm" << term::reset << std::endl << std::endl;
        return pow(10.0, -9);
      }
    }
}
