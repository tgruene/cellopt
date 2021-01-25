/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShelxState.h
 * Author: tg
 *
 * Created on June 8, 2020, 3:59 PM
 */

#ifndef SHELXSTATE_H
#define SHELXSTATE_H

#include <string>

class ShelxState {
private:
    int partnum_;
    float partsof_;
    std::string resiclass_;
    int resinumber_;
    int resialias_;
    

public:
    ShelxState();
    ~ShelxState(){}
    
    //! setters
    int& partnum(int p) { partnum_ = p; return partnum_;}
    float& partsof(float s) { partsof_ = s; return partsof_; }
    std::string& resiclass (const std::string& r) { resiclass_ = r; return resiclass_; }
    int& resinumber (int r) { resinumber_ = r; return resinumber_; }
    int& resialias (int r) { resialias_ = r; return resialias_; }
    
    //! getters
    int partnum() const { return partnum_; }
    float partsof() const { return partsof_; }
    std::string resiclass() const { return resiclass_;}
    int resinumber() const { return resinumber_; }
    int resialias() const { return resialias_; }
    

};

#endif /* SHELXSTATE_H */

