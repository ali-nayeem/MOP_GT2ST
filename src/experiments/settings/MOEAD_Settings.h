//  MOEAD_Settings.h
//
//  Author:
//       Antonio J. Nebro <antonio@lcc.uma.es>
//       Esteban López-Camacho <esteban@lcc.uma.es>
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

#ifndef __MOEAD_SETTINGS__
#define __MOEAD_SETTINGS__

#include <Settings.h>
#include <MOEAD_ST.h>
#include <SBXCrossover.h>
#include <TreeCrossover.h>
#include <PhylogeneticMutation.h>
#include <InferSpeciesTree.h>
#include <BinaryTournament2.h>
#include <algorithm>
#include "MultipleRandomMutation.h"
#include "Checkpoint.h"

class MOEAD_Settings : public Settings{
private:
	int populationSize_                 ;
	int maxEvaluations_                 ;
	double mutationProbability_         ;
	double crossoverProbability_        ;
        int maxGen_;

  Algorithm * algorithm ;
  Operator  * crossover ; // Crossover operator
  Operator  * mutation  ; // Mutation operator
  Operator  * selection ; // Selection operator
  Checkpoint * checkpoint_;
  Operator * initializer;
  

public:
	MOEAD_Settings() ;
	MOEAD_Settings(string problemName, Checkpoint * checkpoint ) ;
	~MOEAD_Settings() ;

  Algorithm * configure() ;
}; // MOEAD_Settings

#endif // __MOEAD_SETTINGS__
