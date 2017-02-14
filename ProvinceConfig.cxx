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
	_controllerType = getParamStr( "controller", "type");
	_selectionProcess= getParamStr( "controller/culture", "transmission");	
	_innovationProcess= getParamStr( "controller/culture", "innovation");	
	_culturalStep= getParamInt( "controller/culture", "step");	

	_mutationRate= getParamFloat( "controller/culture", "mutation");

	_marketSize= getParamFloat( "market", "size");
	_goodsParam= getParamStr( "controller/good", "type");
	_networkType= getParamStr( "network", "type");
	
	int nParam = getParamInt("network","nparam");
	std::cout<<"Reseau "<<_networkType<<std::endl;
	
	for(int i=0;i<nParam;i++){
		std::ostringstream name;
		name << "network/prop" << i;
		std::string id=getParamStr(name.str(),"id");
		double value= getParamFloat(name.str(),"value");
		std::cout<<"avec "<<id<<"="<<value<<std::endl;
		_networkParam.insert(std::make_pair(id,value));
	}
		

	_numGoods = getParamInt( "goods", "num");
	
	if(_goodsParam== "random" ||_goodsParam== "randn" ){
		std::ostringstream name;
		name << "goods/good";
		std::string id = getParamStr(name.str(),"id");
		double initQuantity = getParamFloat(name.str(),"initialQuantity");
		double maxQuantity = getParamFloat(name.str(),"maxQuantity");
		double price = getParamFloat(name.str(),"price");
		double need = getParamFloat(name.str(),"need");
		double productionRate = getParamFloat(name.str(),"productionRate");
		_protoGood = std::make_tuple(id,initQuantity,maxQuantity,price,need,productionRate);  
	  
	}
	  else{
	for(int i=0; i < _numGoods ; i++)
	{
		std::ostringstream name;
		name << "goods/good" << i;
		std::string id = getParamStr(name.str(),"id");
		double initQuantity = getParamFloat(name.str(),"initialQuantity");
		double maxQuantity = getParamFloat(name.str(),"maxQuantity");
		double price = getParamFloat(name.str(),"price");
		double need = getParamFloat(name.str(),"need");
		double productionRate = getParamFloat(name.str(),"productionRate");
		_paramGoods.push_back(std::make_tuple(id,initQuantity,maxQuantity,price,need,productionRate));
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
}
	
} // namespace Epnet

