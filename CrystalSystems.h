/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CrystalSystems.h
 * Author: tg
 *
 * Created on October 8, 2020, 3:24 PM
 */

#ifndef CRYSTALSYSTEMS_H
#define CRYSTALSYSTEMS_H

#include <gsl/gsl_vector.h>

namespace CrystSyst {
    /**
     * no constraints, vary a, b, c, alpha, beta, gamma
     */
    namespace Triclinic {
        double target(const gsl_vector* cell, void* restraints);
        void   gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradT);
        void   TgradT (const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT);
    }
    /**
     * alpha = gamma = 90 deg, vary a, b, c, beta
     */
    namespace Monoclinic {
        double target(const gsl_vector* cell, void* restraints);
        void   gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradient);
        void   TgradT (const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT);
    }
    /**
     * alpha = beta = gamma = 90 deg, vary a, b, c
     */
    namespace Orthorhombic {
        double target(const gsl_vector* cell, void* restraints);
        void   gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradient);
        void   TgradT (const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT);
    }
    /**
     * a = b and alpha = beta = gamma = 90; vary a, c
     */
    namespace Tetragonal {
        double target(const gsl_vector* cell, void* restraints);
        void   gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradient);
        void   TgradT (const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT);
    }
    /**
     * a = b and alpha = beta = 90 deg and gamma = 120 deg; vary a, c
     */
    namespace Hexagonal {
        double target(const gsl_vector* cell, void* restraints);
        void   gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradient);
        void   TgradT (const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT);
    }
    /**
     * a = b = c and alpha = beta = gamma = 90 deg; vary a
     */
    namespace Cubic {
        double target(const gsl_vector* cell, void* restraints);
        void   gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradient);
        void   TgradT (const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT);
    }
}


#endif /* CRYSTALSYSTEMS_H */

