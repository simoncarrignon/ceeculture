
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

