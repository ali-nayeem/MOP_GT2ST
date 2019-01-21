//  PhyloMOCHC.cpp
//  //
//  //  Author:
//	 Some phylogenetic features were added by Cristian Zambrano-Vega
//       <czambrano@uteq.edu.ec>
//  //
//  //  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//  //
//  //  This program is free software: you can redistribute it and/or modify
//  //  it under the terms of the GNU Lesser General Public License as published by
//  //  the Free Software Foundation, either version 3 of the License, or
//  //  (at your option) any later version.
//  //
//  //  This program is distributed in the hope that it will be useful,
//  //  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  //  GNU Lesser General Public License for more details.
//  //
//  //  You should have received a copy of the GNU Lesser General Public License
//  //  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <PhyloMOCHC.h>

bool PhyloMOCHC::equalsIndividuals(Solution & s1, Solution & s2) {
	
     PhyloTree *Pt1=(PhyloTree *)s1.getDecisionVariables()[0];;
     PhyloTree *Pt2=(PhyloTree *)s2.getDecisionVariables()[0];; 

     TreeTemplate<Node> * t1=  Pt1->getTree();
     TreeTemplate<Node> * t2 = Pt1->getTree();
     
     return  t1->hasSameTopologyAs(*t2,true);
                    
}

bool PhyloMOCHC::exist(Solution & s1, SolutionSet & set2) {
	for (int i = 0; i < set2.size(); i++) {
		if (equalsIndividuals(s1,*set2.get(i)))
			return true;
	}
	return false;
}


bool PhyloMOCHC::equals(SolutionSet & set1, SolutionSet & set2) {

	if (set1.size() != set2.size())
		return false;

	for (int i = 0; i < set1.size(); i++) {
		if (!exist(*set1.get(i),set2))
			return false;
	}
	return true;
} // returns the equal


int PhyloMOCHC::RFDistance(Solution * sol1, Solution * sol2) {

  PhyloTree *Pt1, *Pt2; 
  Pt1 = (PhyloTree *)sol1->getDecisionVariables()[0];
  Pt2 = (PhyloTree *)sol2->getDecisionVariables()[0];
 
  TreeTemplate<Node> * tree1 = Pt1->getTree();
  TreeTemplate<Node> * tree2 = Pt2->getTree();
  
  int distance = TreeTools::robinsonFouldsDistance(*tree1,*tree2, true);
  
  return distance;
  
}

SolutionSet *PhyloMOCHC::rankingAndCrowdingSelection(SolutionSet * pop, int size) {

    SolutionSet *result = new SolutionSet(size);
    // Ranking the union
    Ranking * ranking = new Ranking(pop);    
    Distance * distance = new Distance();
    int remain = size;
    int index = 0;
    SolutionSet * front = NULL;

    // Obtain the next front
    front = ranking->getSubfront(index);

    while ((remain > 0) && (remain >= front->size())) {
      //Assign crowding distance to individuals
      distance->crowdingDistanceAssignment(front, problem_->getNumberOfObjectives());

      //Add the individuals of this front
      for (int k = 0; k < front->size(); k++) {
        result->add(new Solution(front->get(k)));
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
        result->add(new Solution(front->get(k)));
      } // for

      remain = 0;
    } // if

    delete ranking;
    delete distance;

    return result;	
}


SolutionSet *PhyloMOCHC::execute() {
	
  int populationSize;
  int iterations;
  int maxEvaluations;
  int convergenceValue;
  int minimumDistance;
  int evaluations;
  int IntervalOptSubsModel;
   

  double preservedPopulation;
  double initialConvergenceCount;
  bool condition = false;
  SolutionSet *solutionSet, *offSpringPopulation, *newPopulation; 

  Comparator * crowdingComparator = new CrowdingComparator();

  SolutionSet * population;
  SolutionSet * offspringPopulation;
  SolutionSet * unionSolution;

  Operator * cataclysmicMutation;
  Operator * crossover;
  Operator * parentSelection;


  //Read the parameters
  populationSize = *(int *) getInputParameter("populationSize");
  maxEvaluations = *(int *) getInputParameter("maxEvaluations");
  IntervalOptSubsModel = *(int *) getInputParameter("intervalupdateparameters");
   
  convergenceValue = *(int *) getInputParameter("convergenceValue");
  initialConvergenceCount = *(double *)getInputParameter("initialConvergenceCount");
  preservedPopulation = *(double *)getInputParameter("preservedPopulation");
  
  //Read the operators
  cataclysmicMutation = operators_["mutation"];
  crossover	      = operators_["crossover"];
  parentSelection     = operators_["selection"];
  
  iterations  = 0;
  evaluations = 0;

   // calculating the maximum problem sizes .... 
   int size = 0;

    Solution * sol = new Solution(problem_);
    PhyloTree *Pt1 = (PhyloTree *)sol->getDecisionVariables()[0];
    TreeTemplate<Node> * tree1 = Pt1->getTree();
    BipartitionList* bipL1 = new BipartitionList(*tree1, true);
    bipL1->removeTrivialBipartitions();
    
    size = bipL1->getNumberOfBipartitions() * 2;
    

    delete bipL1;
    delete sol;
  
  minimumDistance = (int) std::floor(initialConvergenceCount*size);

  cout << "Minimun Distance " << minimumDistance << endl;
  
  // Create the initial solutionSet
  Solution * newSolution;
  
  ApplicationTools::displayTask("Initial Population", true);
   
  population = new SolutionSet(populationSize);
  Phylogeny * p = (Phylogeny *) problem_;
  
  for (int i = 0; i < populationSize; i++) {
    
      newSolution = new Solution(problem_);
     
      if(p->StartingOptRamas){
        p->BranchLengthOptimization(newSolution,p->StartingMetodoOptRamas,p->StartingNumIterOptRamas,p->StartingTolerenciaOptRamas);
      }
    
      if(p->OptimizacionSubstModel){
          p->OptimizarParamModeloSust(newSolution);
      }

    problem_->evaluate(newSolution);
    problem_->evaluateConstraints(newSolution);
    
    evaluations++;
    population->add(newSolution);
  } //for

  ApplicationTools::displayTaskDone();

  
  while (!condition) {
      
        cout << "Evaluating  " <<  evaluations << endl;
       
	offSpringPopulation = new SolutionSet(populationSize);
 	Solution **parents = new Solution*[2];
	
	for (int i = 0; i < population->size()/2; i++) {
               
  		parents[0] = (Solution *) (parentSelection->execute(population));
		parents[1] = (Solution *) (parentSelection->execute(population));

		if (RFDistance(parents[0],parents[1])>= minimumDistance) {
                    
		   Solution ** offSpring = (Solution **) (crossover->execute(parents));
                   
                   ((Phylogeny *)problem_)->Optimization(offSpring[0]); //Optimize and update the scores (Evaluate OffSpring)
                   ((Phylogeny *)problem_)->Optimization(offSpring[1]);
        
		   /*problem_->evaluate(offSpring[0]);
		   problem_->evaluateConstraints(offSpring[0]);
	           problem_->evaluate(offSpring[1]);
		   problem_->evaluateConstraints(offSpring[1]);*/
                   
		   evaluations+=2;
                   
		   offSpringPopulation->add(offSpring[0]);
		   offSpringPopulation->add(offSpring[1]);
                   delete[] offSpring;
		}		
	}  
        
	SolutionSet *join = population->join(offSpringPopulation);
 	delete offSpringPopulation;

	newPopulation = rankingAndCrowdingSelection(join,populationSize);
	delete join;
        if (equals(*population,*newPopulation)) {
		minimumDistance--;
	}   

	if (minimumDistance <= -convergenceValue) {
		minimumDistance = (int) (1.0/size * (1-1.0/size) * size);
		int preserve = (int) std::floor(preservedPopulation*populationSize);
		newPopulation->clear(); 
		population->sort(crowdingComparator);
		for (int i = 0; i < preserve; i++) {
			newPopulation->add(new Solution(population->get(i)));
		}
		for (int i = preserve; i < populationSize; i++) {
			Solution * solution = new Solution(population->get(i));
			cataclysmicMutation->execute(solution);
			problem_->evaluate(solution);
			problem_->evaluateConstraints(solution);	
			newPopulation->add(solution);
		}
		
	}

        //Update Interval
     if(evaluations%IntervalOptSubsModel==0 and IntervalOptSubsModel > 0){ 
        Solution * sol;  double Lk;
        Phylogeny * p = (Phylogeny*) problem_;
        cout << "Updating and Optimizing Parameters.." << endl;
        for(int i=0; i<newPopulation->size(); i++){
            sol =  newPopulation->get(i);
            Lk=  p->BranchLengthOptimization(sol,p->OptimizationMetodoOptRamas,p->OptimizationNumIterOptRamas,p->OptimizationTolerenciaOptRamas);
            sol->setObjective(1,Lk*-1);
        }
        cout << "Update Interval Done!!" << endl;
      }
        
	iterations++;
	delete population;
	population = newPopulation;
	if (evaluations >= maxEvaluations) {
		condition = true;		
	}
  }

  return population;

}
