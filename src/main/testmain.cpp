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
#include "TreeInitializer.h"
#include <BinaryTournament2.h>

//#define MAN_DEBUG 1

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
        string dataset = "10-taxon";
        string tool = "astral";
        string pre_path = "/Users/ali_nayeem/PycharmProjects/GT2ST/data/combined/data/Tools/" + dataset + "/R";
	for(int i=1; i<=10; i++){
		string data = "10-taxon.higher-ILS.estimated-genetrees.R" + to_string(i);  
                //"10-taxon.higher-ILS.estimated-genetrees.R3"; "15-taxon.100gene-100bp.estimated-genetrees.R3" "37-taxon.noscale_200g_500b.estimated-genetrees.R5
		std::replace( data.begin(), data.end(), '.', '/');
		vector<int> obj{ InferSpeciesTree::MAX_ASTRAL, InferSpeciesTree::MAX_STELAR, InferSpeciesTree::MAX_MPEST}; 
                cout<<"here now -1 "+data<<endl;
		InferSpeciesTree * problem = new InferSpeciesTree(data, obj);
                //cout<<"here now"<<endl;
		SolutionSet * solSet = problem->getSolutionSetFromVarFile(pre_path + to_string(i) + "/" + tool+ "_SPR.txt");
		//cout<<"here now 1"<<endl;
                problem->evaluate(solSet);
                //cout<<"here now 2"<<endl;
		solSet->printObjectivesToFile(pre_path + to_string(i) + "/" + tool+ "_SPR_obj.txt");
	}
    return 0;
}
/*int main(int argc, char** argv) {
    PseudoRandom::bppRand_->setSeed(01234567);
    string data = "10-taxon.higher-ILS.estimated-genetrees.R3";  //"10-taxon.higher-ILS.estimated-genetrees.R3"; "15-taxon.100gene-100bp.estimated-genetrees.R3" "37-taxon.noscale_200g_500b.estimated-genetrees.R5"
    std::replace( data.begin(), data.end(), '.', '/');
    vector<int> obj{ InferSpeciesTree::MAX_ASTRAL, InferSpeciesTree::MAX_STELAR}; 
    InferSpeciesTree * problem = new InferSpeciesTree(data, obj);
    //    for (int i = 0; i < 3; i++) {
    //        SolutionSet * pop = problem->createInitialPopulation(20);
    //        problem->evaluate(pop, 0);
    //    }
    int populationSize = 50, maxEvaluations = 10;
    Problem * prob = problem; // The problem to solve
    Algorithm * algorithm = new NSGAII_ST(prob); // The algorithm to use
    Operator * crossover; // Crossover operator
    Operator * mutation; // Mutation operator
    Operator * selection; // Selection operator
    Operator * initializer;

    map<string, void *> parameters;
    double pb = 1.0;
    int numDes = 1;
    parameters["probability"] = &pb;
    parameters["numDescendientes"] = &numDes;
    crossover = new TreeCrossover(parameters);

    parameters.clear();
    pb = 0.8;
    parameters["probability"] = &pb;
    string method = "SPR";
    parameters["metodo"] = &method;
    mutation = new PhylogeneticMutation(parameters);
    
    parameters.clear();
    pb = 1.0;
    parameters["probability"] = &pb;
    string methods[] = {"NNI", "SPR", "TBR"};
    parameters["metodo"] = &methods[0];
    Mutation * NNI = new PhylogeneticMutation(parameters);
    parameters["metodo"] = &methods[1];
    Mutation * SPR = new PhylogeneticMutation(parameters);
    parameters["metodo"] = &methods[2];
    Mutation * TBR = new PhylogeneticMutation(parameters);
    vector<Mutation *> mutList1;
    //mutList1.push_back(NNI);
    //mutList1.push_back(SPR);
    mutList1.push_back(TBR);
    pb = 1.0;
    parameters["probability"] = &pb;
    parameters["mutationList"] = &mutList1;
    Mutation * mulMut = new MultipleRandomMutation(parameters);
    bool unique = true;
    method = "from_true_tree";
    parameters["problem"] = problem;
    //parameters["crossover"] = crossover;
    parameters["mutation"] = mulMut;
    parameters["method"] = &method;
    parameters["unique"] = &unique;
    initializer = new TreeInitializer(parameters);

    selection = new BinaryTournament2(parameters);

    algorithm->setInputParameter("populationSize", &populationSize);
    algorithm->setInputParameter("maxEvaluations", &maxEvaluations);
    algorithm->addOperator("crossover", crossover);
    algorithm->addOperator("mutation", mutation);
    algorithm->addOperator("selection", selection);
    algorithm->addOperator("initializer", initializer);

    //for(int i=0; i<1; i++)
    SolutionSet * result = algorithm->execute();
    
    result->printVariablesToFile("experiment/VAR.br");
    result->printObjectivesToFile("experiment/FUN.br");


    delete crossover;
    delete mutation;
    delete selection;
    //delete problem;
    delete algorithm;

    return 0;
}*/

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