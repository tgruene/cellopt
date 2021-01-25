/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymOp.cpp
 * Author: tg
 * 
 * Created on June 9, 2020, 8:58 AM
 */

#include "SymOp.h"
#include "myExceptions.h"

#include <iomanip>

SymOp::SymOp(const std::vector<float>& R) {
    if (R.size() != 9 && R.size() != 12)
        throw myExcepts::Dimension("Symop must be initialised from 9 or 12 numbers.");
    for (std::vector<float>::const_iterator it = R.begin(); it != R.end(); ++it) {
        size_t idx = it - R.begin();
        op_[idx] = *it;
    }
}

std::ostream& operator<<(std::ostream& out, const SymOp& R) {
    for (int i = 0; i < 12; i += 4) {
        out << std::fixed << std::setprecision(0)
                << std::setw(3) << R.op_[i]
                << std::setw(3) << R.op_[i + 1]
                << std::setw(3) << R.op_[i + 2] << "   "
                << std::setw(6) << std::setprecision(3) << R.op_[i + 3]
                << '\n';
    }
    return out;
}
