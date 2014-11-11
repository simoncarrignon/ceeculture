
#include <Roman.hxx>
#include <EatAction.hxx>
#include <Statistics.hxx>
#include <World.hxx>

namespace Epnet
{

Roman::Roman( const std::string & id ) : Agent(id), _resources(5)
{
}

Roman::~Roman()
{
}

void Roman::selectActions()
{
	_actions.push_back(new EatAction());
}

void Roman::updateState()
{
	if(_resources<0)
	{
		remove();
	}
}

void Roman::registerAttributes()
{
	registerIntAttribute("resources");
}

void Roman::serialize()
{
	serializeAttribute("resources", _resources);
}

void Roman::setResources( int resources )
{
	_resources = resources;
}

int Roman::getResources() const
{
	return _resources;
}

} // namespace Roman

