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
	getDynamicRaster("ess-a").setInitValues(0, 20, 20);
	for(auto index:getBoundaries())
	{
		int value = Engine::GeneralState::statistics().getUniformDistValue(0,20);
        setMaxValue("ess-a", index, value);
	}
	updateRasterToMaxValues("ess-a");

	registerDynamicRaster("ess-b", true);
	getDynamicRaster("ess-b").setInitValues(0, 20, 20);
	for(auto index:getBoundaries())
	{
		int value = Engine::GeneralState::statistics().getUniformDistValue(0,20);
        setMaxValue("ess-b", index, value);
	}
	updateRasterToMaxValues("ess-b");

	registerDynamicRaster("nonEss-a", true);
	getDynamicRaster("nonEss-a").setInitValues(0, 20, 20);
	for(auto index:getBoundaries())
	{
		int value = Engine::GeneralState::statistics().getUniformDistValue(0,20);
        setMaxValue("nonEss-a", index, value);
	}
	updateRasterToMaxValues("nonEss-a");

	registerDynamicRaster("nonEss-b", true);
	getDynamicRaster("nonEss-b").setInitValues(0, 20, 20);
	for(auto index:getBoundaries())
	{
		int value = Engine::GeneralState::statistics().getUniformDistValue(0,20);
        setMaxValue("nonEss-b", index, value);
	}
	updateRasterToMaxValues("nonEss-b");
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
			agent->addGoodType("currency",1000);
			agent->addGood("currency",500);

			agent->addGoodType("ess-a",100);
			agent->addGood("ess-a",50);

			agent->addGoodType("ess-b",100);
			agent->addGood("ess-b",50);

			agent->addGoodType("nonEss-a",100);

			agent->addGoodType("nonEss-b",100);

			log_INFO(logName.str(), getWallTime() << " new agent: " << agent);
		}
	}
}

void Province::proposeConnection(std::string source, std::string target)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}
	sourcePtr->proposeConnectionTo(target);
}

void Province::buildConnection(std::string source, std::string target)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	Roman* targetPtr = dynamic_cast<Roman*> (getAgent(target));
	if (targetPtr == NULL)
	{
		std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	sourcePtr->proposeConnectionTo(target);
	targetPtr->acceptConnectionFrom(source);
}

void Province::killConnection(std::string source, std::string target)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	sourcePtr->killConnectionTo(target);
}

void Province::proposeTwoWayConnection(std::string source, std::string target)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	Roman* targetPtr = dynamic_cast<Roman*> (getAgent(target));
	if (targetPtr == NULL)
	{
		std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	sourcePtr->proposeConnectionTo(target);
	targetPtr->proposeConnectionTo(source);
}

void Province::buildTwoWayConnection(std::string source, std::string target)
{
	buildConnection(source,target);
	buildConnection(target,source);
}

void Province::killTwoWayConnection(std::string source, std::string target)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	Roman* targetPtr = dynamic_cast<Roman*> (getAgent(target));
	if (targetPtr == NULL)
	{
		std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	sourcePtr->killConnectionTo(target);
	targetPtr->killConnectionTo(source);
}

} // namespace Roman

