/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NSGAII_ST.cpp
 * Author: ali_nayeem
 * 
 * Created on January 11, 2019, 11:38 AM
 */

#include "NSGAII_ST.h"
#include <InferSpeciesTree.h>

using namespace bpp;

NSGAII_ST::NSGAII_ST(Problem *problem) : Algorithm(problem) {
}

SolutionSet * NSGAII_ST::execute() {
    int populationSize;
    int maxEvaluations;
    int evaluations;
    int IntervalOptSubsModel;

    //  QualityIndicator * indicators; // QualityIndicator object
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
    //IntervalOptSubsModel = *(int *) getInputParameter("intervalupdateparameters");
    //  indicators = (QualityIndicator *) getInputParameter("indicators");

    //Initialize the variables
    //population = new SolutionSet(populationSize);
    evaluations = 0;

    requiredEvaluations = 0;

    //Read the operators
    mutationOperator = operators_["mutation"];
    crossoverOperator = operators_["crossover"];
    selectionOperator = operators_["selection"];

    //ApplicationTools::displayTask("Initial Population", true);
    // Create the initial solutionSet
    Solution * newSolution;



    InferSpeciesTree * prob = (InferSpeciesTree *) problem_;
    // Create the initial solutionSet
    population = prob->createInitialPopulation(populationSize);
    prob->evaluate(population);
    evaluations += populationSize;

    // Generations
    //ApplicationTools::displayTask("Generations", true);
    while (evaluations < maxEvaluations) {
        unordered_set<string> uniqueSolutionSet;
        // Create the offSpring solutionSet
        offspringPopulation = new SolutionSet(populationSize);
        Solution ** parents = new Solution*[2];

        //cout << "Evaluating  " << evaluations << endl;

        for (int i = 0; i < (populationSize); i++) {

            if (evaluations < maxEvaluations) {
                //obtain parents
                parents[0] = (Solution *) (selectionOperator->execute(population));
                parents[1] = (Solution *) (selectionOperator->execute(population));

                Solution * offSpring = (Solution *) (crossoverOperator->execute(parents));
                if(prob->isMultifurcating(offSpring))
                    continue;
                Solution * copy;
                do {
                    copy = new Solution(offSpring);
                    mutationOperator->execute(copy);
                } while ( (prob->getNumberOfLeaves(copy) != prob->getNumberOfTaxa()) );
                if(prob->isMultifurcating(copy))
                {
                    continue;
                }
                
                /*Variable **variables = copy->getDecisionVariables();
                if(uniqueSolutionSet.find(variables[0]->toString()) != uniqueSolutionSet.end())
                {
                    cout<<"Duplicate found!"<<endl;
                    continue;
                }
                else
                {
                    uniqueSolutionSet.insert(variables[0]->toString());
                }*/
                
                offspringPopulation->add(copy);
                //offspringPopulation->add(offSpring[1]);
                //cout << "Next " << evaluations << endl;
                delete offSpring;

            } // if
        } // for

        delete[] parents;

        prob->evaluate(offspringPopulation);
        evaluations += offspringPopulation->size();

        // Create the solutionSet union of solutionSet and offSpring
        unionSolution = population->join(offspringPopulation);
        delete offspringPopulation;

        // Ranking the union
        Ranking * ranking = new Ranking(unionSolution);

        int remain = populationSize;
        int index = 0;
        SolutionSet * front = NULL;
        for (int i = 0; i < population->size(); i++) {
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
        if (remain > 0) { // front contains individuals to insert
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

    }// while

    //ApplicationTools::displayTaskDone();

    delete distance;

    // Return as output parameter the required evaluations
    //  setOutputParameter("evaluations", &requiredEvaluations);

    // Return the first non-dominated front
    Ranking * ranking = new Ranking(population);
    SolutionSet * result = new SolutionSet(ranking->getSubfront(0)->size());
    for (int i = 0; i < ranking->getSubfront(0)->size(); i++) {
        result->add(new Solution(ranking->getSubfront(0)->get(i)));
    }
    delete ranking;
    delete population;

    return result;

} // execute

