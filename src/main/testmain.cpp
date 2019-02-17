/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testmain.cpp
 * Author: ali_nayeem
 *
 * Created on December 24, 2018, 8:37 PM
 */

#include <cstdlib>
#include <iostream>
#include <InferSpeciesTree.h>

#include "NSGAII_ST.h"
#include <BinaryTournament2.h>

#define MAN_DEBUG 1

using namespace std;

/*
 * 
 */
string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    //cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

int main(int argc, char** argv) {
    string data = "10-taxon/higher-ILS/estimated-genetrees/R2";
    InferSpeciesTree * problem = new InferSpeciesTree(data, 3);
    //    for (int i = 0; i < 3; i++) {
    //        SolutionSet * pop = problem->createInitialPopulation(20);
    //        problem->evaluate(pop, 0);
    //    }
    int populationSize = 10, maxEvaluations = 200;
    Problem * prob = problem; // The problem to solve
    Algorithm * algorithm = new NSGAII_ST(prob); // The algorithm to use
    Operator * crossover; // Crossover operator
    Operator * mutation; // Mutation operator
    Operator * selection; // Selection operator

    map<string, void *> parameters;
    double pb = 6.0;
    int numDes = 1.0;
    parameters["probability"] = &pb;
    parameters["numDescendientes"] = &numDes;
    crossover = new TreeCrossover(parameters);

    parameters.clear();
    pb = 0.8;
    parameters["probability"] = &pb;
    //parameters["metodo"] = "NNI";
    mutation = new PhylogeneticMutation(parameters);

    selection = new BinaryTournament2(parameters);

    algorithm->setInputParameter("populationSize", &populationSize);
    algorithm->setInputParameter("maxEvaluations", &maxEvaluations);
    algorithm->addOperator("crossover", crossover);
    algorithm->addOperator("mutation", mutation);
    algorithm->addOperator("selection", selection);

    //for(int i=0; i<1; i++)
    SolutionSet * result = algorithm->execute();
    
    result->printVariablesToFile("experiment/StandardStudy/data/NSGAII/10-taxon.higher-ILS.estimated-genetrees.R1/VAR.x");
    result->printObjectivesToFile("experiment/StandardStudy/data/NSGAII/10-taxon.higher-ILS.estimated-genetrees.R1/FUN.x");

    //pop->printVariablesToFile(data+"/VAR");
    //    map<string, void *> parameters;
    //    parameters.clear();
    //    double prob = 1.0; int numDes = 1;
    //    parameters["probability"] = &prob;
    //    parameters["numDescendientes"] = &numDes;
    //    cout<<*(int*)parameters["numDescendientes"];
    //    TreeCrossover crossover = TreeCrossover(parameters);
    //    Solution ** parents = new Solution*[2];
    //    int parent1, parent2;
    //    //vector<Solution> children, parents;   
    //    parents[0] = pop->get(4);
    //    parents[1] = pop->get(5);
    //    Solution * offSpring = (Solution *) (crossover.execute(parents));
    delete crossover;
    delete mutation;
    delete selection;
    //delete problem;
    delete algorithm;

    return 0;
}

