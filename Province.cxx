#include <Province.hxx>
#include <ProvinceConfig.hxx>
#include <TradeAction.hxx>
#include <CulturalAction.hxx>
#include <ConsumptionAction.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <Logger.hxx>
#include <Scheduler.hxx>

#include <map>

namespace Epnet
{
	

	Province::Province(Engine::Config * config, Engine::Scheduler * scheduler ) : World(config, scheduler, true)
	{
		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		
		double all_needs=0.0;
		_minScore=0.0;
		_maxScore=0.0;
		_nbProds=provinceConfig._nbProds;
		_totNbProds=provinceConfig._totNbProds;
		_schedule = provinceConfig._historicalSchedule;
		_totPopSize=0;
	

		//initialize knowledge about the n goods in our economy: what are the different types, what is their absolute value
		//Maybe that should go in the constructor
		if(provinceConfig._goodsParam == "random" ||provinceConfig._goodsParam == "randn" ){
		    for (int i = 0; i < provinceConfig._numGoods ; i++)
		    {
			double tneed=(double)(Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0; //for totally random absolute value, initialize here
			//double tneed=bneed * (i+1.0);

			all_needs += tneed; //tneeds is the sum of all value. It could be use to normalize the privec
			std::ostringstream sgoodType;
			sgoodType << "g"<< i;				
			std::string goodType = sgoodType.str();
			_needs.push_back(std::make_tuple(goodType,tneed+1));
			_maxscore.push_back(std::make_tuple(goodType,0.0));
			_minscore.push_back(std::make_tuple(goodType,1000.0));
			_typesOfGood.push_back(goodType);
			_good2Producers.insert(std::pair<std::string,std::vector<std::string>>(goodType,{}));
			//				_good2CulturalNetwork.insert(std::pair<std::string,Network>(goodType,Network()));
		    }	  
		    //used to normalised the needs such as sum(needs(i))=1
		    if(provinceConfig._goodsParam == "randn"){
			for (auto it = _needs.begin(); it != _needs.end() ; it++){
			    double p = std::get<1>(*it);
			    *it=std::make_tuple(std::get<0>(*it),p/all_needs+1);
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
			    _minscore.push_back(std::make_tuple(goodType,1000.0));
			    _typesOfGood.push_back(goodType);
			    _good2Producers.insert(std::pair<std::string,std::vector<std::string>>(goodType,{}));
			    //_good2CulturalNetwork.insert(std::pair<std::string,Network>(goodType,Network()));
			}	  

		}
		else if(provinceConfig._goodsParam == "manual"){
			//In that case each good and the properties of thoses good have to be manually set by the user in the config file
			//At this stage each good should be already well configured by ProvinceConfig 
			//Here we just update global variable later used to compute some metrics
			for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
			{
			    std::string goodType = std::get<0>(*it);

			    _needs.push_back(std::make_tuple(goodType,std::get<4>(*it)));  
			    _maxscore.push_back(std::make_tuple(goodType,0.0));
			    _minscore.push_back(std::make_tuple(goodType,1000.0));
			    if(provinceConfig._eventsHistory){
				//if we have historical chronoliges, check if we push this good yet or not
				float startingstep=std::get<0>(_schedule[goodType]);  
				//during the initialization phase only the good traded at 0 are added to the market
				if(startingstep == 0){
					std::cout<<goodType<<" will be traded from start"<<std::endl;
				    _typesOfGood.push_back(goodType);
				    _good2Producers.insert(std::pair<std::string,std::vector<std::string>>(goodType,{}));
				}
			    }
			    else{
				_typesOfGood.push_back(goodType);
				_good2Producers.insert(std::pair<std::string,std::vector<std::string>>(goodType,{}));
			    }

			}

		}
		else{
		    //error no good type
		}


		for(auto it = _schedule.begin();it!=_schedule.end();it++){
		    std::cout<<"good "<< it->first << ", startime="<<std::get<0>(_schedule[it->first])*provinceConfig._numSteps<< ", endtime="<<std::get<1>(_schedule[it->first])*provinceConfig._numSteps<<std::endl;
		    //std::tuple<float,float> a = schedule.at(*it)

		}

		

		if(logTrade()){
		    //initialize a log file that allows to log why trades fail
		    std::stringstream tradeHeaders ;
		    tradeHeaders<< "timestep,goodwanted,goodproposed,requestedQuantity,responderTradCounter,responder.getQuantity-goodWanted,proposedQuantity,responderTradeWill,offerer.getQuantity-offererProducedGood,responder.getQuantity-offererProducedGood,responderTradeWill_SUP_proposedQuantity,responderTradeWill_INFEQ_0,requestedQuantity_INFEQ_0,responder.getQuantity-goodWanted-_INF_requestedQuantity,offerer.getQuantity-offererProducedGood-_INF_proposedQuantity,responderTradCounter_INF_requestedQuantity-";
		    log_INFO("trade",tradeHeaders.str());
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
				//std::cout << "Roman_" << i <<"log"<<std::log(double(i))<< " preceiling:"<<(1.0/std::log(double(i))) *4.0<< " size:"<<size<<std::endl;

				int size=getASize();
				Roman * agent = new Roman(oss.str(),provinceConfig._controllerType,provinceConfig._mutationRate,provinceConfig._selectionProcess,provinceConfig._innovationProcess,provinceConfig._culturalStep,provinceConfig._aType,size);
				addAgent(agent);
				//position is actually not interesting
				agent->setRandomPosition();

				_totPopSize+=size; //add up the size of the new agent to be able to compute cumulate size
			
				//for each agent initialize all good. Even goods that are not trade yet, are registered. They will just appears with zeros everywhere.
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
						while(agent->getPrice(goodType)<=0.0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);// market clearing price : 1.0/(i+1)
						if(agent->getQuantity(goodType)<0)agent->setQuantity(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);
						if(agent->getProductionRate(goodType)<0)agent->setProductionRate(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);

						//---------------/*
						//set the need value for each good. Remember: in Basic Simulation the need is the same for everyone
						agent->setNeed(goodType,std::get<1>(_needs[g]));
						//agent->setPrice(goodType,1.0/std::get<1>(_needs[g]));
					}			


				}
				else if(provinceConfig._goodsParam== "gintis07" )
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
						while(agent->getPrice(goodType)<=0.0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);// market clearing price : 1.0/(i+1)
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


				}
				else if(provinceConfig._goodsParam == "manual"){
					for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
					{
						//id, maxQuantity, price, need and production rate of the good
					    std::string goodType = std::get<0>(*it);
						agent->addGoodType(goodType,std::get<2>(*it),std::get<3>(*it),std::get<4>(*it),std::get<5>(*it));

						//add init quantity to new good
						agent->addGood(goodType,std::get<1>(*it));
						//set a random properties for each goods
						if (getTradeVolSelFunction()=="brughman17" )
						    while(agent->getPrice(goodType)<=0.0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,10));// market clearing price : 1.0/(i+1)
						else
						    while(agent->getPrice(goodType)<=0.0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);// market clearing price : 1.0/(i+1)

						//set a random price for each goods

					}			

				}
				else{
				    //
				}

				if(provinceConfig._goodsParam == "gintis07")
					log_INFO(logName.str(), getWallTime() << " new agent: " << agent << "\n" << agent->getSegmentsProp());

				///choose the production good
				std::tuple< std::string, double, double, double, double, double > producedGood =  std::tuple< std::string, double, double, double, double, double >();

				if(provinceConfig._typeProd == "unbalanced"){
				    if(provinceConfig._eventsHistory){
					
					auto it = _good2Producers.begin();
					bool found=false;
					while(it!=_good2Producers.end() && !found){
					    std::string good= it->first;
					    int curnum =(it->second).size();
					    int total =_totNbProds[good];
					    if(curnum < total || total < 0){
						producedGood=agent->getFullGood(good);
						found=true;
					    }
					    it++;
					}

				    }
				    else{ //an algo to setup 'on the fly' who produces what in an unbalanced scenario


					bool found=0; //a stop condition
					int list=0; //indice that will be incremented
					int ind=0;
					int ng=_nbProds.size(); //number of goods

					while(!found && list <= (ng-1)){ //loop to see where the id of the roman fall in => the index where he falls in gives us his production good

					    if(std::get<1>(_nbProds[list]) < 0 || i < ind + std::get<1>(_nbProds[list]) ){
						//if the id is less than ...
						found=1;
						producedGood = agent->getListGoods()[list];
					    }

					    ind+=std::get<1>(_nbProds[list]);
					    list++;
					}
				    }

				}
				else{
				    //Set producedGood as a modulo of the agent ID
				    int randg = i%provinceConfig._numGoods;
				    producedGood = agent->getListGoods()[randg];
				}

				//We add this agent to the list of productor of this good
				//this is used afterward to simplify the way people are going to market
				_good2Producers[std::get<0>(producedGood)].push_back(oss.str());

				if(std::get<0>(producedGood) != "coins") agent->setSize(1);

				//set rate of production of the production good
				if(agent->getProductionRate(std::get<0>(producedGood))<=0) //this is all about HOW you want to initialize your production rate. So far it's really badly done if you want to set them up manually
					agent->setProductionRate(std::get<0>(producedGood),1.0); 

				//loop for initialize the commercial connection of the current agent with all previously created agents.
				for(int j=(i-1); j>=0; j--)
				{
					std::ostringstream ossb;
					ossb << "Roman_" << j;
					///this->buildTwoWayConnection(oss.str(),ossb.str());//TODO here check the this->network
					if(provinceConfig._networkType == "integrate"){
					    Roman* agb = dynamic_cast<Roman*> (getAgent(ossb.str()));
					    agent->addCulturalNeighbour(ossb.str());
					    agb->addCulturalNeighbour(oss.str());
					}
					
				}
			}
		}
		
		//Update social and economics networks
		if(provinceConfig._networkType != "integrate"){

		for (std::map< std::string, std::vector< std::string > >::iterator it = _good2Producers.begin(); it != _good2Producers.end();it++) {
				
			std::vector<std::string> groupOfproducer = it->second;
			std::string type = provinceConfig._networkType; 
			
			
			//first we generate a social network for the producers
			Network n = Network(groupOfproducer,type,it->first,provinceConfig._networkParam);
			//Network n = Network(groupOfproducer,type,it->first,true);
			if(provinceConfig._networkOut == "true")
			    n.write();
			_good2CulturalNetwork.insert(std::pair<std::string,Network>(it->first,n));

			//then we use this social network to setup the social neighbour of every producer of this network
			for (std::vector<std::string>::iterator producer = groupOfproducer.begin(); producer != groupOfproducer.end(); producer++){
				
				Roman* romanProducer = dynamic_cast<Roman*> (getAgent(*producer));
				if (romanProducer == NULL)
				{
					std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
					exit(1);
					
				}
				std::vector< std::string > u= n.getNeighboursOf(*producer);
				romanProducer->setListOfCulturalNeighbours( n.getNeighboursOf(*producer));
				
			}
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
		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		if(provinceConfig._networkType == "integrate")
		    return _maxScore;

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

		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		//so fare the integrate case mean that cultural network == whole network wich mean that it's emaningless to look to the score associate to one good
		if(provinceConfig._networkType == "integrate")
		{
		    return _minScore;
		}

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
	//
	//return the ration of people producing the good `pgood`
	double Province::getRatio(std::string pgood){
		int totalAgents = this->getNumberOfAgents();
		int nbAgentsProducingSameGood = this->getListOfProd(pgood).size();

		double ratio = (double)(totalAgents)/(double)nbAgentsProducingSameGood; 
		return ratio;
	}

	float Province::getMarketSize(){

		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		return provinceConfig._marketSize;
	}

	std::string Province::getTradeUtilFunction(){

		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		return provinceConfig._tradeUtilFunction;
	}

	std::string Province::getTradeVolSelFunction(){

		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		return provinceConfig._tradeVolSelFunction;
	}

	//Get the maximum amplitude of Mu
	double Province::getMuMax(){

		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		return (double) provinceConfig._muMax;
	}

	double Province::getCopyRate(){

		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		return provinceConfig._copyRate;
	}


	double Province::getNeed(std::string good){
	    //check if a good of that type exist in the list
	    std::vector<std::tuple<std::string,double> >::iterator it = std::find_if(_needs.begin(), _needs.end(), [=](const std::tuple<std::string,double>& n) {return std::get<0>(n) == good;});
	    if ( it != _needs.end() )
	    {
		return std::get<1>(*it);
	    }

	    //return something impossible as an error
	    return -1.0;
	}

	void Province::printAllCulturalNerwork(){
		
		for(std::map< std::string, Network >::iterator it= _good2CulturalNetwork.begin();it!=_good2CulturalNetwork.end();it++)
		{
			std::cout<<"The network of producers of"<<it->first<<" is :"<<it->second<<std::endl;
			
		}
	}


	void Province::step()
	{
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
	    _scheduler->executeAgents();
	    _scheduler->removeAgents();
	    log_INFO(logName.str(), getWallTime() << " finished step: " << _step);

	    const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();

	    if ( ((_step)%(3 * (provinceConfig._culturalStep)))   == 0 && (_step> 4 )){
		//std::cout<<"reset the bango"<<std::endl;
		for(int i=0; i<provinceConfig._numAgents; i++){
		    std::ostringstream oss;
		    oss << "Roman_" << i;
		    Roman* romanAgent = dynamic_cast<Roman*> (getAgent(oss.str()));
		    romanAgent->setScore(0.0);
		}
	    }
	    if (_step%3  == 0){
		//!TODO THIS IS CRITICAL UPDATE that should be merged everywhere asap
		for( std::vector<std::string>::iterator it= _typesOfGood.begin() ; it!=_typesOfGood.end(); it++){

		    //std::cout<<"reset scores"<< std::endl;
		    //std::cout<<*it<< std::endl;
		    std::string goodType = *it;
		    setMaxScore(goodType,0.0);
		    setMinScore(goodType,1000.0);

		}
		setMaxScore(0.0);
		setMinScore(1000.0);
	    }

	}

	void Province::stepEnvironment()
	{
	    //std::cout<<"step:=="<<_step<<std::endl;

	    const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
	    //std::cout<<"resetscore All agents: aka ( if (_step%(3 * (provinceConfig._culturalStep) + 1) == 0) ) "<<(_step%(3 * (provinceConfig._culturalStep) + 1) == 0)<<std::endl;
	    //std::cout<<"resminmax : aka ( (_step%3  == 2) ) "<<(_step%3== 2)<<std::endl;
	    //std::cout<<"Production Action : aka ( (_step%3  == 0) ) "<<(_step%3== 0)<<std::endl;
	    //std::cout<<"trade Action : aka ( (_step%3  == 1) ) "<<(_step%3== 1)<<std::endl;
	    //std::cout<<"Consumption Action : aka ( (_step%3  == 2) ) "<<(_step%3== 2)<<std::endl;
	    //std::cout<<"Cultural Action : aka  ((_step%%(3 * (_culturalStep)) == 0) && (timestep> 3 * (_culturalStep) )) "<<( (_step%(3 * (provinceConfig._culturalStep)) == 0) && (_step> 3 * (provinceConfig._culturalStep) ) )<<std::endl;

	    for(size_t d=0; d<_rasters.size(); d++)
	    {
		if(!_rasters.at(d) || !_dynamicRasters.at(d))
		{
		    continue;
		}
		stepRaster(d);
	    }
	    if( provinceConfig._events == "rate"){
		if( getCurrentTimeStep() >= provinceConfig._culturalStep* 3 *provinceConfig._eventsRate  && getCurrentTimeStep() % (provinceConfig._culturalStep* 3 * provinceConfig._eventsRate ) == 0 && getCurrentTimeStep() <= provinceConfig._eventsStop ){

		    std::ostringstream sgoodType;
		    sgoodType << "g"<< _typesOfGood.size()-1;	//the new good will be simply called gN
		    std::string goodType = sgoodType.str();
		    _maxscore.push_back(std::make_tuple(goodType,0.0));
		    _minscore.push_back(std::make_tuple(goodType,1000.0));

		    _needs.push_back(std::make_tuple(goodType,1)); //add a new need for this new good to the global vector of need
		    //this->normalizeNeeds(); //update the need of this and all other good

		    _typesOfGood.push_back(goodType); //add the good to the gloabl list of good


		    int newProd=0;
		    for(auto it=_agents.begin(); it != _agents.end() ; it++)
		    {

			Roman * agent  = dynamic_cast<Roman *>(getAgent( (*(*it)).getId()));
			agent->addGoodType(goodType,0.0,0.0,0.0,0.0);

			//    //set a random properties for each goods
			if (getTradeVolSelFunction()=="brughman17" )
			    while(agent->getPrice(goodType)<=0.0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,10));// market clearing price : 1.0/(i+1)
			else
			    while(agent->getPrice(goodType)<=0.0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);// market clearing price : 1.0/(i+1)
			if( std::get<0>(agent->getProducedGood()) == "coins" && newProd<5){
			    agent->setProductionRate(goodType,1.0);
			    agent->setProductionRate("coins",0.0);
			    agent->setSize(1);
			    if(_good2Producers.find(goodType) == _good2Producers.end())
				_good2Producers.insert(std::pair<std::string,std::vector<std::string>>(goodType,{agent->getId()}));
			    else
				_good2Producers[goodType].push_back(agent->getId());
			    removeFromListOfProd(agent->getId(),"coins");
			    newProd++;
			}

			agent->setNeed(goodType,1.0);
		    }			

		}
	    }
	    ///UPDATE GOODS
	    if( provinceConfig._eventsHistory){
		updateGoodList();
	    }

	}

	///This function go through the map `_schedule` to check if some good have to be removed or added
	void Province::updateGoodList(){
	    std::stringstream logName;
	    logName << "simulation_" << getId();
	    const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();

	    for(auto it = _schedule.begin();it!=_schedule.end();it++){
		int currentStep=_step;
		int totstep=provinceConfig._numSteps;
		std::string good = it->first;
		int absoluteStartStep = totstep * std::get<0>(it->second);
		int absoluteEndtStep = totstep * std::get<1>(it->second);

		bool isTraded =(std::find(_typesOfGood.begin(), _typesOfGood.end(), good) != _typesOfGood.end() );//tru if the good is in the list of traded good, false else

		//if the current step is more than the supopsed starting step of the good
		if((absoluteStartStep <= currentStep) && !isTraded && currentStep < absoluteEndtStep){
		    log_INFO(logName.str(),"NEW good:"<<good<<",curr:"<<currentStep<<",start:"<<absoluteStartStep<<",end:"<<absoluteEndtStep);
		    _typesOfGood.push_back(good); //ad the good to the list of tradded good

		    while(_good2Producers[good].size()<_totNbProds[good]){ 
			std::string randAgent=_good2Producers["coins"][0]; //get a coins porducer aka a consumer
			switchAgentProduction(randAgent,good); 		   //switch its porduction good
		    }
		    log_DEBUG(logName.str(),printListOfProd(good));
		}

		if(absoluteEndtStep <= currentStep && isTraded){
		    log_INFO(logName.str(),"REMOVE good:"<<good<<",curr:"<<currentStep<<",start:"<<absoluteStartStep<<",end:"<<absoluteEndtStep);
		    while(!_good2Producers[good].empty()){
			std::string randAgent=_good2Producers[good][0]; 
			switchAgentProduction(randAgent,"coins");
		    }
		    _typesOfGood.erase(std::remove(_typesOfGood.begin(), _typesOfGood.end(), good), _typesOfGood.end());
		    log_DEBUG(logName.str(),printListOfProd(good));
		}
	    }

	}

	//This function take  change the production good of an agent and update the list in consequence
	void Province::switchAgentProduction(std::string agent, std::string good){
	    Roman * ragent  = dynamic_cast<Roman *>(getAgent(agent));
	    std::string originalGood=std::get<0>(ragent->getProducedGood());
	    removeFromListOfProd(agent,originalGood);//remove agent from the list of producers of its initial production good
	    _good2Producers[good].push_back(agent); //add agent to the list of producer of its new production good
	    ragent->setProductionRate(originalGood,0.0); //agent will not receive coins anymore
	    ragent->setProductionRate(good,1.0); //set the production rate of `good` as 1
	    if(isPopSize()){
		if(good=="coins")ragent->setSize(getASize());
		else ragent->setSize(1);
	    }
	}

	//this function normlaize all the need of the non monetary goods. Whatever was the need __before__ the call of this function, it become 1/#(!monetarygood). ie if thre is 6 goods in the province, 5 of them beings non monetary, the need for the five non monetary goods will be 0.2
	void Province::normalizeNeeds()
	{
	    for( auto it = _needs.begin() ; it != _needs.end(); it++){
		//std::cout<<"nouveau needs for"<<std::get<0>(*it)<<":"<<_needs.size()<<" moins un " <<_needs.size()-1<<" celuila:"<<std::get<0>(*it)<< "vaut : "<<std::get<1>(*it)<<std::endl;
		if(std::get<0>(*it) != "coins")
		    std::get<1>(*it)=1.0/(_needs.size()-1);
		//std::cout<<"after  needs:"<<_needs.size()<<" moins un " <<_needs.size()-1<<" celuila:"<<std::get<0>(*it)<< "vaut : "<<std::get<1>(*it)<<std::endl;;

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

	bool Province::hasEvent()
	{ 
	    const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
	    if(provinceConfig._events == "rate") 
	       	return(true) ; 
	    else
		return(false);
	}
	bool Province::logTrade()
	{ 
	    const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
	    if(provinceConfig._logTrade == "true") 
	       	return(true) ; 
	    else
		return(false);
	}
	void Province::removeFromListOfProd(std::string agentId,std::string good)
    	{
	    std::stringstream logName;
	    logName << "simulation_" << getId();
	    std::vector<std::string> * listToRemove= & _good2Producers[good];
	    auto idx = std::find(listToRemove->begin(),listToRemove->end(),agentId);
	    if(idx==listToRemove->end()){
		log_INFO(logName.str(),"warnings:"<<agentId<<" is not in the list of producer of "<<good<<" when it should");
	    }
	    else{
		log_DEBUG(logName.str(),agentId<<" has been removed from producers list of "<<good);
		listToRemove->erase(idx);
	    }
	}

	int Province::getASize(){
	    const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();

	    if(provinceConfig._distrib == "pl")
		return(	Engine::GeneralState::statistics().getPowerLawDistValue (provinceConfig._plMin,provinceConfig._plMax,provinceConfig._plAlpha));
	    else
		return(Engine::GeneralState::statistics().getUniformDistValue(0,1000));

	}
	bool Province::isPopSize(){ //return true if pop size is taken into account, false else
	    const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
	    if(provinceConfig._distribUse == "on")return (true);
	    else return (false);
	}


} // namespace Roman


