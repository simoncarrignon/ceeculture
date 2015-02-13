#include <RandomController.hxx>

namespace Epnet
{
	RandomController::RandomController()
	{
	}

	RandomController::~RandomController()
	{
	}

	void RandomController::updateState()
	{
		treatIncomingConnections();
		treatIncomingTrades();
	}

	void RandomController::selectActions()
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

	void RandomController::updateKnowledge()
	{
	}

	void RandomController::treatIncomingConnections()
	{
		/*
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
		*/
	}

	void RandomController::treatIncomingTrades()
	{
		/*
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
		*/
	}
} // namespace Epnet
