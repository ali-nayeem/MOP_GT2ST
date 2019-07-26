/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MOEAD_ST.h
 * Author: ali_nayeem
 *
 * Created on June 5, 2019, 9:02 AM
 */

#ifndef MOEAD_ST_H
#define MOEAD_ST_H

#include "MOEAD.h"
#include "Checkpoint.h"


class MOEAD_ST : public MOEAD{
public:
    MOEAD_ST(Problem * problem, Checkpoint *checkpoint);
    SolutionSet * execute();
    
    //MOEAD_ST(const MOEAD_ST& orig);
    //virtual ~MOEAD_ST();
protected:
    void initPopulation();
    Checkpoint *checkpoint_; 
};

#endif /* MOEAD_ST_H */

