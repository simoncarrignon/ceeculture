#include <ProvinceConfig.hxx>

#include <iostream>

namespace Epnet
{

ProvinceConfig::ProvinceConfig( const std::string & xmlFile ) : Config(xmlFile), _numAgents(0)
{
}

ProvinceConfig::ProvinceConfig( const Engine::Size <int> & size, const int & numSteps ) : Engine::Config(size,numSteps), _numAgents(0)
{
}

ProvinceConfig::~ProvinceConfig()
{
}

void ProvinceConfig::loadParams()
{
	_numAgents = getParamInt( "numAgents", "value");

	_numGoods = getParamInt( "goods", "num");
	for(int i=0; i < _numGoods ; i++)
	{
		std::ostringstream name;
		name << "goods/good" << i;
		std::string id = getParamStr(name.str(),"id");
		double initQuantity = getParamFloat(name.str(),"initialQuantity");
		double maxQuantity = getParamFloat(name.str(),"maxQuantity");
		double price = getParamFloat(name.str(),"price");
		double need = getParamFloat(name.str(),"need");
		_paramGoods.push_back(std::make_tuple(id,initQuantity,maxQuantity,price,need));
	}

	_numRasters = getParamInt( "rasters", "num");
	for(int i=0; i < _numRasters ; i++)
	{
		std::ostringstream name;
		name << "rasters/raster" << i;
		std::string id = getParamStr(name.str(),"id");
		double min = getParamFloat(name.str(),"min");
		double max = getParamFloat(name.str(),"max");
		double init = getParamFloat(name.str(),"init");
		_paramRasters.push_back(std::make_tuple(id,min,max,init));
	}
}
	
} // namespace Epnet

