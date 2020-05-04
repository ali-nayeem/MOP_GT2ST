//  gGA_Settings.cpp
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

#include <gGA_Settings.h>

#include "DetTournament.h"
#include "TreeInitializer.h"
#include "NormalizedSumComparator.h"
#include "NSGAII_ST_MidDistRank.h"

/**
 * Default constructor
 */
gGA_Settings::gGA_Settings() : Settings()
{
} // gGA_Settings

/**
 * Destructor
 */
gGA_Settings::~gGA_Settings()
{
    delete algorithm;
    delete crossover; // Crossover operator
    delete mutation; // Mutation operator
    //delete selection; // Selection operator
} // ~gGA_Settings

/**
 * Constructor
 */
gGA_Settings::gGA_Settings(string problemName, Checkpoint * checkpoint)
{
    problemName_ = problemName;
    string path(problemName_);
    std::replace( path.begin(), path.end(), '.', '/');
    //cout<<path;

    //problem_ = ProblemFactory::getProblem((char *) problemName_.c_str());
    vector<int> obj{ InferSpeciesTree::MAX_ASTRAL, InferSpeciesTree::MAX_STELAR, InferSpeciesTree::MAX_MPEST}; //, InferSpeciesTree::MIN_PHYLONET}; 
    problem_ = new InferSpeciesTree(path, obj);

    // Algorithm parameters
    populationSize_ = 100;
    maxEvaluations_ = 10000;
    //maxGen_ = 44;
    mutationProbability_ = 1.0;
    crossoverProbability_ = 0.3;
    tournamentSize_ = 10;
    checkpoint_ = checkpoint;
    comparator_ = new NormalizedSumComparator() ;

} // gGA_Settings

/**
 * Configure method
 */
Algorithm * gGA_Settings::configure()
{

    algorithm = new NSGAII_ST_MidDistRank(problem_); //gGA_ST
    algorithm->setInputParameter("populationSize", &populationSize_);
    algorithm->setInputParameter("maxEvaluations", &maxEvaluations_);
    algorithm->setInputParameter("checkpoint", checkpoint_);
    algorithm->setInputParameter("comparator", comparator_);

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
    mutList1.push_back(SPR);
    mutList1.push_back(TBR);
    parameters.clear();
    mutationProbability = mutationProbability_;
    parameters["probability"] = &mutationProbability;
    parameters["mutationList"] = &mutList1;
    mutation = new MultipleProbMutation(parameters);

    // Selection Operator
    parameters.clear();
    parameters["comparator"] = comparator_;
    parameters["size"] = &tournamentSize_;
    selection = new DetTournament(parameters); //BinaryTournament2(parameters);
    
    //initializer
    parameters.clear();
    double pb = 1.0;
    int numDes = 1;
    parameters["probability"] = &pb;
    parameters["numDescendientes"] = &numDes;
    Operator * initCross = new TreeCrossover(parameters);
    
    parameters.clear();
    mutationProbability = 0.3;
    parameters["probability"] = &mutationProbability;
    parameters["metodo"] = &method[0]; //NNI
    Operator * initNNI = new PhylogeneticMutation(parameters);
    
    parameters.clear();
    bool unique = true;
    string initMethod = "from_gene_trees";
    parameters["problem"] = problem_;
    parameters["crossover"] = initCross;
    parameters["mutation"] = NULL;
    parameters["method"] = &initMethod;
    //parameters["unique"] = &unique;
    //string prevVAR = "/Users/ali_nayeem/PycharmProjects/GT2ST/data/Nayeem20OctGGA_37taxa/data/gGA/" + problemName_ + "/VAR." + to_string(checkpoint_->getRunNumber());
    //parameters["prevVARPath"] = &prevVAR;
    initializer = new TreeInitializer(parameters);

    // Add the operators to the algorithm
    algorithm->addOperator("crossover", crossover);
    algorithm->addOperator("mutation", mutation);
    algorithm->addOperator("selection", selection);
    algorithm->addOperator("initializer", initializer);
    algorithm->addOperator("selection", selection);
    //cout << "NGSAII algorithm initialized." << endl;

    return algorithm;
} // configure

