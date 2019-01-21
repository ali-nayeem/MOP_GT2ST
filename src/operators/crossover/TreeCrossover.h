/** 
 * MO-Phylogenetics (version 1.0.0) a software tool for multi-objective 
 * phylogenetic inference. This software integrates features 
 * of the jMetalCpp, Bio++ and PLL frameworks.
 * 
 * Copyright (C) 2015 Cristian Zambrano-Vega, Antonio J. Nebro.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For any other enquiries send an Email to Cristian Zambrano
 * czambrano@uteq.edu.ec
 *
 * When publishing work that uses this software please cite us.
 * 
 * @file TreeCrossover.h
 */

#ifndef TREECROSSOVER_H_
#define TREECROSSOVER_H_

#include <Crossover.h>
#include <Solution.h>
#include <math.h>
#include <PhyloTree.h>
//#include <Phylogeny.h>
#include <InferSpeciesTree.h>

#include <PseudoRandom.h>

#include <Bpp/Phyl/Io/Newick.h>
#include <Bpp/Phyl/Node.h>
#include <Bpp/Phyl/TreeTemplate.h>
#include <Bpp/Phyl/TreeTemplateTools.h>



using namespace bpp;

/**
 * This class allows to apply a Tree crossover operator using two parent
 * solutions.
**/
class TreeCrossover : public Crossover {

public:
  TreeCrossover(map<string, void *> parameters);
  ~TreeCrossover();
  void* execute(void *);
  void printParameters();
  
protected:


private:
  double crossoverProbability_;
  int numDescendientes_ ;
  Solution * doCrossover(double probability, Solution * parent1, Solution * parent2);
  void CrossTrees(PhyloTree * PtMon, PhyloTree * PtDad);
  boolean implementCrossover(Solution *parent1, Solution *parent2, Solution *offSpring);
  Node * selectNodeToCross(TreeTemplate<Node> * tree_, vector<int> nodosIDs );




};

#endif /* TREECROSSOVER_H_ */
