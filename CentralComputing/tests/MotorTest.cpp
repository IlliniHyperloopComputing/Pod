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
  pod->processing_command.reset();
  MoveState(NetworkManager::Network_Command_ID::DISABLE_MOTOR, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  pod->processing_command.wait();
  SendCommand(NetworkManager::Network_Command_ID::ENABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);
  check_test_eq(path+"enable", "0");

  //Enable motors
  pod->processing_command.reset();
  MoveState(NetworkManager::Network_Command_ID::ENABLE_MOTOR, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);
  check_test_eq(path+"enable", "1");

  //Disable motors
  pod->processing_command.reset();
  MoveState(NetworkManager::Network_Command_ID::DISABLE_MOTOR, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);
  check_test_eq(path+"enable", "0");

  //Enable motors
  pod->processing_command.reset();
  MoveState(NetworkManager::Network_Command_ID::ENABLE_MOTOR, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);
  check_test_eq(path+"enable", "1");

  check_test_eq(path+"duty_cycle", "950000");



  //Try setting motor values
  pod->state_machine->motor.set_throttle(0);
  check_test_eq(path+"duty_cycle", "1000000");

  pod->state_machine->motor.set_throttle(1);
  check_test_eq(path+"duty_cycle", "1001000");

  pod->state_machine->motor.set_throttle(500);
  check_test_eq(path+"duty_cycle", "1500000");

  pod->state_machine->motor.set_throttle(1000);
  check_test_eq(path+"duty_cycle", "2000000");

  //Disable again
  MoveState(NetworkManager::Network_Command_ID::DISABLE_MOTOR, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  usleep(100000);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);
  check_test_eq(path+"enable", "0");
  check_test_eq(path+"duty_cycle", "950000");




}
