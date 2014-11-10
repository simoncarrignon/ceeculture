
#ifndef __TradeWorld_hxx
#define __TradeWorld_hxx

#include <World.hxx>

namespace Roman 
{

class TradeWorldConfig;

class TradeWorld : public Engine::World
{
	void createRasters();
	void createAgents();
public:
	TradeWorld(Engine::Config * config, Engine::Scheduler * scheduler = 0);
	virtual ~TradeWorld();
};

} // namespace Examples 

#endif // __RandomWorld_hxx

