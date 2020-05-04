/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NSGAII_ST_MidDistRank.cpp
 * Author: ali_nayeem
 * 
 * Created on January 11, 2019, 11:38 AM
 */

#include "NSGAII_ST_MidDistRank.h"
#include "Checkpoint.h"
#include "RandomSelection.h"
#include "BinaryTournament2.h"
#include <InferSpeciesTree.h>

using namespace bpp;

NSGAII_ST_MidDistRank::NSGAII_ST_MidDistRank(Problem *problem) : Algorithm(problem)
{
    checkpoint_ = NULL;
}

SolutionSet *NSGAII_ST_MidDistRank::execute()
{
    int populationSize;
    int maxEvaluations;
    int evaluations;
    int IntervalOptSubsModel;

    //  QualityIndicator * indicators; // QualityIndicator object
    int requiredEvaluations; // Use in the example of use of the
    // indicators object (see below)

    SolutionSet *population;
    SolutionSet *offspringPopulation;
    SolutionSet *unionSolution;

    Operator *mutationOperator;
    Operator *crossoverOperator;
    Operator *selectionOperator;
    Operator *initializerOperator;
    map<string, void *> parameters;
    Operator *randSelection = new RandomSelection(parameters);
    Selection *binTourSel = new BinaryTournament2(parameters);
    Distance *distance = new Distance();

    //Read the parameters
    populationSize = *(int *)getInputParameter("populationSize");
    maxEvaluations = *(int *)getInputParameter("maxEvaluations");
    checkpoint_ = (Checkpoint *)getInputParameter("checkpoint");
    //IntervalOptSubsModel = *(int *) getInputParameter("intervalupdateparameters");
    //  indicators = (QualityIndicator *) getInputParameter("indicators");

    //Initialize the variables
    //population = new SolutionSet(populationSize);
    double minCosineSim = 0.936585;
    evaluations = 0;

    requiredEvaluations = 0;

    //Read the operators
    mutationOperator = operators_["mutation"];
    crossoverOperator = operators_["crossover"];
    selectionOperator = operators_["selection"];
    initializerOperator = operators_["initializer"];

    //ApplicationTools::displayTask("Initial Population", true);
    // Create the initial solutionSet
    Solution *newSolution;

    InferSpeciesTree *p = (InferSpeciesTree *)problem_;
    // Create the initial solutionSet
    population = (SolutionSet *)initializerOperator->execute(&populationSize); //p->createInitialPopulationGeneTrees(populationSize);
    p->evaluate(population);
    evaluations += populationSize;

    // Generations
    //ApplicationTools::displayTask("Generations", true);
    int gen = 0;
    while (evaluations < maxEvaluations)
    {
        if (checkpoint_ != NULL)
        {
            checkpoint_->logVAR(population, gen);
        }
        cout << "Thread[" << checkpoint_->getThreadId() << "]: "
             << "Generation: " << gen++ << endl;
        // Create the offSpring solutionSet
        offspringPopulation = new SolutionSet(populationSize);
        Solution **parents = new Solution *[2];

        //cout << "Evaluating  " << evaluations << endl;

        for (int i = 0; i < (populationSize); i++)
        {

            if (evaluations < maxEvaluations)
            {
                //obtain parents
                parents[0] = ((Solution **)(randSelection->execute(population)))[0];
                parents[1] = (Solution *)(selectionOperator->execute(population));
                //Apply Crossover
                Solution *offSpring = (Solution *)(crossoverOperator->execute(parents));
                //Apply Mutation
                mutationOperator->execute(offSpring);
                offspringPopulation->add(offSpring);
            } // if
        }     // for

        delete[] parents;

        p->evaluate(offspringPopulation);
        evaluations += offspringPopulation->size();

        // Create the solutionSet union of solutionSet and offSpring
        unionSolution = population->join(offspringPopulation);
        delete offspringPopulation;

        //        p->evaluateFitness(unionSolution);
        //        unionSolution->sortFitness();
        //        for(int i = 0; i < population->getMaxSize()/2; i++)
        //        {
        //            delete unionSolution->get(unionSolution->size()-1-i);
        //            unionSolution->remove(unionSolution->size()-1-i);
        //        }

        // Ranking the union
        Ranking *ranking = new Ranking(unionSolution);

        int remain = populationSize;
        int index = 0;
        SolutionSet *front = NULL;
        for (int i = 0; i < population->size(); i++)
        {
            delete population->get(i);
        }
        population->clear();

        // Obtain the next front
        front = ranking->getSubfront(index);
        SolutionSet *reminent = new SolutionSet(2 * populationSize);
        while ((remain > 0) && (remain >= front->size()) )
        {

            //Add the individuals of this front
            for (int k = 0; k < front->size(); k++)
            {
                if (insideFocusAngle(front->get(k), p, minCosineSim))
                {
                    population->add(new Solution(front->get(k)));
                    remain--;
                }
                else
                {
                    reminent->add(front->get(k));
                }

            } // for

            //Decrement remain
            //remain = remain - front->size();

            //Obtain the next front
            index++;
            if (index == ranking->getNumberOfSubfronts())
            {
                break;
            }
            
            if (remain > 0)
            {
                front = ranking->getSubfront(index);
            } // if

        } // while

        // Remain is less than front(index).size, insert only the best one
        if (remain > 0)
        { 
            for (size_t i = index; i < ranking->getNumberOfSubfronts(); i++)
            {
                for (size_t j = 0; j < ranking->getSubfront(i)->size(); j++)
                {
                    reminent->add(ranking->getSubfront(i)->get(j));
                }
            }

            distance->crowdingDistanceAssignment(reminent, problem_->getNumberOfObjectives());
            Comparator *c = new CrowdingComparator();
            reminent->sort(c);
            delete c;
            for (int k = 0; k < remain; k++)
            {
                population->add(new Solution(reminent->get(k)));
            } // for

            remain = 0;
        } // if

        delete ranking;
        delete unionSolution;

    } // while

    //ApplicationTools::displayTaskDone();

    delete distance;

    // Return as output parameter the required evaluations
    //  setOutputParameter("evaluations", &requiredEvaluations);

    // Return the first non-dominated front
    //    Ranking * ranking = new Ranking(population);
    //    SolutionSet * result = new SolutionSet(ranking->getSubfront(0)->size());
    //    for (int i = 0; i < ranking->getSubfront(0)->size(); i++) {
    //        result->add(new Solution(ranking->getSubfront(0)->get(i)));
    //    }
    //      delete ranking;
    if (checkpoint_ != NULL)
    {
        checkpoint_->logVARforce(population, gen);
    }

    //delete population;

    return population; //result;

} // execute

bool NSGAII_ST_MidDistRank::insideFocusAngle(Solution *sol, InferSpeciesTree *prob, double minCosineSim)
{
    sol->conv_objs().resize(prob->getNumberOfObjectives());

    for (size_t i = 0; i < prob->getNumberOfObjectives(); i++)
    {
        sol->conv_objs()[i] = (sol->getObjective(i) - prob->getMinObjective(i)) / (prob->getMaxObjective(i) - prob->getMinObjective(i));
    }
    
    if (MathAux::CosineSimilarityFromMid(sol->conv_objs()) > minCosineSim) //MathAux::CosineSimilarityFromMid(sol->conv_objs()) > minCosineSim   MathAux::CosineSimilarityFromMid(sol->conv_objs()) >= PseudoRandom::randDouble(0, minCosineSim)
    {
        return true;
    }
    else
    {
        return false;
    }
}
