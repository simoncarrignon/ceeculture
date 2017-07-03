#ifndef __ProvinceConfig_hxx__
#define __ProvinceConfig_hxx__

#include <Config.hxx>
#include <vector>
#include <tuple>
#include <map>

namespace Epnet
{

class ProvinceConfig : public Engine::Config
{	
	int _numAgents;

	int _numGoods;
	std::vector<std::tuple<std::string,double,double,double,double,double> > _paramGoods;

	int _numRasters;
	std::vector<std::tuple<std::string,double,double,double> > _paramRasters;
	
	std::string _controllerType;
	std::string _controllerRandom;
	std::tuple<std::string,double,double,double,double,double>  _protoGood;
	std::string _selectionProcess;
	int _culturalStep;
	std::string _goodsParam;
	std::string _events;
	int _eventsRate;
	std::string _networkType;
	std::string _networkOut;
	std::string _aType;
	std::map<std::string,double> _networkParam;
	float _mutationRate;
	std::string _innovationProcess;
	float _marketSize;
	std::string _tradeType;
	float _muMax;

public:
	ProvinceConfig( const std::string & xmlFile );
	ProvinceConfig( const Engine::Size <int> & size, const int & numSteps );
	virtual ~ProvinceConfig();

	void loadParams();

	friend class Province;
};

} // namespace Epnet

#endif // __ProvinceConfig_hxx__

