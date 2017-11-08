#include <TradeAction.hxx>

#include <World.hxx>
#include <Province.hxx>
#include <ctime>
#include <iostream>
#include <Logger.hxx>

namespace Epnet
{

	TradeAction::TradeAction()
	{
	}

	TradeAction::~TradeAction()
	{
	}

	void TradeAction::execute(Engine::Agent& agent)
	{
	  
	//       std::clock_t    start;
	//	start = std::clock();
		Roman & offerer = (Roman&)agent;
		Engine::World * world = agent.getWorld();
		Province & provinceWorld = (Province&) *world;

		std::vector<std::string> gto= provinceWorld.getTypesOfGood();

		std::random_shuffle(gto.begin(),gto.end());//random the order of good like that we don't go to buy the same product first

		std::vector<std::string>::iterator  it = gto.begin();
		float epsilon=0.0001;

		std::string offererProducedGood= std::get<0>(offerer.getProducedGood()); 
		//std::cout<<gto.size()<<" prod."<< agent.getId()<< "trading";

		//For all type of ressource
		while(it != gto.end()){	

			std::string goodWanted = *it;
			//std::cout<<" "<< goodWanted << ", "<<provinceWorld.getListOfProd(goodWanted).size()<< " prod, "<< std::endl;
			if(goodWanted != offererProducedGood){

				//std::cout<<" \t "<< goodWanted << ", producing "<<offererProducedGood<< " prod, "<< std::endl;
				//std::vector< std::string > exchangeNetwork =offerer.getValidRcvConnections();

				std::vector< std::string > exchangeNetwork =provinceWorld.getListOfProd(goodWanted);
				std::random_shuffle(exchangeNetwork.begin(),exchangeNetwork.end()); //random the network of 

				std::vector< std::string >::iterator itO=exchangeNetwork.begin();

				std::tuple< std::string, double, double> bestTrade=std::make_tuple<std::string,double,double>(std::string(""),0,0);

				//qt is the quantity that of is own produced good the agent is willing to exchange
				double ratio =  provinceWorld.getRatio(offererProducedGood);

				double requestedQuantity=0.0; //__requestedQuantity__ is the quantity of good __goodWanded__ that satisify the demand of __offerer__ give is own endowment and private price
				double proposedQuantity =0.0; //__proposedQuantity__ is the quantity of good __offererProducedGood__ that the agent __offerer__ propose in exchange of the quantity __requestedQuantity__ 
				double universalNeed =0.0; 

				if(provinceWorld.getTradeVolSelFunction() =="gintis07"){
				    requestedQuantity= offerer.getNeed(goodWanted);//-offerer.getQuantity(goodWanted); //the need as computed given the private utility function of the agent
				}
				else if (provinceWorld.getTradeVolSelFunction()=="gintis06" ||provinceWorld.getTradeVolSelFunction() =="gintis06-outneed" ){
				    double M=0.0;
				    double N=0.0;
				
				    std::vector<std::string> goods= provinceWorld.getTypesOfGood();
				    std::vector<std::string>::iterator g = goods.begin();
				    //compute the lagrangian optmizer
				    while(g!=goods.end()){
					M+=offerer.getPrice(*g) * offerer.getQuantity(*g);//*provinceWorld.getRatio(offererProducedGood);
					if( provinceWorld.getTradeVolSelFunction() =="gintis06")
					    N+=offerer.getPrice(*g) * offerer.getNeed(*g)*ratio;
					if( provinceWorld.getTradeVolSelFunction() =="gintis06-outneed")
					    N+=offerer.getPrice(*g) * 1/offerer.getPrice(*g)*provinceWorld.getRatio(*g); // as is this line is useless, in gintis06 1/getNeed(*g)
					    //N+=offerer.getPrice(*g) * 1/offerer.getPrice(*g)*ratio; // as is this line is useless, in gintis06 1/getNeed(*g)
					//std::cout<<"M:"<< M <<",N:"<<N<<std::endl;
					g++;
				    }

				    double lambda=M/(N);
				    if( provinceWorld.getTradeVolSelFunction() =="gintis06")
					requestedQuantity= offerer.getNeed(goodWanted)*lambda;//-offerer.getQuantity(goodWanted);
				    if( provinceWorld.getTradeVolSelFunction() =="gintis06-outneed")
					requestedQuantity= 1/offerer.getPrice(goodWanted)*lambda;//-offerer.getQuantity(goodWanted);
				    //requestedQuantity= requestedQuantity ;
				    //std::cout<<"lambda="<<lambda<<" requestedQuantity="<<requestedQuantity<<" req:"<<requestedQuantity<<std::endl;
				    if(goodWanted != "coins") requestedQuantity= requestedQuantity-offerer.getQuantity(goodWanted);
				}
				else if (provinceWorld.getTradeVolSelFunction()=="brughman17" ){
				    double M=0.0;
				    double N=0.0;
				
				    std::vector<std::string> goods= provinceWorld.getTypesOfGood();
				    std::vector<std::string>::iterator g = goods.begin();
				   
				    //This is a really useless and slow stuff: should be done just once for the all simu, and not eveyr timestp
				    while(g!=goods.end()){
					if(*g != offererProducedGood && *g != "coins"){
					    M+=offerer.getNeed(*g);
					    N+=offerer.getQuantity(*g);
					}
					g++;
				    }

				    if(goodWanted != "coins") 
					requestedQuantity= (M-N)/(goods.size()-1);
				    else
					requestedQuantity= offerer.getNeed(goodWanted);


				}
				else{
				    requestedQuantity= offerer.getNeed(goodWanted);
				    if(goodWanted != "coins") requestedQuantity= requestedQuantity-offerer.getQuantity(goodWanted);
				}

				proposedQuantity=requestedQuantity*(offerer.getPrice(goodWanted)/offerer.getPrice(offererProducedGood));


				int noffer=0;
				bool tradeDone = 0; //bolean to stop when
				int noffer_max=exchangeNetwork.size()*provinceWorld.getMarketSize();//if noffer_max is < numagents/ngoods, it means that we limite the research of the agent in the markert

	            //std::cout.precision(std::numeric_limits<double>::digits10);
				//std::cout<<offerer.getId()<<" requested ("<<goodWanted<<"):"<<requestedQuantity<<", proposed("<<offererProducedGood<<"):"<<proposedQuantity<<", pricewanted("<<goodWanted<<"):"<<offerer.getPrice(goodWanted)<<", priceproduce("<<offererProducedGood<<"): "<<offerer.getPrice(offererProducedGood)<<std::endl;

				if(requestedQuantity < 0.0 || AlmostEqualRelative(requestedQuantity,0.0,epsilon))tradeDone=1;  //we already satisfied this demand no need to exchange for this good 
				//in all the following and has requestedQuantity =  optimalQuantity - currentQauntity and optimal quantity > = 0 => currentQuantity < optimalQuantity

				//std::cout<< goodWanted<<" has  "<<exchangeNetwork.size()<< " prod :";
				while(itO != exchangeNetwork.end() && !tradeDone && noffer<=noffer_max){ // if we haven't satified our demand yet, we look into our echange network until the `max number of offer` we can do
					noffer++;
					Roman & responder = (Roman&)(*world->getAgent(*itO));

					//std::cout<<" "<<responder.getId()<< " among :"<<responder.getListOfCulturalNeighbours().size()<<":";

					std::string responderProducedGood = std::get<0>(responder.getProducedGood()); //if there is no redistribution: `responderProducedGood == goodWanted` always
					


					if(responderProducedGood == goodWanted && responder.getQuantity(responderProducedGood) > 0  ){ //OPTIMIZATION: remove the agent with getQuantity(goodWanted) <= 0 from exchangeNetwork

					    double ratio =  provinceWorld.getRatio(goodWanted);
					    double responderTradeWill =  0.0;	 //__responderTradeWill__ is the optmial amount of the good __offererProducedGood__ that the agent __responder__ need to satisfy is demand.
					    double responderTradCounter =  0.0;	 //__responderTradeCounter__ is the amount that the agent __responder__ is will to give in exchange of __responderTradeWill__ amount of __offererProducedGood__

					    if(provinceWorld.getTradeVolSelFunction()=="gintis07"){
						responderTradeWill =  responder.getNeed(offererProducedGood);//-responder.getQuantity(offererProducedGood); //in this version the demand is simply the need, as defined by the utility function of the people
					    }
					    else if (provinceWorld.getTradeVolSelFunction()=="gintis06" ||provinceWorld.getTradeVolSelFunction() =="gintis06-outneed" ){
						double M=0.0;
						double N=0.0;

						std::vector<std::string> goods= provinceWorld.getTypesOfGood();
						std::vector<std::string>::iterator g = goods.begin();
						while(g!=goods.end()){
						    M+=responder.getPrice(*g) * responder.getQuantity(*g);//*provinceWorld.getRatio(goodWanted);
						    if (provinceWorld.getTradeVolSelFunction()=="gintis06")
							    N+=responder.getPrice(*g) * responder.getNeed(*g)*ratio;
						    if( provinceWorld.getTradeVolSelFunction() =="gintis06-outneed")
							N+=responder.getPrice(*g) * 1/responder.getPrice(*g)*provinceWorld.getRatio(*g) ;//in that case the ratio should be his own ratio
							//N+=responder.getPrice(*g) * 1/responder.getPrice(*g)*ratio;//in that case the ratio should be his own ratio
						    g++;
						}

						double lambda=M/(N);
						if (provinceWorld.getTradeVolSelFunction()=="gintis06")
						    responderTradeWill =  responder.getNeed(offererProducedGood)*lambda;//-responder.getQuantity(offererProducedGood); 
						if( provinceWorld.getTradeVolSelFunction() =="gintis06-outneed")
						    responderTradeWill =  1/responder.getPrice(offererProducedGood)*lambda;//-responder.getQuantity(offererProducedGood); 
						//std::cout<<"lambda="<<lambda<<"resptradewill="<<responderTradeWill<<" req:"<<requestedQuantity<<std::endl;
						if(offererProducedGood != "coins") responderTradeWill= responderTradeWill-responder.getQuantity(offererProducedGood);

					    }
					    else if (provinceWorld.getTradeVolSelFunction()=="brughman17" ){

						double M=0.0;
						double N=0.0;

						std::vector<std::string> goods= provinceWorld.getTypesOfGood();
						std::vector<std::string>::iterator g = goods.begin();
						//
						//This is a really useless and slow stuff: should be done just once for the all simu, and not eveyr timestp
						while(g!=goods.end()){
						    if(*g != responderProducedGood && *g != "coins"  ){
							M+=responder.getNeed(*g);
							N+=responder.getQuantity(*g);
						    }
						    g++;
						}

						if(offererProducedGood != "coins") 
						    responderTradeWill= (M-N)/(goods.size()-1);
						else
						    responderTradeWill= responder.getNeed(offererProducedGood);
//std::cout<<responderTradeWill<<" MN:"<<M<<", "<<N<<std::endl;
					    }
					    else{
						responderTradeWill= responder.getNeed(goodWanted);//-responder.getQuantity(goodWanted);
						if(responderProducedGood != "coins") responderTradeWill= responderTradeWill-responder.getQuantity(offererProducedGood);
					    }
					    responderTradCounter= responderTradeWill*responder.getPrice(goodWanted)/(responder.getPrice(goodWanted)); 

					//    if( responder.getQuantity(goodWanted) < requestedQuantity ||AlmostEqualRelative(responder.getQuantity(goodWanted),requestedQuantity, epsilon))
					//	requestedQuantity=responder.getQuantity(goodWanted);

					    bool excess= false;
					    //if( ( responderTradeWill < 0.0 || AlmostEqualRelative(responderTradeWill,0.0, epsilon) ) && 				//the quantity offered is at least egual to the quantity the other estim good for him
					    //( responder.getQuantity(goodWanted) > 0 )  
					    ////(proposedQuantity < offerer.getQuantity(offererProducedGood) || AlmostEqualRelative(proposedQuantity,offerer.getQuantity(offererProducedGood),epsilon)) 
					    //)
					    //{
					    //    responderTradeWill=std::min( responder.getQuantity(goodWanted),responder.getPrice(offererProducedGood) /responder.getPrice(goodWanted) * proposedQuantity);
					    //    //std::cout<<"==sowtf?"<<responderTradeWill<<std::endl;
					    //    excess=true;
					    //}
					    if(
						    excess ||
						    !(provinceWorld.getTradeVolSelFunction()=="brughman17") &&
						    ( responder.getPrice(offererProducedGood)*responderTradeWill > responder.getPrice(goodWanted) * requestedQuantity || AlmostEqualRelative(responder.getPrice(offererProducedGood)*responderTradeWill,responder.getPrice(goodWanted) * requestedQuantity , epsilon))&&// 		//the quantity asked is available in the stock of the responder
					    //  )
					    //if(
					    //        //( responderTradeWill < proposedQuantity || AlmostEqualRelative(responderTradeWill,proposedQuantity, epsilon) ) && 				//the quantity offered is at least egual to the quantity the other estim good for him
					    //        responderTradeWill > 0.0 && 				//the quantity offered is at least egual to the quantity the other estim good for him
					    //        requestedQuantity > 0.0 && 				//the quantity offered is at least egual to the quantity the other estim good for him
					            ( responder.getQuantity(goodWanted) > requestedQuantity ||AlmostEqualRelative(responder.getQuantity(goodWanted),requestedQuantity, epsilon)) &&		//the quantity asked is available in the stock of the responder
					            ( offerer.getQuantity(offererProducedGood) > proposedQuantity||AlmostEqualRelative(offerer.getQuantity(offererProducedGood),proposedQuantity, epsilon) )  	//the quantity proposed is available in the offerer stock
					    //        //( responderTradCounter > requestedQuantity ||AlmostEqualRelative(responderTradCounter,requestedQuantity, epsilon))				//the quantity asked is less that the value estimated by the responder
					      )
					    {
						//trade is a success
						if(responderTradeWill<proposedQuantity){
						    //std::cout<<"|| had to cut?"<<std::endl;	
						    proposedQuantity=responderTradeWill;
						    //if(requestedQuantity>responderTradCounter) requestedQuantity = responderTradCounter;
						}

						//std::cout<< "endowment: resp-quantity-wanted sup requestQuantity:"<<( responder.getQuantity(goodWanted) > requestedQuantity ||AlmostEqualRelative(responder.getQuantity(goodWanted),requestedQuantity, epsilon))<<std::endl; 		//the quantity asked is available in the stock of the responder
					        //    std::cout<< "endowment: off-quantity-off sup offQuantity:"<<( offerer.getQuantity(offererProducedGood) > proposedQuantity||AlmostEqualRelative(offerer.getQuantity(offererProducedGood),proposedQuantity, epsilon) )<<std::endl;  	//the quantity proposed is available in the offerer stock
						//std::cout<<"|| success?"<<std::endl;	

						//if we didn't had a better trade before we set this one as the best
						if(std::get<2>(bestTrade) <= requestedQuantity){ 
						    bestTrade=std::make_tuple(*itO,requestedQuantity,proposedQuantity);
						    tradeDone=1;
						}
					    }
					    else if(
						    (provinceWorld.getTradeVolSelFunction()=="brughman17") &&
						    ( responder.getPrice(goodWanted) < offerer.getPrice(goodWanted) || AlmostEqualRelative(responder.getPrice(goodWanted),offerer.getPrice(goodWanted) , epsilon))&&// 		//the quantity asked is available in the stock of the responder
					            ( responder.getQuantity(goodWanted) > requestedQuantity ||AlmostEqualRelative(responder.getQuantity(goodWanted),requestedQuantity, epsilon)) &&		//the quantity asked is available in the stock of the responder
					            ( offerer.getQuantity(offererProducedGood) > proposedQuantity||AlmostEqualRelative(offerer.getQuantity(offererProducedGood),proposedQuantity, epsilon) )  	//the quantity proposed is available in the offerer stock
					    //        //( responderTradCounter > requestedQuantity ||AlmostEqualRelative(responderTradCounter,requestedQuantity, epsilon))				//the quantity asked is less that the value estimated by the responder
					      )
					    {
						//trade is a success
						if(responderTradeWill<proposedQuantity){
						    //std::cout<<"|| had to cut?"<<std::endl;	
						    proposedQuantity=responderTradeWill;
						    //if(requestedQuantity>responderTradCounter) requestedQuantity = responderTradCounter;
						}


						//if we didn't had a better trade before we set this one as the best
						if(std::get<2>(bestTrade) <= requestedQuantity){ 
						    bestTrade=std::make_tuple(*itO,requestedQuantity,proposedQuantity);
						    tradeDone=1;
						}
					    }
					    else{
						    //The trade fail: print why
						    //timestep,requestedQuantity,responderTradCounter,responder.getQuantity(goodWanted),proposedQuantity,responderTradeWill,offerer.getQuantity(offererProducedGood),responder.getQuantity(offererProducedGood),(responderTradeWill<=proposedQuantity),(responderTradeWill>0),(requestedQuantity>0),(responder.getQuantity(goodWanted)>=requestedQuantity),(offerer.getQuantity(offererProducedGood)>=proposedQuantity),(responderTradCounter>=requestedQuantity);

					    if(responderTradeWill >  proposedQuantity){

						    //std::cout<<offerer.getId()<<" requested ("<<goodWanted<<"):"<<requestedQuantity<<", proposed("<<offererProducedGood<<"):"<<proposedQuantity<<", pricewanted("<<goodWanted<<"):"<<offerer.getPrice(goodWanted)<<", priceproduce("<<offererProducedGood<<"): "<<offerer.getPrice(offererProducedGood)<<" respondertradewill:"<<responderTradeWill<<", ratio resp:"<<provinceWorld.getRatio(goodWanted)<<", ratio off:"<<provinceWorld.getRatio(offererProducedGood)<<std::endl;
					    }
						    if(provinceWorld.logTrade()){
							std::stringstream logTrade;
							std::string sep= ",";
							logTrade<<world->getCurrentTimeStep()<<sep;
							logTrade<<goodWanted<<sep;
							logTrade<<offererProducedGood<<sep;
							logTrade<<requestedQuantity<< sep;
							logTrade<<responderTradCounter<< sep;
							logTrade<<responder.getQuantity(goodWanted)<< sep;
							logTrade<<proposedQuantity<< sep;
							logTrade<<responderTradeWill<< sep;
							logTrade<<offerer.getQuantity(offererProducedGood)<< sep;
							logTrade<<responder.getQuantity(offererProducedGood)<< sep;
							logTrade<<( responderTradeWill < proposedQuantity || AlmostEqualRelative(responderTradeWill,proposedQuantity, epsilon) ) <<sep; 				//the quantity offered is at least egual to the quantity the other estim good for him
							logTrade<< (responderTradeWill > 0 ) <<sep; 				//the quantity offered is at least egual to the quantity the other estim good for him
							logTrade<<(requestedQuantity > 0) <<sep; 				//the quantity offered is at least egual to the quantity the other estim good for him
							logTrade<<( responder.getQuantity(goodWanted) > requestedQuantity ||AlmostEqualRelative(responder.getQuantity(goodWanted),requestedQuantity, epsilon)) <<sep;		//the quantity asked is available in the stock of the responder
							logTrade<<( offerer.getQuantity(offererProducedGood) > proposedQuantity||AlmostEqualRelative(offerer.getQuantity(offererProducedGood),proposedQuantity, epsilon) )  <<sep;	//the quantity proposed is available in the offerer stock
							//	   logTrade<<( responderTradCounter > requestedQuantity ||AlmostEqualRelative(responderTradCounter,requestedQuantity, epsilon));				//the quantity asked is less that the value estimated by the responder
							log_INFO("trade", logTrade.str());

						    }
					    }


					}
					else{
					    //This case has to be implemented when redistribution is allowed!
					    //std::cout<<"good "<<goodWanted<<std::endl;
					}

					itO++;		    
				}

				if(std::get<0>(bestTrade) != ""){

					//The usage of a best trade tuple have sense only if users could exchange less than Requested quantity. 
					//This would imply that an offerer will check each person in the market and trade with each one of them and keep the best offerer
					//But in the Gintis version people leave the market when they did a trade so not useful now.

					offerer.setQuantity(goodWanted,offerer.getQuantity(goodWanted)+std::get<1>(bestTrade));
					offerer.setQuantity(offererProducedGood,offerer.getQuantity(offererProducedGood)-std::get<2>(bestTrade));


					Roman & responder = (Roman&)(*world->getAgent(std::get<0>(bestTrade)));
					responder.setQuantity(offererProducedGood,responder.getQuantity(offererProducedGood)+std::get<2>(bestTrade));
					responder.setQuantity(goodWanted,responder.getQuantity(goodWanted)-std::get<1>(bestTrade));

				}
				else{
					//the agent has leaved the market without doing any trade
				}
			}


			it++;
		}
//      std::cout << "tradetime," << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << ","<< world->getCurrentTimeStep()<<std::endl;
//	if(offerer.getQuantity(offererProducedGood) <= 0.0||AlmostEqualRelative(offerer.getQuantity(offererProducedGood),0.0, epsilon))
//	    offerer.setQuantity(offererProducedGood,3);




	}

	double TradeAction::getRequestedQuantity(Engine::Agent & agent, std::string goodWanted){


		Roman & r = (Roman&)agent;
		std::string producedGood= std::get<0>(r.getProducedGood()); 
		double Mo = r.getPrice(producedGood)*r.getQuantity(producedGood);
		double SumOj=0;
		//  	  double Mo=0;

		std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods = r.getListGoods();
		for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
			std::string ressource= std::get<0>(*ot);
			SumOj+=r.getPrice(ressource)*r.getNeed(ressource);
			//		Mo+=r.getPrice(ressource);


		}  
		return ((r.getNeed(goodWanted)*Mo/SumOj)-r.getQuantity(goodWanted));

	}

	std::string TradeAction::describe() const
	{
		return "Trade action";
	}

	bool TradeAction::AlmostEqualRelative(float A, float B,
		                         float maxRelDiff )
	{
	    // Calculate the difference.
	    float diff = fabs(A - B);
	    A = fabs(A);
	    B = fabs(B);
	    // Find the largest
	    float largest = (B > A) ? B : A;

	    if (diff <= largest * maxRelDiff)
		return true;
	    return false;
	}
} // namespace Epnet


