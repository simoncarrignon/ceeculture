
#include <ProductionAction.hxx>

#include <World.hxx>
#include <Province.hxx>
#include <math.h>

#include <Logger.hxx>

namespace Epnet
{

	ProductionAction::ProductionAction()
	{
	}

	ProductionAction::~ProductionAction()
	{
	}

	void ProductionAction::execute(Engine::Agent& agent)
	{
		Roman & romanAgent = (Roman&)agent;
		Engine::World * world = agent.getWorld();
		Province & provinceWorld = (Province&) *world;
		std::string pgood = std::get<0>(romanAgent.getProducedGood());
		double ratio =  provinceWorld.getRatio(pgood);


		std::vector< std::tuple< std::string, double, double, double, double, double > > allGood= (romanAgent.getListGoods());

		for( auto it = allGood.begin(); it != allGood.end();it++)
		{
			std::string good = std::get<0>(*it);
			// 	 double producedQuantity = (double)(romanAgent.getProductionRate(good)*provinceWorld.getNumberOfAgents()*romanAgent.getNeed(good));
			//double producedQuantity = (double)(romanAgent.getProductionRate(good)*romanAgent.getNeed(good));
			//double producedQuantityold = (double)(romanAgent.getProductionRate(good)*romanAgent.getNeed(good)*allGood.size());
			double producedQuantity;
			if(provinceWorld.isPopSize())producedQuantity = (double)(romanAgent.getProductionRate(good)*romanAgent.getNeed(good)*provinceWorld.getTotPopSize());
			else  producedQuantity = (double)(romanAgent.getProductionRate(good)*romanAgent.getNeed(good)*(ratio)*allGood.size());
			if(good=="coins" && romanAgent.getProductionRate(good) > 0  ) producedQuantity = allGood.size();
			//std::cout<<producedQuantity<<std::endl;
			romanAgent.setQuantity(good,producedQuantity);

		}


	}

	std::string ProductionAction::describe() const
	{
		return "Production action";
	}

} // namespace Epnet


