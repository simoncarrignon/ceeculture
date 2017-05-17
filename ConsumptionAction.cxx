#include <ConsumptionAction.hxx>

#include <World.hxx>
#include <Province.hxx>
#include <cmath>

#include <Logger.hxx>

namespace Epnet
{

	ConsumptionAction::ConsumptionAction()
	{
	}

	ConsumptionAction::~ConsumptionAction()
	{
	}

	void ConsumptionAction::execute(Engine::Agent& agent)
	{
		Roman & romanAgent = (Roman&)agent;
		Engine::World * world = agent.getWorld();
		Province & provinceWorld = (Province&) *world;

		double consumptionScore=0.0;

		if(romanAgent.getType() == "gintis07")
		    //in this experiment the idea is different: the utility function IS NOT the consumption function. The utility function is used to know the amount of good wanted.
		    romanAgent.setDemand(false);
		    //consumptionScore = romanAgent.consume(); //in that case this is then meaningless? or not?

		std::vector< std::tuple< std::string, double, double, double, double, double > > allGood= romanAgent.getListGoods();
		std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator it = allGood.begin();

		//std::cout<<"Consumption (fitness) value of agent" << romanAgent << std::endl;
		while(it!=allGood.end())
		{
		    std::string good=std::get<0>(*it);
		    if(romanAgent.getType() == "gintis07"){
			//if(good == std::get<0>(romanAgent.getProducedGood())){
			//    double curNeedForMyGood = romanAgent.getNeed(good);
			//    if(curNeedForMyGood > 10000){ 
			//	std::cout<<"plot whats appening here"<< std::get<0>(romanAgent.getProducedGood())<<std::endl;
			//    }
			//    romanAgent.setQuantity(good,std::min(curNeedForMyGood,1.0)); //this should use the optimal value for its the production's good
			//}

			double val=romanAgent.getQuantity(good);
			if(romanAgent.getQuantity(good) > romanAgent.getNeed(good))val=romanAgent.getNeed(good); //if we have collected more than what we need, we consume only what we need
			else val=romanAgent.getQuantity(good);
			consumptionScore+=val*romanAgent.getPrice(good);
			romanAgent.setUtility(false);
		//	std::cout<<consumptionScore<<std::endl;
		    }

		    else{
			if(good == std::get<0>(romanAgent.getProducedGood())) 
			    romanAgent.setQuantity(good,romanAgent.getNeed(good)); //use the optimal value for its the production's good
			//  			//	romanAgent.setQuantity(good,romanAgent.getPrice(good)); //use the estimated value for its the production's good
			//	
			////fit= |a-b|/euclideDist(a,b) my favorite one
			if(romanAgent.getQuantity(good)==(romanAgent.getNeed(good)))consumptionScore+=0; //undefined fitness function for 0
			else consumptionScore+=std::abs((romanAgent.getQuantity(good))-(romanAgent.getNeed(good)) )/(std::sqrt(std::abs((romanAgent.getQuantity(good))*(romanAgent.getQuantity(good))+(romanAgent.getNeed(good))*(romanAgent.getNeed(good)))));

			//fit= |a-b|/b : In that one I cut its too long right leg.
			// 	double cur=std::abs((romanAgent.getQuantity(good))-(romanAgent.getNeed(good)))/romanAgent.getNeed(good);
			// 	  if(cur>1)cur=1;
			// 	 consumptionScore+=cur;


			//////////////////////
			//high debugging (to check if work in perfect case)
			//romanAgent.setQuantity(good,romanAgent.getNeed(good)); //use the optimal value for its the production's good
			//consumptionScore+=1-std::abs((romanAgent.getQuantity(good))-(romanAgent.getNeed(good)) )/(std::sqrt(std::abs((romanAgent.getQuantity(good))*(romanAgent.getQuantity(good))+(romanAgent.getNeed(good))*(romanAgent.getNeed(good)))));
			//////////////////////
		    }

		    it++;
		}

		double score=romanAgent.getScore()+consumptionScore;
		romanAgent.setScore(score);
		

		//Update province min and max score to compute relative selections probabilites
		if(score >= provinceWorld.getMaxScore(std::get<0>(romanAgent.getProducedGood())))provinceWorld.setMaxScore(std::get<0>(romanAgent.getProducedGood()),score);
		if(score <= provinceWorld.getMinScore(std::get<0>(romanAgent.getProducedGood())))provinceWorld.setMinScore(std::get<0>(romanAgent.getProducedGood()),score);



	}

	std::string ConsumptionAction::describe() const
	{
		return "Consumption action";
	}

} // namespace Epnet


