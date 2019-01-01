/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InferSpeciesTree.cpp
 * Author: ali_nayeem
 *
 * Created on December 28, 2018, 10:09 AM
 */

#include <InferSpeciesTree.h>
#include "vector"
#include "SpeciesTree.h"
#include "SpeciesTreeSolutionType.h"
#include "SolutionSet.h"
#include "ShuffledMutation.h"
#include "MultipleRandomMutation.h"

InferSpeciesTree::InferSpeciesTree(string & _datapath, int _numOfObj) {

    this->datapath = _datapath + "/";
    numberOfVariables_ = 1;
    numberOfObjectives_ = _numOfObj;
    numberOfConstraints_ = 0;
    problemName_ = "Infer Species Tree: " + _datapath;
    solutionType_ = new PhyloTreeSolutionType(this);
    treeFiles.push_back(datapath + speciesTreeFileName + "_astral");
    treeFiles.push_back(datapath + speciesTreeFileName + "_mpest");
    treeFiles.push_back(datapath + speciesTreeFileName + "_phylonet");
    treeFiles.push_back(datapath + speciesTreeFileName + "_triplet");
    newick = new Newick;

    readPrecomputedSpeciesTree();


    //GenerateInitialTrees();

    //if(printtrace) OpenScores();


} // InferSpeciesTree

void InferSpeciesTree::readPrecomputedSpeciesTree() {
    for (int i = 0; i < treeFiles.size(); i++) {
        cout << treeFiles[i] << endl;
        if (fileExistsTest(treeFiles.at(i))) {
            precomputedTrees.push_back(new PhyloTree(treeFiles[i]));
            //             Variable **variables = new Variable*[this->getNumberOfVariables()];
            //             variables[0] = new SpeciesTree(treeFiles[i]);
            //             Solution * newSolution = new Solution(this, variables);
            //             cout << variables[0]->toString()<<endl;        
            //cout << precomputedTrees[i]->getName();
            TreeTemplate<Node> * tree = precomputedTrees[i]->getTree();
            cout << tree->getNumberOfLeaves() << endl;
            Node * Nodo2 = precomputedTrees[i]->selectrandomnodeToCross();//selectNodeToCross(tree, tree->getNodesId());
            Node * padre= Nodo2->getFather();
            //double distancetofather = Nodo2->getDistanceToFather();
            int PosNodo2= padre->getSonPosition(Nodo2);
            //cout<< distancetofather << ", "<<PosNodo2;
        }
        this->numberOfTaxa_ = precomputedTrees[0]->getNumberOfLeaves();
    }
//    if(precomputedTrees.size()<2){
//        throw Exception("More than one PreComputedSpeciesTree is needed");
//    }
}

SolutionSet * InferSpeciesTree::createInitialPopulation(int size) {
    SolutionSet * pop = new SolutionSet(size);
    for (int i = 0; i < precomputedTrees.size(); i++) {
        Variable **variables = new Variable*[this->getNumberOfVariables()];
        variables[0] = new PhyloTree(treeFiles[i]);
        Solution * newSolution = new Solution(this, variables);
        pop->add(newSolution);
    }
    map<string, void *> parameters;
    double prob = 1.0; int numDes = 1.0;
    parameters["probability"] = &prob;
    parameters["numDescendientes"] = &numDes;
    TreeCrossover * crossover = new TreeCrossover(parameters);

    parameters.clear();
    parameters["probability"] = &prob;

    string method[] = {"NNI", "SPR", "TBR"};
    vector<Mutation *> mutList1, mutList2;

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
    parameters["probability"] = &prob;
    parameters["mutationList"] = &mutList1;
    Mutation * ShufMut = new ShuffledMutation(parameters);

    mutList2.push_back(NNI);
    mutList2.push_back(SPR);
    mutList2.push_back(TBR);
    //mutList2.push_back(ShufMut);
    parameters.clear();
    prob = 0.6;
    parameters["probability"] = &prob;
    parameters["mutationList"] = &mutList2;
    Mutation * mulMut = new MultipleRandomMutation(parameters);
    
    Solution ** parents = new Solution*[2];
    int parent1, parent2;
    //vector<Solution> children, parents;
    while (pop->size() < size) {

        parent1 = PseudoRandom::randInt(0, pop->size() - 1);
        do {
            parent2 = PseudoRandom::randInt(0, pop->size() - 1);
        } while (parent1 == parent2);
        
        parents[0] = pop->get(parent1);
        parents[1] = pop->get(parent2);
        
        //Solution * sol = new Solution(parents[0]);
        
        Solution * offSpring = (Solution *) (crossover->execute(parents));
        mulMut->execute(offSpring);   
        if( getNumberOfLeaves(offSpring) != this->numberOfTaxa_ )
        {   
            cout<<"When less taxa, then pllvalidator: "<<PLLisTreeValidate(offSpring)<<endl;
            continue;
        }
        pop->add(offSpring);
        
        //delete[] offSpring;
    }
    delete[] parents;
    return pop;
}

boolean InferSpeciesTree::PLLisTreeValidate(TreeTemplate<Node> * tree){
  pllNewickTree * newick;
  boolean res=true;
  string treenewick = TreeTemplateTools::treeToParenthesis(*tree) ;
  newick = pllNewickParseString	(treenewick.c_str());
  if (!newick){
      res = false;
  }else {
      if (!pllValidateNewick (newick))  res=false;  //if (!pllValidateNewick (newick))  !pllNewickUnroot(newick)
  }
  pllNewickParseDestroy (&newick);
  return res;
}

boolean InferSpeciesTree::PLLisTreeValidate(Solution * solution)
{
    Variable **variables = solution->getDecisionVariables();
    PhyloTree * Pt = (PhyloTree*) variables[0];
    TreeTemplate<Node> * tree = Pt->getTree();
    return PLLisTreeValidate(tree);
}

int InferSpeciesTree::getNumberOfLeaves(Solution * solution)
{
    Variable **variables = solution->getDecisionVariables();
    PhyloTree * Pt = (PhyloTree*) variables[0];
    TreeTemplate<Node> * tree = Pt->getTree();
    return tree->getNumberOfLeaves();
}
void InferSpeciesTree::evaluate(Solution *solution) {

}

InferSpeciesTree::~InferSpeciesTree() {


    delete newick;



    //if (ParsimonyEvaluator!=NULL)   delete ParsimonyEvaluator;
    //if (LikelihoodEvaluator!=NULL)  delete LikelihoodEvaluator;

    delete solutionType_;

    for (int i = 0; i < precomputedTrees.size(); i++) {
        //TreeTemplateTools::deleteSubtree(*trees[i],trees[i]->getRootId());
        delete precomputedTrees[i];
    }

    //if(printtrace)  CloseScores();


} // ~InferSpeciesTree