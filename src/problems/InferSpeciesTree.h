/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InferSpeciesTree.h
 * Author: ali_nayeem
 *
 * Created on December 28, 2018, 10:09 AM
 */

#ifndef INFERSPECIESTREE_H
#define INFERSPECIESTREE_H

#include <Problem.h>
//#include <PhyloTreeSolutionType.h>
#include <Solution.h>
//#include <SolutionSet.h>

#include <time.h>

#include <Bpp/Phyl/TreeTemplate.h>
#include <Bpp/Phyl/TreeTemplateTools.h>


//#include <Bpp/Phyl/Model/RateDistribution/GammaDiscreteRateDistribution.h>
//#include <Bpp/Phyl/Io/Newick.h>
#include <Bpp/Phyl/Node.h>

#include <pll/pll.h>
//#include<libpll/pll.h>
//#include <Bpp/Text/KeyvalTools.h>
#include <sys/stat.h>
//#include <SpeciesTree.h>
#include <SolutionSet.h>
#include <TreeCrossover.h>
#include <PhylogeneticMutation.h>
#include "vector"
#include "SpeciesTree.h"
#include "SpeciesTreeSolutionType.h"
#include "ShuffledMutation.h"
#include "MultipleRandomMutation.h"
//#define MAN_DEBUG 1

class InferSpeciesTree;
typedef string(InferSpeciesTree::*GetScoreFuncPointer)(string, int);

using namespace std;
using namespace bpp;
/**
  * @class InferSpeciesTree
  * @brief Class representing problem InferSpeciesTree
  **/

class InferSpeciesTree : public Problem {
private:
    void PrintScores(double p,double l);
    int numberOfTaxa_;
    int timestamp_;
    int threadId_;
    string varFile_;
    vector <int> selectedObjectives;
   //enum Objective { MAX_ASTRAL=0, MIN_PHYLONET, MAX_STELAR, MAX_MPEST };
    int objNegIfMax[4] = {-1, 1, -1, -1};
    string getAstralScoreList(string varFile, int popSize);
    string getPhylonetScoreList(string varFile, int popSize);
    string getMpestScoreList(string varFile, int popSize);
    string getStelarScoreList(string varFile, int popSize);
    GetScoreFuncPointer getScoreFunctions[4] = {&InferSpeciesTree::getAstralScoreList, 
                                                &InferSpeciesTree::getPhylonetScoreList, 
                                                &InferSpeciesTree::getStelarScoreList, 
                                                &InferSpeciesTree::getMpestScoreList};
    string os;
public:
  enum Objective { MAX_ASTRAL=0, MIN_PHYLONET, MAX_STELAR, MAX_MPEST };
  //InferSpeciesTree(string & _datapath);
  InferSpeciesTree(string & _datapath, vector <int> & _selectedObjectives);
  ~InferSpeciesTree();
  void setThreadId(int id);
  vector<string> getLeavesName();
  boolean PLLisTreeValidate(TreeTemplate<Node> * tree);
  boolean PLLisTreeValidate(Solution * sol);
  int getNumberOfLeaves(Solution * sol);
  int getNumberOfTaxa();
  void evaluate(Solution *solution);
  //void evaluate(Solution *solution,float p, float l);
  void evaluate(SolutionSet *pop, int gen=-1);
  bool isMultifurcating(Solution * solution);
  void printParameters();
//  void readParameters(BppApplication *objApp);

 SolutionSet * getSolutionSetFromVarFile(string varFileName);
 vector< PhyloTree* > getPrecomputedTrees(){return precomputedTrees;}
 string getGeneTreePath(){return datapath + geneTreeFileName;}
 vector< PhyloTree* > readPrecomputedSpeciesTree();
 bool matchObjectiveValues(Solution * one, Solution * two );
 Solution * getTrueTree(){return trueTree;}
 double getMinObjective(int i){ return objMin[i];}
 double getMaxObjective(int i){ return objMax[i];}
 string getTrueTreePath(){return datapath + trueTreeFileName;}
 static string GetStdoutFromCommand(string cmd);
private:
  //Newick * newick;
  string datapath;
  vector<string> treeFiles;
  string speciesTreeFileName = "species.tre";
  string trueTreeFileName = "true_st.tre";
  string geneTreeFileName = "gene.tre";
  Solution * trueTree;
  vector< PhyloTree* > precomputedTrees;
  double * objMin, * objMax;
  void updateReference(Solution * individual);
  void updateReference(Solution * individual, int obj);
  static int instanceCount;
  //SolutionSet *createInitialPopulation(int size);
  //SolutionSet * createInitialPopulationGeneTrees(int size); 
  //void fillupNewPopulationUsingOld(SolutionSet * newPop, SolutionSet * oldPop);

  inline bool fileExistsTest (const string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}
 
 

};

#endif /* INFERSPECIESTREE_H */

