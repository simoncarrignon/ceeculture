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
	void executeTrade(Roman* r);
	std::vector<std::string> _typesOfGood;
	std::vector<std::tuple<std::string,double>> _needs;
	double _maxscore;
	
		
public:
	Province(Engine::Config * config, Engine::Scheduler * scheduler = 0);
	virtual ~Province();
	
	std::vector<std::string> getTypesOfGood(){return _typesOfGood;};
	double getMaxScore(){return _maxscore;};
	void setMaxScore(double score){_maxscore = score;};
	
	
	void proposeConnection(std::string source, std::string target);
	void buildConnection(std::string source, std::string target);
	void killConnection(std::string source, std::string target);
	virtual void step();
	void proposeTwoWayConnection(std::string source, std::string target);
	void buildTwoWayConnection(std::string source, std::string target);
	void killTwoWayConnection(std::string source, std::string target);
};

} // namespace Epnet

#endif // __Province_hxx

