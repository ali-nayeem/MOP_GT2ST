/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InitSpeciesTree.h
 * Author: ali_nayeem
 *
 * Created on August 19, 2019, 10:21 PM
 */

#ifndef TREE_INITIALIZER_H
#define TREE_INITIALIZER_H

#include <InferSpeciesTree.h>

class TreeInitializer : public Operator{
public:
    TreeInitializer(map<string, void *> parameters);
    ~TreeInitializer();
    void* execute(void *);
    void addOperator(string name, Operator * op);
private:
    string method;
    int popSize;
    InferSpeciesTree *problem;
    bool unique;
    bool discardTool;
    Operator * mut = NULL;
    Operator * cross = NULL;
    void  fillupNewPopulationUsingOld(SolutionSet * newPop, SolutionSet * oldPop);
    SolutionSet * fromGeneTrees(int size);
    SolutionSet * fromTrueTree(int size);
    SolutionSet * fromTools(int size);
    SolutionSet *  fromToolsGeneTrees(int size);
    SolutionSet *  fromPrevVAR(int size);
    map<string, Operator *> operators;
    string prevVARPath;
};

#endif /* USINGGENETREES_H */

