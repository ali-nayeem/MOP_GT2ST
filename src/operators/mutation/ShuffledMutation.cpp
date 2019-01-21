/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShuffledMutation.cpp
 * Author: ali_nayeem
 * 
 * Created on December 29, 2018, 2:33 PM
 */

#include <algorithm>

#include "ShuffledMutation.h"
#include "PseudoRandom.h"

ShuffledMutation::ShuffledMutation(map<string, void *> parameters) : Mutation(parameters){
    if (parameters["mutationList"] != NULL)
        mutList = *(vector <Mutation *> *)parameters["mutationList"];
    if (parameters["probability"] != NULL)
        mutationProbability_ = *(double *)parameters["probability"];
}



ShuffledMutation::~ShuffledMutation() {
}

int ShuffledMutation::random (int i)
{
    return PseudoRandom::randInt(0, i-1);
}

void * ShuffledMutation::execute(void *object)  {
  Solution *solution = (Solution *)object;
  // TODO: VALID_TYPES?
  //double probability = *(double *)getParameter("probability");
  if ( PseudoRandom::randDouble() <= mutationProbability_) {
    random_shuffle(mutList.begin(), mutList.end(), random);

    for(int i=0; i<mutList.size(); i++)
    {
        mutList[i]->execute(solution);
    }
  }
  return solution;
}