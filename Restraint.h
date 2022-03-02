/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Restraint.h
 * Author: tg
 *
 * Created on June 9, 2020, 11:55 AM
 */

#ifndef RESTRAINT_H
#define RESTRAINT_H

#include <string>
#include <vector>

#include "xAtom.h"
#include "Eqiv.h"


class Restraint {
private:
    float target_, sof_;
    std::string resiclass_;
    xAtom::ResiNum resinum_;
    std::vector<std::string> atom_pairs_;
    
    // setup search criteria for an atom
    std::string setup(const std::string& atom, int& symop, xAtom::ResiNum& resinum) const;
    
public:
    struct Numeric {
        XYZ dX_;
        double targetsq_;
        double weight_;
    };
    Restraint(){};
    Restraint(float target, float sof, const xAtom::ResiNum& resinum,
            const std::string& resiclass, const std::vector<std::string>& atom_pairs);
    ~Restraint(){};
    
    // find atoms of atom_pairs in list, apply transforms, and create (x-y) values
    std::vector<Restraint::Numeric> make (const std::vector<xAtom>& atoms, 
        const std::vector<SymOp>& equivs) const;
    
    // printing
    friend std::ostream& operator<<(std::ostream& out, const Restraint& r);

};

#endif /* RESTRAINT_H */

