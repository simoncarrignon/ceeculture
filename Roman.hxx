#ifndef __Roman_hxx__
#define __Roman_hxx__

#include <Agent.hxx>
#include <Action.hxx>
#include <AgentController.hxx>

#include <string>
#include <tuple>

namespace Epnet
{

	class AgentController;

	class Roman : public Engine::Agent
	{
		int _resources; // MpiBasicAttribute

		private:
		std::vector<std::string> proposedConnections;

		void requestConnectionFrom(std::string source);
		std::vector<std::string> receivedConnections;

		std::vector<std::string> validRcvConnections;

		int ackConnectionFrom(std::string target);
		int nackConnectionFrom(std::string target);
		std::vector<std::string> validSendConnections;

		void receiveMessageFrom(std::string source, std::string msg);
		std::vector<std::tuple<std::string,std::string> > receivedMessages;

		//type, quantity, maxQuantity, price, need, productionRate
		std::vector<std::tuple<std::string,double,double,double,double,double> > listGoods;

		//type, utility, optUtility, optNeed
		std::vector<std::tuple<std::string,double,double,double> > goodsUtilities;

		int receiveGoodFrom(std::string source, std::string type, double value);


		std::vector<std::string> listProducedGoods;

		std::vector<int> _culturalNetwork;

		std::vector<int> _segSize;
		std::vector<double> _segWeight;
		std::vector<std::vector<double>> _alphas;
		std::vector<double> _optNeed;
		int _numSeg;
		double _gamma;

		double _optUtility;
		double _curUtility;


		std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades;
		std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades;
		int receiveTradeFrom(std::string source, std::string type, double value, double currency);
		void removeReceivedTrade(std::string source, std::string type, double value, double currency);
		void removeProposedTrade(std::string source, std::string type, double value, double currency);



		void consumeEssentialResources();
		void checkDeath();

		int _maxActions;
		int _nbTrades;
		double _score;
		std::string _type;
		int _size;


		AgentController* _controller;
		double _mutationRate;
		std::vector< std::string > _culturalNeighbours;

		public:
		Roman( const std::string & id, std::string controllerType );
		Roman( const std::string & id, std::string controllerType,double mutationRate,std::string selectionProcess,std::string innovationProcess,int culturalStep);
		Roman( const std::string & id, std::string controllerType,double mutationRate,std::string selectionProcess,std::string innovationProcess,int culturalStep,std::string agentType,int size);

		virtual ~Roman();

		void updateState();
		void selectActions();
		void updateKnowledge();

		void registerAttributes();
		void serialize();

		void registerGood(std::string type);
		void serializeGood(std::string type,int id);//the id is to retrieve the need from _need. 

		void setResources( int resources );
		int getResources() const;

		void resetNbTrades();
		void increaseNbTrades(int value);

		int getMaxActions();

		double getScore() {return _score;};
		void setScore(double value) {_score = value ;};
		int getSize() {return _size;};
		int setSize(int size) {_size = size;};


		void copyPriceFrom(std::string replacerId);
		void setDemand(bool opt); //set the demand for a good given the utility function from gintis 2007. If bool == true set the optimal demande.
		double setUtility(bool opt);
		std::string getType();
		void setType(std::string type);


		// setup connections
		void proposeConnectionTo(std::string target);
		void killConnectionTo(std::string target);
		std::vector<std::string> getProposedConnections() {return proposedConnections;};
		std::vector<std::string> getValidSendConnections() {return validSendConnections;};


		void acceptConnectionFrom(std::string source);
		void refuseConnectionFrom(std::string source);
		void killConnectionFrom(std::string source);
		std::vector<std::string> getReceivedConnections() {return receivedConnections;};
		std::vector<std::string> getValidRcvConnections() {return validRcvConnections;};

		void proposeConnectionBetween(std::string source, std::string target);
		void killConnectionBetween(std::string source, std::string target);

		void killConnections(std::string target);

		//message system
		void sendMessageTo(std::string target, std::string msg);
		std::vector<std::tuple<std::string,std::string> > getReceivedMessages() {return receivedMessages;};


		//good system
		void addGoodType(std::string type,double max,double price,double need,double productionRate);
		void removeGoodType(std::string type);
		std::vector<std::tuple<std::string,double,double,double,double,double> > getListGoods() { return listGoods;};
		std::tuple<double,double,double,double,double> getGood(std::string type);
		std::tuple<std::string,double,double,double,double,double> getFullGood(std::string type);


		void addGood(std::string type,double value);
		void removeGood(std::string type,double value);


		std::tuple<std::string,double,double,double,double,double>  getProducedGood();

		//acess differents values of one ressource

		double getQuantity(std::string type){ return std::get<0>(getGood(type));};
		double getPrice(std::string type){ return std::get<2>(getGood(type));};
		double getNeed(std::string type);
		double getUtility(std::string type){ return std::get<1>(getGood(type));};
		double getProductionRate(std::string type){ return std::get<4>(getGood(type));};

		double getIncome();
		//double getUtility(std::string type){ return std::get<1>(getOptGood(type));};
		//double getOptUtility(std::string type){ return std::get<2>(getOptGood(type));};
		//double getOptNeed(std::string type){ return std::get<3>(getOptGood(type));};

		void setPrice(std::string type,double value);
		void setQuantity(std::string type, double value);
		void setNeed(std::string type, double value);
		void setProductionRate(std::string type, double value);

		std::vector<std::tuple<std::string,double,double,double,double,double> >  getListGoodsFrom(std::string target);
		void printInventory();

		void initSegments();
		std::string getSegmentsProp();

		//sending goods
		void sendGoodTo(std::string target, std::string type, double value);

		//trading goods
		void proposeTradeTo(std::string target, std::string type, double valueGood, double valueCurrency);
		void acceptTradeFrom(std::string source, std::string type, double valueGood, double valueCurrency);
		void refuseTradeFrom(std::string source, std::string type, double valueGood, double valueCurrency);

		std::vector<std::tuple<std::string,std::string,double,double> > getProposedTrades() {return listProposedTrades;};
		std::vector<std::tuple<std::string,double,double> > getProposedTradesTo(std::string target);

		std::vector<std::tuple<std::string,std::string,double,double> > getReceivedTrades(){return listReceivedTrades;};
		std::vector<std::tuple<std::string,double,double> > getReceivedTradesFrom(std::string source);

		void killTradesTo(std::string source);
		void killTradesFrom(std::string source);
		void setListOfCulturalNeighbours(std::vector<std::string> neighbours );
		std::vector<std::string> getListOfCulturalNeighbours(); 
		void addCulturalNeighbour(std::string neighbour); //add a new agent in the cultural network
		std::string intToGoodType(int goodId);

		////////////////////////////////////////////////
		// This code has been automatically generated //
		/////// Please do not modify it ////////////////
		////////////////////////////////////////////////
		Roman( void * );
		void * fillPackage();
		void sendVectorAttributes(int);
		void receiveVectorAttributes(int);
		////////////////////////////////////////////////
		//////// End of generated code /////////////////
		////////////////////////////////////////////////

	};

} // namespace Epnet

#endif // __Roman_hxx__

