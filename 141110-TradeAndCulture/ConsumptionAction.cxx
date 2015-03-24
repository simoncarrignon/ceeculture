#include <ConsumptionAction.hxx>

#include <World.hxx>
#include <Province.hxx>

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
	
	
	std::vector< std::tuple< std::string, double, double, double, double, double > > allGood= romanAgent.getListGoods();
	std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator it = allGood.begin();
	double utilityFunction=double(RAND_MAX);
	
//	it++;//skip money ou plus
	while(it!=allGood.end())
	{
	    std::string good=std::get<0>(*it);
	 
	  if(good == std::get<0>(romanAgent.getProducedGood()))
	    romanAgent.setQuantity(good,romanAgent.getNeed(good)); 
	  
	  if(utilityFunction> romanAgent.getQuantity(good)/romanAgent.getNeed(good))
			utilityFunction=romanAgent.getQuantity(good)/romanAgent.getNeed(good);
	  if(good == std::get<0>(romanAgent.getProducedGood()))
	    romanAgent.setQuantity(good,1); 
	  else
	    romanAgent.setQuantity(good,0.0);
	  it++;
	}
	
	double score=romanAgent.getScore()+utilityFunction;
	romanAgent.setScore(score);

	if(score >= provinceWorld.getMaxScore(std::get<0>(romanAgent.getProducedGood())))provinceWorld.setMaxScore(std::get<0>(romanAgent.getProducedGood()),score);
	if(score <= provinceWorld.getMinScore(std::get<0>(romanAgent.getProducedGood())))provinceWorld.setMinScore(std::get<0>(romanAgent.getProducedGood()),score);


}

std::string ConsumptionAction::describe() const
{
	return "Consumption action";
}

} // namespace Epnet


