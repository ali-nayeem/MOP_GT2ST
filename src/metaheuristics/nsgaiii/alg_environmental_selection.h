#ifndef ENVIRONMENTAL_SELECTION__
#define ENVIRONMENTAL_SELECTION__
#include <vector>
#include "Ranking.h"


// ----------------------------------------------------------------------
//	The environmental selection mechanism is the key innovation of 
//  the NSGA-III algorithm.
//
//  Check Algorithm I in the original paper of NSGA-III.
// ----------------------------------------------------------------------

class CPopulation;
class CReferencePoint;

// void EnvironmentalSelection(CPopulation *pnext, // population in the next generation
// 							CPopulation *pcur,  // population in the current generation
// 							std::vector<CReferencePoint> rps, // the set of reference points
// 							std::size_t PopSize);
vector<double> TranslateObjectives( Ranking * fronts, int lastFront);
void FindExtremePoints(vector<Solution *> *extreme_points,  Ranking * fronts);
void ConstructHyperplane(vector<double> *pintercepts, const vector<Solution *> &extreme_points, Ranking * fronts);
void NormalizeObjectives(Ranking * fronts, int lastFrontRank, const vector<double> &intercepts, const vector<double> &ideal_point);
size_t FindNicheReferencePoint(const vector<CReferencePoint> &rps);
Solution * SelectClusterMember(const CReferencePoint &rp);





#endif
