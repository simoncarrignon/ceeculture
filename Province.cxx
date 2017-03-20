#include <Province.hxx>
#include <ProvinceConfig.hxx>
#include <ProductionAction.hxx>
#include <TradeAction.hxx>
#include <ConsumptionAction.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <Logger.hxx>
#include <Scheduler.hxx>

#include <map>

namespace Epnet
{
	

	Province::Province(Engine::Config * config, Engine::Scheduler * scheduler ) : World(config, scheduler, false)
	{
		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		
		double all_needs=0.0;
		double bneed=(double)(Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0; //if you want relative price (something lik p1=2*x, p2=p1*2, p3=p2*2...., and not totally random) inialize a "base need" here. 
		

		//initialize knowledge about the n goods in our economy: what are the different types, what is their absolute value
		//Maybe that should go in the constructor
		if(provinceConfig._goodsParam == "random" ||provinceConfig._goodsParam == "randn" ){
		    for (int i = 0; i < provinceConfig._numGoods ; i++)
		    {
			//double tneed=(double)(Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0; for totally random absolute value, initialize here
			double tneed=bneed * (i+1.0);

			all_needs += tneed; //tneeds is the sum of all value. It could be use to normalize the privec
			std::ostringstream sgoodType;
			sgoodType << "g"<< i;				
			std::string goodType = sgoodType.str();
			_needs.push_back(std::make_tuple(goodType,tneed));  
			_maxscore.push_back(std::make_tuple(goodType,0.0));
			_minscore.push_back(std::make_tuple(goodType,0.0));
			_typesOfGood.push_back(goodType);
			_good2Producers.insert(std::pair<std::string,std::vector<std::string>>(goodType,{}));
			//				_good2CulturalNetwork.insert(std::pair<std::string,Network>(goodType,Network()));
		    }	  
		    //used to normalised the needs such as sum(needs(i))=1
		    if(provinceConfig._goodsParam == "randn"){
			for (auto it = _needs.begin(); it != _needs.end() ; it++){
			    double p = std::get<1>(*it);
			    *it=std::make_tuple(std::get<0>(*it),p/all_needs);
			}
		    }
		}
		else if(provinceConfig._goodsParam == "gintis07"){
			for (int i = 0; i < provinceConfig._numGoods ; i++)
			{

			    //In Gintis 2007 there is no need for a "needs" (utility are then computed only based on personnal value <= false
			    //We set up -1 to help debugging

			    std::ostringstream sgoodType;
			    sgoodType << "g"<< i;				
			    std::string goodType = sgoodType.str();
			    _needs.push_back(std::make_tuple(goodType,-1));  
			    _maxscore.push_back(std::make_tuple(goodType,0.0));
			    _minscore.push_back(std::make_tuple(goodType,0.0));
			    _typesOfGood.push_back(goodType);
			    _good2Producers.insert(std::pair<std::string,std::vector<std::string>>(goodType,{}));
			    //_good2CulturalNetwork.insert(std::pair<std::string,Network>(goodType,Network()));
			}	  

		}
		else {
			//In that case each good and the properties of thoses good have to be manually set by the user in the config file
			//I have never used that possibility..
			for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
			{
			    double tneed=(double)(Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0;

			    all_needs += tneed;

			    _needs.push_back(std::make_tuple(std::get<0>(*it),tneed));  
			    _maxscore.push_back(std::make_tuple(std::get<0>(*it),0.0));  
			    _minscore.push_back(std::make_tuple(std::get<0>(*it),0.0));
			    _typesOfGood.push_back(std::get<0>(*it));
			    _good2Producers.insert(std::pair<std::string,std::vector<std::string>>(std::get<0>(*it),{}));

			}

		}


		
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
				Roman * agent = new Roman(oss.str(),provinceConfig._controllerType,provinceConfig._mutationRate,provinceConfig._selectionProcess,provinceConfig._innovationProcess,provinceConfig._culturalStep);
				addAgent(agent);
				//position is actually not interesting
				agent->setRandomPosition();
				//currency is not interesting in itself. that may be changed
				//currency has no price by itself
				if(provinceConfig._goodsParam== "random" || provinceConfig._goodsParam== "randn")
				{

					std::tuple< std::string, double, double, double, double, double > protoGood = provinceConfig._protoGood;
					for (int g = 0; g < provinceConfig._numGoods ; g++)
					{

						std::ostringstream sgoodType;
						sgoodType << "g"<< g;				
						std::string goodType = sgoodType.str();
						//id, maxQuantity, price, need and production rate of the good
						agent->addGoodType(goodType,std::get<2>(protoGood),std::get<3>(protoGood),std::get<4>(protoGood),std::get<5>(protoGood));
						//add init quantity to new good
						agent->addGood(goodType,std::get<1>(protoGood));
						//the protoGood is used to calibrate all other goods. 

						//set a random properties for each goods
						if(agent->getPrice(goodType)<0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);// market clearing price : 1.0/(i+1)
						if(agent->getQuantity(goodType)<0)agent->setQuantity(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);
						if(agent->getProductionRate(goodType)<0)agent->setProductionRate(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);

						//---------------/*
						//set the need value for each good. Remember: in Basic Simulation the need is the same for everyone
						agent->setNeed(goodType,std::get<1>(_needs[g]));
					}			



					//Set producedGood as a modulo of the agent ID
					int randg = i%provinceConfig._numGoods;
					std::tuple< std::string, double, double, double, double, double > producedGood = agent->getListGoods()[randg];
					agent->setProductionRate(std::get<0>(producedGood),1.0);
					_good2Producers[std::get<0>(producedGood)].push_back(oss.str());

				}
				else if(provinceConfig._goodsParam== "gintis07" )
				{
				    	
				    agent->setType("gintis07");
					std::tuple< std::string, double, double, double, double, double > protoGood = provinceConfig._protoGood;
					for (int g = 0; g < provinceConfig._numGoods ; g++)
					{

						std::ostringstream sgoodType;
						sgoodType << "g"<< g;				
						std::string goodType = sgoodType.str();
						//id, maxQuantity, price, need and production rate of the good
						agent->addGoodType(goodType,std::get<2>(protoGood),std::get<3>(protoGood),std::get<4>(protoGood),std::get<5>(protoGood));
						//add init quantity to new good
						agent->addGood(goodType,std::get<1>(protoGood));
						//the protoGood is used to calibrate all other goods. 

						//set a random properties for each goods
						if(agent->getPrice(goodType)<0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);// market clearing price : 1.0/(i+1)
						if(agent->getQuantity(goodType)<0)agent->setQuantity(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);
						if(agent->getProductionRate(goodType)<0)agent->setProductionRate(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);

						//---------------/*
						//set the need value for each good. Remember: this is actualised every timestep in gintis07 (need should be -1 at initialisation).
						agent->setNeed(goodType,std::get<1>(_needs[g]));
					}			

					//in gintis 2007, the last good has unit price
					int last = provinceConfig._numGoods-1;
					std::ostringstream sgoodType;
					sgoodType << "g"<< last;				
					std::string goodType = sgoodType.str();
					agent->setPrice(goodType,1.0);
					//---------------/*


					agent->initSegments();
					agent->setDemand(true);
					agent->setUtility(true);

					//Set producedGood as a modulo of the agent ID
					int randg = i%provinceConfig._numGoods;
					std::tuple< std::string, double, double, double, double, double > producedGood = agent->getListGoods()[randg];
					agent->setProductionRate(std::get<0>(producedGood),1.0);
					_good2Producers[std::get<0>(producedGood)].push_back(oss.str());

				}
				else{
					for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
					{
						//id, maxQuantity, price, need and production rate of the good
						agent->addGoodType(std::get<0>(*it),std::get<2>(*it),std::get<3>(*it),std::get<4>(*it),std::get<5>(*it));

						//add init quantity to new good
						agent->addGood(std::get<0>(*it),std::get<1>(*it));

						//set a random price for each goods

						agent->setPrice(std::get<0>(*it),(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0); //TODO:demiurge's job


						//---------------
						//set the need value for each good. Remember: in Basic Simulation the need is the same for everyone
						std::vector<std::tuple<std::string,double> >::iterator need = std::find_if(_needs.begin(), _needs.end(), [=](const std::tuple<std::string,double>& good) {return std::get<0>(good) == std::get<0>(*it);});

						if ( need != _needs.end() )
						{				  
							agent->setNeed(std::get<0>(*it),std::get<1>(*need));
						}
					}			

				}
				log_INFO(logName.str(), getWallTime() << " new agent: " << agent << "\n" << agent->getSegmentsProp());


				//loop for initialize the commercial connection of the current agent with all previously created agents.
				for(int j=(i-1); j>=0; j--)
				{
					std::ostringstream ossb;
					ossb << "Roman_" << j;
					this->buildTwoWayConnection(oss.str(),ossb.str());//TODO here check the this->network
					
				}
			}
		}
		
		for (std::map< std::string, std::vector< std::string > >::iterator it = _good2Producers.begin(); it != _good2Producers.end();it++) {
				
			std::vector<std::string> groupOfproducer = it->second;
			std::string type = provinceConfig._networkType; 
			
			
			Network n = Network(groupOfproducer,type,it->first,provinceConfig._networkParam);
			//Network n = Network(groupOfproducer,type,it->first,true);
			n.write();
			_good2CulturalNetwork.insert(std::pair<std::string,Network>(it->first,n));

			for (std::vector<std::string>::iterator producer = groupOfproducer.begin(); producer != groupOfproducer.end(); producer++){
				
				Roman* romanProducer = dynamic_cast<Roman*> (getAgent(*producer));
				if (romanProducer == NULL)
				{
					std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
					exit(1);
					
				}
				std::vector< std::string > u= n.getNeighboursOf(*producer);
// 				std::cout<<*producer<< " as "<<u.size()<< " getNeighboursOf"<<std::endl;
				romanProducer->setListOfCulturalNeighbours( n.getNeighboursOf(*producer));
				
			}
		}
		createCulturalNetwork();
		
		//printAllCulturalNerwork();
/*
		for( std::vector<std::string>::iterator it= _typesOfGood.begin() ; it!=_typesOfGood.end(); it++){
		  std::cout<<"good "<<*it<<std::endl;
		  printListOfProd(*it);
		  
		}*/
	   
	  
	}

	
	//Create social network for all productors groups and set it for all agents
	void Province::createCulturalNetwork(){
		
		
		
		
	}
  	 
 	 	 
	double Province::getMaxScore(std::string good)
	{
		std::vector< std::tuple< std::string, double > >::iterator it = _maxscore.begin();

		while(it!= _maxscore.end()){
			if(std::get<0>(*it)== good)
				return std::get<1>(*it);
			it++;

		}
		return -1; //error good not found
	}


	void Province::setMaxScore(std::string good, double score)
	{
		std::vector< std::tuple< std::string, double > >::iterator it = _maxscore.begin();

		while(it!= _maxscore.end()){
			if(std::get<0>(*it)== good){
				std::get<1>(*it) = score ;
				return;
			}
			it++;
		}

	}

	double Province::getMinScore(std::string good)
	{
		std::vector< std::tuple< std::string, double > >::iterator it = _minscore.begin();

		while(it!= _minscore.end()){
			if(std::get<0>(*it)== good)
				return std::get<1>(*it);
			it++;

		}
		return -1; //error good not found
	}


	void Province::setMinScore(std::string good, double score)
	{
		std::vector< std::tuple< std::string, double > >::iterator it = _minscore.begin();

		while(it!= _minscore.end()){
			if(std::get<0>(*it)== good){
				std::get<1>(*it) = score ;
				return;
			}
			it++;
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

	
	void Province::printListOfProd(std::string s){
		
		std::vector<std::string> allprod= _good2Producers[s];
		
		std::cout<<allprod.size()<<std::endl;
		std::cout<<"The producers of the good g"<<s<<" are :"<<std::endl;
		
		for (std::vector<std::string>::iterator it = allprod.begin(); it != allprod.end() ; it++)
			std::cout<<" "<<*it;
		std::cout<<std::endl;
		
	}

	//return the list of all agent producing the good "s"
	std::vector<std::string> Province::getListOfProd(std::string s){

		std::vector<std::string> allprod= _good2Producers[s];
		return(allprod);

	}
	float Province::getMarketSize(){

		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		return provinceConfig._marketSize;
	}

	void Province::printAllCulturalNerwork(){
		
		for(std::map< std::string, Network >::iterator it= _good2CulturalNetwork.begin();it!=_good2CulturalNetwork.end();it++)
		{
			std::cout<<"The network of producers of"<<it->first<<" is :"<<it->second<<std::endl;
			
		}
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


