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
public:
    class ResiNum {
    private:    
        char chainID_;
        short resinumber_;
    public:
        ResiNum(): chainID_(0), resinumber_(0){}
        ResiNum(const ResiNum& other) = default;
        ResiNum(char c, short num): chainID_(c), resinumber_(num){}
        ~ResiNum(){}
        friend bool operator== (const ResiNum& rn1, const ResiNum& rn2) = default;
        char chainID() const { return chainID_; }
        short resinumber() const { return resinumber_; }        
    };

private:
    std::string name_;
    int sfac_;
    XYZ xyz_;
    float sof_;
    float U_[6] = {0.05, 0, 0, 0, 0, 0};
    std::string resiclass_;
    ResiNum xresinum_;
    int partnum_;

public:
    xAtom(const std::string& instr, const ShelxState& xstate);
    ~xAtom(){}
    
    std::string name() const { return name_; }
    int sfac() const { return sfac_; }
    XYZ xyz() const { return xyz_; }
    
    std::string resiclass() const { return resiclass_; }
    ResiNum resinum() const { return xresinum_; }
    int partnum() const { return partnum_; }
    
    // convert a potential residue string to ID and number
    static ResiNum resinum(const std::string&);
    
    // matching for atom selection
    bool match(const std::string name, const std::string& resiclass, const xAtom::ResiNum& resinum, int part = 0) const;
    
    friend std::ostream& operator<< (std::ostream& out, const xAtom& atom);

};

std::ostream& operator<< (std::ostream& out, const xAtom& atom);
#endif /* XATOM_H */

