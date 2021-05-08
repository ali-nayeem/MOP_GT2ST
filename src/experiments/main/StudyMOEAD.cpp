//  StandardStudyExecution.cpp
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


#include <StudyMOEAD.h>

/**
 * Class implementing a typical experimental study. Three algorithms are
 * compared when solving the ZDT, DTLZ, and WFG benchmarks, and the hypervolume,
 * spread and additive epsilon indicators are used for performance assessment.
 */


/**
 * Configures the algorithms in each independent run
 * @param problemName Name of the problem to solve
 * @algorithmId Index of the algorithm to be configured
 * @param experimentIndividualId Index of the experiment individual
 */
Algorithm * StudyMOEAD::algorithmSettings(string problemName,
    int algorithmId, int experimentIndividualId, Checkpoint *checkpoint) {

  Algorithm * alg;

  switch (algorithmId) {
  case 0:
    algorithmSettingsList_[experimentIndividualId] =
        new MOEAD_Settings(problemName, checkpoint);
    alg = (algorithmSettingsList_[experimentIndividualId])->configure();
    break;
  }

  return alg;

} // algorithmSettings


int main(int argc, char ** argv) {

  PseudoRandom::bppRand_->setSeed(01234567);
  Checkpoint::interval_ = 5;
  StudyMOEAD * exp = new StudyMOEAD() ;
  
  // Name of the experiment:
  exp->experimentName_ = "MOEAD-WS";
  exp->keepCheckpoint_ = true;

  // List of algorithm names to be used in the experiment
  // (please, refer to the README to check the possible values):
  vector<string> algorithmNameList_ {
      "MOEAD"}; //, "SMPSO", "GDE3"

  // List of problem names to be used in the experiment
  // (please, refer to the README to check the possible values):
  vector<string> problemList_ {
       "15-taxon.100gene-100bp.estimated-genetrees.R5"//, "10-taxon.higher-ILS.estimated-genetrees.R9", "10-taxon.higher-ILS.estimated-genetrees.R4",   
      //"15-taxon.100gene-100bp.estimated-genetrees.R6", "15-taxon.100gene-100bp.estimated-genetrees.R9"  
  };//, "10-taxon.higher-ILS.estimated-genetrees.R2", 
 //, "37-taxon.noscale_200g_500b.estimated-genetrees.R5", "10-taxon.higher-ILS.estimated-genetrees.R2"

  // Directory where the execution results will be stored:
  //exp->experimentBaseDirectory_ = "C:/jMetal/pruebas/jmetal-cpp/" +
  exp->experimentBaseDirectory_ = "experiment/" +
                                 exp->experimentName_;

  // Number of independent runs of each algorithm for each problem:
  exp->independentRuns_ = 1;

  // Number of threads to be used to execute the experiment
  int numberOfThreads = 1;
  
  exp->algorithmNameList_ = algorithmNameList_;
  exp->problemList_ = problemList_;
  int numberOfAlgorithms = exp->algorithmNameList_.size();
  
  cout << "Experiment name: " <<  exp->experimentName_ << endl;
  cout << "Experiment directory: " << exp->experimentBaseDirectory_ << endl;
  cout << "Number of threads: " << numberOfThreads << endl;

  cout << "Experiment (" << exp->experimentName_ << ") is starting." << endl;

  exp->runExperiment(numberOfThreads);
  exp->calculateTreePerf();
  //exp->calculateCheckpointTreePerf();

  cout << "Experiment (" << exp->experimentName_ << ") has finished." << endl;

  delete exp;

} // main

