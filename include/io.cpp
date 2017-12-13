// =======================================================
//  Dynamic Spin Accumulation Calculation
//
//  Author: Luke Elliott
//
//  Test code for simulating the spin accumulation across
//  a given system, assumed to be a spin valve/tunnel ju-
//  nction.
//
//  File: io.cpp <CODE>
//
//  Initial code output for user, argument checking, inp-
//  ut file reading and data output.
//
//  GNU GPLv3. See LICENSE for details.
// =======================================================

// Standard headers
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

// Terminal formatting
#include "term.hpp"
//#include "material.hpp"
#include "system.hpp"

namespace io {
  // IO flag for file/test mode
  unsigned int io_flag;
  bool io_debug = 0; // Assume debugging is turned on

  // Header output
  void io_header() {
    std::cout << " ======================================================" << std::endl;
    std::cout << term::bold << term::fg_green << " Dynamic Spin Accumulation" << term::reset << std::endl << std::endl;
    std::cout << " Author: Luker Elliott" << std::endl;
    std::cout << " Email:  " << term::bold << "lee506@york.ac.uk" << term::reset << std::endl << std::endl;
    std::cout << " Test code for simulating the spin accumulation across" << std::endl;
    std::cout << " a given 1D system,  assumed to be a spin valve/tunnel " << std::endl;
    std::cout << " junction." << std::endl << std::endl;
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
  // ! ! ! ! ! !
  // This should be changed to use getopt
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
      std::cout << term::bold << term::fg_cyan << " Test suite mode" << term::reset << std::endl << std::endl;
    }
    else { // Unknown flag
      io_flag = 2;
    }
  }

  // Wrapper for io_header, io_check, io_get_flag
  void io_init(int argc, char *argv[]) {
    io_header();
    io_check(argc);
    io_get_flag(argv);
  }


  // Read input file
  void io_read_file(int argc, char *argv[]) {
    // Check a filename is given, exit if not
    if(argc<3) {
      std::cerr << term::bold << term::fg_red << " Error: " << term::reset << "no input file specified" << std::endl << std::endl;
      io_help();
    }

    int line_num = 0;

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



    bool mat_num_b  = false;


    std::vector<int> mat_id_array; // Track all material IDs

    // Read file line by line
    for(std::string line; std::getline(io_ifstream, line);) {
      line_num++;                                                           // Increment line number

      // Line parsing
      if(line=="") continue;                                                // Skip blank lines
      if(line.at(0)=='#') continue;                                         // Skip comments
      line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); // Remove



      // Split the input line into property and value strings
      // Value is a string as it is not known whether it is a double or a vector at this point
      std::string mat_id_s = line.substr(line.find("[")+1, line.find("]")-line.find("[")-1);
      std::string property_s = line.substr(line.find(":")+1, line.find("=")-line.find(":")-1);
      std::string value_s = line.substr(line.find("=")+1, *line.end()-line.find("=")-1);

      // Check if input is a system parameter
      if(line.find("system") != std::string::npos) {
        sys::system.set_param(property_s, value_s);
        continue;
      }

      // Handle material ID
      try {
        int mat_id = std::stoi(mat_id_s); // Convert string to integer ID
        if (mat_id < 0 || mat_id >= sys::system.mat_num()) { // ID out of material range
          std::cout << term::bold << term::fg_yellow << " Warning: "
                    << term::fg_white << io_filename << ":" << line_num << ": "
                    << term::reset << "Invalid material ID value (out of range) '"
                    << term::bold << mat_id_s << term::reset << "'"
                    << std::endl;
          continue;
        }
        bool mat_exists = std::any_of(mat_id_array.begin(), mat_id_array.end(), [&](int i) { return i == mat_id; });

        if(!mat_exists){       // Insert material ID if not known
          // Warn user if unusually large material ID is encountered
          if (io_debug) {
            if(mat_id!=0 && mat_id>*std::max_element(mat_id_array.begin(), mat_id_array.end())+1) {
              std::cout << term::bold
                        << term::fg_yellow << " Warning: "
                        << term::fg_white << io_filename << ":" << line_num << ": "
                        << term::reset << "Material ID seems large. "
                        << term::bold << "(" << mat_id << " >> "
                        << *std::max_element(mat_id_array.begin(), mat_id_array.end()) << ")"
                        << term::reset << std::endl;
            }
          }
          std::cout << " Adding material: " << mat_id << std::endl;
          mat_id_array.push_back(mat_id); // Push material ID to back of vector
        }

        // Set relevant property
        sys::system.set_mat_prop(mat_id, property_s, value_s);
      }

      catch (const std::invalid_argument& e) { // Non-number value
        std::cout << term::bold << term::fg_yellow << " Warning: "
                  << term::fg_white << io_filename << ":" << line_num << ": "
                  << term::reset << "Invalid material ID value '"
                  << term::bold << mat_id_s << term::reset << "'"
                  << std::endl;
      }
    }
    if (mat_id_array.size() != sys::system.mat_num()) {
      std::cout << std::endl;
      std::cout << term::bold << term::fg_yellow
                << " Warning: " << term::reset << "mat_num mismatch" << std::endl;
      std::cout << " ----------------------------" << std::endl;
      std::cout << " Expected:    " << term::bold << sys::system.mat_num() << term::reset << std::endl;
      std::cout << " Encountered: " << term::bold << mat_id_array.size() << term::reset << std::endl;
      std::cout << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  //   // Converts string unit input to exponent
  //   double io_unit(std::string io_unit_flag) {
  //     // Check variations for some quality of life
  //     if(io_unit_flag == "!NM" || io_unit_flag == "!nm" || io_unit_flag == "!Nm" || io_unit_flag == "!nM") {
  //       return pow(10.0, -9);
  //     }
  //     else if(io_unit_flag == "!A" || io_unit_flag == "!a") {
  //       return pow(10.0, -10);
  //     }
  //     else {
  //       std::cout << " Unknown unit: " << term::bold << io_unit_flag << term::reset << std::endl;
  //       std::cout << " Defaulting to " << term::bold <<  "!nm" << term::reset << std::endl << std::endl;
  //       return pow(10.0, -9);
  //     }
  //   }
}
