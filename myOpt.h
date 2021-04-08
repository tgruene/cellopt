/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   myOpt.h
 * Author: tg
 *
 * Created on October 10, 2020, 8:51 PM
 */

#ifndef MYOPT_H
#define MYOPT_H

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>
#include "ResParser.h"

class myOpt {
private:
    gsl_multimin_function_fdf fdf_;
    gsl_vector* cell_;
    gsl_vector* gslrestr_;
    double steps_;
    double tolerance_;
    double eps_;
    int maxIter_;
    char crystalSystem_;
    short verbosity_;
    
    // simplex minimiser
    gsl_multimin_function fnm_;
public:
    myOpt(const ResParser& resdata, const char crystsystem, const double steps, 
    const double tol, const double eps_, const int maxIter, const short& verbosity);
    
    ~myOpt();
    
    double optimise();
    
    // simplex version of optimise
    double optimise_nm();
    
    //! provide information about current cell and target value
    void info() const;
    
    //! provide information about parameter settings
    void settings() const;
    
    //! print optimised cell in SHELX format
    std::string cell_xlformat(const float& lambda) const;

};

#endif /* MYOPT_H */

