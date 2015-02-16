#include <MacmillanPlanner.hxx>
#include <iostream>
#include <algorithm>

namespace Epnet
{

	struct cmpStruct{
		double subjectivePrice;
		int utility;
		int id;
	};

	bool compareRank(struct cmpStruct i, struct cmpStruct j)
	{
		if ((i.subjectivePrice / i.utility) > (j.subjectivePrice / j.utility))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	MacmillanPlanner::MacmillanPlanner(void)
	{
		nbGoods = 3;
		lastIdEssentialGood = 1;

		for(int i = 0 ; i <= lastIdEssentialGood ; i++)
		{
			needEssential.push_back(1);
		}

		/*
		for(int i = 0 ; i <= nbGoods ; i++)
		{
			utility.push_back(1);
		}
		*/
		utility.push_back(0.3);
		utility.push_back(0.2);
		utility.push_back(0.5);

		/*
		for(int i = 0 ; i <= nbGoods ; i++)
		{
			subjectivePrice.push_back(0.0);
		}
		*/
		subjectivePrice.push_back(5.0);
		subjectivePrice.push_back(3.0);
		subjectivePrice.push_back(5.0);
	}

	MacmillanPlanner::~MacmillanPlanner()
	{
	}

	std::vector<int> MacmillanPlanner::consumptionPlan(void)
	{
		std::vector<int> goodWanted;

		for(int i = 0 ; i < nbGoods ; i++)
		{
			goodWanted.push_back(0);
		}

		std::vector<double> gamma = computeGamma();

		for(int i = 0 ; i <= lastIdEssentialGood ; i++ )
		{
			goodWanted[i] = ( utility[i] / (subjectivePrice[i] * gamma[i])) + needEssential[i] ;
		}

		return goodWanted;
	}


	std::vector<double> MacmillanPlanner::computeGamma()
	{
		//rank goods
		std::vector<cmpStruct> rankedGoods;
		for (int i = lastIdEssentialGood ; i < nbGoods ; i++)
		{
			struct cmpStruct tmp;
			tmp.subjectivePrice = subjectivePrice[i];
			tmp.utility = utility[i];
			tmp.id = i;
			rankedGoods.push_back(tmp);
		}

		std::sort(rankedGoods.begin(),rankedGoods.end(),compareRank);
		std::vector<int> sortedId;
		for(std::vector<cmpStruct>::iterator it = rankedGoods.begin() ; it != rankedGoods.end() ; it++)
		{
			sortedId.push_back((*it).id);
		}
		int r = computeR(sortedId);

		std::vector<double> gamma;
		for(int i = 0 ; i < nbGoods ; i++)
		{
			gamma.push_back(0.0);
		}
		return gamma;
	}

	int MacmillanPlanner::computeR(std::vector<int> orderedId)
	{
		int r = 0;
		double threshold = 0.0;
		for(int i = 0 ; i <= lastIdEssentialGood ; i++)
		{
			threshold += (subjectivePrice[i] * needEssential[i]);
		}

		for(int i = 0 ; i < orderedId.size() ; i ++)
		{
			double sumUntilI = 0.0;
			for (int j = 0 ; j < i ; j++)
			{
				sumUntilI += utility[j];
			}
			double sumFromI = 0.0;
			for (int j = i ; j < orderedId.size() ; j++)
			{
				sumFromI += subjectivePrice[j];
			}
			double value = ((subjectivePrice[i] / utility[i]) * (1 - sumUntilI) ) - sumFromI;
			if (value < threshold)
			{
				r = i;
				break;
			}
		}

		return r;
	}

} // namespace Roman

