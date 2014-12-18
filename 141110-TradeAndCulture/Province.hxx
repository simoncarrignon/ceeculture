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

	void proposeConnection(std::string source, std::string target);
	void buildConnection(std::string source, std::string target);
	void killConnection(std::string source, std::string target);

	void proposeTwoWayConnection(std::string source, std::string target);
	void buildTwoWayConnection(std::string source, std::string target);
	void killTwoWayConnection(std::string source, std::string target);
};

} // namespace Epnet

#endif // __Province_hxx

