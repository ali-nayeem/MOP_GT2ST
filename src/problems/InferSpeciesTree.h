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
#include <PhyloTreeSolutionType.h>
#include <Solution.h>
//#include <SolutionSet.h>

#include <time.h>

#include <Bpp/Seq/Alphabet/DNA.h>
#include <Bpp/Seq/Alphabet/Alphabet.h>
#include <Bpp/Seq/Container/SiteContainer.h>
#include <Bpp/Seq/Container/SequenceContainer.h>
#include <Bpp/Seq/Container/SiteContainerTools.h>
#include <Bpp/Seq/Io/Phylip.h>
#include <Bpp/Seq/Io/Fasta.h>

#include <Bpp/Phyl/TreeTemplate.h>
#include <Bpp/Phyl/TreeTemplateTools.h>

#include <Bpp/Phyl/Model/Nucleotide/HKY85.h>
#include <Bpp/Phyl/Model/Nucleotide/GTR.h>

#include <Bpp/Phyl/Model/RateDistribution/GammaDiscreteRateDistribution.h>
#include <Bpp/Phyl/Io/Newick.h>
#include <Bpp/Phyl/Node.h>

#include <Bpp/Phyl/Parsimony/DRTreeParsimonyScore.h>
#include <Bpp/Phyl/Likelihood/RHomogeneousTreeLikelihood.h>
#include <Bpp/Phyl/Likelihood/NNIHomogeneousTreeLikelihood.h>


#include <Bpp/Phyl/OptimizationTools.h>

#include <Bpp/Io/FileTools.h>

#include <Bpp/Seq/App/SequenceApplicationTools.h>
#include <Bpp/App/BppApplication.h>


//#include <Bpp/Numeric/ParameterList.h>
//#include <Bpp/Phyl/Likelihood/PseudoNewtonOptimizer.h>
//#include <Bpp/Numeric/Function/BfgsMultiDimensions.h>
//#include <Bpp/Numeric/Function/ThreePointsNumericalDerivative.h>
//#include <Bpp/Numeric/Function/ConjugateGradientMultiDimensions.h>
//#include <Bpp/Numeric/Function/TwoPointsNumericalDerivative.h>
//#include <Bpp/Numeric/Function/MetaOptimizer.h>
//#include <Bpp/Phyl/OptimizationTools.h>

#include <pll/pll.h>

#include <Bpp/Text/KeyvalTools.h>
#include <sys/stat.h>
#include <SpeciesTree.h>
#include <SolutionSet.h>
#include <TreeCrossover.h>
#include <PhylogeneticMutation.h>


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
    
public:
  //Phylogeny(string solutionType);
  /*Phylogeny(string SequenceFile, double  kappa_, double alpha_, double beta_, int NumCat_ ,
                    double piA_, double piC_, double piG_, double piT_,
                    int bootstrapSize_,string BootStrapFilename_);*/
  
  InferSpeciesTree(string & _datapath, int _numOfObj);
  ~InferSpeciesTree();

  vector<string> getLeavesName();
  boolean PLLisTreeValidate(TreeTemplate<Node> * tree);
  boolean PLLisTreeValidate(Solution * sol);
  int getNumberOfLeaves(Solution * sol);

  void evaluate(Solution *solution);
  void evaluate(Solution *solution,float p, float l);
  
  //void evaluate(Solution *solution, DRTreeParsimonyScore* ParsimonyEvaluator); 
  //void evaluate(Solution *solution, DRTreeParsimonyScore* ParsimonyEvaluator, NNIHomogeneousTreeLikelihood * LikelihoodEvaluator );
  
      

  void printParameters();
  void readParameters(BppApplication *objApp);
  void GenerateInitialTrees();
  void GenerateRandomTrees();
  void LoadUserTrees();

  Newick * newick;
  string datapath;
  vector<string> treeFiles;
  string speciesTreeFileName = "species.tre";
  vector< PhyloTree* > precomputedTrees;
  
  void readPrecomputedSpeciesTree();
  SolutionSet *createInitialPopulation(int size);

  vector<Tree*> treesin;
  vector<Tree*> trees;
  int bootstrapSize;
  string bootstrapFilename;
  string initialTrees;
  string FilenameIntree;
  double mp;
  double ml;
  string PartitionModelFilePLL;

  
  size_t NumCat;
  
  int NumEvaluaciones;
  
  
  void OpenScores();
  void CloseScores();
  ofstream ComportamientoML,ComportamientoTime, ComportamientoMP , ComportamientoTimePar, ScoresML, ScoresMP;
  
  bool printtrace, printbestscores;

  inline bool fileExistsTest (const string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}
 
 

};

#endif /* INFERSPECIESTREE_H */

