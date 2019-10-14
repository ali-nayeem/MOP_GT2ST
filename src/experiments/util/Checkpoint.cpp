/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Checkpoint.cpp
 * Author: ali_nayeem
 * 
 * Created on May 26, 2019, 11:15 AM
 */

#include "Checkpoint.h"

int Checkpoint::interval_ = 10;
Checkpoint::Checkpoint(bool keepCheckpoint, string dataStorePath, int runNumber)
{
    keepCheckpoint_ = keepCheckpoint;
    runNumber_ =  runNumber;
    dataStorePath_ = dataStorePath + "/run" + to_string(runNumber);
    //prob_ = prob;
    //interval_ = 10;
    if (keepCheckpoint_ && FileUtils::existsPath(dataStorePath_.c_str()) != 1) {
        FileUtils::createDirectory(dataStorePath_);
        //cout << "Creating directory: " << directory << endl;
      }
}

Checkpoint::Checkpoint(const Checkpoint& orig)
{
}

void Checkpoint::logVAR(SolutionSet *pop, int gen)
{
    if(keepCheckpoint_ )
    {
        if( (gen % interval_)==0 )
        {
            execute(pop, gen);
        }
    }
    
}
void Checkpoint::logVARforce(SolutionSet *pop, int gen)
{
    if(keepCheckpoint_ )
    {
        execute(pop, gen);
    }
    
}
void Checkpoint::execute(SolutionSet *pop, int gen)
{
    pop->printVariablesToFile(dataStorePath_ + "/pop." + to_string(gen));
    pop->printObjectivesToFile(dataStorePath_ + "/obj." + to_string(gen));
    string to = dataStorePath_ + "/pop." + to_string(gen);
    string treePerfPath = dataStorePath_ + "/popTreePerf." + to_string(gen);
    string trueStPath =  ((InferSpeciesTree*)prob_)->getTrueTreePath();
    string cmd = "python2  lib/PyTreePerf/getTreePerfFromVAR.py -t " + trueStPath + " -v " + to + " -o " + treePerfPath;
    InferSpeciesTree::GetStdoutFromCommand(cmd);
    //cmd = "python3  lib/PyTreePerf/drawTreePerfDistrib.py -f " + treePerfPath;
    //InferSpeciesTree::GetStdoutFromCommand(cmd);
}

Checkpoint::~Checkpoint()
{
}

