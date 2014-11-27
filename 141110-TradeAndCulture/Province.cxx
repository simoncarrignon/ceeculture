
#include <Province.hxx>

#include <ProvinceConfig.hxx>
#include <Roman.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <Logger.hxx>

namespace Epnet
{

Province::Province(Engine::Config * config, Engine::Scheduler * scheduler ) : World(config, scheduler, false)
{
}

Province::~Province()
{
}

void Province::createRasters()
{
	//registerStaticRaster("resources", false);
	//
	registerDynamicRaster("ess-a", true);
	getDynamicRaster("ess-a").setInitValues(0, 5, 5);
	for(auto index:getBoundaries())
	{
		int value = Engine::GeneralState::statistics().getUniformDistValue(0,5);
        setMaxValue("ess-a", index, value);
	}
	updateRasterToMaxValues("ess-a");

	registerDynamicRaster("ess-b", true);
	getDynamicRaster("ess-b").setInitValues(0, 5, 5);
	for(auto index:getBoundaries())
	{
		int value = Engine::GeneralState::statistics().getUniformDistValue(0,5);
        setMaxValue("ess-b", index, value);
	}
	updateRasterToMaxValues("ess-b");
}

void Province::createAgents()
{
	std::stringstream logName;
	logName << "agents_" << getId();

	const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
	for(int i=0; i<provinceConfig._numAgents; i++)
	{
		if((i%getNumTasks())==getId())
		{
			std::ostringstream oss;
			oss << "Roman_" << i;
			Roman * agent = new Roman(oss.str());
			addAgent(agent);
			agent->setRandomPosition();
			agent->addGoodType("ess-a",100);
			agent->addGood("ess-a",50);
			agent->addGoodType("ess-b",100);
			agent->addGood("ess-b",50);
			agent->addGoodType("noness-a",100);
			agent->addGoodType("noness-b",100);
			log_INFO(logName.str(), getWallTime() << " new agent: " << agent);
		}
	}
}

void Province::proposeConnection(Roman* source, Roman* target)
{
	source->proposeConnectionTo(target);
}

void Province::buildConnection(Roman* source, Roman* target)
{
	source->proposeConnectionTo(target);
	target->acceptConnectionFrom(source);
}

void Province::killConnection(Roman* source, Roman* target)
{
	source->killConnectionTo(target);
}

void Province::proposeTwoWayConnection(Roman* source, Roman* target)
{
	source->proposeConnectionTo(target);
	target->proposeConnectionTo(source);
}

void Province::buildTwoWayConnection(Roman* source, Roman* target)
{
	buildConnection(source,target);
	buildConnection(target,source);
}

void Province::killTwoWayConnection(Roman* source, Roman* target)
{
	source->killConnectionTo(target);
	target->killConnectionTo(source);
}

} // namespace Roman

