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
	std::vector<std::string> _typesOfGood;
	std::vector<std::tuple<std::string,double>> _needs;
		
public:
	Province(Engine::Config * config, Engine::Scheduler * scheduler = 0);
	virtual ~Province();
	
	std::vector<std::string> getTypesOfGood(){return _typesOfGood;};

	void proposeConnection(std::string source, std::string target);
	void buildConnection(std::string source, std::string target);
	void killConnection(std::string source, std::string target);

	void proposeTwoWayConnection(std::string source, std::string target);
	void buildTwoWayConnection(std::string source, std::string target);
	void killTwoWayConnection(std::string source, std::string target);
};

} // namespace Epnet

#endif // __Province_hxx

