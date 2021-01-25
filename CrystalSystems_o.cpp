/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CrystalSystems.h"
#include <cmath>

/**
 * orthorhombic cell: a, b, c free, alpha = beta = gamma = 90 degree
 * @param cell
 * @param restraints
 * @return 
 */
double CrystSyst::Orthorhombic::target(const gsl_vector* cell, void* restraints) {
     gsl_vector* R = (gsl_vector*) restraints;
    
        const double a (gsl_vector_get(cell, 0));
        const double b (gsl_vector_get(cell, 1));
        const double c (gsl_vector_get(cell, 2));
    double T(0.0);
    for (size_t i = 0; i < R->size; i+=5) {
        const double dx (gsl_vector_get(R, i+0));
        const double dy (gsl_vector_get(R, i+1));
        const double dz (gsl_vector_get(R, i+2));
        const double R2 (gsl_vector_get(R, i+3));
        const double wR (gsl_vector_get(R, i+4));
        double dX2 = dx*dx*a*a + dy*dy*b*b + dz*dz*c*c;
        T += wR*(dX2 - R2)*(dX2 - R2);
    }
    return T;
}

void CrystSyst::Orthorhombic::gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradT) {
    gsl_vector* R = (gsl_vector*) restraints;
    const double a(gsl_vector_get(cell, 0));
    const double b(gsl_vector_get(cell, 1));
    const double c(gsl_vector_get(cell, 2));

    double dTda(0), dTdb(0), dTdc(0);
    for (size_t i = 0; i < R->size; i += 5) {
        const double dx(gsl_vector_get(R, i + 0));
        const double dy(gsl_vector_get(R, i + 1));
        const double dz(gsl_vector_get(R, i + 2));
        const double R2(gsl_vector_get(R, i + 3));
        const double wR(gsl_vector_get(R, i + 4));

        double dX2 = dx * dx * a * a + dy * dy * b * b + dz * dz * c * c;

        double dta = 2 * dx * dx * a;
        double dtb = 2 * dy * dy * b;
        double dtc = 2 * dz * dz * c;

        dTda += 2 * wR * (dX2 - R2) * dta;
        dTdb += 2 * wR * (dX2 - R2) * dtb;
        dTdc += 2 * wR * (dX2 - R2) * dtc;
    }
    gsl_vector_set(gradT, 0, dTda);
    gsl_vector_set(gradT, 1, dTdb);
    gsl_vector_set(gradT, 2, dTdc);

}

void CrystSyst::Orthorhombic::TgradT(const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT) {
        gsl_vector* R = (gsl_vector*) restraints;

    const double a (gsl_vector_get(cell, 0));
    const double b (gsl_vector_get(cell, 1));
    const double c (gsl_vector_get(cell, 2));
   
    *T = 0.0;
    double dTda(0), dTdb(0), dTdc(0);
    for (size_t i = 0; i < R->size; i += 5) {
        const double dx(gsl_vector_get(R, i + 0));
        const double dy(gsl_vector_get(R, i + 1));
        const double dz(gsl_vector_get(R, i + 2));
        const double R2(gsl_vector_get(R, i + 3));
        const double wR(gsl_vector_get(R, i + 4));

        double dX2 = dx * dx * a * a + dy * dy * b * b + dz * dz * c * c;

        double dta = 2 * dx * dx * a;
        double dtb = 2 * dy * dy * b;
        double dtc = 2 * dz * dz * c;

        const double dX2R2 (dX2 - R2);
        *T       += wR*dX2R2*dX2R2;
        dTda     += 2 * wR*dX2R2 * dta;
        dTdb     += 2 * wR*dX2R2 * dtb;
        dTdc     += 2 * wR*dX2R2 * dtc;
    }
    gsl_vector_set(gradT, 0, dTda);
    gsl_vector_set(gradT, 1, dTdb);
    gsl_vector_set(gradT, 2, dTdc);
}

