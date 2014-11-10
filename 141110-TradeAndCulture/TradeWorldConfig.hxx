
#ifndef __TradeWorldConfig_hxx__
#define __TradeWorldConfig_hxx__

#include <Config.hxx>

namespace Roman
{

class TradeWorldConfig : public Engine::Config
{	
	int _numAgents;
public:
	TradeWorldConfig( const std::string & xmlFile );
	virtual ~TradeWorldConfig();

	void loadParams();

	friend class TradeWorld;
};

} // namespace Roman

#endif // __TradeWorldConfig_hxx__

