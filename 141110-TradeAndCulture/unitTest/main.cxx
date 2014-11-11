
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Pandora Sequential Unit Tests"

#include "Roman.hxx"
#include "Province.hxx"
#include "ProvinceConfig.hxx"
#include <Config.hxx>
#include <World.hxx>
#include <Point2D.hxx>
#include <Size.hxx>
#include <ShpLoader.hxx>
#include <GeneralState.hxx>
#include <Exception.hxx>

#include <boost/test/unit_test.hpp>

namespace Epnet
{

BOOST_AUTO_TEST_SUITE( ConnectionFromSourceAgent )

BOOST_AUTO_TEST_CASE( askConnection ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	Roman * myAgent2 = new Roman("agent_2");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	Engine::AgentsVector neighbours = myWorld.getNeighbours(myAgent0, 20);
	BOOST_CHECK_EQUAL(neighbours.size(), 2);
	myWorld.run();

}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test

