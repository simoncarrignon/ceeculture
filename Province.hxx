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
		std::vector<std::tuple<std::string,int>> _nbProds;
		std::vector<std::tuple<std::string,double>> _maxscore;
		std::vector<std::tuple<std::string,double>> _minscore;
		double _maxScore;
		double _minScore;
		int _totPopSize; //this is usefull is Roman Agent represent more than one people



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
		void setMaxScore(double score){_maxScore = score;};
		double getMinScore(){return _minScore;};
		void setMinScore(double score){_minScore = score;};

		double getCopyRate();
		double getTotPopSize(){return _totPopSize;};
		void setTotPopSize(double score){_totPopSize = score;};

		std::string getTradeUtilFunction();
		std::string getTradeVolSelFunction();
		double getMuMax();

		void printListOfProd(std::string s);//print all producer of the good __s__
		std::vector<std::string> getListOfProd(std::string s);
		float getMarketSize();
	 
		double getRatio(std::string pgood);
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
		void normalizeNeeds();
		bool hasEvent(); //return true if event that will change the number of goods can occurs
		bool logTrade(); //return true if a log of the trade has to be done
		void removeFromListOfProd(std::string agent,std::string good);//remove the agent __agent__ from the list of producers of the good __good__ 

		int getASize(); //return a integer give the distrib parameter of the config file
		bool isPopSize(); //return true if pop size is taken into account, false else
	};

} // namespace Epnet

#endif // __Province_hxx

