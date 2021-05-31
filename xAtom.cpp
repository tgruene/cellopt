/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xAtom.cpp
 * Author: tg
 * 
 * Created on June 9, 2020, 8:54 AM
 */

#include "xAtom.h"
#include "myExceptions.h"
#include <iostream>
#include <stdexcept>

xAtom::xAtom(const std::string& instr, const ShelxState& xstate) {
    float x,y,z;
    float u[5];
    name_ = instr.substr(0, 4);
    // strip white space from name_:
    std::istringstream ws(name_);
    ws >> name_;
    
    std::istringstream conv(instr.substr(4, std::string::npos));
    conv >> sfac_ >> x >> y >> z >> sof_ >> U_[0];
    if (conv.fail()) {
        std::cout << "*** Error: cannot convert line " << instr << " to shelxl atom line\n";
        throw myExcepts::ShelxFormat("Atom line");
    }
    xyz_ = XYZ(x,y,z);
    
    if (!conv.eof()) // expect 5x Uaniso
        conv >> u[0] >> u[1] >> u[2] >> u[3] >> u[4];
    if (!conv.fail()) { // proper definition of Uaniso
        U_[1] = u[0];
        U_[2] = u[1];
        U_[3] = u[2];
        U_[4] = u[3];
        U_[5] = u[4];
    }
    
    chainID_ = xstate.chainID();
    resinum_ = xstate.resinumber();
    resiclass_ = xstate.resiclass();
    partnum_  = xstate.partnum();
}

/**
 * check for selection criteria
 * @param name
 * @param resiclass
 * @param part
 * @param resinum
 * @return 
 */
bool xAtom::match(const std::string name, const std::string& resiclass, const xAtom::ResiNum& resinum, int part = 0) const {
    if (name != name_) return false;
    if (part != 0 && partnum_ != 0 && partnum_ != part) return false;
    // when residue number is set, residue class is irrelevant
    if (resinum.chain != 0 && xresinum_ != resinum.chain) return false;
    if (resinum != 0 && xresinum_.resinumber == resinum.resinumber) return true;
    if (resinum.resinumber == 0 && !resiclass.empty() && resiclass == resiclass)
        return true;
    
    return false;
}

/**
 * output string of atom name plus residue number (if set) and part number (if set)
 * @param out
 * @param atom
 * @return 
 */
std::ostream& operator<< (std::ostream& out, const xAtom& atom) {
    out << atom.name_;
    if (atom.resinum_ != 0) out << '_' << atom.resinum_;
    if (atom.partnum_ != 0) out << '^' << atom.partnum_;
    if (atom.resiclass_ != "") out << " Residue " << atom.resiclass_;
    return out;
}

xAtom::ResiNum xAtom::resinum(const std::string resistring) const {
    std::string::size_t colon = resistring.find(':');

    xAtom::ResiNum result;
    if (colon == 1) { // colon found get chainID
        result.chain = resistring.front();
    } else if (colon != std::string::npos) {
        throw myExcepts::Format("Invalid residue string, only 'ID:NUMBER of NUMBER allowed");
    }

    try {
        result.resinumber = std::stoi(resistring.substr(colon + 1, std::string::npos));
    } catch (std::invalid_argument& e) {
        throw myExcepts::Format("Invalid residue string, only 'ID:NUMBER of NUMBER allowed");
    }
    return result;
}