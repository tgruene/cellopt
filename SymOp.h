/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymOp.h
 * Author: tg
 *
 * Created on June 9, 2020, 8:58 AM
 */

#ifndef SYMOP_H
#define SYMOP_H

#include <vector>
#include <ostream>

class XYZ;

class SymOp {
private:
    //! R00, R01, R02, T1, R10, R11, R12, T2, R20, R21, R22, T3
    float op_[12] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0};

public:
    SymOp(){};
    SymOp(const std::vector<float>& R);
    ~SymOp(){};
    
    friend XYZ operator*(const SymOp& R, const XYZ& xyz);
    friend std::ostream& operator<<(std::ostream& out, const SymOp& R);

};

XYZ operator* (const SymOp& R, const XYZ& xyz); // defined in XYZ.cpp
std::ostream& operator<<(std::ostream& out, const SymOp& R); // defined in SymOp.cpp
#endif /* SYMOP_H */

