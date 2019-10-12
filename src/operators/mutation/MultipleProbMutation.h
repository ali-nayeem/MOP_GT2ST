/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MultipleProbMutation.h
 * Author: ali_nayeem
 *
 * Created on December 29, 2018, 2:33 PM
 */

#ifndef MULTIPLE_PROB_MUTATION_H
#define MULTIPLE_PROB_MUTATION_H

#include <vector> 
#include <Mutation.h>
#include <random>
#include <Operator.h>
#include <Solution.h>
#include <algorithm> 
#include "PseudoRandom.h"
#include <InferSpeciesTree.h>



class MultipleProbMutation : public Mutation {
public:
    MultipleProbMutation(map<string, void *> parameters);
    void * execute(void *object);
    ~MultipleProbMutation();
    void increaseWeight(int id);
    void decreaseWeight(int id);
    void decreaseAllWeight(int count);
private:
    vector <Mutation *> mutList; 
    vector <double> weight;
    vector <int> id;
    vector <int> count;
    double mutationProbability_;

};

#endif /* SHUFFLEDMUTATION_H */

