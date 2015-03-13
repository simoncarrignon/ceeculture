#include <CulturalAction.hxx>

#include <World.hxx>
#include <Province.hxx>

#include <Logger.hxx>

namespace Epnet
{

	CulturalAction::CulturalAction()
	{
	}

	
	CulturalAction::CulturalAction(double mutationRate,std::string selectionProcess,std::string innovationProcess)
	{
	  
	  _mutationRate = mutationRate;
	  _selectionProcess = selectionProcess;
	  _innovationProcess = innovationProcess;
	  
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


		if(_selectionProcess == "random"){

		if( (std::rand()%1000)/1000.0 > _mutationRate){

				int wsize = allAgents.size();
				int agId=std::rand()%wsize ;
				std::string rId = allAgents[agId];

				Roman & r= (Roman&)(*world->getAgent(rId));

				std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();


				for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
					std::string ressource= std::get<0>(*ot);

					romanAgent.setPrice(ressource,r.getPrice(ressource));
				}
			}
			
		}
		else{
			std::vector< std::string >::iterator it = allAgents.begin();
			bool reproductionDone = 0;
			if(std::rand()%100 < 20){
				double max_score=0.0;	
				while(it!= allAgents.end())
				{
					Roman & r= (Roman&)(*world->getAgent(*it));
					if(r.getScore()>max_score)max_score=r.getScore();
					it++;
				}
				std::vector< std::string >::iterator it = allAgents.begin();
// 
				
				while(it!= allAgents.end() && !reproductionDone )
				{
					Roman & r= (Roman&)(*world->getAgent(*it));


					//std::cout<<"testscore: "<< r.getScore()/max_score<< ", agent score:" <<(1-romanAgent.getScore()/max_score)<<std::endl;
					//std::cout<<"good"<<std::get<0>(r.getProducedGood())<<" good "<<std::get<0>(romanAgent.getProducedGood())<<std::endl;
					if(std::get<0>(r.getProducedGood()) == std::get<0>(romanAgent.getProducedGood()) && romanAgent.getScore()<r.getScore() && (std::rand()%100)/100.0<r.getScore()/max_score && (std::rand()%100)/100.0 <(1-romanAgent.getScore()/max_score) ){
						reproductionDone = 1;
						std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();

						// 						std::cout<<" done"<<std::endl;

						for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
							std::string ressource= std::get<0>(*ot);

							romanAgent.setPrice(ressource,r.getPrice(ressource));

						}  


					}
					it++;
				}
			}
		}
		
		  
			std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();
			for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
				if((std::rand()%1000)/1000.0 < _mutationRate)
				{
				  
				std::string ressource= std::get<0>(*ot);
				double oldPrice = romanAgent.getPrice(ressource);
				 if(_innovationProcess == "random")
					romanAgent.setPrice(ressource,(double)(std::rand()%RAND_MAX)/RAND_MAX);//*.95
				  else{

				    if(std::rand()%2 < 1)
						romanAgent.setPrice(ressource,oldPrice*.95);//
					else
						romanAgent.setPrice(ressource,oldPrice/.95);//
				 }				   
				}
			}  




	}

	std::string CulturalAction::describe() const
	{
		return "Cultural action";
	}

} // namespace Epnet


