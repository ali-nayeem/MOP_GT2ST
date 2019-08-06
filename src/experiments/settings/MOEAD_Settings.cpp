//  MOEAD_Settings.cpp
//
//  Author:
//       Antonio J. Nebro <antonio@lcc.uma.es>
//       Esteban López-Camacho <esteban@lcc.uma.es>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <MOEAD_Settings.h>

#include "RandomSelection.h"
#include "MOEAD_ST.h"


/**
 * Default constructor
 */
MOEAD_Settings::MOEAD_Settings() : Settings()
{
} // MOEAD_Settings

/**
 * Destructor
 */
MOEAD_Settings::~MOEAD_Settings()
{
    delete algorithm;
    delete crossover; // Crossover operator
    delete mutation; // Mutation operator
    delete selection; // Selection operator
} // ~MOEAD_Settings

/**
 * Constructor
 */
MOEAD_Settings::MOEAD_Settings(string problemName, Checkpoint * checkpoint)
{
    problemName_ = problemName;
    string path(problemName_);
    std::replace( path.begin(), path.end(), '.', '/');
    //cout<<path;

    //problem_ = ProblemFactory::getProblem((char *) problemName_.c_str());
    vector<int> obj{ InferSpeciesTree::MAX_ASTRAL, InferSpeciesTree::MAX_STELAR}; 
    problem_ = new InferSpeciesTree(path, obj);

    // Algorithm parameters
    populationSize_ = 100;
    maxEvaluations_ = 2600;
    maxGen_ = 44;
    mutationProbability_ = 0.8;
    crossoverProbability_ = 0.4;
    checkpoint_ = checkpoint;

} // MOEAD_Settings

/**
 * Configure method
 */
Algorithm * MOEAD_Settings::configure()
{

    algorithm = new MOEAD_ST(problem_, checkpoint_);//new MOEAD_ST(problem_, checkpoint_);
    algorithm->setInputParameter("populationSize", &populationSize_);
    algorithm->setInputParameter("maxEvaluations", &maxEvaluations_);
    string dataDirectory = "data/Weight";
    algorithm->setInputParameter("dataDirectory",&dataDirectory);
    //algorithm->setInputParameter("maxGenerations", &maxGen_);

    // Mutation and Crossover for Real codification
    map<string, void *> parameters;

    double crossoverProbability = crossoverProbability_;
    parameters["probability"] = &crossoverProbability;
    crossover = new TreeCrossover(parameters);

    parameters.clear();
    double mutationProbability = 1.0;
    parameters["probability"] = &mutationProbability;
    //mutation = new PhylogeneticMutation(parameters);
    string method[] = {"NNI", "SPR", "TBR"};
    vector<Mutation *> mutList1;

    parameters["metodo"] = &method[0];
    Mutation * NNI = new PhylogeneticMutation(parameters);
    parameters["metodo"] = &method[1];
    Mutation * SPR = new PhylogeneticMutation(parameters);
    parameters["metodo"] = &method[2];
    Mutation * TBR = new PhylogeneticMutation(parameters);

    //mutList1.push_back(NNI);
    mutList1.push_back(SPR);
    //mutList1.push_back(TBR);
    parameters.clear();
    mutationProbability = mutationProbability_;
    parameters["probability"] = &mutationProbability;
    parameters["mutationList"] = &mutList1;
    mutation = new MultipleRandomMutation(parameters);

    // Selection Operator
    parameters.clear();
    selection = new RandomSelection(parameters); //BinaryTournament2(parameters);

    // Add the operators to the algorithm
    algorithm->addOperator("crossover", crossover);
    algorithm->addOperator("mutation", mutation);
    algorithm->addOperator("selection", selection);

    //cout << "NGSAII algorithm initialized." << endl;

    return algorithm;
} // configure
