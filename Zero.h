/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Zero.h
 * Author: tg
 *
 * Created on April 1, 2021, 9:49 PM
 */

#ifndef ZERO_H
#define ZERO_H

#include <ostream>
#include <iostream>



namespace Log {
class Zero {
private:
    std::ostream outp_;
public:
    Zero() {outp_ = std::cout;}

};

}
#endif /* ZERO_H */

