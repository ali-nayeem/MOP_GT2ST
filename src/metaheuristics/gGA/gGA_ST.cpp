//  gGA_ST.cpp
//
//  Author:
//       Antonio J. Nebro <antonio@lcc.uma.es>
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

#include <gGA_ST.h>

#include "MultipleProbMutation.h"
#include "RandomSelection.h"

/*
 * This class implements the NSGA-II algorithm.
 */


/**
 * Constructor
 * @param problem Problem to solve
 */
gGA_ST::gGA_ST(Problem *problem) : Algorithm(problem) {
} // gGA_ST


/*
 * Runs the NSGA-II algorithm.
 * @return a <code>SolutionSet</code> that is a set of non dominated solutions
 * as a result of the algorithm execution
 */
SolutionSet * gGA_ST::execute() {

  int populationSize;
  int maxEvaluations;
  int evaluations;

  SolutionSet * population;
  SolutionSet * offspringPopulation;
  SolutionSet * unionSolution;

  Operator * mutationOperator;
  Operator * crossoverOperator;
  Operator * selectionOperator;
  map<string, void *> parameters;
  Operator * randSelection = new RandomSelection(parameters);
  Operator * initializerOperator;
  Distance * distance = new Distance();
  

  //Read the parameters
  populationSize = *(int *) getInputParameter("populationSize");
  maxEvaluations = *(int *) getInputParameter("maxEvaluations");
  Checkpoint * checkpoint_ = (Checkpoint *) getInputParameter("checkpoint");
  Comparator * comparator = (Comparator *) getInputParameter("comparator");

  //Initialize the variables
  population = new SolutionSet(populationSize);
  evaluations = 0;


  //Read the operators
  mutationOperator = operators_["mutation"];
  crossoverOperator = operators_["crossover"];
  selectionOperator = operators_["selection"];
  initializerOperator = operators_["initializer"];

//  cout << "Comienza la inicializacion de la poblacion con size " << populationSize << endl;

  InferSpeciesTree * p = (InferSpeciesTree *) problem_;
  // Create the initial solutionSet
  population = (SolutionSet*) initializerOperator->execute(&populationSize);//p->createInitialPopulationGeneTrees(populationSize);
  p->evaluate(population);
  evaluations += populationSize;
  p->evaluateFitness(population); 

  // Generations
  int gen = 0;
  while (evaluations < maxEvaluations) 
  {
   addedMutIdCount = 0;   
   if (checkpoint_ != NULL)
    {
        checkpoint_->logVAR(population, gen);
    }
    cout<< "Thread[" << checkpoint_->getThreadId() << "]: " <<"Generation: "<<gen++ << endl;
  
    // Create the offSpring solutionSet
    offspringPopulation = new SolutionSet(populationSize);
    Solution ** parents = new Solution*[2];

    for (int i = 0; i < (populationSize); i++) {
      if (evaluations < maxEvaluations) {
        //obtain parents
        parents[0] = ((Solution **) (randSelection->execute(population)))[0];
//        parents[0] = (Solution *) (selectionOperator->execute(population));
        parents[1] = (Solution *) (selectionOperator->execute(population));
        Solution * offSpring = (Solution *) (crossoverOperator->execute(parents));
        mutationOperator->execute(offSpring);

        offspringPopulation->add(offSpring);

        //delete offSpring;
      } // if
    } // for
    //if(gen > 1)
    ((MultipleProbMutation *) mutationOperator)->decreaseAllWeight(population->getMaxSize());
    delete[] parents;
    p->evaluate(offspringPopulation);
    evaluations += offspringPopulation->size();
    
    unionSolution = population->join(offspringPopulation);
    delete offspringPopulation;
    
    p->evaluateFitness(unionSolution);
    //unionSolution->sort(comparator);
    unionSolution->sortFitness();
//    population->sort(comparator) ;
//    offspringPopulation->sort(comparator) ;
//
//    delete offspringPopulation->get(offspringPopulation->size()-1);
//    delete offspringPopulation->get(offspringPopulation->size()-2);
//    offspringPopulation->replace(offspringPopulation->size()-1, new Solution(population->get(0))) ;
//    offspringPopulation->replace(offspringPopulation->size()-2, new Solution(population->get(1))) ;

    for (int i=0;i<population->size();i++) {
      delete population->get(i);
    }
    population->clear() ;
    double lastFitness = -1.0e+30;
    for (int i = 0; i < population->getMaxSize(); i++)
    {
        if((unionSolution->get(0)->getFitness() - lastFitness) > 0.0001)
        {
            population->add(unionSolution->get(0)) ;
            increaseMutWeight(unionSolution->get(0), mutationOperator);
            lastFitness = unionSolution->get(0)->getFitness();
        }
        //delete unionSolution->get(0);
        unionSolution->remove(0);
    }
    //cout<< "pop, union: "<<population->size()<<", "<<unionSolution->size()<<endl;
    distance->crowdingDistanceAssignment(unionSolution, problem_->getNumberOfObjectives());
    Comparator * c = new CrowdingComparator();
    unionSolution->sort(c);
    delete c;
    
    for (int i = population->size(); i < population->getMaxSize(); i++)
    {
        increaseMutWeight(unionSolution->get(0), mutationOperator);
        population->add(unionSolution->get(0)) ;
        unionSolution->remove(0);
    }
    
    while (unionSolution->size() > 0)
    {
        //decreaseMutWeight(unionSolution->get(0), mutationOperator);
        delete unionSolution->get(0);
        unionSolution->remove(0);
    }
    
    
    //unionSolution->clear() ;
    delete unionSolution;
  }

  delete comparator;
  if (checkpoint_ != NULL)
  {
    checkpoint_->logVARforce(population, gen);
  }
//  SolutionSet * resultPopulation  = new SolutionSet(1) ;
//  resultPopulation->add(new Solution(population->get(0))) ;
//  delete population;

  return population;//resultPopulation ;
} // execute

void gGA_ST::increaseMutWeight(Solution * sol, Operator * mut)
{
//    if(sol->getMutId() > -1)
//    {
//        ((MultipleProbMutation *) mut)->increaseWeight(sol->getMutId());
//        sol->setMutId(-1);
//        addedMutIdCount++;
//    }
//    else
//    {
//    }
}

void gGA_ST::decreaseMutWeight(Solution * sol, Operator * mut)
{
    if(sol->getMutId() > -1)
    {
        ((MultipleProbMutation *) mut)->decreaseWeight(sol->getMutId());
    }
    else
    {
        sol->setMutId(-1);
    }
}
