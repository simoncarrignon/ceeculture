#include <Province.hxx>

#include <ProvinceConfig.hxx>
#include <Roman.hxx>
#include <ProductionAction.hxx>
#include <TradeAction.hxx>
#include <ConsumptionAction.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <Logger.hxx>
#include <Scheduler.hxx>

namespace Epnet
{

	Province::Province(Engine::Config * config, Engine::Scheduler * scheduler ) : World(config, scheduler, false)
	{
	  _maxscore=0.0;
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


		//initialize some usefull stuff to know about good : the differents type that exist and the their associated need value
		if(provinceConfig._goodsParam== "random"){
			for (int i = 0; i < provinceConfig._numGoods ; i++)
			{

				std::ostringstream sgoodType;
				sgoodType << "g"<< i;				
				std::string goodType = sgoodType.str();
				_needs.push_back(std::make_tuple(goodType,(double)(std::rand()%100)/100.0));  
				_typesOfGood.push_back(goodType);
			}	  
		}
		else
		{
			for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
			{
				_needs.push_back(std::make_tuple(std::get<0>(*it),(double)(std::rand()%100)/100.0));  
				_typesOfGood.push_back(std::get<0>(*it));
			}
		}	

		for(int i=0; i<provinceConfig._numAgents; i++)
		{

			if((i%getNumTasks())==getId())
			{
				std::ostringstream oss;
				oss << "Roman_" << i;
				Roman * agent = new Roman(oss.str(),provinceConfig._controllerType,provinceConfig._mutationRate,provinceConfig._selectionProcess,provinceConfig._innovationProcess);
				addAgent(agent);
				//position is actually not interesting
				agent->setRandomPosition();
				//currency is not interesting in itself. that may be changed
				//currency has no price by itself
				if(provinceConfig._goodsParam== "random")
				{

					std::tuple< std::string, double, double, double, double, double > protoGood = provinceConfig._protoGood;
					for (int i = 0; i < provinceConfig._numGoods ; i++)
					{

						std::ostringstream sgoodType;
						sgoodType << "g"<< i;				
						std::string goodType = sgoodType.str();
						//id, maxQuantity, price, need and production rate of the good
						agent->addGoodType(goodType,std::get<2>(protoGood),std::get<3>(protoGood),std::get<4>(protoGood),std::get<5>(protoGood));
						//add init quantity to new good
						agent->addGood(goodType,std::get<1>(protoGood));
						//the protoGood is used to calibrate all other goods. 

						//set a random properties for each goods
						if(agent->getPrice(goodType)<0)agent->setPrice(goodType,(double)(std::rand()%1000)/1000.0);
						if(agent->getQuantity(goodType)<0)agent->setQuantity(goodType,(double)(std::rand()%1000)/1000.0);
						if(agent->getProductionRate(goodType)<0)agent->setProductionRate(goodType,(double)(std::rand()%1000)/1000.0);
						//---------------/*
						//set the need value for each good. Remember: in Basic Simulation the need is the same for everyone
						agent->setNeed(goodType,std::get<1>(_needs[i]));
					}			



					//Set producedGood
					int randg = i%provinceConfig._numGoods;
					std::tuple< std::string, double, double, double, double, double > producedGood = agent->getListGoods()[randg];
					agent->setProductionRate(std::get<0>(producedGood),1.0);

				}
				else{
					for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
					{
						//id, maxQuantity, price, need and production rate of the good
						agent->addGoodType(std::get<0>(*it),std::get<2>(*it),std::get<3>(*it),std::get<4>(*it),std::get<5>(*it));
						//add init quantity to new good
						agent->addGood(std::get<0>(*it),std::get<1>(*it));

						//set a random price for each goods

						agent->setPrice(std::get<0>(*it),(double)(std::rand()%1000)/1000.0); //TODO:demiurge's job


						//---------------
						//set the need value for each good. Remember: in Basic Simulation the need is the same for everyone
						std::vector<std::tuple<std::string,double> >::iterator need = std::find_if(_needs.begin(), _needs.end(), [=](const std::tuple<std::string,double>& good) {return std::get<0>(good) == std::get<0>(*it);});

						if ( need != _needs.end() )
						{				  
							agent->setNeed(std::get<0>(*it),std::get<1>(*need));
						}
					}			

				}
				log_INFO(logName.str(), getWallTime() << " new agent: " << agent);
				
				
				//loop for initialize the connection of the current agent with all previously created agents.
				for(int j=(i-1); j>=0; j--)
				{
					std::ostringstream ossb;
					ossb << "Roman_" << j;
					this->buildTwoWayConnection(oss.str(),ossb.str());//TODO here check the this->network
				}
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

	void Province::step(){
	          std::stringstream logName;
        logName << "simulation_" << getId();
        log_INFO(logName.str(), getWallTime() << " executing step: " << _step );

        if(_step%_config->getSerializeResolution()==0)
        {
                _scheduler->serializeRasters(_step);
                _scheduler->serializeAgents(_step);
                log_DEBUG(logName.str(), getWallTime() << " step: " << _step << " serialization done");
        }
        stepEnvironment();
        log_DEBUG(logName.str(), getWallTime() << " step: " << _step << " has executed step environment");



	for( std::list< Engine::AgentPtr >::iterator ag=_agents.begin(); ag != _agents.end();ag++){
	  Engine::AgentPtr oneA = *ag;
	  Roman * r = (Roman *) (oneA.get());
		 ProductionAction * PA = new ProductionAction();
	}
	
	
		 PA->execute(*r);
		 	 while(it != gto.end()){	
		   // offerer.printInventory();

	   
		    std::string goodWanted = *it;
		    if(goodWanted != offererProducedGood){
		    std::vector< std::string > exchangeNetwork =offerer.getValidRcvConnections();

		    std::random_shuffle(exchangeNetwork.begin(),exchangeNetwork.end());

		    std::vector< std::string >::iterator itO=exchangeNetwork.begin();
		   
		    
		    std::tuple< std::string, double, double> bestTrade=std::make_tuple<std::string,double,double>(std::string(""),0,0);

		    //qt is the quantity that of is own produced good the agent is willing to exchange
		    
		    
    		    double requestedQuantity= getRequestedQuantity(offerer,goodWanted);
		    			
		    double proposedQuantity = (offerer.getPrice(offererProducedGood)/offerer.getPrice(goodWanted))*requestedQuantity;
		   
		    int noffer=0;
		    bool tradeDone = 0;

		    while(itO != exchangeNetwork.end() && !tradeDone && noffer<=5) //TODO:number of max test <nbmax (maybe gintis use it only because of computational limitiation)
		    {
		      noffer++;
		      Roman & responder = (Roman&)(*world->getAgent(*itO));
		      
		      std::string responderProducedGood = std::get<0>(responder.getProducedGood());
		      
		      
		      if(responderProducedGood == goodWanted ){ 

			
			      
			      double responderTradeWill = getRequestedQuantity(responder,offererProducedGood); 
      			      double responderTradCounter= responderTradeWill*(offerer.getPrice(offererProducedGood)/offerer.getPrice(goodWanted)); 

			      if( responderTradeWill <= proposedQuantity && responder.getQuantity(offererProducedGood) <= responderTradeWill &&
				( proposedQuantity*responder.getPrice(offererProducedGood) <= requestedQuantity*responder.getPrice(goodWanted)) &&
				responder.getQuantity(goodWanted) >= requestedQuantity && offerer.getQuantity(offererProducedGood) > proposedQuantity )
			      {
				if(responderTradCounter<requestedQuantity)requestedQuantity=responderTradCounter;
// 				std::cout<<"responderWill:"<<responderTradeWill<<"Proposed:"<<proposedQuantity<<std::endl;
// 			   	std::cout<<"responderCounterOffer:"<<responderTradCounter<<"offrequest:"<<requestedQuantity<<std::endl;
			   
				if(std::get<2>(bestTrade) < requestedQuantity){ 

				  bestTrade=std::make_tuple(*itO,requestedQuantity,proposedQuantity);
				  tradeDone=1;
				}
			      }
			
		      }
		      itO++;		    
		  }
		  
		  if(std::get<0>(bestTrade) != ""){
		    
		    //The usage of a best trade tuple have sense only if users could exchange less than Requested quantity. 
		    //This would imply that an offerer will check each person in the market and trade with each one of them and keep the best offerer
		    //But in the Gintis version people leave the market when they did a trade so not useful now.
		    
		    //output every  thing:
// 		    std::cout<<"best trade w/"<< std::get<0>(bestTrade)<< " de "<<offererProducedGood<<" get :"<< std::get<1>(bestTrade)<<" give :"<<std::get<2>(bestTrade)<<std::endl;; 
// 		    std::cout<<"Before offerer-------------------"<<std::endl;
// 		  offerer.printInventory();
		    
		    offerer.setQuantity(goodWanted,std::get<1>(bestTrade));		    
		    offerer.setQuantity(offererProducedGood,offerer.getQuantity(offererProducedGood)-std::get<2>(bestTrade));

		    
		    Roman & responder = (Roman&)(*world->getAgent(std::get<0>(bestTrade)));
		    
// 		  std::cout<<"Before receiver-------------------"<<std::endl;
// 		  responder.printInventory();
		 
		    
		    
		    responder.setQuantity(offererProducedGood,std::get<2>(bestTrade));
    		    responder.setQuantity(goodWanted,responder.getQuantity(goodWanted)-std::get<1>(bestTrade));
// 		  std::cout<<"After offerer-------------------"<<std::endl;
// 		  offerer.printInventory();
// 		  std::cout<<"After receiver-------------------"<<std::endl;
// 		  responder.printInventory();
		
		  }
		  else{
		    //the agent has leaved the market without doing any trade
		  }
		  
		    }


	  
	  
	}
// 		int action = _agent->getMaxActions();
// 		std::list<Engine::Action*> actions;
// 		int timestep = _agent->getWorld()->getCurrentStep();
// 		if(timestep%3 == 1 && _selectionProcess == "trade" )actions.push_back(new TradeAction());
// 		if(timestep%3 == 2 && _selectionProcess == "trade" )actions.push_back(new ConsumptionAction());
// 		if(timestep%30 == 0)actions.push_back(new CulturalAction(_mutationRate,_selectionProcess,_innovationProcess));
// 		if(timestep%31 == 0)_agent->setScore(0.0);

	
	
	_scheduler->executeAgents();
        _scheduler->removeAgents();
        log_INFO(logName.str(), getWallTime() << " finished step: " << _step);
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

