/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ResParser.cpp
 * Author: tg
 * 
 * Created on June 6, 2020, 9:56 PM
 */

/* list of shelxl commands according to
 * http://shelx.uni-goettingen.de/shelxl_html.php
 * ABIN   ACTA   AFIX   ANIS   ANSC   ANSR   BASF   BIND   BLOC   BOND   
 * BUMP   CELL   CGLS   CHIV   CONF   CONN   DAMP   DANG   DEFS   DELU   
 * DFIX   DISP   EADP   END   EQIV   EXTI   EXYZ   FEND   FLAT   FMAP   
 * FRAG   FREE   FVAR   GRID   HFIX   HKLF   HTAB   ISOR   LATT   LAUE   
 * LIST   L.S.   MERG   MORE   MOVE   MPLA   NCSY   NEUT   OMIT   PART   
 * PLAN   PRIG   REM   RESI   RIGU   RTAB   SADI   SAME   SFAC   SHEL   
 * SIMU   SIZE   SPEC   STIR   SUMP   SWAT   SYMM   TEMP   TITL   TWIN   
 * TWST   UNIT   WGHT   WIGL   WPDB   XNPD   ZERR 
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>

#include "ResParser.h"
#include "myExceptions.h"
#include "Restraint.h"
#include "Eqiv.h"

ResParser::ResParser(const char* resfile, const short& verbosity) : resfile_(resfile), verbosity_(verbosity), eqivs_(255) {
    // read res-file
    readres(resfile_);
    // interpret res-file, i.e. generate restraints
    interpres();

    if (verbosity > 0) {
        std::cout << "---> Number of DFIX or DANG instructions: " << restraints_.size() << "\n";
    }
    if (restraints_.empty()) {
        std::cout << " *** Error: no restraints found\n";
        throw myExcepts::Usage("Missing restraints");
    }
}

/**
 *  open res-file, read lines while concatenating continuation lines
 * include-files '+' are added just the same
 */
void ResParser::readres(const std::string& filename) {
    std::ifstream inp(filename.c_str());
    if (!inp.is_open()) {
        std::cout << "*** Error: cannot open res-file " << resfile_
                << std::endl;
        throw myExcepts::FileMissing(resfile_.c_str());
    }

    while (!inp.eof()) {
        std::string myline;
        std::getline(inp, myline);
        if (inp.eof() || inp.fail()) break;
        // check for nested input
        if (myline.empty()) continue;
        if (myline.at(0) == '+') {
            std::istringstream nested(myline);
            char plus;
            nested >> plus;
            if (myline.at(1) == '+') {
                nested >> plus;
            }
            std::string nestedfilename;
            nested >> nestedfilename;
            readres(nestedfilename);
            continue;
        }
        while (myline.back() == '=') {
            std::string contline;
            std::getline(inp, contline);
            if (inp.eof() || inp.fail()) break;
            myline += contline;
        }
        // skip empty lines
        if (!myline.empty()) {
            myline = upcase(myline);
            reslines_.push_back(myline);
        }

        continue;
    }

    inp.close();
    if (verbosity_ > 0) {
        std::cout << "---> Input of " << std::setw(5) << reslines_.size() << " lines from file "
                << filename << '\n';
    }

}

/**
 * interpret @c line as PART instruction; expects 'PART' be stripped from line
 * @param line
 */
void ResParser::part(const std::string& line) {
    std::istringstream conv(line);
    int part;
    float sof;
    conv >> part;
    if (conv.fail()) {
        std::cout << "*** Error reading PART line\n";
        throw myExcepts::ShelxFormat("Part parameter not a number");
    }
    shelxState_.partnum(part);
    conv >> sof;
    if (conv.fail()) return;
    shelxState_.partsof(sof);
}

/**
 * RESI lines are difficult, because they are allowed to contain a residue number (digits only)
 * or a residue class (four characters, first one a letter) in either order. residue number can be preceded by 
 * a chain ID, separated by a colon ':'
 * @param line
 */
void ResParser::xresi(const std::string& line) {
    std::istringstream conv(line);
    int num(0);
    std::string first, second;
    conv >> first;
    if (conv.fail()) {
        std::cout << "*** Error: RESI requires at least number of name of residue class.\n";
        throw myExcepts::ShelxFormat("RESI missing argument");
    }

    try {
        xresinum(first);
        // try for second argument
        conv >> second;
        if (conv.fail()) return; // no second argument
        else {
            shelxState_.resiclass(second);
            return;
        }
    }    catch (std::invalid_argument& e) {
        // this string is not a number - set as residue class
        shelxState_.resiclass(first);
        // try for second argument
        conv >> second;
        if (conv.fail()) return; // no second argument

        // second argument must be a residue number
        try {
            xresinum(second);
        }        catch (...) {
            // this is indeed an error, has to be 
            std::cout << "*** Error: illegal residue number in " << first << '\n';
            throw myExcepts::Format("Illegal residue number in " + first);
        }
    }    catch (myExcepts::Format& e) {
        std::cout << "*** Error: illegal residue number in " << first << '\n';
        throw;
    }

    conv >> num;
    if (conv.fail() || conv.eof()) return;

    // alias: unused
    shelxState_.resialias(num);

}
/**
 * Expects a string that represents a number, or a combination of 'ID:number', 
 * where ID is a single alpha-numerical character
 * @param line
 */
void ResParser::xresinum(const std::string& myresi) {
    int resistart(0);
    try {
        char chainid(0);

        if (myresi.length() >= 2 && myresi.at(1) == ':') { // chain ID+resinumber
            chainid = myresi.front();
            resistart = 2;
        }
        // only set states if conversion worked
        const int num = std::stoi(myresi.substr(resistart, 4));
        shelxState_.chainID(chainid);
        shelxState_.resinumber(num);
    } catch (std::invalid_argument &e) {
        // 'num' is not a number
        if (resistart == 2) throw (myExcepts::Format("Error in SHELX RESI car: not a number after colon in " + myresi));
            // fine - string that is not a number
        else throw;
    }
}

/**
 * expect a string that starts with 'CELL' and read values for
 * lambda and unit cell parameters
 * @param line
 */
void ResParser::getcell(const std::string& line) {
    std::istringstream conv(line);
    conv >> lambda_ >> a_ >> b_ >> c_
            >> alpha_ >> beta_ >> gamma_;
    if (conv.fail()) {
        std::cout << "*** Error reading wavelength and cell parameters.\n"
                << "      exiting.\n";
        throw myExcepts::Format(line.c_str());
    }
    if (verbosity_ > 0) {
        std::cout << "---> Cell constants: "
                << std::fixed
                << std::setw(9) << std::setprecision(5) << a_
                << std::setw(9) << std::setprecision(5) << b_
                << std::setw(9) << std::setprecision(5) << c_
                << std::setw(9) << std::setprecision(4) << alpha_
                << std::setw(9) << std::setprecision(4) << beta_
                << std::setw(9) << std::setprecision(4) << gamma_
                << std::endl;
    }
}

/**
 * Check res-file line by line and interpret
 */
void ResParser::interpres() {
    for (std::vector<std::string>::const_iterator it = reslines_.begin();
            it != reslines_.end(); ++it) {
        if (std::isspace(it->front())) continue;
        try {
            if (xcmd(*it)) continue;
        } catch (myExcepts::ShelxEoF& e) {
            break;
        }
        // not a shelxl command: read as atoms
        xatoms_.push_back(xAtom(*it, shelxState_));
    }
    if (verbosity_) {
        std::cout << "---> Read in " << xatoms_.size() << " atoms from ins-file\n";
    }
}

/**
 * interpret "DANG" or "DFIX" line. Sets default sof depending on DFIX (0.02) 
 * or DANG (0.04), i.e. instruction must still contain the keyword
 * @param line
 */
void ResParser::addrestraint(const std::string& line) {
    std::istringstream conv(line.substr(4, std::string::npos));
    std::string resi(""), word;
    xAtom::ResiNum resinum;
    float target, sof;
    std::vector<std::string> atom_pairs;

    float default_sof(0.02);
    if (line.substr(0, 4) == "DANG") default_sof = 0.04;
    char c = conv.peek();
    if (c == '_') conv >> c >> resi;

    try {
        resinum = xAtom::resinum(resi);
        resi = "";
    } catch (std::invalid_argument& e) {
        // nothing to do, make default residue
        resinum = xAtom::ResiNum();
    }

    conv >> target >> word;
    try {
        sof = std::stof(word);
        // if stof does not throuw an error, we can continue
        conv >> word;
    } catch (std::invalid_argument& e) { // sof not provided, word is firt atom
        sof = default_sof;
    }
    while (!conv.fail()) {
        atom_pairs.push_back(word);
        conv >> word;
    }

    // double check: need even number of atom names
    if (atom_pairs.size() % 2) {
        std::cout << "*** Error: Extracted odd number of atoms from " << line
                << "\n";
        throw myExcepts::ShelxFormat("Odd atom names in DFIX/DANG");
    }

    Restraint myrestraint(target, sof, resinum, resi, atom_pairs);
    restraints_.push_back(myrestraint);
}

/**
 * process string as shelx-command. If not a shelx command return false
 */
bool ResParser::xcmd(const std::string& instr) {
    // don't read beyond HKLF or END
    if (instr.substr(0, 3) == "END" || instr.substr(0, 4) == "HKLF") {
        throw myExcepts::ShelxEoF("END or HKLF encountered");
    }

    if (instr.substr(0, 3) == "REM") {
        return true;
    }

    // empty lines were checked earler. If shorter, cannot be a command
    if (instr.size() < 4) return false;


    const char* cmdlist[] = {"ABIN", "ACTA", "AFIX", "ANIS", "ANSC", "ANSR",
        "BASF", "BIND", "BLOC", "BOND", "BUMP", "CELL",
        "CGLS", "CHIV", "CONF", "CONN", "DAMP", "DANG",
        "DEFS", "DELU", "DFIX", "DISP", "EADP", "END",
        "EQIV", "EXTI", "EXYZ", "FEND", "FLAT", "FMAP",
        "FRAG", "FREE", "FVAR", "GRID", "HFIX", "HKLF",
        "HTAB", "ISOR", "LATT", "LAUE", "LIST", "L.S.",
        "MERG", "MORE", "MOVE", "MPLA", "NCSY", "NEUT",
        "OMIT", "PART", "PLAN", "PRIG", "REM ", "RESI",
        "RIGU", "RTAB", "SADI", "SAME", "SFAC", "SHEL",
        "SIMU", "SIZE", "SPEC", "STIR", "SUMP", "SWAT",
        "SYMM", "TEMP", "TITL", "TWIN", "TWST", "UNIT",
        "WGHT", "WIGL", "WPDB", "XNPD", "ZERR"};

    if (instr.substr(0, 4) == "EQIV") {
        Eqiv eqiv(instr.substr(4, std::string::npos));
        eqivs_[eqiv.n()] = eqiv.symop();
        return true;
    }

    if (instr.substr(0, 4) == "CELL") {
        getcell(instr.substr(4, std::string::npos));
        return true;
    }

    if (instr.substr(0, 4) == "DFIX" || instr.substr(0, 4) == "DANG") {
        addrestraint(instr);
        return true;
    }

    if (instr.substr(0, 4) == "RESI") {
        xresi(instr.substr(4, std::string::npos));
        return true;
    }

    if (instr.substr(0, 4) == "PART") {
        part(instr.substr(4, std::string::npos));
        return true;
    }

    // check for the remaining commands, not processed
    for (int i = 0; i < sizeof (cmdlist) / sizeof (cmdlist[0]); ++i) {
        if (instr.substr(0, 4) == cmdlist[i]) {
            return true;
        };
    }

    return false;
}

/**
 * Create list of numeric restraints 
 * @return 
 */
std::vector<Restraint::Numeric> ResParser::restraints() const {
    std::vector<Restraint>::const_iterator it;
    std::vector<Restraint::Numeric> numrestr(0);

    for (it = restraints_.begin(); it != restraints_.end(); ++it) {
        std::vector<Restraint::Numeric> localrestr = it->make(xatoms_, eqivs_);
        numrestr.insert(numrestr.end(), localrestr.begin(), localrestr.end());
    }
    if (verbosity_ > 0) {
        std::cout << "---> Number of restraints from res-file: " << numrestr.size() << '\n';
    }
    return numrestr;

}

std::string ResParser::upcase(const std::string& word) {
    std::string up(word);
    for (std::string::iterator it = up.begin(); it != up.end(); ++it) {
        *it = std::toupper(*it);
    }
    return up;
}