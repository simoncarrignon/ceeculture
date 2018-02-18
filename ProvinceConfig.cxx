#include <ProvinceConfig.hxx>
#include <Exception.hxx>

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
	_aType = getParamStr( "controller/agent", "type");	
	_tradeUtilFunction= getParamStr( "controller/trade", "util");
	_tradeVolSelFunction= getParamStr( "controller/trade", "volsel");
	_culturalStep= getParamInt( "controller/culture", "step");	
	_muMax= getParamFloat( "controller/culture", "mumax");	


	_mutationRate= getParamFloat( "controller/culture", "mutation");
	_copyRate= getParamFloat( "controller/culture", "copy");

	_marketSize= getParamFloat( "market", "size");
	_goodsParam= getParamStr( "controller/good", "type");
	_networkType= getParamStr( "network", "type");

	//events setup
	try{
	    _distrib = getParamStr( "popdistrib", "type");
	    _distribUse = getParamStr( "popdistrib", "impact");
	    std::cout<<"fdsdfsd"<<_distrib<<std::endl;

	    try{
		if(_distrib == "pl"){
		    _plMax= getParamFloat( "popdistrib", "max");//param for opo size distribution
		    _plMin= getParamFloat( "popdistrib", "min");
		    _plAlpha= getParamFloat( "popdistrib", "alpha");
		}
	    }
	    catch(const Engine::Exception & e){
		std::cerr<<e.what()<<std::endl;
		std::cerr<<"when defining a power lave you have to give: max,min and alpha "<<std::endl;
		exit(0);
	    }
	}
	catch(const Engine::Exception & e){
	    _distrib = "random";
	}

	try{
	    _events = getParamStr( "events", "type");
	    if(_events == "rate")
		_eventsRate =getParamInt( "events", "rate");
		_eventsStop =getParamInt( "events", "stop");
	}
	catch(const Engine::Exception & e){
	    //Create a load_config log file?
	    _events = "no";
	}


	//loggfiels
	try{
	    //what should be stored here is a map for all differen kind of log and if they are true or not
	    _logTrade = getParamStr( "logs/trade", "value");
	}
	catch(const Engine::Exception & e){
	    //Create a load_config log file?
	    _logTrade = "false";
	}

	
	////networks
	//read if network will be manually set up or nottj
	try{
	    _networkOut = getParamStr( "network", "output");
	}
	catch(const Engine::Exception & e){
	    //Create a load_config log file?
	    std::cerr<<e.what()<<std::endl;
	    std::cerr<<"Network wont be recorded"<<std::endl;
	    _networkOut = "false";
	}
	
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
	try{
	    _typeProd = getParamStr( "goods", "prod");
	}
	catch( std::exception & exceptionThrown )
	{
	    _typeProd="balanced";
	}
	
	if(_goodsParam== "random" ||_goodsParam== "randn"||_goodsParam== "gintis07" ){
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

		if(_typeProd == "unbalanced"){
		    try{
			int nbprod = getParamInt(name.str(),"nprod"); //number of producers
			_nbProds.push_back(std::make_tuple(id,nbprod)); //and with store it in a map with key="good names" =?> nprod, ie: "good0"=>3 means that we will have 3 producers for good 0
		    }
		    catch( std::exception & exceptionThrown){
			std::cout<<"you configure an unbalanced market but number of productors aren't defined"<<std::endl;
			exit(0);
		    }
		}
		_paramGoods.push_back(std::make_tuple(id,initQuantity,maxQuantity,price,need,productionRate));

	    }

	}
	try{
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
	catch( std::exception & exceptionThrown )
	{
	    std::cout << "exception thrown: " << exceptionThrown.what() << std::endl;
	    std::cout << "Raster are not used" << std::endl;
	    _numRasters = 0;
	}
}
	
} // namespace Epnet

