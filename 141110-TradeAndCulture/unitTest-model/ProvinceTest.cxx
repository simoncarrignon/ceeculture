#include <ProvinceTest.hxx>

namespace Epnet
{

ProvinceTest::ProvinceTest(Engine::Config * config, Engine::Scheduler * scheduler ) : Province(config, scheduler)
{
}

ProvinceTest::~ProvinceTest()
{
}

void ProvinceTest::stepTest()
{
	step();
}


} // namespace Roman

