#include "alg_reference_point.h"
#include "Ranking.h"
#include "aux_math.h"
#include "InferSpeciesTree.h"

#include <limits>
using namespace std;

void CReferencePoint::clear()
{
	member_size_ = 0;
	potential_members_.clear();
}
// ----------------------------------------------------------------------
void CReferencePoint::AddMember()
{
	member_size_ += 1;
}
// ----------------------------------------------------------------------
void CReferencePoint::AddPotentialMember(Solution *member_ind, double distance)
{
	potential_members_.push_back(make_pair(member_ind, distance));
}
// ----------------------------------------------------------------------
Solution *CReferencePoint::FindClosestMember() const
{
	double min_dist = numeric_limits<double>::max();
	Solution *min_indv = nullptr;
	for (size_t i = 0; i < potential_members_.size(); i += 1)
	{
		if (potential_members_[i].second < min_dist)
		{
			min_dist = potential_members_[i].second;
			min_indv = potential_members_[i].first;
		}
	}

	return min_indv;
}
// ----------------------------------------------------------------------
Solution *CReferencePoint::PickWeightedMemberProbabilisticWithoutReplace()
{
	Solution *good_weight = nullptr;
	if (potential_members_.size() > 0)
	{
		if (member_size_ >= 0)
		{
			double max = potential_members_weights_[0];
			int maxId = 0;
			for (size_t i = 1; i < potential_members_weights_.size(); i++)
			{
				if (potential_members_weights_[i] > max)
				{
					max = potential_members_weights_[i];
					maxId = i;
				}				
			}
			good_weight = potential_members_[maxId].first;
			potential_members_[maxId] = potential_members_.back();
			potential_members_.pop_back();
			potential_members_weights_[maxId] = potential_members_weights_.back();
			potential_members_weights_.pop_back();		
		}
		// else
		// {
		// 	good_weight = (RandomTools::pickOne(potential_members_, potential_members_weights_, false, *PseudoRandom::getRndFactory())).first;
		// }
	}

	return good_weight;
}
// ----------------------------------------------------------------------
Solution *CReferencePoint::RandomMember() const
{
	if (potential_members_.size() > 0)
	{
		return potential_members_[PseudoRandom::randInt(0, potential_members_.size() - 1)].first;
	}
	else
	{
		return nullptr;
	}
}
// ----------------------------------------------------------------------
void CReferencePoint::RemovePotentialMember(Solution *member_ind)
{
	for (size_t i = 0; i < potential_members_.size(); i += 1)
	{
		if (potential_members_[i].first == member_ind)
		{
			potential_members_.erase(potential_members_.begin() + i);
			return;
		}
	}
}

void CReferencePoint::CalculateWeightForAllPotentialMember()
{
	for (size_t i = 0; i < potential_members_.size(); i += 1)
	{
		double weight = 1.0 / (0.01 +  MathAux::ProjectedDistanceFromOrigin(position_, potential_members_[i].first->conv_objs()) + potential_members_[i].second) ;
		potential_members_weights_.push_back(weight);
		//potential_members_weights_.push_back(1.78 - MathAux::ProjectedDistanceFromOrigin(position_, (potential_members_[i].first)->conv_objs()));
		if (potential_members_weights_[potential_members_weights_.size() - 1] < 0)
		{
			/* code */
			cout << "Negative weigth in CalculateWeightForAllPotentialMember";
			exit(-1);
		}
	}
}
// ----------------------------------------------------------------------
// Other utility functions
// ----------------------------------------------------------------------
void generate_recursive(vector<CReferencePoint> *rps, CReferencePoint *pt, size_t num_objs,
						size_t left, size_t total, size_t element)
{
	if (element == num_objs - 1)
	{
		pt->pos()[element] = static_cast<double>(left) / total;
		rps->push_back(*pt);
	}
	else
	{
		for (size_t i = 0; i <= left; i += 1)
		{
			pt->pos()[element] = static_cast<double>(i) / total;
			generate_recursive(rps, pt, num_objs, left - i, total, element + 1);
		}
	}
}
// ----------------------------------------------------------------------
void GenerateReferencePoints(vector<CReferencePoint> *rps, size_t M, const std::vector<std::size_t> &p)
{
	CReferencePoint pt(M);

	generate_recursive(rps, &pt, M, p[0], p[0], 0);

	if (p.size() > 1) // two layers of reference points (Check Fig. 4 in NSGA-III paper)
	{
		vector<CReferencePoint> inside_rps;
		generate_recursive(&inside_rps, &pt, M, p[1], p[1], 0);

		double center = 1.0 / M;

		for (size_t i = 0; i < inside_rps.size(); i += 1)
		{
			for (size_t j = 0; j < inside_rps[i].pos().size(); j += 1)
			{
				inside_rps[i].pos()[j] = (center + inside_rps[i].pos()[j]) / 2; // (k=num_divisions/M, k, k, ..., k) is the center point
			}
			rps->push_back(inside_rps[i]);
		}
	}
}
// ----------------------------------------------------------------------
void Associate(std::vector<CReferencePoint> *prps, Ranking *fronts, int lastFrontRank)
{
	std::vector<CReferencePoint> &rps = *prps;

	for (size_t t = 0; t < lastFrontRank + 1; t += 1)
	{
		for (size_t i = 0; i < fronts->getSubfront(t)->size(); i += 1)
		{
			size_t min_rp = rps.size();
			double min_dist = numeric_limits<double>::max();
			for (size_t r = 0; r < rps.size(); r += 1)
			{
				double d = MathAux::PerpendicularDistance(rps[r].pos(), fronts->getSubfront(t)->get(i)->conv_objs());
				if (d < min_dist)
				{
					min_dist = d;
					min_rp = r;
				}
			}

			if (t != lastFrontRank) // associating members in St/Fl (only counting)
			{
				rps[min_rp].AddMember();
			}
			else
			{
				rps[min_rp].AddPotentialMember(fronts->getSubfront(t)->get(i), min_dist);
			}

		} // for - members in front
	}	  // for - fronts
}

void AssociateAll(std::vector<CReferencePoint> *prps, Ranking *fronts, int lastFrontRank)
{
	std::vector<CReferencePoint> &rps = *prps;

	for (size_t t = 0; t < lastFrontRank + 1; t += 1)
	{
		for (size_t i = 0; i < fronts->getSubfront(t)->size(); i += 1)
		{
			size_t min_rp = rps.size();
			double min_dist = numeric_limits<double>::max();
			for (size_t r = 0; r < rps.size(); r += 1)
			{
				double d = MathAux::PerpendicularDistance(rps[r].pos(), fronts->getSubfront(t)->get(i)->conv_objs());
				if (d < min_dist)
				{
					min_dist = d;
					min_rp = r;
				}
			}

			// if (t != lastFrontRank) // associating members in St/Fl (only counting)
			// {
			// 	rps[min_rp].AddMember();
			// }
			// else
			rps[min_rp].AddPotentialMember(fronts->getSubfront(t)->get(i), min_dist);
			fronts->getSubfront(t)->get(i)->setPerpendicularDist(min_dist);
			double projDist = MathAux::ProjectedDistanceFromOrigin(rps[min_rp].pos(), fronts->getSubfront(t)->get(i)->conv_objs());
			fronts->getSubfront(t)->get(i)->setProjectedDist(projDist);
		} // for - members in front
	}	  // for - fronts
}
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------

bool matchWeightValues(const CReferencePoint &one, const CReferencePoint &two, size_t M)
{
	//bool match = true;
	for (size_t i = 0; i < M; i++)
	{
		if (one.pos()[i] != two.pos()[i]) //fabs(one->getObjective(i) - two->getObjective(i)) > 0.00001
		{
			return false;
		}
	}
	return true;
}

void GenerateReferencePointsRand(vector<CReferencePoint> *rps, size_t M, size_t N, double mean, double stdDev)
{
	for (int n = 0; n < N;)
	{
		CReferencePoint pt(M);
		double sum = 0;
		for (int obj = 0; obj < M; obj++)
		{
			pt.pos()[obj] = RandomTools::randGaussian(mean, stdDev * stdDev, *PseudoRandom::getRndFactory());
			sum += pt.pos()[obj];
		}
		for (int obj = 0; obj < M; obj++)
		{
			pt.pos()[obj] = pt.pos()[obj] / sum;
		}
		bool match = false;
		for (int i = 0; i < rps->size(); i++)
		{
			if (matchWeightValues(pt, (*rps)[i], M)) //(*vecPtr)
			{
				cout << "Duplicate weight vector found. Discarded. " << endl;
				match = true;
				break;
			}
			//for()
		}
		if (match)
		{
			continue;
		}
		else
		{
			n++;
			//  cout << n-1 <<"-th Weight:";
			//  for(int obj = 0; obj < M; obj++)
			//    {
			//        cout << pt.pos()[obj]<<", ";
			//    }
			//  cout<<endl;
			(*rps).push_back(pt);
		}

	} // for
}
