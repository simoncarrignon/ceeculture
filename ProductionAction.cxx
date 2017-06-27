
#include <ProductionAction.hxx>

#include <World.hxx>
#include <Province.hxx>

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

		std::vector< std::tuple< std::string, double, double, double, double, double > > allGood= (romanAgent.getListGoods());

		for( auto it = allGood.begin(); it != allGood.end();it++)
		{
			std::string good = std::get<0>(*it);
			// 	 double producedQuantity = (double)(romanAgent.getProductionRate(good)*provinceWorld.getNumberOfAgents()*romanAgent.getNeed(good));
			//double producedQuantity = (double)(romanAgent.getProductionRate(good)*romanAgent.getNeed(good));
			double producedQuantity = (double)(romanAgent.getProductionRate(good)*romanAgent.getNeed(good)*allGood.size());
			//double producedQuantity = (double)(romanAgent.getProductionRate(good));

			romanAgent.setQuantity(good,producedQuantity);

		}


	}

	std::string ProductionAction::describe() const
	{
		return "Production action";
	}

} // namespace Epnet


