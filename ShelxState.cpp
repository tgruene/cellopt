/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShelxState.cpp
 * Author: tg
 * 
 * Created on June 8, 2020, 3:59 PM
 */

#include "ShelxState.h"
#include <cmath>

ShelxState::ShelxState():
partnum_(0), 
        partsof_(std::nan("")), 
        resinumber_(0), 
        resiclass_(""), 
        resialias_(0) {
}

