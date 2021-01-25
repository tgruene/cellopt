/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ResParser.h
 * Author: tg
 *
 * Created on June 6, 2020, 9:56 PM
 */

#ifndef RESPARSER_H
#define RESPARSER_H

#include<string>
#include<vector>

#include "ShelxState.h"
#include "SymOp.h"
#include "Restraint.h"
#include "xAtom.h"

class ResParser {
private:
    const std::string resfile_;
    short verbosity_;

    std::vector<std::string> reslines_;
    float lambda_;
    float a_, b_, c_, alpha_, beta_, gamma_;
    ShelxState shelxState_;
    // derived from eqivs: $n corresponds to symop at position n 
    std::vector<SymOp> eqivs_;
    // list of restraints
    std::vector<Restraint> restraints_;
    // list of atoms
    std::vector<xAtom> xatoms_;

    // read res-file
    void readres(const std::string& filename);
    // interprete res-file and extract values
    bool xcmd(const std::string& instr);
    // read cell from a linw
    void getcell(const std::string& line);
    
    void resi(const std::string& line);
    
    void part(const std::string& line);
    
    // process DFIX and DANG line
    void addrestraint(const std::string& line);
    
    // interpret res-file
    void interpres();
    
    // return upper case version of string
    std::string upcase(const std::string& word);
    
public:
    ResParser(const char* resfile, const short& verbosity);
    ~ResParser(){};
    
    // create numeric restraints
    std::vector<Restraint::Numeric> restraints() const;

    // return cell parameters
    float a() const { return a_; }
    float b() const { return b_; }
    float c() const { return c_; }
    float alpha() const { return alpha_; }
    float beta() const { return beta_; }
    float gamma() const { return gamma_; }  
    
    float lambda() const { return lambda_; }
};

#endif /* RESPARSER_H */

