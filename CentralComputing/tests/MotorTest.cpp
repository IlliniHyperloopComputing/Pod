#include "PodTest.cpp"

void check_test_eq(std::string file, std::string val);

void check_test_eq(std::string file, std::string val){
  std::ifstream in(file);
  std::string line;
  std::getline(in, line);
  in.close();
  EXPECT_EQ(line, val);
}


TEST_F(PodTest, MotorTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);

  //Set output path
  system("touch tests/enable");
  system("touch tests/duty_cycle");
  std::string path = "tests/";

  //Enable motors
  SendCommand(NetworkManager::Network_Command_ID::ENABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);
  check_test_eq(path+"enable", "1");

  //Disable motors
  SendCommand(NetworkManager::Network_Command_ID::DISABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);
  check_test_eq(path+"enable", "0");

  //Enable motors
  SendCommand(NetworkManager::Network_Command_ID::ENABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);
  check_test_eq(path+"enable", "1");

  //Disable motors
  SendCommand(NetworkManager::Network_Command_ID::DISABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);
  check_test_eq(path+"enable", "0");

  //Enable motors
  SendCommand(NetworkManager::Network_Command_ID::ENABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);
  check_test_eq(path+"enable", "1");

  check_test_eq(path+"duty_cycle", "950000");



  //Try setting motor values
  SendCommand(NetworkManager::Network_Command_ID::SET_MOTOR_SPEED, 0);
  check_test_eq(path+"duty_cycle", "1000000");


  SendCommand(NetworkManager::Network_Command_ID::SET_MOTOR_SPEED, 125);
  check_test_eq(path+"duty_cycle", "1500000");

  SendCommand(NetworkManager::Network_Command_ID::SET_MOTOR_SPEED, 250);
  check_test_eq(path+"duty_cycle", "2000000");

  //Disable again
  SendCommand(NetworkManager::Network_Command_ID::DISABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);
  check_test_eq(path+"enable", "0");
  check_test_eq(path+"duty_cycle", "950000");




}
