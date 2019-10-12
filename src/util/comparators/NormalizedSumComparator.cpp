//  NormalizedSumComparator.cpp
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


#include <NormalizedSumComparator.h>


/**
 * Constructor.
 * @param nObj The index of the objective to compare
 */
NormalizedSumComparator::NormalizedSumComparator() {
  //this->nObj = nObj;
} // NormalizedSumComparator


/**
 * Compare two solutions.
 * @param o1 Object representing the first <code>Solution</code>.
 * @param o2 Object representing the second <code>Solution</code>.
 * @return -1, or 0, or 1 if o1 is less than, equal, or greater than o2,
 * respectively.
**/
int NormalizedSumComparator::compare(void *o1, void *o2) {
  if (o1==NULL)
    return 1;
  else if (o2 == NULL) 
    return -1;
  Solution * s1 = (Solution *) o1;
  Solution * s2 = (Solution *) o2;
  //InferSpeciesTree * inferST = (InferSpeciesTree *) s1->getProblem();
  //double * normalizedObjective = new double[prob->getNumberOfObjectives()];
//  double objetive1 = 0, objetive2 = 0;
//    for(int n = 0; n < inferST->getNumberOfObjectives(); n++)
//    {
//        //cout << "Min and max are " << inferST->getMaxObjective(n) << " " << inferST->getMinObjective(n)<<endl;
//        objetive1 += (s1->getObjective(n) - inferST->getMinObjective(n)) / (inferST->getMaxObjective(n) - inferST->getMinObjective(n));      
//        objetive2 += (s2->getObjective(n) - inferST->getMinObjective(n)) / (inferST->getMaxObjective(n) - inferST->getMinObjective(n));
//    }
  double objetive1, objetive2;
  objetive1 = s1->getFitness(); 
  objetive2 = s2->getFitness();
  if (objetive1 < objetive2) {
    return -1;
  } else if (objetive1 > objetive2) {
    return 1;
  } else {
    return 0;
  }
} // compare

