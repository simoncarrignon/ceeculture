
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

/*
BOOST_AUTO_TEST_SUITE( DeathTests )

BOOST_AUTO_TEST_CASE( DeathHappens ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 3), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myWorld.step();

	int count = 0;
	for(auto it = myWorld.beginAgents() ; it != myWorld.endAgents() ; it++)
	{
		count++;
	}

	std::cout << count <<std::endl;

	BOOST_CHECK_EQUAL(1,1);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()
*/

BOOST_AUTO_TEST_SUITE( VisibleGoods )

BOOST_AUTO_TEST_CASE( getGoods ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 3), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGoodType("B",50.0);
	myAgent0->addGood("B",30.0);

	std::vector<std::tuple<std::string,double,double> > listAgent0 = myAgent1->getListGoodsFrom(myAgent0);

	BOOST_CHECK_EQUAL(std::get<0>(listAgent0[0]),"A");
	BOOST_CHECK_EQUAL(std::get<1>(listAgent0[0]),20.0);
	BOOST_CHECK_EQUAL(std::get<2>(listAgent0[0]),100.0);
	BOOST_CHECK_EQUAL(std::get<0>(listAgent0[1]),"B");
	BOOST_CHECK_EQUAL(std::get<1>(listAgent0[1]),30.0);
	BOOST_CHECK_EQUAL(std::get<2>(listAgent0[1]),50.0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test

