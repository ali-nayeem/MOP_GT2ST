/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MultipleProbMutation.cpp
 * Author: ali_nayeem
 * 
 * Created on December 29, 2018, 2:33 PM
 */

#include <Bpp/Numeric/Random/RandomTools.h>

#include "MultipleProbMutation.h"

MultipleProbMutation::MultipleProbMutation(map<string, void *> parameters) : Mutation(parameters){
    if (parameters["mutationList"] != NULL)
        mutList = *(vector <Mutation *> *)parameters["mutationList"];
    if (parameters["probability"] != NULL)
        mutationProbability_ = *(double *)parameters["probability"];
    for(int i = 0; i < mutList.size(); i++)
    {
        weight.push_back(1.0);
        id.push_back(i);
        count.push_back(0);
    }

}


MultipleProbMutation::~MultipleProbMutation() {
    cout << "MultipleProbMutation weights: ";
    for(int i=0; i < mutList.size(); i++)
    {
        cout << weight[i] << ",";   
    }
    cout << endl;
    cout << "Mutation prop: ";
    for(int i=0; i < mutList.size(); i++)
    {
        cout << count[i] << ",";   
    }
    cout << endl;
}

void * MultipleProbMutation::execute(void *object)  {
  Solution *solution = (Solution *)object;
  // TODO: VALID_TYPES?
  //double probability = *(double *)getParameter("probability");
  if ( PseudoRandom::randDouble() <= mutationProbability_) {
      
    int randIndex = RandomTools::pickOne(id, weight, true, *PseudoRandom::getRndFactory()); //PseudoRandom::randInt(0, mutList.size()-1);
    //RandomTools::pickOne(id, weight, true, *PseudoRandom::getRndFactory())->execute(solution);
    #ifdef MAN_DEBUG
    cout<<"Apply Mut: "<<randIndex<<endl;
    #endif
    mutList[randIndex]->execute(solution);
    solution->setMutId(randIndex);
    count[randIndex] = count[randIndex] + 1;
  }
  
  return solution;
}

void MultipleProbMutation::increaseWeight(int id)
{
    weight[id] = weight[id] + 1.0;
}

void MultipleProbMutation::decreaseWeight(int id)
{
    if(weight[id] > 1.0)
    {
        weight[id] = weight[id] - 1.0;
    }
}

void MultipleProbMutation::decreaseAllWeight( int count)
{
//    cout << "MultipleProbMutation weights: ";
//    for(int i=0; i < mutList.size(); i++)
//    {
//        cout << weight[i] << ",";   
//    }
//    cout << endl;
//    int dec = count / (weight.size()+1);
    for(int id=0; id < weight.size(); id++)
    {
      
       weight[id] = 1.0;
       
    }
//    
    //cout << "Mutation prop: ";
}