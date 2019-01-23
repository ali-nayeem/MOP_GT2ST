/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MultipleRandomMutation.h
 * Author: ali_nayeem
 *
 * Created on December 29, 2018, 2:33 PM
 */

#ifndef MULTIPLEMUTATION_H
#define MULTIPLEMUTATION_H

#include <vector> 
#include <Mutation.h>
#include <random>
#include <Operator.h>
#include <Solution.h>
#include <algorithm> 
#include "PseudoRandom.h"


class MultipleRandomMutation : public Mutation {
public:
    MultipleRandomMutation(map<string, void *> parameters);
    void * execute(void *object);
    ~MultipleRandomMutation();
private:
    vector <Mutation *> mutList; 
    double mutationProbability_;

};

#endif /* SHUFFLEDMUTATION_H */

