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

		std::string offererProducedGood= std::get<0>(offerer.getProducedGood()); 

		//For all type of ressource
		while(it != gto.end()){	


			std::string goodWanted = *it;
			if(goodWanted != offererProducedGood){

				//std::vector< std::string > exchangeNetwork =offerer.getValidRcvConnections();

				std::vector< std::string > exchangeNetwork =provinceWorld.getListOfProd(goodWanted);
				std::random_shuffle(exchangeNetwork.begin(),exchangeNetwork.end()); //random the network of 

				std::vector< std::string >::iterator itO=exchangeNetwork.begin();

				std::tuple< std::string, double, double> bestTrade=std::make_tuple<std::string,double,double>(std::string(""),0,0);

				//qt is the quantity that of is own produced good the agent is willing to exchange

				double requestedQuantity=0;
				double proposedQuantity =0;
				if(provinceWorld.getTradeVolSelFunction() =="gintis07"){
				    requestedQuantity= offerer.getNeed(goodWanted)-offerer.getQuantity(goodWanted);
				}
				else if( provinceWorld.getTradeVolSelFunction() =="gintis06"){
				    double M=0.0;
				    double N=0.0;
				
				    std::vector<std::string> goods= provinceWorld.getTypesOfGood();
				    std::vector<std::string>::iterator g = goods.begin();
				    //compute the lagrangian optmizer
				    while(g!=goods.end()){
					M+=offerer.getPrice(*g) * offerer.getQuantity(*g);
					N+=offerer.getPrice(*g) * offerer.getNeed(*g);
					g++;
				    }

				    double lambda=M/N;
				    requestedQuantity= offerer.getNeed(goodWanted)*lambda-offerer.getQuantity(goodWanted);
				}
				else if( provinceWorld.getTradeVolSelFunction() =="gintis06-outneed"){
				    double M=0.0;
				    double N=0.0;
				
				    std::vector<std::string> goods= provinceWorld.getTypesOfGood();
				    std::vector<std::string>::iterator g = goods.begin();
				    //compute the lagrangian optmizer
				    while(g!=goods.end()){
					M+=offerer.getPrice(*g) * offerer.getQuantity(*g);
					N+=offerer.getPrice(*g) * 1/offerer.getPrice(*g); // as is this line is useless, in gintis06 1/getNeed(*g)
					g++;
				    }

				    double lambda=M/N;
				    requestedQuantity= 1/offerer.getPrice(goodWanted)*lambda-offerer.getQuantity(goodWanted);
				}
				else{
				    requestedQuantity= offerer.getPrice(goodWanted)-offerer.getQuantity(goodWanted);
				}

				    proposedQuantity=requestedQuantity*(offerer.getPrice(goodWanted)/offerer.getPrice(offererProducedGood));


				int noffer=0;
				bool tradeDone = 0;
				int noffer_max=exchangeNetwork.size()*provinceWorld.getMarketSize();//if noffer_max is < numagents/ngoods, it means that we limite the research of the agent in the markert
				//std::cout<<offerer.getId()<<" requested ("<<goodWanted<<"):"<<requestedQuantity<<", proposed("<<offererProducedGood<<"):"<<proposedQuantity<<", pricewanted("<<goodWanted<<"):"<<offerer.getPrice(goodWanted)<<", priceproduce("<<offererProducedGood<<"): "<<offerer.getPrice(offererProducedGood)<<std::endl;
				if(requestedQuantity < 0.0 || AlmostEqualRelative(requestedQuantity,0.0,0.01))tradeDone=1;
				while(itO != exchangeNetwork.end() && !tradeDone && noffer<=noffer_max){
					noffer++;
					Roman & responder = (Roman&)(*world->getAgent(*itO));

					std::string responderProducedGood = std::get<0>(responder.getProducedGood());


					if(responderProducedGood == goodWanted ){ 

					    double responderTradeWill =  0;
					    double responderTradCounter =  0;
					    if(provinceWorld.getTradeVolSelFunction()=="gintis07"){
						responderTradeWill =  responder.getNeed(offererProducedGood)-responder.getQuantity(offererProducedGood); 
					    }
					    else if (provinceWorld.getTradeVolSelFunction()=="gintis06"){
						double M=0.0;
						double N=0.0;

						std::vector<std::string> goods= provinceWorld.getTypesOfGood();
						std::vector<std::string>::iterator g = goods.begin();
						while(g!=goods.end()){
						    M+=responder.getPrice(*g) * responder.getQuantity(*g);
						    N+=responder.getPrice(*g) * responder.getNeed(*g);
						    g++;
						}

						double lambda=M/N;
						responderTradeWill =  responder.getNeed(offererProducedGood)*lambda-responder.getQuantity(offererProducedGood); 
					    }
					    else if( provinceWorld.getTradeVolSelFunction() =="gintis06-outneed"){
						double M=0.0;
						double N=0.0;

						std::vector<std::string> goods= provinceWorld.getTypesOfGood();
						std::vector<std::string>::iterator g = goods.begin();
						while(g!=goods.end()){
						    M+=responder.getPrice(*g) * responder.getQuantity(*g);
						    N+=responder.getPrice(*g) * 1/responder.getPrice(*g);
						    g++;
						}

						double lambda=M/N;
						responderTradeWill =  1/responder.getPrice(offererProducedGood)*lambda-responder.getQuantity(offererProducedGood); 
					    }
					    else{
						responderTradeWill= offerer.getPrice(goodWanted)-offerer.getQuantity(goodWanted);
					    }
						responderTradCounter= responderTradeWill*responder.getPrice(offererProducedGood)/(responder.getPrice(goodWanted)); 


						if(
						  ( responderTradeWill < proposedQuantity || AlmostEqualRelative(responderTradeWill,proposedQuantity, 0.01) ) && 				//the quantity offered is at least egual to the quantity the other estim good for him
						  responderTradeWill > 0 && 				//the quantity offered is at least egual to the quantity the other estim good for him
						  requestedQuantity > 0 && 				//the quantity offered is at least egual to the quantity the other estim good for him
						  ( responder.getQuantity(goodWanted) > requestedQuantity ||AlmostEqualRelative(responder.getQuantity(goodWanted),requestedQuantity, 0.01)) &&		//the quantity asked is available in the stock of the responder
						  ( offerer.getQuantity(offererProducedGood) > proposedQuantity||AlmostEqualRelative(offerer.getQuantity(offererProducedGood),proposedQuantity, 0.01) )  &&	//the quantity proposed is available in the offerer stock
						  ( responderTradCounter > requestedQuantity ||AlmostEqualRelative(responderTradCounter,requestedQuantity, 0.01))				//the quantity asked is less that the value estimated by the responder
						  )
						{
							if(responderTradeWill<proposedQuantity){
								proposedQuantity=responderTradeWill;
								//if(requestedQuantity>responderTradCounter) requestedQuantity = responderTradCounter;
							}

							if(std::get<2>(bestTrade) <= requestedQuantity){ 
								bestTrade=std::make_tuple(*itO,requestedQuantity,proposedQuantity);
								tradeDone=1;
							}
						}else{
							//trade fail
						}


					}
					else{
						std::cout<<"ça narrive pluusuus"<<std::endl;
					}

					itO++;		    
				}
				//std::cout<<noffer<<std::endl;

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


