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
	int utilityFunction=1;
	while(it!=allGood.end())
	{
	  
	  if(utilityFunction> romanAgent.getQuantity(std::get<0>(*it))/romanAgent.getNeed(std::get<0>(*it)))
			utilityFunction=romanAgent.getQuantity(std::get<0>(*it))/romanAgent.getNeed(std::get<0>(*it));
	  it++;
	}
	
	double score=romanAgent.getScore();
	romanAgent.setScore(score+utilityFunction);


}

std::string ConsumptionAction::describe() const
{
	return "Consumption action";
}

} // namespace Epnet


