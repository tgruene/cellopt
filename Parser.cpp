/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Parser.cpp
 * Author: tg
 * 
 * Created on June 12, 2020, 9:51 AM
 */

#include "Parser.h"
#include "myExceptions.h"
#include "usage.h"
#include "defines.h"

#include <iostream>
#include <string>

/**
 * verbose_: verbosity level. 0: minimal output, only CELL
 * @param argc
 * @param argv
 */
Parser::Parser(int argc, const char* const argv[]) :
verbose_(1),
gsl_epsgrad_(Defs::GSL::eps_grad),
gsl_maxIter_(Defs::GSL::maxIter),
gsl_steps_(Defs::GSL::step),
gsl_tolerance_(Defs::GSL::tol),
csystem_('a'),
insfile_("") {
    for (int i = 1; i < argc; ++i) {
        std::string option = argv[i];
        if (option.substr(0, 2) == "-h" || option.substr(0, 2) == "-?") {
            usage();
            throw myExcepts::Usage("Help message");
            return;
        }

        // set verbosity level
        if (getoption(option, "-v", verbose_, i, argc, argv)) continue;
        if (getoption(option, "-I", gsl_maxIter_, i, argc, argv)) continue;
        if (getoption(option, "-s", gsl_steps_, i, argc, argv)) continue;
        if (getoption(option, "-t", gsl_tolerance_, i, argc, argv)) continue;
        if (getoption(option, "-e", gsl_epsgrad_, i, argc, argv)) continue;
        if (getoption(option, "-x", csystem_, i, argc, argv)) continue;
        if (getoption(option, "-f", insfile_, i, argc, argv)) continue;

        if (option.at(0) == '-') {
            std::cout << "Unknown option string " << option << std::endl;
            usage();
            throw myExcepts::Usage(option.c_str());
        }
    }
    if (insfile_.empty()) {
        std::cout << "*** Error: missing SHELXL ins- or res-file \n"
                << "*** (Option -f)\n";
        usage();
        throw myExcepts::Usage("Ins file");
    }
}

/**
 * extract option from an option string
 * @param option command line string
 * @param opt    option under consideration
 * @param optval value assigned of option starts with opt
 * @param idx running index for argv
 * @param argc
 * @param argv
 * @return 
 */
template <typename T> bool Parser::getoption(const std::string& option,
        const std::string& opt, T& optval, int& idx, int argc,
        const char* const argv[]) {

    if (option.substr(0, 2) == opt) {
        if (option.length() > 2) {
            std::string fn = option.substr(2, option.length());
            std::istringstream conv(fn);
            conv >> optval;
            return true;
        } else {
            if (idx + 1 >= argc) {
                usage();
                throw myExcepts::Usage("");
            }
            std::istringstream conv(argv[idx + 1]);
            conv >> optval;
            ++idx;
            return true;
        }
    } else return false;
}