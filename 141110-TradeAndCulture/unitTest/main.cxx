
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

BOOST_AUTO_TEST_CASE( AgentsBuildTwoWayConnection ) 
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

	myAgent1->proposeConnectionTo(myAgent0);
	myAgent0->acceptConnectionFrom(myAgent1);

	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<Roman*> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<Roman*> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], myAgent1);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent0[0], myAgent1);

	std::vector<Roman*> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<Roman*> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent1[0], myAgent0);
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

BOOST_AUTO_TEST_CASE( SourceKillOneWayConnection ) 
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
	myAgent0->killConnectionTo(myAgent1);
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

BOOST_AUTO_TEST_CASE( TargetKillOneWayConnection ) 
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
	myAgent1->killConnectionFrom(myAgent0);
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

BOOST_AUTO_TEST_CASE( killTwoWayConnection ) 
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

	myAgent1->proposeConnectionTo(myAgent0);
	myAgent0->acceptConnectionFrom(myAgent1);

	myAgent1->killConnections(myAgent0);
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

BOOST_AUTO_TEST_CASE( killHalfOfTwoWayConnection ) 
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

	myAgent1->proposeConnectionTo(myAgent0);
	myAgent0->acceptConnectionFrom(myAgent1);

	myAgent1->killConnectionTo(myAgent0);
	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<Roman*> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<Roman*> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], myAgent1);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 0);

	std::vector<Roman*> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<Roman*> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], myAgent0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE( ConnectionFromWorld )

BOOST_AUTO_TEST_CASE( WorldProposeConnection ) 
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

BOOST_AUTO_TEST_CASE( WorldKillConnection ) 
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
	myWorld.killConnection(myAgent0, myAgent1);

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

BOOST_AUTO_TEST_CASE( ProposeTwoWayConnection ) 
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

	myWorld.proposeTwoWayConnection(myAgent0,myAgent1);

	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<Roman*> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<Roman*> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent0[0], myAgent1);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent0[0], myAgent1);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 0);

	std::vector<Roman*> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<Roman*> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent1[0], myAgent0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent1[0], myAgent0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildTwoWayConnection ) 
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

	myWorld.buildTwoWayConnection(myAgent0,myAgent1);

	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<Roman*> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<Roman*> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], myAgent1);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent0[0], myAgent1);

	std::vector<Roman*> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<Roman*> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent1[0], myAgent0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], myAgent0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildKillHalfOfTwoWayConnection ) 
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

	myWorld.buildTwoWayConnection(myAgent0,myAgent1);
	myWorld.killConnection(myAgent0, myAgent1);

	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<Roman*> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<Roman*> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent0[0], myAgent1);

	std::vector<Roman*> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<Roman*> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent1[0], myAgent0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildKillTwoWayConnection ) 
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

	myWorld.buildTwoWayConnection(myAgent0,myAgent1);
	myWorld.killTwoWayConnection(myAgent0, myAgent1);

	std::vector<Roman*> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<Roman*> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<Roman*> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<Roman*> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 0);

	std::vector<Roman*> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<Roman*> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE( ConnectionFromOther )

BOOST_AUTO_TEST_CASE( AgentProposeOtherConnection ) 
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

	myAgent0->proposeConnectionBetween(myAgent1, myAgent2);
	std::vector<Roman*> proposedAgent1 = myAgent1->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent1[0], myAgent2);

	std::vector<Roman*> receivedAgent2 = myAgent2->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent2.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent2[0], myAgent1);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentKillOtherConnection ) 
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

	myAgent1->proposeConnectionTo(myAgent2);
	myAgent2->acceptConnectionFrom(myAgent1);
	myAgent0->killConnectionBetween(myAgent1, myAgent2);

	std::vector<Roman*> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<Roman*> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<Roman*> receivedAgent1 = myAgent1->getReceivedConnections();
	std::vector<Roman*> validRcvAgent1 = myAgent1->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	std::vector<Roman*> proposedAgent2 = myAgent2->getProposedConnections();
	std::vector<Roman*> validSendAgent2 = myAgent2->getValidSendConnections();
	std::vector<Roman*> receivedAgent2 = myAgent2->getReceivedConnections();
	std::vector<Roman*> validRcvAgent2 = myAgent2->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent2.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent2.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent2.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent2.size(), 0);

	myWorld.run();
}
BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE( MessagesBtwAgents )

BOOST_AUTO_TEST_CASE( SourceSendMsg ) 
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
	myAgent1->acceptConnectionFrom(myAgent0);
	myAgent0->sendMessageTo(myAgent1,"hello");

	std::vector<std::tuple<Roman*,std::string> > receivedMsgAgent1 = myAgent1->getReceivedMessages();
	BOOST_CHECK_EQUAL(receivedMsgAgent1.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(receivedMsgAgent1[0]), myAgent0);
	BOOST_CHECK_EQUAL(std::get<1>(receivedMsgAgent1[0]), "hello");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendMsgToProposed ) 
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
	myAgent0->sendMessageTo(myAgent1,"hello");

	std::vector<std::tuple<Roman*,std::string> > receivedMsgAgent1 = myAgent1->getReceivedMessages();
	BOOST_CHECK_EQUAL(receivedMsgAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendMsgToUnconnected ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->sendMessageTo(myAgent1,"hello");

	std::vector<std::tuple<Roman*,std::string> > receivedMsgAgent1 = myAgent1->getReceivedMessages();
	BOOST_CHECK_EQUAL(receivedMsgAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendMsgAfterConnectionKilled ) 
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
	myAgent1->acceptConnectionFrom(myAgent0);
	myAgent0->killConnectionTo(myAgent1);
	myAgent0->sendMessageTo(myAgent1,"hello");

	std::vector<std::tuple<Roman*,std::string> > receivedMsgAgent1 = myAgent1->getReceivedMessages();
	BOOST_CHECK_EQUAL(receivedMsgAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( GoodsSystem )

BOOST_AUTO_TEST_CASE( AddTypeGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);

	std::vector<std::tuple<std::string, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AddTwoTimesTypeGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGoodType("A",99.0);

	std::vector<std::tuple<std::string, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AddTwoDifferentTypeGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGoodType("B",50.0);

	std::vector<std::tuple<std::string, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 2);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[1]), "B");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[1]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[1]), 50.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveTypeGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGoodType("B",50.0);
	myAgent0->removeGoodType("A");

	std::vector<std::tuple<std::string, double,double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "B");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 50.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveNoneExistingGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->removeGoodType("B");

	std::vector<std::tuple<std::string, double,double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}


BOOST_AUTO_TEST_CASE( AddGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",2.0);

	std::vector<std::tuple<std::string, double,double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 2.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AddTooMuchGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",2.0);
	myAgent0->addGood("A",98.1);

	std::vector<std::tuple<std::string, double,double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 100.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AddGoodToNotKnownType ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("B",2.0);

	std::vector<std::tuple<std::string, double,double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",50.0);
	myAgent0->removeGood("A",2.0);

	std::vector<std::tuple<std::string, double,double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 48.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveTooMuchGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",10.0);
	myAgent0->removeGood("A",20.0);

	std::vector<std::tuple<std::string, double,double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveGoodNotKnown ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",10.0);
	myAgent0->removeGood("B",20.0);

	std::vector<std::tuple<std::string, double,double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 10.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( GetGood ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",2.0);

	std::tuple<double,double> goods = myAgent0->getGood("A");
	BOOST_CHECK_EQUAL(std::get<0>(goods), 2.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( GetGoodUnknwon ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",2.0);

	std::tuple<double,double> goods = myAgent0->getGood("B");
	BOOST_CHECK_EQUAL(std::get<0>(goods), -1.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods), -1.0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE( SendingGoods )

BOOST_AUTO_TEST_CASE( SendGoodToReceiver ) 
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
	myAgent1->acceptConnectionFrom(myAgent0);

	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",20.0);
	myAgent1->addGoodType("A",100.0);

	myAgent0->sendGoodTo(myAgent1,"A",15);

	std::tuple<double,double> goods0 = myAgent0->getGood("A");
	std::tuple<double,double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 5.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 15.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendUnknownGood ) 
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
	myAgent1->acceptConnectionFrom(myAgent0);

	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",20.0);
	myAgent1->addGoodType("A",100.0);

	myAgent0->sendGoodTo(myAgent1,"B",15);

	std::tuple<double,double> goods0 = myAgent0->getGood("A");
	std::tuple<double,double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendGoodUnknownToReceiver ) 
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
	myAgent1->acceptConnectionFrom(myAgent0);

	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGoodType("B",100.0);
	myAgent0->addGood("B",20.0);
	myAgent1->addGoodType("A",100.0);

	myAgent0->sendGoodTo(myAgent1,"B",15);

	std::tuple<double,double> goodsA0 = myAgent0->getGood("A");
	std::tuple<double,double> goodsB0 = myAgent0->getGood("A");
	std::tuple<double,double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goodsA0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goodsA0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goodsB0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goodsB0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendGoodWithoutConnection ) 
{
	Province myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	Roman * myAgent0 = new Roman("agent_0");
	Roman * myAgent1 = new Roman("agent_1");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",20.0);
	myAgent1->addGoodType("A",100.0);

	myAgent0->sendGoodTo(myAgent1,"A",15);

	std::tuple<double,double> goods0 = myAgent0->getGood("A");
	std::tuple<double,double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendGoodToSender ) 
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
	myAgent1->acceptConnectionFrom(myAgent0);

	myAgent0->addGoodType("A",100.0);
	myAgent1->addGoodType("A",100.0);
	myAgent1->addGood("A",20.0);

	myAgent1->sendGoodTo(myAgent0,"A",15);

	std::tuple<double,double> goods0 = myAgent0->getGood("A");
	std::tuple<double,double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendSaturatingGood ) 
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
	myAgent1->acceptConnectionFrom(myAgent0);

	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",100.0);
	myAgent1->addGoodType("A",100.0);
	myAgent1->addGood("A",20.0);

	myAgent0->sendGoodTo(myAgent1,"A",90);

	std::tuple<double,double> goods0 = myAgent0->getGood("A");
	std::tuple<double,double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 10.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendGoodNotInStock ) 
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
	myAgent1->acceptConnectionFrom(myAgent0);

	myAgent0->addGoodType("A",100.0);
	myAgent0->addGood("A",20.0);
	myAgent1->addGoodType("A",100.0);

	myAgent0->sendGoodTo(myAgent1,"A",30);

	std::tuple<double,double> goods0 = myAgent0->getGood("A");
	std::tuple<double,double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test

