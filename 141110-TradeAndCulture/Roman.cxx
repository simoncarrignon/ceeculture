
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

void Roman::proposeConnectionTo(Roman* target)
{
	//if the connection is not already among the valid one, ask for it
	if( std::find(validSendConnections.begin(), validSendConnections.end(), target) == validSendConnections.end() )
	{
		target->requestConnectionFrom(this);
		//if the connection is not already in the proposed one, add it
		if( std::find(proposedConnections.begin(), proposedConnections.end(), target) == proposedConnections.end() )
		{
			proposedConnections.push_back(target);
		}
	}
}

void Roman::killConnectionTo(Roman* target)
{
	bool wasPresent = false;
	//remove traces from the proposedConnections
	if( std::find(proposedConnections.begin(), proposedConnections.end(), target) != proposedConnections.end() )
	{
		proposedConnections.erase(std::remove(proposedConnections.begin(), proposedConnections.end(), target), proposedConnections.end());
		wasPresent = true;
	}

	//remove trances from valid connections
	if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
	{
		validSendConnections.erase(std::remove(validSendConnections.begin(), validSendConnections.end(), target), validSendConnections.end());
		wasPresent = true;
	}

	//if the target was present there is a chance that we are in its lists
	if (wasPresent == true)
	{
		target->killConnectionFrom(this);
	}
}

void Roman::killConnectionFrom(Roman* source)
{
	bool wasPresent = false;
	//remove traces from the receivedConnections
	if( std::find(receivedConnections.begin(), receivedConnections.end(), source) != receivedConnections.end() )
	{
		receivedConnections.erase(std::remove(receivedConnections.begin(), receivedConnections.end(), source), receivedConnections.end());
		wasPresent = true;
	}

	//remove trances from valid connections
	if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
	{
		validRcvConnections.erase(std::remove(validRcvConnections.begin(), validRcvConnections.end(), source), validRcvConnections.end());
		wasPresent = true;
	}

	//if the source was present there is a chance that we are in its lists
	if (wasPresent == true)
	{
		source->killConnectionTo(this);
	}
}

void Roman::killConnections(Roman* target)
{
	killConnectionTo(target);
	killConnectionFrom(target);
}

void Roman::requestConnectionFrom(Roman* source)
{
	//if the connection has not been received, put it among the received ones
	if( std::find(receivedConnections.begin(), receivedConnections.end(), source) == receivedConnections.end() )
	{
		receivedConnections.push_back(source);
	}
}

void Roman::acceptConnectionFrom(Roman* source)
{
	//if the source has not been already validated
	if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) == validRcvConnections.end() )
	{
		//if the source is effectively in the received connection
		if( std::find(receivedConnections.begin(), receivedConnections.end(), source) != receivedConnections.end() )
		{
			receivedConnections.erase(std::remove(receivedConnections.begin(), receivedConnections.end(), source), receivedConnections.end());
			//confirm the connection to source, and verify that source is aware of it
			if (source->ackConnectionFrom(this))
			{
				validRcvConnections.push_back(source);
			}
		}
		//if the source is not in the lit of received, something went wrong, kill things
		else
		{
			source->killConnectionTo(this);
		}
	}
}

void Roman::refuseConnectionFrom(Roman* source)
{
	//refuse connection from source
	source->nackConnectionFrom(this);
	//remove source
	receivedConnections.erase(std::remove(receivedConnections.begin(), receivedConnections.end(), source), receivedConnections.end());
}

int Roman::ackConnectionFrom(Roman* target)
{
	//if we had sent a connection we acknowledge it, otherwise we just return an error
	if( std::find(proposedConnections.begin(), proposedConnections.end(), target) != proposedConnections.end() )
	{
		proposedConnections.erase(std::remove(proposedConnections.begin(), proposedConnections.end(), target), proposedConnections.end());
		validSendConnections.push_back(target);
		return 1;
	}
	else
	{
		return 0;
	}
}

int Roman::nackConnectionFrom(Roman* target)
{
	//if we had sent a connection we kill it, otherwise we just return an error
	if( std::find(proposedConnections.begin(), proposedConnections.end(), target) != proposedConnections.end() )
	{
		proposedConnections.erase(std::remove(proposedConnections.begin(), proposedConnections.end(), target), proposedConnections.end());
		return 1;
	}
	else
	{
		return 0;
	}
}

} // namespace Roman

