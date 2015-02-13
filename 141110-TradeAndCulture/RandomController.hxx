#ifndef __RandomController_hxx__
#define __RandomController_hxx__

#include <string>
#include <tuple>

namespace Epnet
{

class RandomController
{
public:
	RandomController();
	virtual ~RandomController();
	
	virtual void updateState();
	virtual void selectActions();
	virtual void updateKnowledge();

	virtual void treatIncomingConnections();
	virtual void treatIncomingTrades();
};

} // namespace Epnet

#endif // __RandomController_hxx__

