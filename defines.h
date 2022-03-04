/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   defines.h
 * Author: tg
 *
 * Created on June 12, 2020, 9:45 AM
 */

#ifndef DEFINES_H
#define DEFINES_H

namespace Defs {
    namespace GSL {
        //! initial step size (small, cell should already be close to min
        const double step (0.01);
        //! tolerance of BFGS2 minimisation algorithm. 0.1 recommended
        const double tol  (0.1);
        //! gradient cut-off for stable minimum (expected precision)
        const double eps_grad(1e-9);
        //! maximum number of iterations
        const int maxIter (500);
    }
}


#endif /* DEFINES_H */

