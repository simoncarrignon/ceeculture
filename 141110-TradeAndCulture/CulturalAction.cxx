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


		// 		//Cultural innovation and transmission
  

		if(_selectionProcess == "random"){

			if( (Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0 > _mutationRate){

				int wsize = allAgents.size();
				int agId=Engine::GeneralState::statistics().getUniformDistValue(0,wsize) ;
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


			//Here do a "I choose a guy of same type given my proba and is probab"
			// 				std::random_shuffle(_typesOfGood.begin(),_typesOfGood.end());


			/*			for(std::vector<std::string>::iterator good = _typesOfGood.begin(); good != _typesOfGood.end();good++){
						int toGet= getNumberOfAgents()/(getTypesOfGood().size()) * .02;

						std::vector<std::string> toChange;

						int MaxTry=2*getNumberOfAgents();
						int tr=0;

			//Following Gintis 2006, we look for 2% of the best agent for each market. 
			//This is highly time consuming if only so we set a max limit and it's badly implementend (should have list of the agent sorted by the agent's score)
			while(toChange.size() < toGet && tr < MaxTry ){

			std::ostringstream oss;
			int rint = (Engine::GeneralState::statistics().getUniformDistValue(0,_agents.size()-1));
			oss << "Roman_" << rint;
			Roman * r= (Roman *) getAgent(oss.str());
			if(std::get<0>(r->getProducedGood()) == *good){
			double relScore = (r->getScore()-getMinScore(*good))/(getMaxScore(*good)-getMinScore(*good));

			if((Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX) <= (relScore) )
			toChange.push_back(r->getId());
			}
			tr++;

			}

			while(toChange.size() > 0){

			std::ostringstream oss;
			int rint = (Engine::GeneralState::statistics().getUniformDistValue(0,_agents.size()-1));
			oss << "Roman_" << rint;

			Roman * r= (Roman *) getAgent(oss.str());
			if(std::get<0>(r->getProducedGood()) == *good){

			double relScore = (r->getScore()-getMinScore(*good))/(getMaxScore(*good)-getMinScore(*good));

			if(Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX <= 1-relScore){

			std::string replaced= toChange.back();

			toChange.pop_back();
			copyPrice(replaced,r->getId()); 

			}

			}
			}*/




			std::random_shuffle(allAgents.begin(),allAgents.end());
// 

			std::vector< std::string >::iterator it = allAgents.begin();
			bool reproductionDone = 0;

			std::string producedGood =std::get<0>(romanAgent.getProducedGood());

			while(it!= allAgents.end() && !reproductionDone )
			{
				Roman & r= (Roman&)(*world->getAgent(*it));
				if(std::get<0>(r.getProducedGood()) == producedGood ){

					double relScore = (r.getScore()-provinceWorld.getMinScore(producedGood))/(provinceWorld.getMaxScore(producedGood)-provinceWorld.getMinScore(producedGood));
					double selfRelScore = (romanAgent.getScore()-provinceWorld.getMinScore(producedGood))/(provinceWorld.getMaxScore(producedGood)-provinceWorld.getMinScore(producedGood));

					// a simple cultural exchange based on my score and the score of the other agents I know
					if(relScore < selfRelScore &&  Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX < relScore){
						reproductionDone = 1;
						std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();

						for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
							std::string ressource= std::get<0>(*ot);

							romanAgent.setPrice(ressource,r.getPrice(ressource));

						}  

					}
				}
				it++;

			}

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

		//During the cultural step, there is also the mutation of the prices 
		std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();
		for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
			std::string ressource= std::get<0>(*ot);
			if(Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0 < _mutationRate)
			{

				double oldPrice = romanAgent.getPrice(ressource);
				if(_innovationProcess == "random")
					romanAgent.setPrice(ressource,(double)(Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX));//*.95
				else{

					if(Engine::GeneralState::statistics().getUniformDistValue(0,2) < 1)
						romanAgent.setPrice(ressource,oldPrice+Engine::GeneralState::statistics().getUniformDistValue(0,50)/1000.0 );//
					else
						romanAgent.setPrice(ressource,oldPrice-(Engine::GeneralState::statistics().getUniformDistValue(0,50))/1000.0);//
					if(romanAgent.getPrice(ressource)<0)romanAgent.setPrice(ressource,0.0);
				}				   
			}
			// 					setMaxScore(ressource,0.0);
			// 					setMinScore(ressource,RAND_MAX);

		}  
		romanAgent.setScore(0.0);

		}



	

	std::string CulturalAction::describe() const
	{
		return "Cultural action";
	}

} // namespace Epnet


