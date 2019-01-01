/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShuffledMutation.h
 * Author: ali_nayeem
 *
 * Created on December 29, 2018, 2:33 PM
 */

#ifndef SHUFFLEDMUTATION_H
#define SHUFFLEDMUTATION_H

#include <vector> 
#include <Mutation.h>
#include <random>
#include <Operator.h>
#include <Solution.h>
#include <algorithm> 

class ShuffledMutation : public Mutation {
public:
    ShuffledMutation(map<string, void *> parameters);
    void * execute(void *object);
    static int random(int i);
    ~ShuffledMutation();
private:
    vector <Mutation *> mutList; 
    double mutationProbability_;

};

#endif /* SHUFFLEDMUTATION_H */

