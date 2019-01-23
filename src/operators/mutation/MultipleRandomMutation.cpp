/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MultipleRandomMutation.cpp
 * Author: ali_nayeem
 * 
 * Created on December 29, 2018, 2:33 PM
 */

#include "MultipleRandomMutation.h"

MultipleRandomMutation::MultipleRandomMutation(map<string, void *> parameters) : Mutation(parameters){
    if (parameters["mutationList"] != NULL)
        mutList = *(vector <Mutation *> *)parameters["mutationList"];
    if (parameters["probability"] != NULL)
        mutationProbability_ = *(double *)parameters["probability"];
}



MultipleRandomMutation::~MultipleRandomMutation() {
}

void * MultipleRandomMutation::execute(void *object)  {
  Solution *solution = (Solution *)object;
  // TODO: VALID_TYPES?
  //double probability = *(double *)getParameter("probability");
  if ( PseudoRandom::randDouble() <= mutationProbability_) {
    int randIndex = PseudoRandom::randInt(0, mutList.size()-1);
    #ifdef MAN_DEBUG
    cout<<"Apply Mut: "<<randIndex<<endl;
    #endif
    mutList[randIndex]->execute(solution);
  }
  
  return solution;
}