/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MOEAD_ST.cpp
 * Author: ali_nayeem
 * 
 * Created on June 5, 2019, 9:02 AM
 */

#include "MOEAD_ST.h"
#include "InferSpeciesTree.h"
#include "Selection.h"

MOEAD_ST::MOEAD_ST(Problem * problem, Checkpoint *checkpoint) : MOEAD(problem)
{
    checkpoint_ = checkpoint;
}

SolutionSet * MOEAD_ST::execute() {
  int maxEvaluations;
  functionType_ = "_WS_ADJUSTED";
  evaluations_ = 0;
  maxEvaluations = *(int *) getInputParameter("maxEvaluations");
  populationSize_ = *(int *) getInputParameter("populationSize");
  dataDirectory_ = * (string *) getInputParameter("dataDirectory");
  //IntervalOptSubsModel = *(int *) getInputParameter("intervalupdateparameters");
  
  population_ = new SolutionSet(populationSize_);
  //indArray_ = new Solution*[problem_->getNumberOfObjectives()];
  
  T_ = problem_->getNumberOfObjectives() + 1;
  delta_ = 0.9;
  nr_ = 2;
/*
  T_ = * (int *) (0.1 * populationSize_);
  delta_ = 0.9;
  nr_ = * (int *) (0.01 * populationSize_);
*/
  neighborhood_ = new int*[populationSize_];
  
  z_ = new double[problem_->getNumberOfObjectives()];
  //lambda_ = new Vector(problem_->getNumberOfObjectives()) ;
  lambda_ = new double*[populationSize_];
  
  crossover_ = operators_["crossover"];  // default: DE crossover
  mutation_ = operators_["mutation"];  // default: polynomial mutation
  initializer_ = operators_["initializer"]; 
  map<string, void *> parameters;
  //Selection * randSel = new RandomSelection(parameters);
  // STEP 1. Initialization
  // STEP 1.1. Compute euclidean distances between weight vectors and find T
  initUniformWeight();
  //initRandomWeight(0.5, 0.1);
  //for (int i = 0; i < 300; i++)
  // 	cout << lambda_[i][0] << " " << lambda_[i][1] << endl ;
  
  initNeighborhood();
  
  // STEP 1.2. Initialize population
  initPopulation();
  
  // STEP 1.3. Initialize z_
  initIdealPoint();
  
  
  // STEP 2. Update
  int gen = 0;
  do {
    checkpoint_->logVAR(population_, gen);
    cout<< "Thread[" << checkpoint_->getThreadId() << "]: " <<"Generation: "<<gen++ << endl;
    int * permutation = new int[populationSize_];
    UtilsMOEAD::randomPermutation(permutation, populationSize_);
    for (int i = 0; i < populationSize_; i++) {
      int n = permutation[i]; // or int n = i;
      //int n = i ; // or int n = i;
      int type;
      double rnd = PseudoRandom::randDouble();
      
      
      // STEP 2.1. Mating selection based on probability
      if (rnd < delta_) // if (rnd < realb)
      {
        type = 1;   // neighborhood
      } else {
        type = 2;   // whole population
      }
      vector<int> p;
      matingSelection(p, n, 2, type);
      
      // STEP 2.2. Reproduction
      Solution * child;
      Solution ** parents = new Solution*[2];
      
      parents[0] = population_->get(p[0]);
      parents[1] = population_->get(p[1]);
      //parents[2] = population_->get(n);
      
      // Apply DE crossover
      //void ** object = new void*[2];
      //object[0] = population_->get(n);
      //object[1] = parents;
      child = (Solution *) (crossover_->execute(parents));
      //delete[] object;
      delete[] parents;
      
      InferSpeciesTree * p_ = (InferSpeciesTree *) problem_;
      // Apply mutation
      Solution * copy;
      
      do {
            copy = new Solution(child);
            mutation_->execute(copy);
      } while ( (p_->getNumberOfLeaves(copy) != p_->getNumberOfTaxa()) );
      
      //((Phylogeny *)problem_)->Optimization(child);
        
      // Evaluation
      problem_->evaluate(child);
      
      evaluations_++;
      
      //if((evaluations_ % 100) == 0)   cout << "Evaluations " << evaluations_ << endl;
      // STEP 2.3. Repair. Not necessary
      
      // STEP 2.4. Update z_
      updateReference(child);
      
      // STEP 2.5. Update of solutions
      updateProblem(child, n, type);
    } // for
    
    delete[] permutation;
    
    //Update Interval
//     if(evaluations_%IntervalOptSubsModel==0 and IntervalOptSubsModel > 0){ 
//        Solution * sol;  double Lk;
//        Phylogeny * p = (Phylogeny*) problem_;
//        cout << "Updating and Optimizing Parameters.." << endl;
//        for(int i=0; i < populationSize_; i++){
//            sol =  population_->get(i);
//            Lk=  p->BranchLengthOptimization(sol,p->OptimizationMetodoOptRamas,p->OptimizationNumIterOptRamas,p->OptimizationTolerenciaOptRamas);
//            sol->setObjective(1,Lk*-1);
//        }
//        
//        for (int i = 0; i < populationSize_; i++) {
//                updateReference(population_->get(i));
//        } // for
//        
//        cout << "Update Interval Done!!" << endl;
//    }
    
    
  } while (evaluations_ < maxEvaluations);
  checkpoint_->logVAR(population_, gen);
  // Free memory
  deleteParams();
  
  return population_;
} // execute


void MOEAD_ST::initPopulation() {
    
  InferSpeciesTree * p = (InferSpeciesTree *) problem_;
    
  //population_ = p->createInitialPopulationGeneTrees(populationSize_);
  population_ = (SolutionSet*) initializer_->execute(&populationSize_);
    
  p->evaluate(population_);
  evaluations_ += populationSize_;
  
  cout << "Thread[" << checkpoint_->getThreadId() << "]: " << "Initial Population Done" << endl;
} 

void MOEAD_ST::initRandomWeight(double mean, double stdDev)
{
    
    for (int n = 0; n < this->populationSize_; ) 
    {
      lambda_[n] = new double[problem_->getNumberOfObjectives()];
      //double a = 1.0 * n / (populationSize_ - 1);
      //lambda_[n][0] = a;
      //lambda_[n][1] = 1 - a;
      double sum = 0;
      for(int obj = 0; obj < problem_->getNumberOfObjectives(); obj++)
      {
          lambda_[n][obj] = RandomTools::randGaussian(mean, stdDev*stdDev, *PseudoRandom::getRndFactory());   
          sum  += lambda_[n][obj];
      }
      for(int obj = 0; obj < problem_->getNumberOfObjectives(); obj++)
      {
          lambda_[n][obj] = lambda_[n][obj]/sum;   
      }
      bool match = false;
      for(int i=0; i < n; i++)
      {
          if(matchWeightValues(lambda_[n], lambda_[i]))
          {
            cout << "Duplicate weight vector found. Discarded. " << endl;
            match = true;
            break;
           }
            //for()
      }
      if(match)
      { 
        continue;
      }
      else
      {
          n++;
//          cout << n-1 <<"-th Weight:";
//          for(int obj = 0; obj < problem_->getNumberOfObjectives(); obj++)
//            {
//                cout << lambda_[n-1][obj]<<", ";  
//            }
//          cout<<endl;
      }
    } // for
}

bool MOEAD_ST::matchWeightValues(double * one, double * two)
{
    //bool match = true;
    for (int i = 0; i < problem_->getNumberOfObjectives(); i++)
    {
        if (one[i] != two[i]) //fabs(one->getObjective(i) - two->getObjective(i)) > 0.00001
        {
            return false;
        }
    }
    return true;
}
