#include <BasicController.hxx>
#include <iostream>

#include <HarvestAction.hxx>
#include <ProposeConnectionAction.hxx>
#include <SendGoodsAction.hxx>
#include <ProductionAction.hxx>
#include <ConsumptionAction.hxx>
#include <ProposeTradeAction.hxx>
#include <FunAction.hxx>
#include <TradeAction.hxx>


/*-----------------------------------------------------------------------------
 *  This Controler ensure that all agents meet all agents
 *-----------------------------------------------------------------------------*/
namespace Epnet
{
	BasicController::BasicController()
	{
	}

	BasicController::~BasicController()
	{
	}

	void BasicController::updateState()
	{
		treatIncomingConnections();
		treatIncomingTrades();
	}

	std::list<Engine::Action*> BasicController::selectActions()
	{
		int action = _agent->getMaxActions();
		std::list<Engine::Action*> actions;
		
		actions.push_back(new ProductionAction());
		actions.push_back(new TradeAction());
		actions.push_back(new ConsumptionAction());
	
		return actions;
	}

	void BasicController::updateKnowledge()
	{
	}

	void BasicController::treatIncomingConnections()
	{
		std::vector<std::string> receivedConnections = _agent->getReceivedConnections();
		std::vector<std::string>::iterator it = receivedConnections.begin();
		while(it != receivedConnections.end())
		{
			//accept and refuse remove the connection from receivedConnections
			//as a consequence there is no use to increment it
			int dice = std::rand()%70;
			if (dice <= 0)
			{
				_agent->acceptConnectionFrom(*it);
			}
			else
			{
				_agent->refuseConnectionFrom(*it);
			}
			receivedConnections = _agent->getReceivedConnections();
			it = receivedConnections.begin();
		}
	}

	void BasicController::treatIncomingTrades()
	{
		_agent->resetNbTrades();
		std::vector<std::tuple<std::string, std::string, double, double> > listReceivedTrades = _agent->getReceivedTrades();
		std::vector<std::tuple<std::string, std::string, double, double> >::iterator it = listReceivedTrades.begin();
		while(it != listReceivedTrades.end())
		{
			//accept and refuse remove the trade from listReceivedTrades
			//as a consequence there is no use to increment it
			int dice = std::rand()%70;
			if (dice <= 0)
			{
				_agent->acceptTradeFrom(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it), std::get<3>(*it));
				_agent->increaseNbTrades(1);
			}
			else
			{
				_agent->refuseTradeFrom(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it), std::get<3>(*it));
			}
			listReceivedTrades = _agent->getReceivedTrades();
			it = listReceivedTrades.begin();
		}
	}
} // namespace Epnet
