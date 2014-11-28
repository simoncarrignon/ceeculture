
#include <Roman.hxx>
#include <HarvestAction.hxx>
#include <Statistics.hxx>
#include <World.hxx>
#include <Logger.hxx>

namespace Epnet
{

Roman::Roman( const std::string & id ) : Agent(id), _resources(5)
{
}

Roman::~Roman()
{
}

void Roman::registerAttributes()
{
	registerFloatAttribute("good-a");
	registerFloatAttribute("good-b");
}

void Roman::serialize()
{
	serializeAttribute("good-a", (float)std::get<0>(getGood("ess-a")));
	serializeAttribute("good-b", (float)std::get<0>(getGood("ess-b")));
}

void Roman::updateState()
{
	proposeConnections();
	treatIncomingConnections();
	sendGoods();
	consumeResources();
	checkDeath();
}

void Roman::updateKnowledge()
{
}

void Roman::selectActions()
{
	_actions.push_back(new HarvestAction("ess-a",15));
	_actions.push_back(new HarvestAction("ess-b",15));
}

void Roman::consumeResources()
{
	removeGood("ess-a",2);
	removeGood("ess-b",1);
}

void Roman::checkDeath()
{
	if(std::get<0>(getGood("ess-a")) < 1)
	{
		for(auto it = _world->beginAgents() ; it != _world->endAgents() ; it++)
		{
			std::shared_ptr<Roman> romanAgent = std::dynamic_pointer_cast<Roman> (*it);
			Roman* ptrRoman = romanAgent.get();
			if(ptrRoman != this)
			{
				killConnections(ptrRoman);
			}
		}
		remove();
	}
	else if(std::get<0>(getGood("ess-b")) < 1)
	{
		for(auto it = _world->beginAgents() ; it != _world->endAgents() ; it++)
		{
			std::shared_ptr<Roman> romanAgent = std::dynamic_pointer_cast<Roman> (*it);
			Roman* ptrRoman = romanAgent.get();
			if(ptrRoman != this)
			{
				killConnections(ptrRoman);
			}
		}
		remove();
	}
}

void Roman::proposeConnections()
{
	for(auto it = _world->beginAgents() ; it != _world->endAgents() ; it++)
	{
		std::shared_ptr<Roman> romanAgent = std::dynamic_pointer_cast<Roman> (*it);
		Roman* ptrRoman = romanAgent.get();
		if(ptrRoman != this)
		{
			proposeConnectionTo(ptrRoman);
		}
	}
}

void Roman::treatIncomingConnections()
{
	std::vector<Roman*>::iterator it = receivedConnections.begin();
	while(it != receivedConnections.end())
	{
		int dice = std::rand()%100;
		if (dice <= 0)
		{
			acceptConnectionFrom(*it);
		}
		else
		{
			refuseConnectionFrom(*it);
		}
	}
	++it;
}

void Roman::sendGoods()
{
	for(auto it = validSendConnections.begin() ; it != validSendConnections.end(); ++it)
	{
		sendGoodTo((*it),"ess-a",2);
		sendGoodTo((*it),"ess-b",1);
	}
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

void Roman::proposeConnectionBetween(Roman* source, Roman* target)
{
	source->proposeConnectionTo(target);
}

void Roman::killConnectionBetween(Roman* source, Roman* target)
{
	source->killConnectionTo(target);
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

void Roman::receiveMessageFrom(Roman* source, std::string msg)
{
	if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
	{
		receivedMessages.push_back(std::make_tuple(source,msg));
	}
}

void Roman::sendMessageTo(Roman* target, std::string msg)
{
	// if the connection with the target has been valideted
	if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
	{
		target->receiveMessageFrom(this, msg);
	}
}

void Roman::addGoodType(std::string type,double max)
{
	//check if a good of that type is already in that list
	if ( std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double>& good) {return std::get<0>(good) == type;}) == listGoods.end() )
	{
		//if not, add it
		listGoods.push_back(std::make_tuple(type,0,max));
	}
}

void Roman::removeGoodType(std::string type)
{
	//check if a good of that type exist in that list
	std::vector<std::tuple<std::string,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double>& good) {return std::get<0>(good) == type;});
	while ( it != listGoods.end() )
	{
		//if not, add it
		listGoods.erase(it);
		//update the presence of good of that type in the list
		it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double>& good) {return std::get<0>(good) == type;});
	}
}

std::tuple<double,double> Roman::getGood(std::string type)
{
	//check if a good of that type exist in the list
	std::vector<std::tuple<std::string,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double>& good) {return std::get<0>(good) == type;});
	if ( it != listGoods.end() )
	{
		//return quantity and type of that good
		return std::make_tuple(std::get<1>(*it),std::get<2>(*it));
	}

	//return something impossible as an error
	return std::make_tuple(-1.0,-1.0);
}

void Roman::addGood(std::string type,double value)
{
	//check if a good of that type exists in that list
	std::vector<std::tuple<std::string,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double>& good) {return std::get<0>(good) == type;});
	//if yes add the value to it within the bound
	if ( it != listGoods.end() )
	{
		double toSet = std::min(std::get<1>(*it) + value, std::get<2>(*it));
		std::get<1>(*it) = toSet;
	}
}

void Roman::removeGood(std::string type,double value)
{
	//check if a good of that type exists in that list
	std::vector<std::tuple<std::string,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double>& good) {return std::get<0>(good) == type;});
	//if yes remove the value to it as long as it's superior to 0
	if ( it != listGoods.end() )
	{
		double toSet = std::max(std::get<1>(*it) - value, 0.0);
		std::get<1>(*it) = toSet;
	}
}

void Roman::sendGoodTo(Roman* target, std::string type, double value)
{
	// if the connection with the target has been valideted
	if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
	{
		//check if a good of that type exists in that list
		std::vector<std::tuple<std::string,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double>& good) {return std::get<0>(good) == type;});
		//if yes remove the value to it as long as it's superior to 0
		if ( it != listGoods.end() )
		{
			if ( std::get<1>(*it) >= value)
			{
				if(target->receiveGoodFrom(this, type, value))
				{
					removeGood(type,value);
				}
			}
		}
	}
}

int Roman::receiveGoodFrom(Roman* source, std::string type, double value)
{
	//if in the list of validated senders receive the good
	if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
	{
		//TODO modify add good to add a return value. return 0 if was not able to add the good (good type does not exist)
		addGood(type,value);
		return 1;
	}
	
	//if arrive to that point, the agent was not a valid sender and we return an error
	return 0;
}

} // namespace Roman

