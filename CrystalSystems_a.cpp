/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CrystalSystems.h"
#include <cmath>

/**
 * triclinic cell: all a, b, c, alpha, beta, gamma free
 * @param cell
 * @param restraints
 * @return 
 */
double CrystSyst::Triclinic::target(const gsl_vector* cell, void* restraints) {
     gsl_vector* R = (gsl_vector*) restraints;
    
        const double a (gsl_vector_get(cell, 0));
        const double b (gsl_vector_get(cell, 1));
        const double c (gsl_vector_get(cell, 2));
        const double cosalpha (std::cos(gsl_vector_get(cell, 3)));
        const double cosbeta (std::cos(gsl_vector_get(cell, 4)));
        const double cosgamma (std::cos(gsl_vector_get(cell, 5)));
    double T(0.0);
    for (size_t i = 0; i < R->size; i+=5) {
        const double dx (gsl_vector_get(R, i+0));
        const double dy (gsl_vector_get(R, i+1));
        const double dz (gsl_vector_get(R, i+2));
        const double R2 (gsl_vector_get(R, i+3));
        const double wR (gsl_vector_get(R, i+4));
        double dX2 = dx*dx*a*a + dy*dy*b*b + dz*dz*c*c 
                + 2*dx*dy*a*b*cosgamma
                + 2*dx*dz*a*c*cosbeta
                + 2*dy*dz*b*c*cosalpha;
        T += wR*(dX2 - R2)*(dX2 - R2);
    }
    return T;
}

void CrystSyst::Triclinic::gradT(const gsl_vector* cell, void* restraints, gsl_vector* gradT) {
    gsl_vector* R = (gsl_vector*) restraints;
    const double a(gsl_vector_get(cell, 0));
    const double b(gsl_vector_get(cell, 1));
    const double c(gsl_vector_get(cell, 2));
    const double cosalpha(std::cos(gsl_vector_get(cell, 3)));
    const double cosbeta(std::cos(gsl_vector_get(cell, 4)));
    const double cosgamma(std::cos(gsl_vector_get(cell, 5)));
    const double sinalpha(std::sin(gsl_vector_get(cell, 3)));
    const double sinbeta(std::sin(gsl_vector_get(cell, 4)));
    const double singamma(std::sin(gsl_vector_get(cell, 5)));

    double dTda(0), dTdb(0), dTdc(0), dTdalpha(0), dTdbeta(0), dTdgamma(0);
    for (size_t i = 0; i < R->size; i += 5) {
        const double dx(gsl_vector_get(R, i + 0));
        const double dy(gsl_vector_get(R, i + 1));
        const double dz(gsl_vector_get(R, i + 2));
        const double R2(gsl_vector_get(R, i + 3));
        const double wR(gsl_vector_get(R, i + 4));

        double dX2 = dx * dx * a * a + dy * dy * b * b + dz * dz * c * c
                + 2 * dx * dy * a * b * cosgamma
                + 2 * dx * dz * a * c * cosbeta
                + 2 * dy * dz * b * c*cosalpha;

        double dta = 2 * dx * dx * a
                + 2 * dx * dy * b * cosgamma
                + 2 * dx * dz * c * cosbeta;
        double dtb = 2 * dy * dy * b
                + 2 * dx * dy * a * cosgamma
                + 2 * dy * dz * c * cosalpha;
        double dtc = 2 * dz * dz * c
                + 2 * dx * dz * a * cosbeta
                + 2 * dy * dz * b * cosalpha;
        double dtalpha = -2 * dy * dz * b * c * sinalpha;
        double dtbeta  = -2 * dx * dz * a * c * sinbeta;
        double dtgamma = -2 * dx * dy * a * b * singamma;

        dTda += 2 * wR * (dX2 - R2) * dta;
        dTdb += 2 * wR * (dX2 - R2) * dtb;
        dTdc += 2 * wR * (dX2 - R2) * dtc;
        dTdalpha += 2 * wR * (dX2 - R2) * dtalpha;
        dTdbeta  += 2 * wR * (dX2 - R2) * dtbeta;
        dTdgamma += 2 * wR * (dX2 - R2) * dtgamma;
    }
    gsl_vector_set(gradT, 0, dTda);
    gsl_vector_set(gradT, 1, dTdb);
    gsl_vector_set(gradT, 2, dTdc);
    gsl_vector_set(gradT, 3, dTdalpha);
    gsl_vector_set(gradT, 4, dTdbeta);
    gsl_vector_set(gradT, 5, dTdgamma);

}

void CrystSyst::Triclinic::TgradT(const gsl_vector* cell, void* restraints, double* T, gsl_vector* gradT) {
        gsl_vector* R = (gsl_vector*) restraints;

    const double a (gsl_vector_get(cell, 0));
    const double b (gsl_vector_get(cell, 1));
    const double c (gsl_vector_get(cell, 2));
    const double cosalpha (std::cos(gsl_vector_get(cell, 3)));
    const double cosbeta (std::cos(gsl_vector_get(cell, 4)));
    const double cosgamma (std::cos(gsl_vector_get(cell, 5)));
    const double sinalpha (std::sin(gsl_vector_get(cell, 3)));
    const double sinbeta (std::sin(gsl_vector_get(cell, 4)));
    const double singamma (std::sin(gsl_vector_get(cell, 5)));
   
    *T = 0.0;
    double dTda(0), dTdb(0), dTdc(0), dTdalpha(0), dTdbeta(0), dTdgamma(0);
    for (size_t i = 0; i < R->size; i += 5) {
        const double dx(gsl_vector_get(R, i + 0));
        const double dy(gsl_vector_get(R, i + 1));
        const double dz(gsl_vector_get(R, i + 2));
        const double R2(gsl_vector_get(R, i + 3));
        const double wR(gsl_vector_get(R, i + 4));

        double dX2 = dx * dx * a * a + dy * dy * b * b + dz * dz * c * c
                + 2 * dx * dy * a * b * cosgamma
                + 2 * dx * dz * a * c * cosbeta
                + 2 * dy * dz * b * c*cosalpha;

        double dta = 2 * dx * dx * a
                + 2 * dx * dy * b * cosgamma
                + 2 * dx * dz * c * cosbeta;
        double dtb = 2 * dy * dy * b
                + 2 * dx * dy * a * cosgamma
                + 2 * dy * dz * c * cosalpha;
        double dtc = 2 * dz * dz * c
                + 2 * dx * dz * a * cosbeta
                + 2 * dy * dz * b * cosalpha;
        double dtalpha = -2 * dy * dz * b * c * sinalpha;
        double dtbeta  = -2 * dx * dz * a * c * sinbeta;
        double dtgamma = -2 * dx * dy * a * b * singamma;

        const double dX2R2 (dX2 - R2);
        *T       += wR*dX2R2*dX2R2;
        dTda     += 2 * wR*dX2R2 * dta;
        dTdb     += 2 * wR*dX2R2 * dtb;
        dTdc     += 2 * wR*dX2R2 * dtc;
        dTdalpha += 2 * wR*dX2R2 * dtalpha;
        dTdbeta  += 2 * wR*dX2R2 * dtbeta;
        dTdgamma += 2 * wR*dX2R2 * dtgamma;
    }
    gsl_vector_set(gradT, 0, dTda);
    gsl_vector_set(gradT, 1, dTdb);
    gsl_vector_set(gradT, 2, dTdc);
    gsl_vector_set(gradT, 3, dTdalpha);
    gsl_vector_set(gradT, 4, dTdbeta);
    gsl_vector_set(gradT, 5, dTdgamma);
}

