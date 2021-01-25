/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XYZ.h
 * Author: tg
 *
 * Created on June 9, 2020, 8:55 AM
 */

#ifndef XYZ_H
#define XYZ_H

#include "SymOp.h"

class XYZ {
private:
    float x_, y_, z_;

public:
    XYZ(){};
    XYZ(const float& x, const float& y, const float& z);
    ~XYZ(){}
    
    float x() const { return x_; }
    float y() const { return y_; }
    float z() const { return z_; }
    
    friend XYZ operator* (const SymOp& R, const XYZ& xyz);
    friend XYZ operator+ (const XYZ& x1, const XYZ& x2);
    friend XYZ operator- (const XYZ& x1, const XYZ& x2);

};

XYZ operator* (const SymOp& R, const XYZ& xyz); // defined in XYZ.cpp
XYZ operator+ (const XYZ& x1, const XYZ& x2); // defined in XYZ.cpp
XYZ operator- (const XYZ& x1, const XYZ& x2); // defined in XYZ.cpp
#endif /* XYZ_H */

