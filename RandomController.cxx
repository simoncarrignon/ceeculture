#include <RandomController.hxx>

#include <iostream>

#include <CulturalAction.hxx>
#include <ConsumptionAction.hxx>

namespace Epnet
{
	RandomController::RandomController()
	{
	}

	RandomController::RandomController(double mutationRate,std::string selectionProcess,std::string innovationProcess,int culturalStep)
	{  
	  _mutationRate = mutationRate;
	  _selectionProcess = selectionProcess;
	  _innovationProcess = innovationProcess;
	  _culturalStep = culturalStep;
	}

	RandomController::~RandomController()
	{
	}

	void RandomController::updateState()
	{
	}

	std::list<Engine::Action*> RandomController::selectActions()
	{
		int action = _agent->getMaxActions();
		std::list<Engine::Action*> actions;
		int timestep = _agent->getWorld()->getCurrentStep();
		if(timestep%3 == 2)actions.push_back(new ConsumptionAction());
		if( (timestep%(3 * (_culturalStep)) == 0) && (timestep> 4 ) )actions.push_back(new CulturalAction(_mutationRate,_selectionProcess,_innovationProcess)); 

		return actions;
	}

	void RandomController::updateKnowledge()
	{
	}

	void RandomController::treatIncomingConnections()
	{
	}

	void RandomController::treatIncomingTrades()
	{
	}


} // namespace Epnet
