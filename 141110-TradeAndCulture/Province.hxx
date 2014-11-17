
#ifndef __Province_hxx
#define __Province_hxx

#include <World.hxx>
#include <Roman.hxx>

namespace Epnet
{

class ProvinceConfig;

class Province : public Engine::World
{
	void createRasters();
	void createAgents();

public:
	Province(Engine::Config * config, Engine::Scheduler * scheduler = 0);
	virtual ~Province();

	void proposeConnection(Roman* source, Roman* target);
	void buildConnection(Roman* source, Roman* target);
	void killConnection(Roman* source, Roman* target);

	void proposeTwoWayConnection(Roman* source, Roman* target);
	void buildTwoWayConnection(Roman* source, Roman* target);
	void killTwoWayConnection(Roman* source, Roman* target);
};

} // namespace Epnet

#endif // __Province_hxx

