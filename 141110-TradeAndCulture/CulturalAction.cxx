#include <CulturalAction.hxx>

#include <World.hxx>
#include <Province.hxx>

#include <Logger.hxx>

namespace Epnet
{

CulturalAction::CulturalAction()
{
}

CulturalAction::~CulturalAction()
{
}

void CulturalAction::execute(Engine::Agent& agent)
{
	Roman & romanAgent = (Roman&)agent;
	Engine::World * world = agent.getWorld();
	Province & provinceWorld = (Province&) *world;
	
	  std::vector< std::string > allAgents = romanAgent.getValidRcvConnections();
	   std::vector< std::string >::iterator it = allAgents.begin();
	bool reproductionDone = 0;
	   while(it!= allAgents.end() && !reproductionDone)
	{
	  Roman & r= (Roman&)(*world->getAgent(*it));
	  if(std::get<0>(r.getProducedGood()) == std::get<0>(romanAgent.getProducedGood()) && std::rand()%100 < 2 && romanAgent.getScore()<r.getScore() && r.getScore()<10 ){
	      reproductionDone = 1;
		std::cout<<"mutation "<<std::endl;
	   std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();


	   for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
	   	       std::string ressource= std::get<0>(*ot);

	     romanAgent.setPrice(ressource,r.getPrice(ressource));
	      	    
	  }  
	

	  }
	    it++;
	}
	   std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();
	for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
	  if(std::rand()%100 < 1){
		std::cout<<std::rand()%100 <<" a lilation "<<std::endl;
	    
	       std::string ressource= std::get<0>(*ot);
	    double oldPrice = romanAgent.getPrice(ressource);
	    
	    if(std::rand()%2 < 1)
	      romanAgent.setPrice(ressource,oldPrice*.95);
	    else
	      romanAgent.setPrice(ressource,oldPrice/.95);
	      	    
	  }  
	}
	

}

std::string CulturalAction::describe() const
{
	return "Cultural action";
}

} // namespace Epnet


