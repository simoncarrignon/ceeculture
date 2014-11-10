
#include <TradeWorldConfig.hxx>

namespace Roman
{

TradeWorldConfig::TradeWorldConfig( const std::string & xmlFile ) : Config(xmlFile), _numAgents(0)
{
}

TradeWorldConfig::~TradeWorldConfig()
{
}

void TradeWorldConfig::loadParams()
{
	_numAgents = getParamInt( "numAgents", "value");
}
	
} // namespace Roman

