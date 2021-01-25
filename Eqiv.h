/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Eqiv.h
 * Author: tg
 *
 * Created on June 9, 2020, 9:43 AM
 */

#ifndef EQIV_H
#define EQIV_H

#include <string>

#include "SymOp.h"


class Eqiv {
private:
    int n_;
    SymOp S_;
    
    // convert three lines to symop-line
    std::vector<float> tosymop (std::string X);
public:
    Eqiv(const std::string instr);
    ~Eqiv(){};
    
    int n() const { return n_; }
    SymOp symop() const { return S_; }

};

#endif /* EQIV_H */

