#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"
#include "Command.h"
#include "Defines.hpp"
#include <string>

using namespace std;
//Test Setting error flags without Pod
TEST(ErrorFlagTest, ExpectedFlagOutcome) {
  microseconds();
  memset(Command::error_flag_timers, 0, sizeof(Command::error_flag_timers));
  Utils::busyWait(1000000);
  Command::Network_Command com;

  // Try to set basic flag
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_SETUP_FAILURE);
  EXPECT_TRUE(Command::get(&com));
  EXPECT_EQ(com.id, Command::SET_ADC_ERROR);
  EXPECT_EQ(com.value, ADC_SETUP_FAILURE);

  // Cant set same error again that quickly
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_SETUP_FAILURE);
  EXPECT_FALSE(Command::get(&com));

  // Set different flag
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_READ_ERROR);
  EXPECT_TRUE(Command::get(&com));
  EXPECT_EQ(com.id, Command::SET_ADC_ERROR);
  EXPECT_EQ(com.value, ADC_READ_ERROR);

}

#endif