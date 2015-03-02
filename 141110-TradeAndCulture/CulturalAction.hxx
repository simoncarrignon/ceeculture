#ifndef __CulturalAction_hxx__
#define __CulturalAction_hxx__

#include <Action.hxx>
#include <string>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class CulturalAction : public Engine::Action
{
public:
	CulturalAction();
	virtual ~CulturalAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

};

} // namespace Epnet

#endif // __CulturalAction_hxx__
