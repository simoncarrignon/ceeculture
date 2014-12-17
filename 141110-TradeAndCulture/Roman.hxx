
#ifndef __Roman_hxx__
#define __Roman_hxx__

#include <Agent.hxx>
#include <Action.hxx>

#include <string>
#include <tuple>

namespace Epnet
{

class Roman : public Engine::Agent
{
	int _resources; // MpiBasicAttribute

private:
	std::vector<Roman*> proposedConnections;

	void requestConnectionFrom(Roman* source);
	std::vector<Roman*> receivedConnections;

	std::vector<Roman*> validRcvConnections;

	int ackConnectionFrom(Roman* target);
	int nackConnectionFrom(Roman* target);
	std::vector<Roman*> validSendConnections;

	void receiveMessageFrom(Roman* source, std::string msg);
	std::vector<std::tuple<Roman*,std::string> > receivedMessages;

	std::vector<std::tuple<std::string,double,double> > listGoods;
	int receiveGoodFrom(Roman* source, std::string type, double value);

	std::vector<std::tuple<Roman*,std::string,double,double> > listReceivedTrades;
	std::vector<std::tuple<Roman*,std::string,double,double> > listProposedTrades;
	int receiveTradeFrom(Roman* source, std::string type, double value, double currency);
	void removeReceivedTrade(Roman* source, std::string type, double value, double currency);
	void removeProposedTrade(Roman* source, std::string type, double value, double currency);

	void consumeResources();
	void treatIncomingConnections();
	void treatIncomingTrades();
	void checkDeath();

	int _maxActions;

public:
	// todo remove environment from here
	Roman( const std::string & id );
	virtual ~Roman();
	
	void updateState();
	void selectActions();
	void updateKnowledge();

	void registerAttributes();
	void serialize();

	void setResources( int resources );
	int getResources() const;


	// setup connections
	void proposeConnectionTo(Roman* target);
	void killConnectionTo(Roman* target);
	std::vector<Roman*> getProposedConnections() {return proposedConnections;};
	std::vector<Roman*> getValidSendConnections() {return validSendConnections;};


	void acceptConnectionFrom(Roman* source);
	void refuseConnectionFrom(Roman* source);
	void killConnectionFrom(Roman* source);
	std::vector<Roman*> getReceivedConnections() {return receivedConnections;};
	std::vector<Roman*> getValidRcvConnections() {return validRcvConnections;};

	void killConnections(Roman *target);

	void proposeConnectionBetween(Roman *source, Roman * target);
	void killConnectionBetween(Roman *source, Roman * target);

	//message system
	void sendMessageTo(Roman *target, std::string msg);
	std::vector<std::tuple<Roman*,std::string> > getReceivedMessages() {return receivedMessages;};


	//good system
	void addGoodType(std::string type,double max);
	void removeGoodType(std::string type);
	std::vector<std::tuple<std::string,double,double> > getListGoods() { return listGoods;};
	std::tuple<double,double> getGood(std::string type);

	void addGood(std::string type,double value);
	void removeGood(std::string type,double value);

	std::vector<std::tuple<std::string,double,double> >  getListGoodsFrom(Roman* target);

	//sending goods
	void sendGoodTo(Roman* target, std::string type, double value);

	//trading goods
	void proposeTradeTo(Roman* target, std::string type, double valueGood, double valueCurrency);
	void acceptTradeFrom(Roman* source, std::string type, double valueGood, double valueCurrency);
	void refuseTradeFrom(Roman* source, std::string type, double valueGood, double valueCurrency);
	
	std::vector<std::tuple<Roman*,std::string,double,double> > getProposedTrades() {return listProposedTrades;};
	std::vector<std::tuple<std::string,double,double> > getProposedTradesTo(Roman* target);

	std::vector<std::tuple<Roman*,std::string,double,double> > getReceivedTrades(){return listReceivedTrades;};
	std::vector<std::tuple<std::string,double,double> > getReceivedTradesFrom(Roman* source);

	void killTradeFrom(Roman* source);

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

