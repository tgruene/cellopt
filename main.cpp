/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: tg
 *
 * Created on June 6, 2020, 1:17 PM
 */

#include <iostream>
#include <gsl/gsl_vector_double.h>

#include "Parser.h"
#include "myOpt.h"
#include "ResParser.h"
#include "myExceptions.h"

/*
 * 
 */
int main(int argc, char** argv) {


    Parser parser(argc, argv);

    ResParser resParser(parser.insfile().c_str(), parser.verbose());

    myOpt celloptim(resParser, parser.crystalSystem(), parser.gsl_steps(),
            parser.gsl_tolerance(), parser.gsl_epsgrad(), parser.gsl_maxIter(),
            parser.verbose());

    if (parser.verbose() > 0) {
        celloptim.info();
        celloptim.settings();
    }

    double tgt = celloptim.optimise();

    if (parser.verbose() == 0) {
        std::cout << celloptim.cell_xlformat(resParser.lambda());
    } else {
        celloptim.info();
    }

    return 0;
}
