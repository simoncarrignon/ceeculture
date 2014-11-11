
#ifndef __ProvinceConfig_hxx__
#define __ProvinceConfig_hxx__

#include <Config.hxx>

namespace Epnet
{

class ProvinceConfig : public Engine::Config
{	
	int _numAgents;
public:
	ProvinceConfig( const std::string & xmlFile );
	ProvinceConfig( const Engine::Size <int> & size, const int & numSteps );
	virtual ~ProvinceConfig();

	void loadParams();

	friend class Province;
};

} // namespace Epnet

#endif // __ProvinceConfig_hxx__

