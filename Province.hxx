#ifndef __Province_hxx
#define __Province_hxx

#include <World.hxx>
#include <Roman.hxx>
#include <Network.hxx>

namespace Epnet
{

	class ProvinceConfig;

	class Province : public Engine::World
	{
		void createRasters();
		void createAgents();
		void executeTrade(Roman* r);
		std::vector<std::string> _typesOfGood;
		std::map<std::string,std::vector<std::string>> _good2Producers;
		std::map<std::string,Network> _good2CulturalNetwork;

		std::vector<std::tuple<std::string,double>> _needs;
		std::vector<std::tuple<std::string,double>> _maxscore;
		std::vector<std::tuple<std::string,double>> _minscore;
		double _maxScore;
		double _minScore;



		public:
		Province(Engine::Config * config, Engine::Scheduler * scheduler = 0);
		virtual ~Province();

		std::vector<std::string> getTypesOfGood(){return _typesOfGood;};

		//Min and max scores for each groups of producers
		double getMaxScore(std::string good);
		void setMaxScore(std::string good,double score);
		double getMinScore(std::string good);
		void setMinScore(std::string good,double score);

		///General min and max scores
		double getMaxScore(){return _maxScore;};
		void setMaxScore(double score){_maxScore = score};
		double getMinScore(){return _minScore;};
		void setMinScore(double score){_minScore = score};

		std::string getTradeType();
		double getMuMax();

		void printListOfProd(std::string s);//print all producer of the good "s"
		std::vector<std::string> getListOfProd(std::string s);
		float getMarketSize();
	 
		void proposeConnection(std::string source, std::string target);
		void buildConnection(std::string source, std::string target);
		void killConnection(std::string source, std::string target);
	
		void step();
		void stepEnvironment();
		void proposeTwoWayConnection(std::string source, std::string target);
		void buildTwoWayConnection(std::string source, std::string target);
		void killTwoWayConnection(std::string source, std::string target);
		void createCulturalNetwork();
		void printAllCulturalNerwork();
		double getNeed(std::string good);
	};

} // namespace Epnet

#endif // __Province_hxx

