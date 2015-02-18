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
	
	const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
	for (auto it = provinceConfig._paramRasters.begin(); it != provinceConfig._paramRasters.end() ; it++)
	{
		registerDynamicRaster(std::get<0>(*it), true);
		getDynamicRaster(std::get<0>(*it)).setInitValues(std::get<1>(*it), std::get<2>(*it), std::get<3>(*it));
		for(auto index:getBoundaries())
		{
			int value = Engine::GeneralState::statistics().getUniformDistValue(std::get<1>(*it), std::get<2>(*it));
			setMaxValue(std::get<0>(*it), index, value);
		}
		updateRasterToMaxValues(std::get<0>(*it));
	}
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
			//currency is not interesting in itself. that may be changed
			//currency has no price by itself

			for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
			{
				//id, maxQuantity, price and need of the good
				agent->addGoodType(std::get<0>(*it),std::get<2>(*it),std::get<3>(*it),std::get<4>(*it));
				//add init quantity to new good
				agent->addGood(std::get<0>(*it),std::get<1>(*it));
			}

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

