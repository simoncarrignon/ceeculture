
#include <EatAction.hxx>

#include <World.hxx>
#include <Roman.hxx>

namespace Roman
{

EatAction::EatAction()
{
}

EatAction::~EatAction()
{
}

void EatAction::execute( Engine::Agent & agent )
{
	Roman & simpleAgent = (Roman&)agent;
	Engine::World * world = agent.getWorld();
	simpleAgent.setResources(simpleAgent.getResources()+world->getValue("resources", agent.getPosition()));
	world->setValue("resources", agent.getPosition(), 0);
	simpleAgent.setResources(simpleAgent.getResources()-1);
}

std::string EatAction::describe() const
{
	return "Eat action";
}

} // namespace Roman


