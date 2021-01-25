/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Eqiv.cpp
 * Author: tg
 * 
 * Created on June 9, 2020, 9:43 AM
 */

#include "Eqiv.h"
#include "myExceptions.h"

#include <iostream>

/**
 * expect a shelx instruction without the leading 'EQIV':
 * $n x', y', z'
 * @param instr
 */
Eqiv::Eqiv(const std::string instr) {
    char c;
    char X[20], Y[20], Z[20];
    std::istringstream conv(instr);
    conv >> c >> n_;
    if (c != '$' || conv.fail()) {
        std::cout << "*** Error: EQIV expect '$n' first\n";
        throw myExcepts::ShelxFormat("EQIV $n missing");
    }
    conv.getline(X, 20, ',');
    conv.getline(Y, 20, ',');
    conv.getline(Z, 20, ',');
    if (conv.fail()) {
        std::cout << "*** Error: EQIV expects comma-separated list of operators\n";
        throw myExcepts::ShelxFormat("EQIV not comma-separated");
    }
    std::vector<float> xline = tosymop(X);
    std::vector<float> yline = tosymop(Y);
    std::vector<float> zline = tosymop(Z);
    xline.insert(xline.end(), yline.begin(), yline.end());
    xline.insert(xline.end(), zline.begin(), zline.end());
    S_ = SymOp(xline);
}

/**
 * extract symmetry information from string
 * @param X
 * @return 
 */
std::vector<float> Eqiv::tosymop(std::string X) {
    std::vector<float> trafo(4, {0.0});
    size_t found;
    // strip white space
    while ((found = X.find(" ")) != std::string::npos) {
        X.replace(found, 1, "");
    }
    // convert to lower x, y, z
    while ((found = X.find("X")) != std::string::npos) {
        X.replace(found, 1, "x");
    }
    while ((found = X.find("Y")) != std::string::npos) {
        X.replace(found, 1, "y");
    }
    while ((found = X.find("Z")) != std::string::npos) {
        X.replace(found, 1, "z");
    }

    // First entry for X'
    if ((found = X.find("-x")) != std::string::npos) {
        trafo[0] = -1;
        X.erase(found, 2);
    } else if ((found = X.find("+x")) != std::string::npos) {
        trafo[0] = 1;
        X.erase(found, 2);
    } else if ((found = X.find("x")) != std::string::npos) {
        trafo[0] = 1;
        X.erase(found, 1);
    } else trafo[0] = 0;

    // Second entry for Y'
    if ((found = X.find("-y")) != std::string::npos) {
        trafo[1] = -1;
        X.erase(found, 2);
    } else if ((found = X.find("+y")) != std::string::npos) {
        trafo[1] = 1;
        X.erase(found, 2);
    } else if ((found = X.find("y")) != std::string::npos) {
        trafo[1] = 1;
        X.erase(found, 1);
    } else trafo[1] = 0;

    // Third entry for Z'
    if ((found = X.find("-z")) != std::string::npos) {
        trafo[2] = -1;
        X.erase(found, 2);
    } else if ((found = X.find("+z")) != std::string::npos) {
        trafo[2] = 1;
        X.erase(found, 2);
    } else if ((found = X.find("z")) != std::string::npos) {
        trafo[2] = 1;
        X.erase(found, 1);
    } else trafo[2] = 0;

    // the rest of X should now be just a number, but can contain a slash
    if (X.empty()) trafo[3] = 0;
    else {
        float scale(1.0);
        size_t slash = X.find('/');
        size_t sign = X.find('-');
        float z, n(1.0);
        if (sign != std::string::npos) {
            scale = -1.0;
        } else {
            sign = X.find('+');
        }
        if (slash != std::string::npos) {
            n = std::stof(X.substr(slash + 1, std::string::npos));
        }
        if (sign == std::string::npos) {
            z = std::stof(X.substr(0, slash));
        } else {
            z = std::stof(X.substr(sign + 1, slash));
        }

        trafo[3] = scale * z / n;
    }
    return trafo;
}
