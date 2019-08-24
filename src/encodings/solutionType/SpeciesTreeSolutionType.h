/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SpeciesTreeSolutionType.h
 * Author: ali_nayeem
 *
 * Created on December 28, 2018, 10:20 PM
 */

#ifndef SPECIESTREESOLUTIONTYPE_H
#define SPECIESTREESOLUTIONTYPE_H

#include <SolutionType.h>

class SpeciesTreeSolutionType : public SolutionType {
public:
    SpeciesTreeSolutionType(Problem *problem);
    Variable **createVariables();
    ~SpeciesTreeSolutionType();
private:

};

#endif /* SPECIESTREESOLUTIONTYPE_H */

