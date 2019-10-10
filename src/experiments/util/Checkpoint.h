/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Checkpoint.h
 * Author: ali_nayeem
 *
 * Created on May 26, 2019, 11:15 AM
 */

#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <string>
#include <SolutionSet.h>
#include "FileUtils.h"
#include <InferSpeciesTree.h>

using namespace std;


class Checkpoint {
public:
    Checkpoint(bool keepCheckpoint, string dataStorePath, int runNumber);
    Checkpoint(const Checkpoint& orig);
    virtual ~Checkpoint();
    void logVAR(SolutionSet *pop, int gen);
    void logVARforce(SolutionSet *pop, int gen);
    //void setMaxGen(int gen);
    static int interval_;
    void setThreadId(int id){threadId=id;}
    int  getThreadId(){return threadId;}
    void addProblem(Problem * prob){prob_=prob;}
private:
    string dataStorePath_;
    int runNumber_;
    bool keepCheckpoint_ = false;
    int threadId;
    Problem * prob_;
    
    //int maxGen;
};
//int Checkpoint::interval_ = 10;
#endif /* CHECKPOINT_H */

