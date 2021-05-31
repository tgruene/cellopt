/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Restraint.cpp
 * Author: tg
 * 
 * Created on June 9, 2020, 11:55 AM
 */

#include <iostream>

#include "Restraint.h"
#include "myExceptions.h"

Restraint::Restraint(float target, float sof, const xAtom::ResiNum& resinum,
        const std::string& resiclass, const std::vector<std::string>& atom_pairs) :
target_(target), sof_(sof), resinum_(resinum), 
resiclass_(resiclass), atom_pairs_(atom_pairs) {
}

std::vector<Restraint::Numeric> Restraint::make(const std::vector<xAtom>& atoms,
        const std::vector<SymOp>& equivs) const {
    std::vector<std::string>::const_iterator it;
    std::vector<Restraint::Numeric> restraints;

    Restraint::Numeric numrestr;
    numrestr.targetsq_ = target_*target_;
    numrestr.weight_ = 1. / (sof_ * sof_);

    // in case resiclass and resinum are empty, name + residuenumber should be unique
    if (resiclass_.empty()) {
        for (it = atom_pairs_.begin(); it != atom_pairs_.end(); it += 2) {
            xAtom::ResiNum resinum1 (resinum_), resinum2(resinum_);
            int eqiv1(0), eqiv2(0);
            std::string atom1(setup(*it, eqiv1, resinum1));
            std::string atom2(setup(*(it + 1), eqiv2, resinum2));
            XYZ xyz1, xyz2;
            for (std::vector<xAtom>::const_iterator it2 = atoms.begin(); it2 != atoms.end(); ++it2) {
                if (it2->resinum() == resinum1 && it2->name() == atom1)
                    xyz1 = it2->xyz();
                if (it2->resinum() == resinum2 && it2->name() == atom2)
                    xyz2 = it2->xyz();
            }
            xyz1 = equivs[eqiv1] * xyz1;
            xyz2 = equivs[eqiv2] * xyz2;
            numrestr.dX_ = xyz1 - xyz2;
            restraints.push_back(numrestr);
        }

    } else { // resiclass set
        /*
         * find matching atom pairs including residue class and
         * create respective restraint
         */
        for (it = atom_pairs_.begin(); it != atom_pairs_.end(); it += 2) {
            xAtom::ResiNum resinum1(resinum_), resinum2(resinum_);
            int eqiv1(0), eqiv2(0);
            // with 'resiclass', first atom may not have a residue number
            std::string atom1(setup(*it, eqiv1, resinum1));
            std::string atom2(setup(*(it + 1), eqiv2, resinum2));
            XYZ xyz1, xyz2;
            //! debugging

            for (std::vector<xAtom>::const_iterator it2 = atoms.begin(); it2 != atoms.end(); ++it2) {
                if (it2->resiclass() == resiclass_ && it2->name() == atom1) {
                    xyz1 = it2->xyz();
                    for (auto it3 = atoms.begin(); it3 != atoms.end(); ++it3) {
                        if (it3->resiclass() == resiclass_ && it3->resinum() == it2->resinum() && it3->name() == atom2) {
                            xyz2 = it3->xyz();
                            xyz1 = equivs[eqiv1] * xyz1;
                            xyz2 = equivs[eqiv2] * xyz2;
                            numrestr.dX_ = xyz1 - xyz2;
                            restraints.push_back(numrestr);
                            break; // should only occur once
                        }
                    }
                }

            }
        }

    }

    return restraints;
}

/**
 * set up symop and resi number from atom name in restraint list
 * @param atom input name, e.g. O3_$5 or Na1_12
 * @param symop return n in O3_$n
 * @param resinum return residue number in Na1_num
 * @return 
 */
std::string Restraint::setup(const std::string& at, int& symop, const xAtom::ResiNum& resinum) const {
    std::string atom(at);
    size_t found(atom.find("_$"));
    if ((found != std::string::npos)) {
        symop = std::stoi(atom.substr(found + 2, std::string::npos));
        atom.erase(found, std::string::npos);
    }
    found = atom.find('_');
    const char peek = atom.at(found+1);
    if (peek == '+' || peek == '-') {
        // special generalisation of previous or next residue not yet implemente
        resinum = xAtom::ResiNum();
        return atom;
    }
    if (found != std::string::npos) {
        // debugging
        std::cout <<  "---> Debugging: converting \'" << atom << "\' to residue number\n" << std::endl;
        const std::string chain_num = atom.substr(found + 1, std::string::npos);
        size_t colon = chain_num.find(':');
        if (colon != std::string::npos) {
            chainID = chain_num.front();
        }
        else {
            chainID = 0;
            colon = 0;
        }
        resinum = std::stoi(atom.substr(colon + 1, std::string::npos));
        atom.erase(found, std::string::npos);
    }

    return atom;
}

std::ostream& operator<<(std::ostream& outp, const Restraint& r) {
    const std::string a1 = r.atom_pairs_.front();
    const std::string a2 = *(r.atom_pairs_.begin()+1);
    outp << "--> target: " << r.target_ << "+/-" << r.sof_ << " residue class " << r.resiclass_
    << " residue number " << r.resinum_ << " first pair: " << a1 << " " << a2;
    return outp;
}