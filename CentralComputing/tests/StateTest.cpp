#include "PodTest.cpp"


TEST_F(PodTest, ConnectDisconnect) {

}

TEST_F(PodTest, SimpleChange) {
  auto command = std::make_shared<NetworkManager::Network_Command>();
  command->id = NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST;
  command->value = 0;
  pod->processing_command.reset();
  EXPECT_TRUE(simulator->send_command(command));
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->get_current_state(), Pod_State::E_States::ST_FUNCTIONAL_TEST);
}

