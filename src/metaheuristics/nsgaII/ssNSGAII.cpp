//  ssNSGAII.cpp
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//       Some phylogenetic features were added by Cristian Zambrano-Vega
//       <czambrano@uteq.edu.ec>
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


#include <ssNSGAII.h>
#include <Bpp/App/BppApplication.h>
#include <Bpp/App/ApplicationTools.h>
#include <Phylogeny.h>

using namespace bpp;

/*
 * This class implements a steady-state version of NSGA-II.
 */


/**
 * Constructor
 * @param problem Problem to solve
 */
ssNSGAII::ssNSGAII(Problem *problem) : Algorithm(problem) {
} // ssNSGAII


/*
 * Runs the ssNSGA-II algorithm.
 * @return a <code>SolutionSet</code> that is a set of non dominated solutions
 * as a result of the algorithm execution
 */
SolutionSet * ssNSGAII::execute() {

  int populationSize;
  int maxEvaluations;
  int evaluations;
  
  int IntervalOptSubsModel;

  // TODO: QualityIndicator indicators; // QualityIndicator object
  int requiredEvaluations; // Use in the example of use of the
                           // indicators object (see below)

  SolutionSet * population;
  SolutionSet * offspringPopulation;
  SolutionSet * unionSolution;

  Operator * mutationOperator;
  Operator * crossoverOperator;
  Operator * selectionOperator;

  Distance * distance = new Distance();

  //Read the parameters
  populationSize = *(int *) getInputParameter("populationSize");
  maxEvaluations = *(int *) getInputParameter("maxEvaluations");
  IntervalOptSubsModel = *(int *) getInputParameter("intervalupdateparameters");
  // TODO: indicators = (QualityIndicator) getInputParameter("indicators");

  //Initialize the variables
  population = new SolutionSet(populationSize);
  evaluations = 0;

  requiredEvaluations = 0;

  //Read the operators
  mutationOperator = operators_["mutation"];
  crossoverOperator = operators_["crossover"];
  selectionOperator = operators_["selection"];
  
  ApplicationTools::displayTask("Initial Population", true);
  
  // Create the initial solutionSet
  Solution * newSolution;
  Phylogeny * p = (Phylogeny *) problem_;
  
  
  for (int i = 0; i < populationSize; i++) {
    newSolution = new Solution(problem_);
    
    if(p->StartingOptRamas){
        p->BranchLengthOptimization(newSolution,p->StartingMetodoOptRamas,p->StartingNumIterOptRamas,p->StartingTolerenciaOptRamas);
    }
    
    if(p->OptimizacionSubstModel)
        p->OptimizarParamModeloSust(newSolution);
       
    
    problem_->evaluate(newSolution);
    problem_->evaluateConstraints(newSolution);
    evaluations++;
    population->add(newSolution);
  } //for
   ApplicationTools::displayTaskDone();
   
   
  // Generations
  while (evaluations < maxEvaluations) {
    
    // Create the offSpring solutionSet
    offspringPopulation = new SolutionSet(populationSize);
    Solution ** parents = new Solution*[2];
    
     if(evaluations%100==0){ 
         cout << "Evaluating  " <<  evaluations << endl;
     }
     
     
    //obtain parents
    parents[0] = (Solution *) (selectionOperator->execute(population));
    parents[1] = (Solution *) (selectionOperator->execute(population));
    
    // crossover
    Solution ** offSpring = (Solution **) (crossoverOperator->execute(parents));
    
    // mutation
    mutationOperator->execute(offSpring[0]);
    
    ((Phylogeny *)problem_)->Optimization(offSpring[0]); //Optimize and update the scores (Evaluate OffSpring)
    
    // evaluation
    //problem_->evaluate(offSpring[0]);
    //problem_->evaluateConstraints(offSpring[0]);
    
    // insert child into the offspring population
    offspringPopulation->add(offSpring[0]);
    
    evaluations ++;
    delete[] offSpring;
    delete[] parents;
    
    // Create the solutionSet union of solutionSet and offSpring
    unionSolution = population->join(offspringPopulation);
    delete offspringPopulation;

    // Ranking the union
    Ranking * ranking = new Ranking(unionSolution);

    int remain = populationSize;
    int index = 0;
    SolutionSet * front = NULL;
    for (int i=0;i<population->size();i++) {
      delete population->get(i);
    }
    population->clear();

    // Obtain the next front
    front = ranking->getSubfront(index);

    while ((remain > 0) && (remain >= front->size())) {
      //Assign crowding distance to individuals
      distance->crowdingDistanceAssignment(front, problem_->getNumberOfObjectives());
      //Add the individuals of this front
      for (int k = 0; k < front->size(); k++) {
        population->add(new Solution(front->get(k)));
      } // for
      
      //Decrement remain
      remain = remain - front->size();
      
      //Obtain the next front
      index++;
      if (remain > 0) {
        front = ranking->getSubfront(index);
      } // if
    } // while

    // Remain is less than front(index).size, insert only the best one
    if (remain > 0) {  // front contains individuals to insert
      distance->crowdingDistanceAssignment(front, problem_->getNumberOfObjectives());
      Comparator * c = new CrowdingComparator();
      front->sort(c);
      delete c;
      for (int k = 0; k < remain; k++) {
        population->add(new Solution(front->get(k)));
      } // for

      remain = 0;
    } // if

    delete ranking;
    delete unionSolution;

       //Update Interval
    if(evaluations%IntervalOptSubsModel==0 and IntervalOptSubsModel > 0){ 
        Solution * sol;  double Lk;
        Phylogeny * p = (Phylogeny*) problem_;
        //cout << "Updating and Optimizing Parameters.." << endl;
        for(int i=0; i<population->size(); i++){
            sol =  population->get(i);
            Lk=  p->BranchLengthOptimization(sol,p->OptimizationMetodoOptRamas,p->OptimizationNumIterOptRamas,p->OptimizationTolerenciaOptRamas);
            sol->setObjective(1,Lk*-1);
        }
        //cout << "Update Interval Done!!" << endl;
    }
    
    // This piece of code shows how to use the indicator object into the code
    // of NSGA-II. In particular, it finds the number of evaluations required
    // by the algorithm to obtain a Pareto front with a hypervolume higher
    // than the hypervolume of the true Pareto front.
// TODO:
//    if ((indicators != NULL) &&
//      (requiredEvaluations == 0)) {
//      double HV = indicators.getHypervolume(population);
//      if (HV >= (0.98 * indicators.getTrueParetoFrontHypervolume())) {
//        requiredEvaluations = evaluations;
//      } // if
//    } // if

  } // while

  delete distance;

  // Return as output parameter the required evaluations
  // TODO:
  //setOutputParameter("evaluations", requiredEvaluations);

  // Return the first non-dominated front
  Ranking * ranking = new Ranking(population);
  SolutionSet * result = new SolutionSet(ranking->getSubfront(0)->size());
  for (int i=0;i<ranking->getSubfront(0)->size();i++) {
    result->add(new Solution(ranking->getSubfront(0)->get(i)));
  }
  delete ranking;
  delete population;

  return result;

} // execute
