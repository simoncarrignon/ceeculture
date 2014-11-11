
#ifndef __Province_hxx
#define __Province_hxx

#include <World.hxx>

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
};

} // namespace Epnet

#endif // __Province_hxx

