#include <ControllerFactory.hxx>
#include <RandomController.hxx>

namespace Epnet
{

	ControllerFactory* ControllerFactory::_instance = 0;

	ControllerFactory::ControllerFactory()
	{
	}

	ControllerFactory::~ControllerFactory()
	{
	}

	ControllerFactory& ControllerFactory::get()
	{
		if(!_instance)
		{
			_instance = new ControllerFactory;
		}
		return *_instance;
	}

	AgentController* ControllerFactory::makeController(const std::string &controllerName)
	{
		if (controllerName == "random")
		{
			return new RandomController();
		}
	}

} // namespace Epnet
