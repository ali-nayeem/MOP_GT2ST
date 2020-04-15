#include "alg_environmental_selection.h"
//#include "alg_population.h"
#include "aux_math.h"
//#include "alg_nondominated_sort.h"
#include "alg_reference_point.h"
#include "Ranking.h"
#include "InferSpeciesTree.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// ----------------------------------------------------------------------
// TranslateObjectives():
//
// 1. Find the ideal point
// 2. Translate the objective values
// 3. Return the ideal point
//
// Check steps 1-3 in Algorithm 2 in the original paper of NSGAIII.
// ----------------------------------------------------------------------
vector<double> TranslateObjectives( Ranking * fronts, int lastFrontRank)
{
	//CPopulation &pop = *ppop;
	const size_t NumObj = fronts->getSubfront(0)->get(0)->getNumberOfObjectives();
	vector<double> ideal_point(NumObj); 
	
	for (size_t f=0; f<NumObj; f+=1)
	{
		double minf = numeric_limits<double>::max();
		for (size_t i=0; i<fronts->getSubfront(0)->size(); i+=1) // min,i.e. best, values must appear in the first front
		{
			minf = std::min(minf, fronts->getSubfront(0)->get(i)->getObjective(f));  
		}
		ideal_point[f] = minf;

		for (size_t t=0; t < lastFrontRank+1; t+=1)
		{
			for (size_t i=0; i<fronts->getSubfront(t)->size(); i+=1)
			{
				Solution * ind = fronts->getSubfront(t)->get(i);
				ind->conv_objs().resize(NumObj);
				ind->conv_objs()[f] = ind->getObjective(f) - minf;
			}
		}
	}

	return ideal_point;

}// TranslateObjectives()

// ----------------------------------------------------------------------
// FindExtremePoints():
//
// Find the extreme points along each objective axis.
// The extreme point has the minimal ASF value.
// Return pointers to the extreme individuals in the population.
//
// Check step 4 in Algorithm 2 and eq. (4) in the original paper.
// ----------------------------------------------------------------------
void FindExtremePoints(vector<Solution *> *extreme_points, Ranking * fronts)
{
	vector<Solution *> &exp = *extreme_points;
	exp.clear();
	const size_t NumObj = fronts->getSubfront(0)->get(0)->getNumberOfObjectives();


	for (size_t f=0; f<NumObj; f+=1)
	{
		vector<double> w(NumObj, 0.000001);
		w[f] = 1.0;

		double min_ASF = numeric_limits<double>::max();
		Solution * min_indv ; //= fronts->getSubfront(0)->size();

		for (size_t i=0; i<fronts->getSubfront(0)->size(); i+=1)  // only consider the individuals in the first front
		{
			double asf = MathAux::ASF(fronts->getSubfront(0)->get(i)->conv_objs(), w); // nsga3cpp 1.11 (2015.04.26 thanks to Vivek Nair for his correction by email)

			if ( asf < min_ASF )
			{
				min_ASF = asf;
				min_indv = fronts->getSubfront(0)->get(i);
			}
		}

		exp.push_back(min_indv);
	}

}// FindExtremePoints()


// ----------------------------------------------------------------------
// FindMaxObjectives(): (added in nsga3cpp v1.2)
//
// Find the maximal objective values among the current solutions.
// Intercepts are set by these values when we cannot construct the
// hyperplane.
//
// This method follows the implementation in the following paper:
//
// Yuan Yuan, Hua Xu, Bo Wang,
// "An Experimental Investigation of Variation Operators in
//  Reference-Point Based Many-Objective Optimization,"
// GECCO, pp. 775-782, 2015
//
// I think Jerry Lee for finding out the difference between the two
// implementations and recommending the modification.
//
// ----------------------------------------------------------------------
vector<double> FindMaxObjectives(Ranking * fronts)
{
	const size_t NumObj = fronts->getSubfront(0)->get(0)->getNumberOfObjectives();

	vector<double> max_point(NumObj, -numeric_limits<double>::max());
	for(int rank=0; rank < fronts->getNumberOfSubfronts(); rank++) //(size_t i=0; i<pop.size(); i+=1)
	{
		for(size_t i=0; i<fronts->getSubfront(rank)->size(); i+=1)
		{
			Solution * ind = fronts->getSubfront(rank)->get(i);
			for (size_t f=0; f<NumObj; f+=1)
			{
				max_point[f] = std::max(max_point[f], ind->getObjective(f)); //pop[i].objs()[f]); 
			}
		}
	}

	return max_point;
}

// ----------------------------------------------------------------------
// ConstructHyperplane():
//
// Given the extreme points, construct the hyperplane.
// Then, calculate the intercepts.
//
// Check step 6 in Algorithm 2 in the original paper.
// ----------------------------------------------------------------------
void ConstructHyperplane(vector<double> *pintercepts, const vector<Solution *> &extreme_points, Ranking * fronts)
{
	// Check whether there are duplicate extreme points.
	// This might happen but the original paper does not mention how to deal with it.
	bool duplicate = false;
	for (size_t i=0; !duplicate && i<extreme_points.size(); i+=1)
	{
		for (size_t j=i+1; !duplicate && j<extreme_points.size(); j+=1)
		{
			duplicate = (extreme_points[i] == extreme_points[j]);
		}
	}

	const size_t NumObj = extreme_points[0]->getNumberOfObjectives();
	vector<double> &intercepts = *pintercepts;
	intercepts.assign(NumObj, 0);

	bool negative_intercept = false;
	if (!duplicate)
	{
		// Find the equation of the hyperplane
		vector<double> b(NumObj, 1.0);
		vector< vector<double> > A;
		for (size_t p=0; p<extreme_points.size(); p+=1)
		{
			A.push_back(extreme_points[p]->conv_objs()); // v1.11: objs() -> conv_objs()
		}
		vector<double> x;
		MathAux::GuassianElimination(&x, A, b);

		// Find intercepts
		for (size_t f=0; f<intercepts.size(); f+=1)
		{
			intercepts[f] = 1.0/x[f];

			if(x[f] < 0)
			{
				negative_intercept = true;
				break;
			}
		}
	}

	if (duplicate || negative_intercept) // v1.2: follow the method in Yuan et al. (GECCO 2015)
	{
		vector<double> max_objs = FindMaxObjectives(fronts);
		for (size_t f=0; f<intercepts.size(); f+=1)
		{
			intercepts[f] = max_objs[f];
		}
	}
}

// ----------------------------------------------------------------------
// NormalizeObjectives():
//
// Normalize objective values with respect to the intercepts and the ideal point.
// Check step  7 in Algorithm 2 and eq. (5) in the original paper.
// ----------------------------------------------------------------------
void NormalizeObjectives(Ranking * fronts, int lastFrontRank, const vector<double> &intercepts, const vector<double> &ideal_point)
{
	for (size_t t=0; t<lastFrontRank+1; t+=1)
	{
		for (size_t i=0; i<fronts->getSubfront(t)->size(); i+=1)
		{
			Solution * ind = fronts->getSubfront(t)->get(i);
			for (size_t f=0; f<ind->conv_objs().size(); f+=1)
			{
				double denom = intercepts[f] - ideal_point[f];
				if ( denom > 10e-10 ) // avoid the divide-by-zero error. prev:fabs(intercepts[f])>10e-10
					ind->conv_objs()[f] = ind->conv_objs()[f]/(denom); // v1.11: fixed
				else
					ind->conv_objs()[f] = ind->conv_objs()[f]/10e-10;
			}
		}
	}

}// NormalizeObjectives()

// ----------------------------------------------------------------------
// FindNicheReferencePoint():
//
// Find the reference point with the minimal cluster size.
// Return one randomly if there is more than one point.
//
// Check steps 3-4 in Algorithm 4 in the original paper.
// ----------------------------------------------------------------------
size_t FindNicheReferencePoint(const vector<CReferencePoint> &rps)
{
	// find the minimal cluster size
	size_t min_size = numeric_limits<size_t>::max();
	for (size_t r=0; r<rps.size(); r+=1)
	{
		min_size = std::min(min_size, rps[r].MemberSize());
	}

	// find the reference points with the minimal cluster size Jmin
	vector<size_t> min_rps;
	for (size_t r=0; r<rps.size(); r+=1)
	{
		if (rps[r].MemberSize() == min_size)
		{
			min_rps.push_back(r);
		}
	}

	// return a random reference point (j-bar)
	return min_rps[PseudoRandom::randInt(0, min_rps.size() - 1)]; //PseudoRandom::randInt(0, min_rps.size() - 1);  rand()%min_rps.size()
}

// ----------------------------------------------------------------------
// SelectClusterMember():
//
// Select a potential member (an individual in the front Fl) and associate
// it with the reference point.
//
// Check the last two paragraphs in Section IV-E in the original paper.
// ----------------------------------------------------------------------
Solution * SelectClusterMember(const CReferencePoint &rp)
{
	Solution * chosen =  nullptr;
	if (rp.HasPotentialMember())
	{
		if (rp.MemberSize() == 0) // currently has no member
		{
			chosen =  rp.FindClosestMember();
		}
		else
		{
			chosen =  rp.RandomMember();
		}
	}
	return chosen;
}
Solution * SelectClusterMemberProbabilistic(const CReferencePoint &rp)
{
	Solution * chosen =  nullptr;
	if (rp.HasPotentialMember())
	{
		if (rp.MemberSize() == 0) // currently has no member
		{
			chosen =  rp.FindClosestMember();
		}
		else
		{
			chosen =  rp.RandomMember();
		}
	}
	return chosen;
}
// ----------------------------------------------------------------------
// EnvironmentalSelection():
//
// Check Algorithms 1-4 in the original paper.
// ----------------------------------------------------------------------
// void EnvironmentalSelection(CPopulation *pnext, CPopulation *pcur, vector<CReferencePoint> rps, size_t PopSize)
// {
// 	CPopulation &cur = *pcur, &next = *pnext;
// 	next.clear();

// 	// ---------- Step 4 in Algorithm 1: non-dominated sorting ----------
// 	CNondominatedSort::TFronts fronts = NondominatedSort(cur);

// 	// ---------- Steps 5-7 in Algorithm 1 ----------
// 	vector<size_t> considered; // St
// 	size_t last = 0, next_size = 0;
// 	while (next_size < PopSize)
// 	{
// 		next_size += fronts[last].size();
// 		last += 1;
// 	}
// 	fronts.erase(fronts.begin()+last, fronts.end()); // remove useless individuals

// 	for (size_t t=0; t<fronts.size()-1; t+=1)
// 	{
// 		for (size_t i=0; i<RemovePotentialMember.size(); i+=1)
// 		{
// 			next.push_back(cur[ fronts[t][i] ]);
// 		}
// 	}

// 	// ---------- Steps 9-10 in Algorithm 1 ----------
// 	if (next.size() == PopSize) return;


// 	// ---------- Step 14 / Algorithm 2 ----------
// 	vector<double> ideal_point = TranslateObjectives(&cur, fronts);

// 	vector<size_t> extreme_points;
// 	FindExtremePoints(&extreme_points, cur, fronts);

// 	vector<double> intercepts;
// 	ConstructHyperplane(&intercepts, cur, extreme_points);

// 	NormalizeObjectives(&cur, fronts, intercepts, ideal_point);

// 	// ---------- Step 15 / Algorithm 3, Step 16 ----------
// 	Associate(&rps, cur, fronts);

// 	// ---------- Step 17 / Algorithm 4 ----------
// 	while (next.size() < PopSize)
// 	{
// 		size_t min_rp = FindNicheReferencePoint(rps);

// 		int chosen = SelectClusterMember(rps[min_rp]);
// 		if (chosen < 0) // no potential member in Fl, disregard this reference point
// 		{
// 			rps.erase(rps.begin()+min_rp);
// 		}
// 		else
// 		{
// 			rps[min_rp].AddMember();
// 			rps[min_rp].RemovePotentialMember(chosen);
// 			next.push_back(cur[chosen]);
// 		}
// 	}

// }
// ----------------------------------------------------------------------