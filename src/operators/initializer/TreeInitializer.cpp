/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InitSpeciesTree.cpp
 * Author: ali_nayeem
 * 
 * Created on August 19, 2019, 10:21 PM
 */

#include "TreeInitializer.h"

TreeInitializer::TreeInitializer(map<string, void *> parameters) : Operator(parameters)
{
    method = "from_gene_trees";
    if (parameters["method"] != NULL)
        method = *(string *) parameters["method"];
    popSize = 20;
    if (parameters["popSize"] != NULL)
        popSize = *(int *) parameters["popSize"];
    if (parameters["problem"] != NULL)
        problem = (InferSpeciesTree *) parameters["problem"];
    unique = false;
    if (parameters["unique"] != NULL)
        unique = *(bool *) parameters["unique"];
    if (parameters["mutation"] != NULL)
        mut = (Operator *) parameters["mutation"];
    if (parameters["crossover"] != NULL)
        cross = (Operator *) parameters["crossover"];
}

void TreeInitializer::addOperator(string name, Operator* op)
{
    operators[name] = op;
}

void * TreeInitializer::execute(void *object)
{
//    if (operators["mutation"] != NULL)
//        mut = (Operator *) parameters["mutation"];
//    if (operators["crossover"] != NULL)
//        cross = (Operator *) parameters["crossover"];
    if(object != NULL)
        popSize = *(int *) object;
    //InferSpeciesTree *problem = (InferSpeciesTree *) object;
    SolutionSet * pop;
    if (method == "from_tools")
        pop = fromTools(popSize);
    else if (method == "from_gene_trees")
        pop = fromGeneTrees(popSize);
    else if (method == "from_true_tree")
        pop = fromTrueTree(popSize);
    else
        throw Exception("Population initialization method name doesn't matched with any!");
    return pop;
}

TreeInitializer::~TreeInitializer()
{
}

void TreeInitializer::fillupNewPopulationUsingOld(SolutionSet * newPop, SolutionSet * oldPop)
{
    
    Solution ** parents = new Solution*[2];
    Solution * offSpring;
    int parent1, parent2;
    //vector<Solution> children, parents;
    while (newPop->size() < newPop->getMaxSize())
    {
        if(cross != NULL)
        {

            parent1 = PseudoRandom::randInt(0, oldPop->size() - 1);
            do
            {
                parent2 = PseudoRandom::randInt(0, oldPop->size() - 1);
            }
            while (parent1 == parent2);

            parents[0] = oldPop->get(parent1);
            parents[1] = oldPop->get(parent2);

            //Solution * sol = new Solution(parents[0]);

            offSpring = (Solution *) (cross->execute(parents));
            if (problem->isMultifurcating(offSpring))
            {
            #ifdef MAN_DEBUG
                        cout << "TreeCrossover returned a non-binary tree. " << endl;
            #endif
                continue;
            }
        }
        else //No crossover
        {
            offSpring = new Solution(oldPop->get(PseudoRandom::randInt(0, oldPop->size() - 1)));
        }
        if(mut != NULL)
        {
            mut->execute(offSpring);
            if (problem->isMultifurcating(offSpring))
            {
            #ifdef MAN_DEBUG
                cout << "When non-binary, then pllvalidator: " << problem->PLLisTreeValidate(offSpring) << endl;
            #endif
                continue;
            }
            if (problem->getNumberOfLeaves(offSpring) != problem->getNumberOfTaxa())
            {
            #ifdef MAN_DEBUG
                cout << "When less taxa, then pllvalidator: " << problem->PLLisTreeValidate(offSpring) << endl;
            #endif
                continue;
            }
        }
        if (unique)
        {
            problem->evaluate(offSpring);
            bool match = false;
            for(int i=0; i < newPop->size(); i++)
            {
                if(problem->matchObjectiveValues(offSpring, newPop->get(i)))
                {
                    #ifdef MAN_DEBUG
                        cout << "Duplicate tree found. Discarded. " << endl;
                    #endif
                    match = true;
                    break;
                }
                //for()
            }
            if(match)
                    continue;
        }
        newPop->add(offSpring);
        #ifdef MAN_DEBUG
                   cout << "So far added solutions: " << newPop->size()<< endl;
        #endif
    }
    delete[] parents;
    //return newPop;
}

SolutionSet * TreeInitializer::fromGeneTrees(int size)
{
    SolutionSet * genePop = problem->getSolutionSetFromVarFile(problem->getGeneTreePath()); //datapath + "gene.tre"
    SolutionSet * outPop = new SolutionSet(size);
    fillupNewPopulationUsingOld(outPop, genePop);
    return outPop;
}

SolutionSet * TreeInitializer::fromTools(int size)
{
    SolutionSet * pop = new SolutionSet(size);
    //unordered_set<string> uniqueSolutions;
    vector< PhyloTree* > PrecomputedTrees = problem->readPrecomputedSpeciesTree();
    for (int i = 0; i < PrecomputedTrees.size(); i++)
    {
        Variable **variables = new Variable*[problem->getNumberOfVariables()];
        variables[0] = new PhyloTree(PrecomputedTrees[i]);
        Solution * newSolution = new Solution(problem, variables);
        pop->add(newSolution);
    }
    if (unique)
    {
        problem->evaluate(pop);
    }
    fillupNewPopulationUsingOld(pop, pop);
    return pop;
}

SolutionSet * TreeInitializer::fromTrueTree(int size)
{
    SolutionSet * trueTreePop = new SolutionSet(1);
    trueTreePop->add(problem->getTrueTree());
    SolutionSet * outPop = new SolutionSet(size);
    unique = true;
    fillupNewPopulationUsingOld(outPop, trueTreePop);
    return outPop;
}

//newPop = new SolutionSet(size);
    /*map<string, void *> parameters;
    double prob = 0.8; int numDes = 1.0;
    parameters["probability"] = &prob;
    parameters["numDescendientes"] = &numDes;
    TreeCrossover * crossover = new TreeCrossover(parameters);

    prob = 1.0;
    parameters.clear();
    parameters["probability"] = &prob;

    string method[] = {"NNI", "SPR", "TBR"};
    vector<Mutation *> mutList1, mutList2;

    parameters["metodo"] = &method[0];
    Mutation * NNI = new PhylogeneticMutation(parameters);
    parameters["metodo"] = &method[1];
    Mutation * SPR = new PhylogeneticMutation(parameters);
    parameters["metodo"] = &method[2];
    Mutation * TBR = new PhylogeneticMutation(parameters);


    mutList1.push_back(NNI);
    mutList1.push_back(SPR);
    mutList1.push_back(TBR);
    parameters.clear();
    parameters["probability"] = &prob;
    parameters["mutationList"] = &mutList1;
    Mutation * ShufMut = new ShuffledMutation(parameters);

    mutList2.push_back(NNI);
    mutList2.push_back(SPR);
    mutList2.push_back(TBR);
    //mutList2.push_back(ShufMut);
    parameters.clear();
    prob = 1.0;
    parameters["probability"] = &prob;
    parameters["mutationList"] = &mutList2;
    Mutation * mulMut = new MultipleRandomMutation(parameters);*/