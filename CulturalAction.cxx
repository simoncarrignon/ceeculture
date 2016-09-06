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
  
// 		std::cout<<"network size"<<allAgents.size()<<std::endl;
// 		for(int i = 0 ; i< allAgents.size();i++)
// 		    std::cout<<allAgents[i]<<std::endl;
// 		std::cout<<"---"<<std::endl;


		// Cultural innovation and transmission


		if(_selectionProcess == "random"){

			if( (Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0 > _mutationRate){

				int wsize = allAgents.size();
				int agId=Engine::GeneralState::statistics().getUniformDistValue(0,wsize-1) ;
				std::string rId = allAgents[agId];

				romanAgent.copyPriceFrom(rId);

			}

		}
		else{

			//The actual cultural exchange done when trade is on is done as follow: Each roman will look at the other roman 
			std::vector< std::string > nb=romanAgent.getListOfCulturalNeighbours();
			std::random_shuffle(nb.begin(),nb.end());
			std::vector< std::string >::iterator it = nb.begin();
			bool reproductionDone = 0;

			std::string producedGood =std::get<0>(romanAgent.getProducedGood());

//  			std::cout<<romanAgent.getId()<<", "<<producedGood<<"will try to copy:"<<std::endl;
			while(it!= nb.end() && !reproductionDone )
			{
				
				if(*it != romanAgent.getId()){//this limitation should appears given how the cultural networks are done
					
					Roman & r= (Roman&)(*world->getAgent(*it));
//  					std::cout<<"\t "<<r.getId()<<" that "<<std::get<0>(r.getProducedGood())<<std::endl;
					
					//should be good : they are all from same producedgood
					//moreover this limit should be removed in an ideal case where every body can copy price of everybody
					if(std::get<0>(r.getProducedGood()) == producedGood ){
						
						double relScore = (r.getScore()-provinceWorld.getMinScore(producedGood))/(provinceWorld.getMaxScore(producedGood)-provinceWorld.getMinScore(producedGood));
						double selfRelScore = (romanAgent.getScore()-provinceWorld.getMinScore(producedGood))/(provinceWorld.getMaxScore(producedGood)-provinceWorld.getMinScore(producedGood));
						
						// if(Engine::GeneralState::statistics().getUniformDistValue(0,1000)/(double)1000< _mutationRate){
						// a simple cultural exchange based on my score and the score of the other agents I know
						if(relScore < selfRelScore &&  Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX < relScore){
							reproductionDone = 1;
							romanAgent.copyPriceFrom(r.getId());
							
						}
						// 					}
					}
				}
				it++;
				
				
			}
//  			std::cout<<"----"<<std::endl;

		}

		//gintis way of mutate
		// 			std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();
		// 			for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
		// 				if((Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0 < _mutationRate)
		// 				{
		// 				  
		// 				std::string ressource= std::get<0>(*ot);
		// 				double oldPrice = romanAgent.getPrice(ressource);
		// 				 if(_innovationProcess == "random")
		// 					romanAgent.setPrice(ressource,(double)(Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX));//*.95
		// 				  else{
		// 
		// 				    if(Engine::GeneralState::statistics().getUniformDistValue(0,2) < 1)
		// 						romanAgent.setPrice(ressource,oldPrice*.95);//
		// 					else
		// 						romanAgent.setPrice(ressource,oldPrice/.95);//
		// 				 }				   
		// 				}
		// 			}  

		//After the cultural copy, price are randomly mutated (the innovation process)
		std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();
		for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
			std::string ressource= std::get<0>(*ot);
			if(Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0 < _mutationRate)
			{

				double oldPrice = romanAgent.getPrice(ressource);
				if(_innovationProcess == "random")
					romanAgent.setPrice(ressource,(double)(Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX));
				else{

					if(Engine::GeneralState::statistics().getUniformDistValue(0,2) < 1)
						romanAgent.setPrice(ressource,oldPrice+Engine::GeneralState::statistics().getUniformDistValue(0,50)/1000.0 );
					else
						romanAgent.setPrice(ressource,oldPrice-(Engine::GeneralState::statistics().getUniformDistValue(0,50))/1000.0);
					if(romanAgent.getPrice(ressource)<0)romanAgent.setPrice(ressource,0.0);
				}				   
			}

		}  
		romanAgent.setScore(0.0);

	}





	std::string CulturalAction::describe() const
	{
		return "Cultural action";
	}

} // namespace Epnet


