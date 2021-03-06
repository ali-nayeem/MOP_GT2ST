/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NSGAII_ST_MidDistRank.h
 * Author: ali_nayeem
 *
 * Created on January 11, 2019, 11:38 AM
 */

#ifndef NSGAII_ST_MID_DISTANCE_RANK_H
#define NSGAII_ST_MID_DISTANCE_RANK_H

#include <Algorithm.h>
#include <Problem.h>
#include <SolutionSet.h>
#include <Distance.h>
#include <MidDistanceRanking.h>
#include <CrowdingComparator.h>
#include "Checkpoint.h"

class NSGAII_ST_MidDistRank : public Algorithm {

public:
  NSGAII_ST_MidDistRank(Problem * problem);
  SolutionSet * execute();
private:
  Checkpoint *checkpoint_;    

};

#endif /* NSGAII_ST_MidDistRank_H */

