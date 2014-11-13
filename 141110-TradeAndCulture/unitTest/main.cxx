
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

BOOST_AUTO_TEST_CASE( AgentAskConnection ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo(myAgent1);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent0[0], myAgent1);

	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent1[0], myAgent0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentAsk2Times ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo(myAgent1);
	myAgent0->proposeConnectionTo(myAgent1);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent0[0], myAgent1);

	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent1[0], myAgent0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentAcceptConnection ) 
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

	myAgent0->proposeConnectionTo(myAgent1);
	myAgent1->acceptConnectionFrom(myAgent0);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], myAgent1);

	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], myAgent0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( DuplicateConnection ) 
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

	myAgent0->proposeConnectionTo(myAgent1);
	myAgent1->acceptConnectionFrom(myAgent0);

	myAgent0->proposeConnectionTo(myAgent1);
	myAgent1->acceptConnectionFrom(myAgent0);

	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], myAgent1);

	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], myAgent0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentRefuseConnection ) 
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

	myAgent0->proposeConnectionTo(myAgent1);
	myAgent1->refuseConnectionFrom(myAgent0);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( FailAcceptIfNotSent ) 
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

	myAgent1->acceptConnectionFrom(myAgent0);
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RefuseConnectionNotSent ) 
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

	myAgent1->refuseConnectionFrom(myAgent0);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE( ConnectionFromSourceAgent )

BOOST_AUTO_TEST_CASE( WorldAskConnection ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myWorld.proposeConnection(myAgent0, myAgent1);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent0[0], myAgent1);

	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent1[0], myAgent0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildConnection ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myWorld.buildConnection(myAgent0, myAgent1);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], myAgent1);

	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], myAgent0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildSameConnection ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myWorld.buildConnection(myAgent0, myAgent1);
	myWorld.buildConnection(myAgent0, myAgent1);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], myAgent1);

	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], myAgent0);

	myWorld.run();
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace Test

