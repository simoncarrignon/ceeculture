#include <MacmillanPlanner.hxx>
#include <iostream>
#include <algorithm>

namespace Epnet
{

	struct cmpStruct{
		double subjectivePrice;
		double utility;
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
		nbGoods = 5;
		nbEssentialGood = 1;

		for(int i = 0 ; i < nbEssentialGood ; i++)
		{
			needEssential.push_back(1);
		}
		std::cout << needEssential.size() << std::endl;

		/*
		for(int i = 0 ; i <= nbGoods ; i++)
		{
			utility.push_back(1);
		}
		*/
		utility.push_back(0.3);
		utility.push_back(0.2);
		utility.push_back(0.4);
		utility.push_back(0.05);
		utility.push_back(0.05);

		/*
		for(int i = 0 ; i <= nbGoods ; i++)
		{
			subjectivePrice.push_back(0.0);
		}
		*/
		subjectivePrice.push_back(3.9);
		subjectivePrice.push_back(3.0);
		subjectivePrice.push_back(5.0);
		subjectivePrice.push_back(5.0);
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

		for(int i = 0 ; i < nbEssentialGood ; i++ )
		{
			goodWanted[i] = ( utility[i] / (subjectivePrice[i] * gamma[i])) + needEssential[i] ;
		}

		return goodWanted;
	}


	std::vector<double> MacmillanPlanner::computeGamma()
	{
		//rank goods
		std::vector<cmpStruct> rankedGoods;
		for (int i = nbEssentialGood ; i < nbGoods ; i++)
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
			std::cout << (*it).id << " ";
		}
		std::cout << std::endl;
		int r = computeR(sortedId);
		std::cout << r << std::endl;
		exit(1);

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
		for(int i = 0 ; i < nbEssentialGood ; i++)
		{
			threshold += (subjectivePrice[i] * needEssential[i]);
		}
		std::cout << "threshold " << threshold << std::endl;

		for(unsigned int i = 0 ; i < orderedId.size() ; i ++)
		{
			std::cout << i << " " << orderedId[i] << std::endl;
			double sumUntilI = 0.0;
			for (unsigned int j = 0 ; j <= i ; j++)
			{
				sumUntilI += utility[orderedId[j]];
			}
			std::cout << "sum until " << sumUntilI << std::endl;
			double sumFromI = 0.0;
			for (unsigned int j = (i+1) ; j < orderedId.size() ; j++)
			{
				sumFromI += subjectivePrice[orderedId[j]];
			}
			std::cout << "sum from " << sumFromI << std::endl;
			double value = ((subjectivePrice[orderedId[i]] / utility[orderedId[i]]) * (1.0 - sumUntilI) ) - sumFromI;
			std::cout << "value " << value << std::endl ;
			if (value < threshold)
			{
				r = orderedId[i];
				break;
			}
			std::cout << "#####" << std::endl;
		}

		return r;
	}

} // namespace Roman

