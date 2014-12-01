#ifndef __ProposeConnectionAction_hxx__
#define __ProposeConnectionAction_hxx__

#include <Action.hxx>
#include <string>
#include <Roman.hxx>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class ProposeConnectionAction : public Engine::Action
{
public:
	ProposeConnectionAction(Roman* target);
	virtual ~ProposeConnectionAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

private:
	Roman* _target;
};

} // namespace Epnet

#endif // __ProposeConnectionAction_hxx__

