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
	std::string _typeProd;
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
	bool _eventsHistory;
	int _eventsRate;
	int _eventsStop;
	std::string _networkType;
	std::string _networkOut;
	std::string _aType;
	std::map<std::string,double> _networkParam;
	float _mutationRate;
	float _copyRate;
	std::string _innovationProcess;
	float _marketSize;
	std::string _tradeUtilFunction; //what kind of utility function will be used to trade
	std::string _tradeVolSelFunction; //what kind of function will be sued to select the amount of good wanted
	float _muMax; //range of the mutations
	std::vector<std::tuple<std::string,int>> _nbProds; //total number of producters for each products of the simulation
	std::map<const std::string,int> _totNbProds;
	std::map<const std::string,std::tuple<float,float>> _historicalSchedule; //total number of producters for each products of the simulation
	std::string _logTrade; //a string that says if a log of all the trade (amount, kind, why they failed) should be printed

	std::string _distrib;//how population size should be distributed
	float _plMax;//param for opo size distribution
	float _plMin;
	float _plAlpha;
	std::string _distribUse;

public:
	ProvinceConfig( const std::string & xmlFile );
	ProvinceConfig( const Engine::Size <int> & size, const int & numSteps );
	virtual ~ProvinceConfig();

	void loadParams();

	friend class Province;
};

} // namespace Epnet

#endif // __ProvinceConfig_hxx__

