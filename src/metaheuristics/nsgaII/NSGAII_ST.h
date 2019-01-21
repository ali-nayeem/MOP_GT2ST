/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NSGAII_ST.h
 * Author: ali_nayeem
 *
 * Created on January 11, 2019, 11:38 AM
 */

#ifndef NSGAII_ST_H
#define NSGAII_ST_H

#include <Algorithm.h>
#include <Problem.h>
#include <SolutionSet.h>
#include <Distance.h>
#include <Ranking.h>
#include <CrowdingComparator.h>

class NSGAII_ST : public Algorithm {

public:
  NSGAII_ST(Problem * problem);
  SolutionSet * execute();

};

#endif /* NSGAII_ST_H */

