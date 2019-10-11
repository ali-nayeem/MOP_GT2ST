//  DetTournament2.cpp
//
//  Author:
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


#include <DetTournament.h>


/**
 * @class DetTournament
 * @brief This class implements a binary tournament selection operator
**/


/**
 * Constructor
 * Creates a new Binary tournament operator using a DetTournamentComparator
 */
DetTournament::DetTournament(map<string, void *> parameters)
: Selection(parameters) {
  if (parameters["comparator"] != NULL)
    comparator_ = (Comparator *) parameters["comparator"];
  else
    comparator_ = new DominanceComparator();
  if (parameters["size"] != NULL)
    size_ = *(int *) parameters["size"];
  else
      size_ = 5;
} // DetTournament


/**
 * Destructor
 */
DetTournament::~DetTournament() {
  delete comparator_;
} // ~DetTournament


/**
* Performs the operation
* @param object Object representing a SolutionSet
* @return the selected solution
*/
void * DetTournament::execute(void * object) {

  SolutionSet * solutionSet = (SolutionSet *) object;
  int * perm = new int[solutionSet->size()];
  UtilsMOEAD::randomPermutation(perm, solutionSet->size());
  int index = perm[0];
  Solution * best = solutionSet->get(index);
  for(int i = 1; i < size_; i++)
  {
      index = perm[i];
      Solution * solution = solutionSet->get(index);
      int comp = comparator_->compare(best, solution);
      if (comp == 1) {
          best = solution;
      }
  }
  delete[] perm;
  return best;

} // execute
