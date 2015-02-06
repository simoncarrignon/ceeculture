#ifndef __MacmillanPlanner_hxx__
#define __MacmillanPlanner_hxx__

#include <vector>

namespace Epnet
{

	class MacmillanPlanner 
	{
		private:
			int nbGoods;
			int nbEssentialGood;
			std::vector<int> needEssential;
			std::vector<double> utility;
			std::vector<double> subjectivePrice;

			std::vector<double> computeGamma();
			int computeR(std::vector<int> sortedId);

		public:
			// todo remove environment from here
			MacmillanPlanner(void);
			virtual ~MacmillanPlanner();

			std::vector<int> consumptionPlan(void);
	};

} // namespace Epnet

#endif // __MacmillanPlanner_hxx__

