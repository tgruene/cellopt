/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CrystalSystems.h"
#include <cmath>

/**
 * hexagonal cell:  a = b and alpha = beta = 90 deg and gamma = 120 deg; vary a, c
 * cos(90) = 0, cos(120) = -0.5
 * @param cell
 * @param restraints
 * @return 
 */
double CrystSyst::Hexagonal::target(const gsl_vector* cell, void* restraints) {
     gsl_vector* R = (gsl_vector*) restraints;
    
        const double a (gsl_vector_get(cell, 0));
        const double c (gsl_vector_get(cell, 1));
    double T(0.0);
    for (size_t i = 0; i < R->size; i+=5) {
        const double dx (gsl_vector_get(R, i+0));
        const double dy (gsl_vector_get(R, i+1));
        const double dz (gsl_vector_get(R, i+2));
        const double R2 (gsl_vector_get(R, i+3));
        const double wR (gsl_vector_get(R, i+4));
        double dX2 = (dx*dx+dy*dy)*a*a + dz*dz*c*c 
                - dx*dy*a*a;
        T += wR*(dX2 - R2)*(dX2 - R2);
    }
    return T;
}

void CrystSyst::Hexagonal::gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradT) {
    gsl_vector* R = (gsl_vector*) restraints;
    const double a(gsl_vector_get(cell, 0));
    const double c(gsl_vector_get(cell, 1));

    double dTda(0), dTdc(0);
    for (size_t i = 0; i < R->size; i += 5) {
        const double dx(gsl_vector_get(R, i + 0));
        const double dy(gsl_vector_get(R, i + 1));
        const double dz(gsl_vector_get(R, i + 2));
        const double R2(gsl_vector_get(R, i + 3));
        const double wR(gsl_vector_get(R, i + 4));

        double dX2 = (dx * dx +dy*dy) * a * a + dz * dz * c * c
                - dx * dy * a * a; 

        double dta = 2*(dx * dx + dy * dy - dx * dy)* a ;
        double dtc = 2 * dz * dz * c;

        dTda += 2 * wR * (dX2 - R2) * dta;
        dTdc += 2 * wR * (dX2 - R2) * dtc;
    }
    gsl_vector_set(gradT, 0, dTda);
    gsl_vector_set(gradT, 1, dTdc);

}

void CrystSyst::Hexagonal::TgradT(const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT) {
        gsl_vector* R = (gsl_vector*) restraints;

    const double a (gsl_vector_get(cell, 0));
    const double c (gsl_vector_get(cell, 1));
   
    *T = 0.0;
    double dTda(0), dTdc(0);
    for (size_t i = 0; i < R->size; i += 5) {
        const double dx(gsl_vector_get(R, i + 0));
        const double dy(gsl_vector_get(R, i + 1));
        const double dz(gsl_vector_get(R, i + 2));
        const double R2(gsl_vector_get(R, i + 3));
        const double wR(gsl_vector_get(R, i + 4));

        double dX2 = (dx * dx + dy*dy)* a * a + dz * dz * c * c
                - dx * dy * a * a;

        double dta = 2*(dx * dx + dy * dy - dx * dy)* a ;
        double dtc = 2 * dz * dz * c;

        const double dX2R2 (dX2 - R2);
        *T       += wR*dX2R2*dX2R2;
        dTda     += 2 * wR*dX2R2 * dta;
        dTdc     += 2 * wR*dX2R2 * dtc;
    }
    gsl_vector_set(gradT, 0, dTda);
    gsl_vector_set(gradT, 1, dTdc);
}

