#ifndef __BasicController_hxx__
#define __BasicController_hxx__

#include <AgentController.hxx>

namespace Epnet
{

	/*-----------------------------------------------------------------------------
	 *  
	 *-----------------------------------------------------------------------------*/
class BasicController : public AgentController
{
  
public:
	BasicController();
	virtual ~BasicController();
	
	virtual void updateState();
	virtual std::list<Engine::Action*> selectActions();
	virtual void updateKnowledge();

	virtual void treatIncomingConnections();
	virtual void treatIncomingTrades();
	double getScore();
  
};

} // namespace Epnet

#endif // __BasicController_hxx__

