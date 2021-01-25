/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xAtom.h
 * Author: tg
 *
 * Created on June 9, 2020, 8:54 AM
 */

#ifndef XATOM_H
#define XATOM_H

#include <ostream>

#include "ShelxState.h"
#include "XYZ.h"

class xAtom {
private:
    std::string name_;
    int sfac_;
    XYZ xyz_;
    float sof_;
    float U_[6] = {0.05, 0, 0, 0, 0, 0};
    std::string resiclass_;
    int resinum_;
    int partnum_;

public:
    xAtom(const std::string& instr, const ShelxState& xstate);
    ~xAtom(){}
    
    std::string name() const { return name_; }
    int sfac() const { return sfac_; }
    XYZ xyz() const { return xyz_; }
    
    std::string resiclass() const { return resiclass_; }
    int resinum() const { return resinum_; }
    int partnum() const { return partnum_; }
    
    // matching for atom selection
    bool match(const std::string name, const std::string& resiclass, int resinum, int part = 0) const;
    
    friend std::ostream& operator<< (std::ostream& out, const xAtom& atom);

};

std::ostream& operator<< (std::ostream& out, const xAtom& atom);
#endif /* XATOM_H */

