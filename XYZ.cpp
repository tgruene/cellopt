/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XYZ.cpp
 * Author: tg
 * 
 * Created on June 9, 2020, 8:55 AM
 */

#include <iomanip>

#include "XYZ.h"

XYZ::XYZ(const float& x, const float& y, const float& z): 
x_(x), y_(y), z_(z){
}

XYZ& XYZ::operator +=(const XYZ& other) {
    x_ += other.x_;
    y_ += other.y_;
    z_ += other.z_;
    return *this;
}

XYZ& XYZ::operator *=(const float& s) {
    x_ *= s;
    y_ *= s;
    z_ *= s;
    return *this;
}

XYZ operator*(const SymOp& R, const XYZ& xyz) {
    XYZ res;
    res.x_ = R.op_[0] * xyz.x_ + R.op_[1] * xyz.y_ + R.op_[2] * xyz.z_ + R.op_[3];
    res.y_ = R.op_[4] * xyz.x_ + R.op_[5] * xyz.y_ + R.op_[6] * xyz.z_ + R.op_[7];
    res.z_ = R.op_[8] * xyz.x_ + R.op_[9] * xyz.y_ + R.op_[10] * xyz.z_ + R.op_[11];
    return res;
}

XYZ operator*(const float& s, const XYZ& xyz) {
    XYZ res;
    res.x_ = s * xyz.x_;
    res.y_ = s * xyz.y_;
    res.z_ = s * xyz.z_;
    return res;
}

XYZ operator+(const XYZ& x1, const XYZ& x2) {
    const XYZ y(x1.x_+x2.x_, x1.y_+x2.y_, x1.z_+x2.z_);
    return y;
}

XYZ operator-(const XYZ& x1, const XYZ& x2) {
    const XYZ y(x1.x_-x2.x_, x1.y_-x2.y_, x1.z_-x2.z_);
    return y;
}

std::ostream& operator<<(std::ostream& out, const XYZ& xyz) {
    
    out << "(" << std::setw(6) << std::setprecision(3) << xyz.x_ << ", "
            << std::setw(6) << std::setprecision(3) << xyz.y_ << ", "
            << std::setw(6) << std::setprecision(3) << xyz.z_ << ")";
            
    return out;
}