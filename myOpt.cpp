/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   myOpt.cpp
 * Author: tg
 * 
 * Created on October 10, 2020, 8:51 PM
 */

#include "myOpt.h"

#include "CrystalSystems.h"
#include "myExceptions.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <complex>

myOpt::myOpt(const ResParser& resdata, const char crystsystem, const double steps,
        const double tol, const double eps, const int maxIter, const short& verbosity) :
steps_(steps), tolerance_(tol), eps_(eps), maxIter_(maxIter), crystalSystem_(crystsystem),
verbosity_(verbosity) {

    switch (crystsystem) {
        case 'a':
            cell_ = gsl_vector_alloc(6);

            gsl_vector_set(cell_, 0, resdata.a());
            gsl_vector_set(cell_, 1, resdata.b());
            gsl_vector_set(cell_, 2, resdata.c());
            gsl_vector_set(cell_, 3, M_PI / 180. * resdata.alpha());
            gsl_vector_set(cell_, 4, M_PI / 180. * resdata.beta());
            gsl_vector_set(cell_, 5, M_PI / 180. * resdata.gamma());

            fdf_.n = (cell_)->size; // a, b, c, alpha, beta, gamma
            fdf_.f = CrystSyst::Triclinic::target;
            fdf_.df = CrystSyst::Triclinic::gradT;
            fdf_.fdf = CrystSyst::Triclinic::TgradT;
            break;
        case 'm':
            cell_ = gsl_vector_alloc(4);
            gsl_vector_set(cell_, 0, resdata.a());
            gsl_vector_set(cell_, 1, resdata.b());
            gsl_vector_set(cell_, 2, resdata.c());
            gsl_vector_set(cell_, 3, M_PI / 180. * resdata.beta());

            fdf_.n = cell_->size;
            fdf_.f = CrystSyst::Monoclinic::target;
            fdf_.df = CrystSyst::Monoclinic::gradT;
            fdf_.fdf = CrystSyst::Monoclinic::TgradT;
            break;

        case 'o':
            cell_ = gsl_vector_alloc(3);
            gsl_vector_set(cell_, 0, resdata.a());
            gsl_vector_set(cell_, 1, resdata.b());
            gsl_vector_set(cell_, 2, resdata.c());

            fdf_.n = cell_->size;
            fdf_.f = CrystSyst::Orthorhombic::target;
            fdf_.df = CrystSyst::Orthorhombic::gradT;
            fdf_.fdf = CrystSyst::Orthorhombic::TgradT;
            break;

        case 't':
            cell_ = gsl_vector_alloc(2);
            gsl_vector_set(cell_, 0, resdata.a());
            gsl_vector_set(cell_, 1, resdata.c());

            fdf_.n = cell_->size;
            fdf_.f = CrystSyst::Tetragonal::target;
            fdf_.df = CrystSyst::Tetragonal::gradT;
            fdf_.fdf = CrystSyst::Tetragonal::TgradT;
            break;

        case 'h':
            cell_ = gsl_vector_alloc(2);
            gsl_vector_set(cell_, 0, resdata.a());
            gsl_vector_set(cell_, 1, resdata.c());

            fdf_.n = cell_->size;
            fdf_.f = CrystSyst::Hexagonal::target;
            fdf_.df = CrystSyst::Hexagonal::gradT;
            fdf_.fdf = CrystSyst::Hexagonal::TgradT;
            break;

        case 'c':
            cell_ = gsl_vector_alloc(1);
            gsl_vector_set(cell_, 0, resdata.a());

            fdf_.n = cell_->size;
            fdf_.f = CrystSyst::Cubic::target;
            fdf_.df = CrystSyst::Cubic::gradT;
            fdf_.fdf = CrystSyst::Cubic::TgradT;
            break;

        default:
            std::cout << "Unknown Crystal System, number " << crystsystem << std::endl;
            throw myExcepts::Usage("Unknown Crystal System, should be between 0 and 5");
            break;
    }

    const std::vector<Restraint::Numeric> numrestraints(resdata.restraints());
    if (numrestraints.empty()) {
        std::cout << "*** Error: no restraint atom pairs found.\n"
                << "*** Make sure there are no typos, and resi names are correct.\n";
        throw myExcepts::Usage("No restrained atom pairs");
    }
    // create a vector sum of all restraints
    if (verbosity_ > 1) {
        XYZ sumr (0,0,0), wsumr(0,0,0);
        double sumweights (0.0);
        for (auto it = numrestraints.begin(); it != numrestraints.end(); ++it) {
            sumr += it->dX_;
            wsumr += it->weight_ * it->dX_;
            sumweights += it->weight_;
        }
        wsumr *= 1.0/sumweights;
        wsumr *= 1./std::sqrt(wsumr.norm2());
        sumr  *= 1.0/numrestraints.size();
        sumr  *= 1./std::sqrt(sumr.norm2());
        std::cout << "---> Normalised vector sum of restraints vectors:          " << sumr << '\n'
                << "     Normalised vector sum of weighted restraints vectors: " << wsumr << '\n';
    }

    // make numeric restraints, 
    gslrestr_ = gsl_vector_alloc(5 * numrestraints.size());
    double W(0.0);
    for (std::vector<Restraint::Numeric>::const_iterator it = numrestraints.begin(); it != numrestraints.end(); ++it) {
        size_t idx = 5 * (it - numrestraints.begin());
        gsl_vector_set(gslrestr_, idx + 0, it->dX_.x());
        gsl_vector_set(gslrestr_, idx + 1, it->dX_.y());
        gsl_vector_set(gslrestr_, idx + 2, it->dX_.z());
        gsl_vector_set(gslrestr_, idx + 3, it->targetsq_);
        gsl_vector_set(gslrestr_, idx + 4, it->weight_);
        W += it->weight_;
    }
    // normalise weights
    W = 1.0 / W;
    for (size_t idx = 0; idx < (gslrestr_)->size; idx += 5) {
        gsl_vector_set(gslrestr_, idx + 4, W * gsl_vector_get(gslrestr_, idx + 4));
    }
    fdf_.params = (void*) gslrestr_;
}

double myOpt::optimise() {
    int iter(0);
    // define target function

    const gsl_multimin_fdfminimizer_type *optType = gsl_multimin_fdfminimizer_vector_bfgs2;
    gsl_multimin_fdfminimizer* minimiser = gsl_multimin_fdfminimizer_alloc(optType, fdf_.n);

    gsl_multimin_fdfminimizer_set(minimiser, &fdf_, cell_, steps_, tolerance_);

    // run the mininisation
    int status;
    do {
        ++iter;
        status = gsl_multimin_fdfminimizer_iterate(minimiser);
        // an error occurred
        if (status) break;
        status = gsl_multimin_test_gradient(minimiser->gradient, eps_);
        if (status == GSL_SUCCESS) break;
    } while (status == GSL_CONTINUE && iter < maxIter_);

    if (status == GSL_ENOPROG && verbosity_ > 0) {
        std::cout << "---> GSL Optimisation: local minimum reached after " << iter << " iterations.\n";
    } else {
        if (verbosity_ > 0) {
            std::cout << "---> Status value: " << status << " after " << iter << " iterations\n";
        }
    }

    gsl_vector_memcpy(cell_, gsl_multimin_fdfminimizer_x(minimiser));
    const double tgt_value = gsl_multimin_fdfminimizer_minimum(minimiser);

    gsl_multimin_fdfminimizer_free(minimiser);

    // update cell with minimum
    return tgt_value;
}

myOpt::~myOpt() {
    gsl_vector_free(cell_);
    gsl_vector_free(gslrestr_);
}
/**
 * Information about current target value.
 * TODO: differentiate between crystal systems
 */
void myOpt::info() const {
    /* Information */
    switch (crystalSystem_) {
        case 'a':
            std::cout << "---> Triclinic crystal system\n"
                    << "---> Cell parameters (a, b, c, alpha, beta, gamma) \n    "
                    << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << std::setw(9) << gsl_vector_get(cell_, 2)
                    << std::setprecision(4)
                    << std::setw(9) << 180. / M_PI * gsl_vector_get(cell_, 3)
                    << std::setw(9) << 180. / M_PI * gsl_vector_get(cell_, 4)
                    << std::setw(9) << 180. / M_PI * gsl_vector_get(cell_, 5)
                    << "\n";
            break;
        case 'm':
            std::cout << "---> Monoclinic crystal system\n"
                    << "---> Cell parameters (a, b, c, 90, beta, 90) \n    "
                    << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << std::setw(9) << gsl_vector_get(cell_, 2)
                    << " 90.0000"
                    << std::setprecision(4) << std::setw(9) << 180. / M_PI * gsl_vector_get(cell_, 3)
                    << " 90.0000"
                    << "\n";
            break;
        case 'o':
            std::cout << "---> Orthorhombic crystal system\n"
                    << "---> Cell parameters (a, b, c, 90, 90, 90) \n    "
                    << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << std::setw(9) << gsl_vector_get(cell_, 2)
                    << " 90.00 90.00 90.00"
                    << "\n";
            break;
        case 'h':
            std::cout << "---> Hexagonal crystal system\n"
                    << "---> Cell parameters (a = b, c, 90, 90, 120) \n    "
                    << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << " 90.00 90.00 120.00"
                    << "\n";
            break;
        case 't':
            std::cout << "---> Tetragonal crystal system\n"
                    << "---> Cell parameters (a = b, c, 90, 90, 90) \n    "
                    << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << " 90.00 90.00 90.00"
                    << "\n";
            break;
        case 'c':
            std::cout << "---> Tetragonal crystal system\n"
                    << "---> Cell parameters (a = b = c, 90, 90, 90) \n    "
                    << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << " 90.00 90.00 90.00"
                    << "\n";
            break;
        default: throw myExcepts::Format("Illegal Crystal System");
            break;
    }
    std::cout << "---> Target value: "
            << fdf_.f(cell_, (void*) gslrestr_)
            << std::endl;

}

void myOpt::settings() const {
    std::cout << "---> Optimisation parameters: \n"
            << std::setprecision(7)
            << "     Gradient cut-off (eps):  " << std::scientific << eps_ << "\n"
            << "     Maximum number of steps: " << maxIter_ << "\n"
            << "     Initial step size:       " << std::fixed << steps_ << "\n"
            << "     Tolerance:               " << tolerance_ << "\n"
            << "     Crystal system:          " << crystalSystem_ << "\n";

}

std::string myOpt::cell_xlformat(const float& lambda) const {
    std::ostringstream outp;
    outp << "CELL " << lambda;
    switch (crystalSystem_) {
        case 'a':
            outp << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << std::setw(9) << gsl_vector_get(cell_, 2)
                    << std::setprecision(4)
                    << std::setw(9) << 180. / M_PI * gsl_vector_get(cell_, 3)
                    << std::setw(9) << 180. / M_PI * gsl_vector_get(cell_, 4)
                    << std::setw(9) << 180. / M_PI * gsl_vector_get(cell_, 5)
                    << "\n";
            break;
        case 'm':
            outp
                    << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << std::setw(9) << gsl_vector_get(cell_, 2)
                    << " 90.0000"
                    << std::setprecision(4) << std::setw(9) << 180. / M_PI * gsl_vector_get(cell_, 3)
                    << " 90.0000"
                    << "\n";
            break;
        case 'o':
            outp << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << std::setw(9) << gsl_vector_get(cell_, 2)
                    << " 90.00 90.00 90.00"
                    << "\n";
            break;
        case 'h':
            outp << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << " 90.00 90.00 120.00"
                    << "\n";
            break;
        case 't':
            outp << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 1)
                    << " 90.00 90.00 90.00"
                    << "\n";
            break;
        case 'c':
            outp << std::fixed << std::setprecision(5)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << std::setw(9) << gsl_vector_get(cell_, 0)
                    << " 90.00 90.00 90.00"
                    << "\n";
            break;
        default: throw myExcepts::Format("Illegal Crystal System");
            break;
    }
    outp << "REM Target value " << fdf_.f(cell_, (void*) gslrestr_) << '\n';
    return outp.str();
}