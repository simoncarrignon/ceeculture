
#ifndef __Roman_hxx__
#define __Roman_hxx__

#include <Agent.hxx>
#include <Action.hxx>

#include <string>

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
	std::vector<std::string> receivedMessages;

public:
	// todo remove environment from here
	Roman( const std::string & id );
	virtual ~Roman();
	
	void selectActions();
	void updateState();
	void registerAttributes();
	void serialize();

	void setResources( int resources );
	int getResources() const;

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

	void sendMessageTo(Roman *target, std::string msg);
	std::vector<std::string> getReceivedMessages() {return receivedMessages;};

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

