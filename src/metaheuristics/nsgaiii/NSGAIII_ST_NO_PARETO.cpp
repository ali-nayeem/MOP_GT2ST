/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NSGAIII_ST.cpp
 * Author: ali_nayeem
 * 
 * Created on January 11, 2019, 11:38 AM
 */

#include "NSGAIII_ST_NO_PARETO.h"
#include "Checkpoint.h"
#include "RandomSelection.h"
#include "BinaryTournament2.h"
#include <InferSpeciesTree.h>

using namespace bpp;

NSGAIII_ST_NO_PARETO::NSGAIII_ST_NO_PARETO(Problem *problem) : NSGAIII_ST(problem)
{
   // checkpoint_ = NULL;
}

SolutionSet * NSGAIII_ST_NO_PARETO::execute()
{
    int populationSize;
    int maxEvaluations;
    int evaluations;
    //int IntervalOptSubsModel;

    //  QualityIndicator * indicators; // QualityIndicator object
    //int requiredEvaluations; // Use in the example of use of the
    // indicators object (see below)

    SolutionSet *population;
    SolutionSet *offspringPopulation;
    SolutionSet *unionSolution;

    Operator *mutationOperator;
    Operator *crossoverOperator;
    Operator *selectionOperator;
    Operator *initializerOperator;
    map<string, void *> parameters;
    //Selection * randSel = new RandomSelection(parameters);
    //Selection * binTourSel = new BinaryTournament2(parameters);
    //Distance * distance = new Distance();

    //Read the parameters
    populationSize = *(int *)getInputParameter("populationSize");
    maxEvaluations = *(int *)getInputParameter("maxEvaluations");
    checkpoint_ = (Checkpoint *)getInputParameter("checkpoint");
    //IntervalOptSubsModel = *(int *) getInputParameter("intervalupdateparameters");
    //  indicators = (QualityIndicator *) getInputParameter("indicators");
    // obj_division_p_.push_back(*(int *)getInputParameter("p1"));
    // if (getInputParameter("p2") != NULL)
    // {
    //     obj_division_p_.push_back(*(int *)getInputParameter("p2"));
    // }

    //Initialize the variables
    //population = new SolutionSet(populationSize);
    evaluations = 0;

    //requiredEvaluations = 0;

    //Read the operators
    mutationOperator = operators_["mutation"];
    crossoverOperator = operators_["crossover"];
    selectionOperator = operators_["selection"];
    initializerOperator = operators_["initializer"];

    vector<CReferencePoint> rps;
    GenerateReferencePoints(&rps, problem_->getNumberOfObjectives(), obj_division_p_);
    populationSize = rps.size();
    //GenerateReferencePointsRand(&rps, problem_->getNumberOfObjectives(), populationSize, 0.5, 0.004);
    //while (populationSize%4) populationSize += 1;

    //ApplicationTools::displayTask("Initial Population", true);
    // Create the initial solutionSet
    //Solution * newSolution;

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
                parents = (Solution **)(selectionOperator->execute(population));
                //parents[1] = (Solution *)(selectionOperator->execute(population));
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

        //SolutionSet fro
        // Obtain the next front
        // front = ranking->getSubfront(index);

        int lastFrontRank = ranking->getNumberOfSubfronts() - 1;//index; //Fl in the paper
        if (remain > 0)
        {
            Niching(population, populationSize, rps, ranking, lastFrontRank, p);
            remain = 0;
        }

        delete ranking;
        delete unionSolution;

    } // while

    //ApplicationTools::displayTaskDone();

    //delete distance;

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

void NSGAIII_ST_NO_PARETO::Niching(SolutionSet *population, int populationSize, vector<CReferencePoint> rps, Ranking *ranking, int lastFrontRank, InferSpeciesTree *prob)
{
    // ---------- Step 14 / Algorithm 2 ----------
    vector<double> ideal_point = TranslateObjectives(ranking, lastFrontRank);
    vector<Solution *> extreme_points;
    FindExtremePoints(&extreme_points, ranking);
    vector<double> intercepts;
    ConstructHyperplane(&intercepts, extreme_points, ranking);
    NormalizeObjectives(ranking, lastFrontRank, intercepts, ideal_point);
    // ---------- Step 15 / Algorithm 3, Step 16 ----------
    AssociateAll(&rps, ranking, lastFrontRank);
    for (size_t i = 0; i < rps.size(); i++)
    {
        rps[i].CalculateWeightForAllPotentialMember();
    }
    
    while (population->size() < populationSize) 
    {
        size_t min_rp = FindNicheReferencePoint(rps);

        Solution *chosen = rps[min_rp].PickWeightedMemberProbabilisticWithoutReplace(); //SelectClusterMember(rps[min_rp]);
        if (chosen == nullptr) // no potential member in Fl, disregard this reference point
        {
            rps.erase(rps.begin() + min_rp);
        }
        else
        {
            rps[min_rp].AddMember();
            //rps[min_rp].RemovePotentialMember(chosen);
            population->add(new Solution(chosen));
        }
    }
}

// void NSGAIII_ST::Assignment(SolutionSet *population, int populationSize, vector<CReferencePoint> rps)
// {
//     while (population->size() < populationSize) //for (int k = 0; k < remain; k++) while (population->size() < populationSize)
//     {
//         size_t min_rp = FindNicheReferencePoint(rps);

//         Solution *chosen = SelectClusterMember(rps[min_rp]);
//         if (chosen == nullptr) // no potential member in Fl, disregard this reference point
//         {
//             rps.erase(rps.begin() + min_rp);
//         }
//         else
//         {
//             rps[min_rp].AddMember();
//             rps[min_rp].RemovePotentialMember(chosen);
//             //next.push_back(cur[chosen]);
//             population->add(new Solution(chosen));
//         }
//     }
// }
