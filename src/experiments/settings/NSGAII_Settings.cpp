//  NSGAII_Settings.cpp
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

#include <NSGAII_Settings.h>


/**
 * Default constructor
 */
NSGAII_Settings::NSGAII_Settings() : Settings()
{
} // NSGAII_Settings

/**
 * Destructor
 */
NSGAII_Settings::~NSGAII_Settings()
{
    delete algorithm;
    delete crossover; // Crossover operator
    delete mutation; // Mutation operator
    delete selection; // Selection operator
} // ~NSGAII_Settings

/**
 * Constructor
 */
NSGAII_Settings::NSGAII_Settings(string problemName, Checkpoint * checkpoint)
{
    problemName_ = problemName;
    string path(problemName_);
    std::replace( path.begin(), path.end(), '.', '/');
    //cout<<path;

    //problem_ = ProblemFactory::getProblem((char *) problemName_.c_str());
    problem_ = new InferSpeciesTree(path, 3);

    // Algorithm parameters
    populationSize_ = 100;
    maxEvaluations_ = 20100;
    mutationProbability_ = 0.2;
    crossoverProbability_ = 0.8;
    checkpoint_ = checkpoint;

} // NSGAII_Settings

/**
 * Configure method
 */
Algorithm * NSGAII_Settings::configure()
{

    algorithm = new NSGAII_ST(problem_, checkpoint_);
    algorithm->setInputParameter("populationSize", &populationSize_);
    algorithm->setInputParameter("maxEvaluations", &maxEvaluations_);

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

    mutList1.push_back(NNI);
    //mutList1.push_back(SPR);
    //mutList1.push_back(TBR);
    parameters.clear();
    mutationProbability = mutationProbability_;
    parameters["probability"] = &mutationProbability;
    parameters["mutationList"] = &mutList1;
    mutation = new MultipleRandomMutation(parameters);

    // Selection Operator
    parameters.clear();
    selection = new BinaryTournament2(parameters);

    // Add the operators to the algorithm
    algorithm->addOperator("crossover", crossover);
    algorithm->addOperator("mutation", mutation);
    algorithm->addOperator("selection", selection);

    //cout << "NGSAII algorithm initialized." << endl;

    return algorithm;
} // configure

