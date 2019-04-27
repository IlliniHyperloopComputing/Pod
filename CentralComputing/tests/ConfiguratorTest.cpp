#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"
#include <string>
using namespace std;
//Test Configurator without Pod
TEST(ConfiguratorTest, LoadOutputVariables) {
  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/test.txt"), true);
  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/notrealfile.txt"), false);

  int64_t val;
  ConfiguratorManager::config.getValue("Variable", val);
  EXPECT_EQ(val, (int64_t) 12);

  ConfiguratorManager::config.getValue("Number", val);
  EXPECT_EQ(val, (int64_t) 2342);

  ConfiguratorManager::config.getValue("MaxDecel", val);
  EXPECT_EQ(val, (int64_t) 231);

  float val2;
  ConfiguratorManager::config.getValue("MaxAccel", val2);
  EXPECT_EQ(val2, (float) 1231.23);


  EXPECT_EQ(ConfiguratorManager::config.getValue("FakeName", val), false);

}
// Test Configurator as it is loaded on startup
TEST_F(PodTest, ConfigManagerTimeouts) {
  string val = "";

  ConfiguratorManager::config.getValue("can_manager_timeout", val);
  EXPECT_EQ(std::stoll(val), SourceManager::CAN.refresh_timeout());

  ConfiguratorManager::config.getValue("tmp_manager_timeout", val);
  EXPECT_EQ(std::stoll(val), SourceManager::TMP.refresh_timeout());

  ConfiguratorManager::config.getValue("i2c_manager_timeout", val);
  EXPECT_EQ(std::stoll(val), SourceManager::I2C.refresh_timeout());

  ConfiguratorManager::config.getValue("adc_manager_timeout", val);
  EXPECT_EQ(std::stoll(val), SourceManager::ADC.refresh_timeout());

  ConfiguratorManager::config.getValue("pru_manager_timeout", val);
  EXPECT_EQ(std::stoll(val), SourceManager::PRU.refresh_timeout());

}
#endif
