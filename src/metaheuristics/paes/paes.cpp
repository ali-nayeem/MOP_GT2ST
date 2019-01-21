//  paes.cpp
//
//  Author:
//       Cristian Zambrano <czambrano@uteq.edu.ec>
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


#include <paes.h>

 /**
  * Create a new PAES instance for resolve a problem
  * @param problem Problem to solve
  */

paes::paes(Problem * problem) : Algorithm(problem){ }

/**
   * Tests two solutions to determine which one becomes be the guide of PAES
   * algorithm
   * @param solution The actual guide of PAES
   * @param mutatedSolution A candidate guide
  */

Solution * paes::test(Solution * solution, Solution * mutatedSolution,
                       AdaptiveGridArchive * archive){

    int originalLocation = archive->getGrid()->location(solution);
    int mutatedLocation  = archive->getGrid()->location(mutatedSolution);

    if (originalLocation == -1) {
      return new Solution(mutatedSolution);
    }

    if (mutatedLocation == -1) {
      return new Solution(solution);
    }

    if (archive->getGrid()->getLocationDensity(mutatedLocation) <
        archive->getGrid()->getLocationDensity(originalLocation)) {
      return new Solution(mutatedSolution);
    }

    return new Solution(solution);
  } // test

  /**
  * Runs of the Paes algorithm.
  * @return a <code>SolutionSet</code> that is a set of non dominated solutions
  * as a result of the algorithm execution
  */

SolutionSet * paes::execute() {
    int bisections, archiveSize, maxEvaluations, evaluations,IntervalOptSubsModel;
    AdaptiveGridArchive * archive;

    Operator * mutationOperator;
    Comparator * dominance;

    //Read the params
    bisections = *(int *) getInputParameter("biSections");
    maxEvaluations = *(int *) getInputParameter("maxEvaluations");
    archiveSize = *(int *) getInputParameter("archiveSize");
    IntervalOptSubsModel = *(int *) getInputParameter("intervalupdateparameters");

    //Read the operators
    mutationOperator = operators_["mutation"];

    //Initialize the variables
    evaluations = 0;
    archive     = new AdaptiveGridArchive(archiveSize,bisections,problem_->getNumberOfObjectives());
    dominance = new DominanceComparator();

    //-> Create the initial solution and evaluate it and his constraints
    Solution * solution = new Solution(problem_);
    
    Phylogeny * p = (Phylogeny *) problem_;
    if(p->StartingOptRamas)
         p->BranchLengthOptimization(solution,p->StartingMetodoOptRamas,p->StartingNumIterOptRamas,p->StartingTolerenciaOptRamas);
    
    if(p->OptimizacionSubstModel)
        p->OptimizarParamModeloSust(solution);

    problem_->evaluate(solution);
    problem_->evaluateConstraints(solution);
    evaluations++;

    // Add it to the archive
    archive->add(new Solution(solution),evaluations );

    //Iterations....
    do {
      // Create the mutate one
      Solution * mutatedIndividual = new Solution(solution);
      mutationOperator->execute(mutatedIndividual);

      ((Phylogeny *)problem_)->Optimization(mutatedIndividual); //Optimize and update the scores (Evaluate OffSpring)
      
      
      problem_->evaluate(mutatedIndividual);
      problem_->evaluateConstraints(mutatedIndividual);
      evaluations++;
      
      // Check dominance
      int flag = dominance->compare(solution,mutatedIndividual);

      if (flag == 1) { //If mutate solution dominate
        solution = new Solution(mutatedIndividual);
        archive->add(mutatedIndividual,evaluations );
      } else if (flag == 0) { //If none dominate the other
            if (archive->add(mutatedIndividual,evaluations )) {
              solution = test(solution,mutatedIndividual,archive);
        }
      }

      
      if(evaluations%IntervalOptSubsModel==0 and IntervalOptSubsModel > 0){ 
          Solution * sol;  double Lk;
          Phylogeny * p = (Phylogeny*) problem_;
          cout << "Updating and Optimizing Parameters.." << endl;
          for(int i=0; i<archive->size(); i++){
            sol =  archive->get(i);
            Lk=  p->BranchLengthOptimization(sol,p->OptimizationMetodoOptRamas,p->OptimizationNumIterOptRamas,p->OptimizationTolerenciaOptRamas);
            sol->setObjective(1,Lk*-1);
          }
          cout << "Update Interval Done!!" << endl;
       }  
      
      if((evaluations % 100) == 0)   cout << "Evaluations " << evaluations << endl;
      
    } while (evaluations < maxEvaluations);

    //Return the  population of non-dominated solution
    return archive;
 }  // execute

