/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NSGAIII_ST_NO_PARETO_H.h
 * Author: ali_nayeem
 *
 * Created on January 11, 2019, 11:38 AM
 */

#ifndef NSGAIII_ST_DIST_PARETO_H
#define NSGAIII_ST_DIST_PARETO_H

#include <NSGAIII_ST_NO_PARETO.h>
#include <Problem.h>
#include <SolutionSet.h>
#include <Distance.h>
#include <Ranking.h>
#include <CrowdingComparator.h>
#include "Checkpoint.h"
#include "alg_reference_point.h"
#include "alg_environmental_selection.h"

class NSGAIII_ST_DIST_PARETO : public NSGAIII_ST_NO_PARETO
{
public:
  NSGAIII_ST_DIST_PARETO(Problem *problem);
  SolutionSet *execute();

protected:
  //   Checkpoint *checkpoint_;
  //   std::vector<std::size_t> obj_division_p_;
  //   //void Assignment(SolutionSet *population, int populationSize, vector<CReferencePoint> rps);
  void Niching(SolutionSet *population, int populationSize, vector<CReferencePoint> rps, Ranking *ranking, int lastFrontRank, InferSpeciesTree *p);
};

#endif /* NSGAIII_ST_DIST_PARETO_H */
