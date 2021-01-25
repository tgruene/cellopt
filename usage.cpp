/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "usage.h"
#include "defines.h"
#include  <string>
#include <iomanip>
#include <iostream>

void usage() {
    std::cout << " * Usage: cellopt [OPTIONS] -f ins-file\n"
            << "    Optimise cell parameters from restraints in shelx ins- or res-file\n"
            << "    Options (square brackets: [defaults]\n"
            << "    -f ins  : read ins-file (mandatory)\n"
            << "    -x <c>  : crystal system, a: triclinic,\n"
            << "                              m: monoclinic,\n"
            << "                              o: orthorhombic,\n"
            << "                              t: tetragonal,\n"
            << "                              h: tri-/hexagonal,\n"
            << "                              c: cubic\n"
            << "    -s stp  : GSL step size [" << Defs::GSL::step << "]\n" 
            << "    -t tol  : GSL tolerance [" << Defs::GSL::tol  << "]\n"
            << "    -I iter : maximum number of iterations [" << Defs::GSL::maxIter << "]\n"
            << "    -e eps  : expected precision (epsilon gradient) [" << Defs::GSL::eps_grad << "]\n"
            << "    -h / -? : print this usage and exit\n"
            << "    -v num  : verbosity level 'num'. Use 'zero' for iteration\n"
            << "              with SHELXL refinement.\n"
            << std::endl;
}

/**
 * Print a simple version string consisting of built date and time.
 */
std::string version() {
    std::ostringstream builder;
    builder << __DATE__ << ", time " << __TIME__;
    return builder.str();
}

/**
 *print a short welcome notice
 */
void hello() {
    std::cout << "#---------------------------------------------------------------#\n"
              << "# cellopt: Optmise cell parameters from geometric restraints    #\n"
              << "# Version: 1.0                                                  #\n"
              << "# Built:   " << std::setw(31) << version() << "                      #\n"
              << "# Copyright:    Tim Gruene, 2020                                #\n"
              << "#---------------------------------------------------------------#\n"
            ;
}
