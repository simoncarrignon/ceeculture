
#include <TradeWorld.hxx>

#include <TradeWorldConfig.hxx>
#include <Roman.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <Logger.hxx>

namespace Roman 
{

TradeWorld::TradeWorld(Engine::Config * config, Engine::Scheduler * scheduler ) : World(config, scheduler, false)
{
}

TradeWorld::~TradeWorld()
{
}

void TradeWorld::createRasters()
{
	registerDynamicRaster("resources", true);
	getDynamicRaster("resources").setInitValues(0, 5, 0);

	for(auto index:getBoundaries())
	{
		int value = Engine::GeneralState::statistics().getUniformDistValue(0,5);
        setMaxValue("resources", index, value);
	}
	updateRasterToMaxValues("resources");
}

void TradeWorld::createAgents()
{
    std::stringstream logName;
	logName << "agents_" << getId();

    const TradeWorldConfig & randomConfig = (const TradeWorldConfig&)getConfig();
	for(int i=0; i<randomConfig._numAgents; i++)
	{
		if((i%getNumTasks())==getId())
		{
			std::ostringstream oss;
			oss << "Roman_" << i;
			Roman * agent = new Roman(oss.str());
			addAgent(agent);
			agent->setRandomPosition();
	        log_INFO(logName.str(), getWallTime() << " new agent: " << agent);
		}
	}
}

} // namespace Roman

