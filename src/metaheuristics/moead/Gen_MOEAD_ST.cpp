#include "Gen_MOEAD_ST.h"
#include <InferSpeciesTree.h>

Gen_MOEAD_ST::Gen_MOEAD_ST(Problem *problem) : Algorithm(problem) //, Checkpoint *checkpoint
{
  checkpoint_ = NULL;
}

SolutionSet *Gen_MOEAD_ST::execute()
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
    obj_division_p_.push_back(*(int *)getInputParameter("p1"));
    if (getInputParameter("p2") != NULL)
    {
        obj_division_p_.push_back(*(int *)getInputParameter("p2"));
    }

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

    // for (size_t i = 0; i < rps.size(); i++)
    // {
    //     if (rps[i].pos()[1] > 1.6) //to be discard
    //     {
    //        rps.erase(rps.begin() + i);
    //        i--;
    //     }
        
    // }
    

    InferSpeciesTree *p = (InferSpeciesTree *)problem_;
    // Create the initial solutionSet
    population = (SolutionSet *)initializerOperator->execute(&populationSize); //p->createInitialPopulationGeneTrees(populationSize);
    p->evaluate(population);
    evaluations += populationSize;
    double** aggr_score = new double*[rps.size()];
    
    //double aggr_score[6][10];
    vector<double> min_score(rps.size());
    vector<int> min_index(rps.size());
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

        for (int i = 0; i < population->size(); i++)
        {
            delete population->get(i);
        }
        population->clear();

        //Ranking the union
        //Ranking *ranking = new Ranking(unionSolution, true);
        Ranking *ranking = new Ranking(unionSolution);
        
        int index = 0, lastFrontRank = 0;
        SolutionSet *front = NULL;
        front = ranking->getSubfront(index);    
        while (population->size() + front->size() < 1.5*populationSize)
        {
            /* code */
            for (int k = 0; k < front->size(); k++)
            {
                //if(population->size() < populationSize)
                    population->add(new Solution(front->get(k)));
            } // for

            index++;
            front = ranking->getSubfront(index);
            lastFrontRank = index; //Fl in the paper
            //break;
            //int lastFrontRank = ranking->getNumberOfSubfronts() - 1;//index; //Fl in the paper
        }
        
        vector<double> ideal_point = TranslateObjectives(ranking, ranking->getNumberOfSubfronts()-1);
        vector<Solution *> extreme_points;
        FindExtremePoints(&extreme_points, ranking);
        vector<double> intercepts;
        ConstructHyperplane(&intercepts, extreme_points, ranking);
        NormalizeObjectives(ranking, ranking->getNumberOfSubfronts()-1, intercepts, ideal_point);
        
        SolutionSet *remaining = new SolutionSet(unionSolution->size() - population->size());
        for (size_t f = lastFrontRank; f < ranking->getNumberOfSubfronts(); f++)
        {
            for (size_t i = 0; i < ranking->getSubfront(f)->size(); i++)
            {
                remaining->add(ranking->getSubfront(f)->get(i));
            }        
        }
        for(int i = 0; i < rps.size(); ++i)
            aggr_score[i] = new double[remaining->size()];

        for(int r = 0; r < rps.size(); ++r)
        {
            for (size_t j = 0; j < remaining->size(); j++)
            {
                aggr_score[r][j] = MathAux::AggregationFunction(remaining->get(j)->conv_objs(), rps[r].pos(), 1);   
            }    
            int min_sol_id = std::min_element(aggr_score[r], aggr_score[r] + remaining->size()) - aggr_score[r];
            min_index[r] = min_sol_id;
            min_score[r] = aggr_score[r][min_sol_id];    
        }
        while (population->size() < populationSize)
        {
            int selected_ref = std::distance(min_score.begin(), std::min_element(min_score.begin(), min_score.end()));
            int selected_solution = min_index[selected_ref];
            population->add(new Solution(remaining->get(selected_solution)));

            //min_score[selected_ref] = std::numeric_limits<double>::max(); //inf
            //min_index[selected_ref] = -1; //no longer consider the selected_ref
            
            for (size_t j = 0; j < rps.size(); j++)
            {
                aggr_score[j][selected_solution] = std::numeric_limits<double>::max();
            }
            for (size_t j = 0; j < min_index.size(); j++)
            {
                if (min_index[j] == selected_solution)
                {
                    min_index[j] = std::distance(aggr_score[j], std::min_element(aggr_score[j], aggr_score[j] + remaining->size()));
                    min_score[j] = aggr_score[j][min_index[j]];
                }
                
            }
        }
        for(int i = 0; i < rps.size(); ++i)
           delete aggr_score[i];
        //delete unionSolution;
        delete ranking;
        delete unionSolution;
    } // while
    if (checkpoint_ != NULL)
    {
        checkpoint_->logVARforce(population, gen);
    }

//   delete [] ranking_;
    //delete population;

    return population; //result;
}