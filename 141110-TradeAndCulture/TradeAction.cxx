#include <TradeAction.hxx>

#include <World.hxx>
#include <Province.hxx>

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
	Roman & offerer = (Roman&)agent;
	Engine::World * world = agent.getWorld();
	Province & provinceWorld = (Province&) *world;
	
	
	
	 std::vector<std::string> gto= provinceWorld.getTypesOfGood();
	
	 
	 std::vector<std::string>::iterator  it = gto.begin();

	 //offerer.printInventory();
	  std::string offererProducedGood= std::get<0>(offerer.getProducedGood()); 
	 //For all type of ressource
	 while(it != gto.end()){	
		    offerer.printInventory();

	   
		    std::string goodWanted = *it;
		    
		    std::vector< std::string > mara =offerer.getValidRcvConnections();
		    std::vector< std::string >::iterator itO=mara.begin();
		   
		    
		    std::tuple< std::string, double, double> bestTrade=std::make_tuple<std::string,double,double>(std::string(""),0,0);

		    //qt is the quantity that of is own produced good the agent is willing to exchange
		    double requestedQuantity= (offerer.getNeed(goodWanted)-offerer.getQuantity(goodWanted));
		    double proposedQuantity = (offerer.getPrice(goodWanted)/offerer.getPrice(offererProducedGood))*requestedQuantity;
		    
		    while(itO != mara.end() && 1==1) //TODO:number of max test <nbmax (maybe gintis use it only because of computational limitiation)
		    {
		      Roman & responder = (Roman&)(*world->getAgent(*itO));
		      
		      std::string responderProducedGood = std::get<0>(responder.getProducedGood());
		      
		      
		      if(responderProducedGood == goodWanted){ 


			
			      if( responder.getNeed(offererProducedGood) <= proposedQuantity && 
				( proposedQuantity*responder.getPrice(offererProducedGood) <= requestedQuantity*responder.getPrice(goodWanted)) &&
				responder.getQuantity(goodWanted) >= requestedQuantity && offerer.getQuantity(offererProducedGood) > proposedQuantity )
			      {
				if(std::get<2>(bestTrade) < requestedQuantity) bestTrade=std::make_tuple(*itO,requestedQuantity,proposedQuantity);
				//break; à sérieusemen envisager. Implémente le "offerer leaves the market." de gintis p. 6 et permet d'eviter pas mal de boucle inutile
			      }
			
		      }
		      itO++;		    
		  }
		  
		  if(std::get<0>(bestTrade) != ""){
		    //The usage of a best trade tuple have sense only if users could exchange less than Requested quantity. 
		    //This would imply that an offerer will check each person in the market and trade with each one of them and keep the best offerer
		    //but on better things is to actually DO the trade EVERY time till the offerer had all he want.
		    std::cout<<"best trade w/"<< std::get<0>(bestTrade)<< " de "<<offererProducedGood<<" get :"<< std::get<1>(bestTrade)<<" give :"<<std::get<2>(bestTrade)<<std::endl;; 
		    
		    offerer.setQuantity(goodWanted,std::get<1>(bestTrade));		    
		    offerer.setQuantity(offererProducedGood,offerer.getQuantity(offererProducedGood)-std::get<2>(bestTrade));

		    Roman & responder = (Roman&)(*world->getAgent(std::get<0>(bestTrade)));
		    responder.setQuantity(offererProducedGood,std::get<2>(bestTrade));
    		    responder.setQuantity(goodWanted,responder.getQuantity(goodWanted)-std::get<1>(bestTrade));

		    offerer.printInventory();

		  }
		  else{
		    std::cout<<"nop"<< std::endl;
		    
		  }


	  it++;
	}
	

	
	
}

std::string TradeAction::describe() const
{
	return "Trade action";
}

} // namespace Epnet


