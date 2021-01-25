/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Parser.h
 * Author: tg
 *
 * Created on June 12, 2020, 9:51 AM
 */

#ifndef PARSER_H
#define PARSER_H

#include <string>

class Parser {
private:
    short verbose_;
    float gsl_steps_;
    float gsl_tolerance_;
    float gsl_epsgrad_;
    int   gsl_maxIter_;
    char csystem_;
    std::string insfile_;
    
    
    template <typename T> bool getoption(const std::string& option,
            const std::string& opt, T& optval, int& idx, int argc,
            const char* const argv[]);
public:
    Parser(int argc, const char* const argv[]);
    ~Parser(){};
    
    short verbose() const { return verbose_; }
    float gsl_steps() const { return gsl_steps_; }
    float gsl_tolerance() const { return gsl_tolerance_; }
    float gsl_epsgrad () const { return gsl_epsgrad_; }
    int   gsl_maxIter()  const { return gsl_maxIter_; }
    char  crystalSystem() const { return csystem_; }
    
    std::string insfile() const { return insfile_; }

};

#endif /* PARSER_H */

