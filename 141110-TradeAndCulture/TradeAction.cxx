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
	Roman & romanAgent = (Roman&)agent;
	Engine::World * world = agent.getWorld();
	Province & provinceWorld = (Province&) *world;
	
	
	
	 std::vector<std::string> gto= provinceWorld.getTypesOfGood();
	
	 
	 std::vector<std::string>::iterator  it = gto.begin();

//	 romanAgent.printInventory();
	 
	 //For all type of ressource
	 while(it != gto.end()){
	      
	   if(romanAgent.getProductionRate(*it)== 0.0){
		    std::string goodWanted = *it;
		    
		    std::vector< std::string > mara =romanAgent.getValidRcvConnections();
		    std::vector< std::string >::iterator itO=mara.begin();
		    std::tuple< std::string, double, double, double, double, double > goodProduced = romanAgent.getProducedGood(); 

		    //qt is the quantity that of is own produced good the agent is willing to exchange
		    double qt = .5*std::get<1>(goodProduced);

		    while(itO != mara.end() && 1==1) //TODO:number of max test <nbmax (maybe gintis use it only because of computational limitiation)
		    {
		      romanAgent.sendGoodTo(*itO,*it,qt);
//		      std::cout<<"agent:"<<romanAgent.getId()<<" send "<<qt<<" of "<<std::get<0>(goodProduced)<<" to "<<*itO<<std::endl;
		      itO++;
		    }
	  }

	  it++;
	}
	

	
//	romanAgent.printInventory();

}

std::string TradeAction::describe() const
{
	return "Trade action";
}

} // namespace Epnet


