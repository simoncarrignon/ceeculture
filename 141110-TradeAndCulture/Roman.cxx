#include <Roman.hxx>
#include <HarvestAction.hxx>
#include <ProposeConnectionAction.hxx>
#include <SendGoodsAction.hxx>
#include <ProposeTradeAction.hxx>
#include <FunAction.hxx>
#include <Statistics.hxx>
#include <World.hxx>
#include <Logger.hxx>

namespace Epnet
{

Roman::Roman( const std::string & id ) : Agent(id), _resources(5), _maxActions(20), _nbTrades(0)
{
}

Roman::~Roman()
{
}

void Roman::registerAttributes()
{
	registerFloatAttribute("ess-a");
	registerFloatAttribute("ess-b");
	registerFloatAttribute("nonEss-a");
	registerFloatAttribute("nonEss-b");
	registerFloatAttribute("currency");
	registerIntAttribute("nbConnectionsRcv");
	registerIntAttribute("nbConnectionsSend");
	registerIntAttribute("nbAchievedTrades");
}

void Roman::serialize()
{
	serializeAttribute("ess-a", (float)std::get<0>(getGood("ess-a")));
	serializeAttribute("ess-b", (float)std::get<0>(getGood("ess-b")));
	serializeAttribute("nonEss-a", (float)std::get<0>(getGood("nonEss-a")));
	serializeAttribute("nonEss-b", (float)std::get<0>(getGood("nonEss-b")));
	serializeAttribute("currency", (float)std::get<0>(getGood("currency")));
	serializeAttribute("nbConnectionsRcv", (int) validRcvConnections.size());
	serializeAttribute("nbConnectionsSend", (int) validSendConnections.size());
	serializeAttribute("nbAchievedTrades", _nbTrades);
}



void Roman::updateKnowledge()
{
}

void Roman::selectActions()
{
	/*
	int action = _maxActions;
	while (action >=0)
	{
		int dice = std::rand()%7;
		switch (dice)
		{
			case 0:
				_actions.push_back(new HarvestAction("ess-a",1));
				action--;
				break;

			case 1:
				_actions.push_back(new HarvestAction("ess-b",1));
				action--;
				break;

			case 2:
				_actions.push_back(new HarvestAction("nonEss-a",1));
				action--;
				break;

			case 3:
				_actions.push_back(new HarvestAction("nonEss-b",1));
				action--;
				break;

			case 4:
				for(auto it = _world->beginAgents() ; it != _world->endAgents() ; it++)
				{
					std::string roman_id = (*it)->getId();;
					if(roman_id != _id)
					{
						int dice2 = std::rand()%100;
						if(dice2 < 80)
						{
							_actions.push_back(new ProposeConnectionAction(roman_id));
							action--;
						}
					}
				}
				break;

			case 5:
				if(validSendConnections.size() > 0)
				{
					int target = std::rand()%validSendConnections.size();
					static const std::string types[] = {"ess-a","ess-b","nonEss-a","nonEss-b"};
					std::string type = types[std::rand()%4];
					int quantity = std::rand()%10;
					int currency = std::rand()%50;
					_actions.push_back(new ProposeTradeAction(validSendConnections[target],type,quantity,currency));
					action --;
				}
				break;

			case 6:
				_actions.push_back(new FunAction("nonEss-a",1));
				_actions.push_back(new FunAction("nonEss-b",2));
				action --;
				break;
		}
	}
	*/
}

void Roman::updateState()
{
	treatIncomingConnections();
	treatIncomingTrades();
	consumeResources();
	checkDeath();
}





void Roman::consumeResources()
{
	removeGood("ess-a",1);
	removeGood("ess-b",1);
}

void Roman::checkDeath()
{
	if( (std::get<0>(getGood("ess-a")) < 1) or (std::get<0>(getGood("ess-b")) < 1) )
	{
		while(validSendConnections.size() > 0)
		{
			std::vector<std::string>::iterator it = validSendConnections.begin();

			Roman* targetPtr = dynamic_cast<Roman*> (_world->getAgent(*it));
			if (targetPtr == NULL)
			{
				std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
				exit(1);
			}
			//std::cout << "kill trades" << std::endl;
			targetPtr->killTradesFrom(_id);
			killConnectionTo(*it);
			//std::cout << "done" << std::endl;
		}

		//clean the agents in unfinished connections
		while (receivedConnections.size() > 0)
		{
			std::vector<std::string>::iterator it = receivedConnections.begin();
			killConnectionFrom(*it);
		}
		
		while (proposedConnections.size() > 0)
		{
			std::vector<std::string>::iterator it = proposedConnections.begin();
			killConnectionTo(*it);
		}

		//clean the agents and possible trades in built connections
		while(validRcvConnections.size() > 0)
		{
			std::vector<std::string>::iterator it = validRcvConnections.begin() ;
			killConnectionFrom(*it);
		}

		remove();
	}
}

void Roman::treatIncomingConnections()
{
	std::vector<std::string>::iterator it = receivedConnections.begin();
	while(it != receivedConnections.end())
	{
		//accept and refuse remove the connection from receivedConnections
		//as a consequence there is no use to increment it
		int dice = std::rand()%70;
		if (dice <= 0)
		{
			acceptConnectionFrom(*it);
		}
		else
		{
			refuseConnectionFrom(*it);
		}
	}
}

void Roman::treatIncomingTrades()
{
	_nbTrades = 0;
	std::vector<std::tuple<std::string, std::string, double, double> >::iterator it = listReceivedTrades.begin();
	while(it != listReceivedTrades.end())
	{
		//accept and refuse remove the trade from listReceivedTrades
		//as a consequence there is no use to increment it
		int dice = std::rand()%70;
		if (dice <= 0)
		{
			acceptTradeFrom(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it), std::get<3>(*it));
			_nbTrades ++;
		}
		else
		{
			refuseTradeFrom(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it), std::get<3>(*it));
		}
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

void Roman::proposeConnectionTo(std::string target)
{
	//if the connection is not already among the valid one, ask for it
	if( std::find(validSendConnections.begin(), validSendConnections.end(), target) == validSendConnections.end() )
	{
		Roman* targetPtr = dynamic_cast<Roman*> (_world->getAgent(target));
		if (targetPtr == NULL)
		{
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}
		targetPtr->requestConnectionFrom(_id);
		//if the connection is not already in the proposed one, add it
		if( std::find(proposedConnections.begin(), proposedConnections.end(), target) == proposedConnections.end() )
		{
			proposedConnections.push_back(target);
		}
	}
}

void Roman::killConnectionTo(std::string target)
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
		Roman* targetPtr = dynamic_cast<Roman*> (_world->getAgent(target));
		if (targetPtr == NULL)
		{
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}
		targetPtr->killConnectionFrom(_id);
	}
}

void Roman::killConnectionFrom(std::string source)
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
		Roman* sourcePtr = dynamic_cast<Roman*> (_world->getAgent(source));
		if (sourcePtr == NULL)
		{
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}
		sourcePtr->killConnectionTo(_id);
	}
}

void Roman::killConnections(std::string target)
{
	killConnectionTo(target);
	killConnectionFrom(target);
}

void Roman::requestConnectionFrom(std::string source)
{
	//if the connection has not been received, put it among the received ones
	if( std::find(receivedConnections.begin(), receivedConnections.end(), source) == receivedConnections.end() )
	{
		receivedConnections.push_back(source);
	}
}

void Roman::acceptConnectionFrom(std::string source)
{
	//if the source has not been already validated
	if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) == validRcvConnections.end() )
	{
		Roman* sourcePtr = dynamic_cast<Roman*> (_world->getAgent(source));
		if (sourcePtr == NULL)
		{
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		//if the source is effectively in the received connection
		if( std::find(receivedConnections.begin(), receivedConnections.end(), source) != receivedConnections.end() )
		{
			receivedConnections.erase(std::remove(receivedConnections.begin(), receivedConnections.end(), source), receivedConnections.end());
			//confirm the connection to source, and verify that source is aware of it
			if (sourcePtr->ackConnectionFrom(_id))
			{
				validRcvConnections.push_back(source);
			}
		}
		//if the source is not in the lit of received, something went wrong, kill things
		else
		{
			sourcePtr->killConnectionTo(_id);
		}
	}
}

void Roman::refuseConnectionFrom(std::string source)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (_world->getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	//refuse connection from source
	sourcePtr->nackConnectionFrom(_id);
	//remove source
	receivedConnections.erase(std::remove(receivedConnections.begin(), receivedConnections.end(), source), receivedConnections.end());
}

void Roman::proposeConnectionBetween(std::string source, std::string target)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (_world->getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	sourcePtr->proposeConnectionTo(target);
}

void Roman::killConnectionBetween(std::string source, std::string target)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (_world->getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	sourcePtr->killConnectionTo(target);
}

int Roman::ackConnectionFrom(std::string target)
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

int Roman::nackConnectionFrom(std::string target)
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

void Roman::receiveMessageFrom(std::string source, std::string msg)
{
	if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
	{
		receivedMessages.push_back(std::make_tuple(source,msg));
	}
}

void Roman::sendMessageTo(std::string target, std::string msg)
{
	// if the connection with the target has been valideted
	if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
	{
		Roman* targetPtr = dynamic_cast<Roman*> (_world->getAgent(target));
		if (targetPtr == NULL)
		{
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}
		targetPtr->receiveMessageFrom(_id, msg);
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

std::vector<std::tuple<std::string,double,double> >  Roman::getListGoodsFrom(std::string target)
{
	Roman* targetPtr = dynamic_cast<Roman*> (_world->getAgent(target));
	if (targetPtr == NULL)
	{
		std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}
	return targetPtr->getListGoods();
}

void Roman::sendGoodTo(std::string target, std::string type, double value)
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
				Roman* targetPtr = dynamic_cast<Roman*> (_world->getAgent(target));
				if (targetPtr == NULL)
				{
					std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
					exit(1);
				}

				if(targetPtr->receiveGoodFrom(_id, type, value))
				{
					removeGood(type,value);
				}
			}
		}
	}
}

int Roman::receiveGoodFrom(std::string source, std::string type, double value)
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

int Roman::receiveTradeFrom(std::string source, std::string type, double value, double currency)
{
	auto it = listGoods.begin();
	while( it != listGoods.end() )
	{
		if (std::get<0>(*it) == type) break;
		it++;
	}
	//if arrived at the end of the list of goods without finding the specific good, quit the function now
	if (it == listGoods.end()) return 0;

	//if in the list of validated senders receive the trade offer
	if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
	{
		listReceivedTrades.push_back(make_tuple(source,type,value,currency));
		return 1;
	}
	
	//if arrive to that point, the agent was not a valid sender and we return an error
	return 0;
}

void Roman::proposeTradeTo(std::string target, std::string type, double valueGood, double valueCurrency)
{ 
	auto it = listGoods.begin();
	while( it != listGoods.end() )
	{
		if (std::get<0>(*it) == type) break;
		it++;
	}
	//if arrived at the end of the list of goods without finding the specific good, quit the function now
	if (it == listGoods.end()) return;

	// if the connection with the target has been validated
	if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
	{
		Roman* targetPtr = dynamic_cast<Roman*> (_world->getAgent(target));
		if (targetPtr == NULL)
		{
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		//if the offer is effectively received, add it
		if (targetPtr->receiveTradeFrom(_id,type,valueGood,valueCurrency))
		{
			listProposedTrades.push_back(make_tuple(target,type,valueGood,valueCurrency));
		}
	}
}

void Roman::acceptTradeFrom(std::string source, std::string type, double valueGood, double valueCurrency)
{
	if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
	{
		for (auto it = listReceivedTrades.begin() ; it != listReceivedTrades.end() ; it ++)
		{
			if ((*it) == std::make_tuple(source,type,valueGood,valueCurrency))
			{
				Roman* sourcePtr = dynamic_cast<Roman*> (_world->getAgent(source));
				if (sourcePtr == NULL)
				{
					std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
					exit(1);
				}

				if( (std::get<0>(getGood("currency")) >= valueCurrency) && (std::get<0>(sourcePtr->getGood(type)) >= valueGood) )
				{
					sourcePtr->sendGoodTo(_id,type,valueGood);
					sourcePtr->addGood("currency", valueCurrency);
					removeGood("currency",valueCurrency);
				}
				sourcePtr->removeProposedTrade(_id,type,valueGood,valueCurrency);
				removeReceivedTrade(source,type,valueGood,valueCurrency);
				break;
			}
		}
	}
}

void Roman::refuseTradeFrom(std::string source, std::string type, double valueGood, double valueCurrency)
{
	Roman* sourcePtr = dynamic_cast<Roman*> (_world->getAgent(source));
	if (sourcePtr == NULL)
	{
		std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
		exit(1);
	}

	sourcePtr->removeProposedTrade(_id,type,valueGood,valueCurrency);
	removeReceivedTrade(source,type,valueGood,valueCurrency);
}

std::vector<std::tuple<std::string,double,double> > Roman::getReceivedTradesFrom(std::string source)
{
	std::vector<std::tuple<std::string,double,double> > tmp;
	for (auto it = listReceivedTrades.begin() ; it != listReceivedTrades.end() ; it ++)
	{
		if (std::get<0>(*it) == source)
		{
			tmp.push_back(std::make_tuple(std::get<1>(*it),std::get<2>(*it),std::get<3>(*it)));
		}
	}
	return tmp;
}

std::vector<std::tuple<std::string,double,double> > Roman::getProposedTradesTo(std::string target)
{
	std::vector<std::tuple<std::string,double,double> > tmp;
	for (auto it = listProposedTrades.begin() ; it != listProposedTrades.end() ; it ++)
	{
		if (std::get<0>(*it) == target)
		{
			tmp.push_back(std::make_tuple(std::get<1>(*it),std::get<2>(*it),std::get<3>(*it)));
		}
	}
	return tmp;
}

void Roman::removeReceivedTrade(std::string source, std::string type, double value, double currency)
{
	for (auto it = listReceivedTrades.begin() ; it != listReceivedTrades.end() ;)
	{
		if ((*it) == std::make_tuple(source,type,value,currency))
		{
			listReceivedTrades.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}
}

void Roman::removeProposedTrade(std::string source, std::string type, double value, double currency)
{
	for (auto it = listProposedTrades.begin() ; it != listReceivedTrades.end() ;)
	{
		if ((*it) == std::make_tuple(source,type,value,currency))
		{
			listProposedTrades.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}
}

void Roman::killTradesFrom(std::string source)
{
	//remove traces from the receivedTrades
	for(auto it = listReceivedTrades.begin(); it != listReceivedTrades.end();)
	{
		if(std::get<0>(*it) == source)
		{
			it = listReceivedTrades.erase(it); 
		}
		else
		{
			++it;
		}
	}
}

void Roman::killTradesTo(std::string target)
{
	//remove traces from the proposedTrades
	for(auto it = listProposedTrades.begin(); it != listProposedTrades.end();)
	{
		if(std::get<0>(*it) == target)
		{
			it = listProposedTrades.erase(it); 
		}
		else
		{
			++it;
		}
	}
}

} // namespace Roman

