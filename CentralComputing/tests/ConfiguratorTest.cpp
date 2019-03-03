#include "PodTest.cpp"
#include <string>
using namespace std;
//Test Configurator without Pod
TEST(ConfiguratorTest, LoadOutputVariables) {
  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/test.txt"), true);
  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/notrealfile.txt"), false);

  string val = "";
  ConfiguratorManager::config.getValue("Variable", val);
  EXPECT_EQ(std::stoll(val), (long long) 12);
  EXPECT_EQ(val, "12");

  ConfiguratorManager::config.getValue("Number", val);
  EXPECT_EQ(std::stoll(val),(long long) 2342);
  EXPECT_EQ(val, "2342");

  ConfiguratorManager::config.getValue("MaxDecel", val);
  EXPECT_EQ(std::stoll(val),(long long) 231);
  EXPECT_EQ(val, "231");
  ConfiguratorManager::config.getValue("MaxAccel", val);
  EXPECT_EQ(val,"1231.23");
  EXPECT_EQ(std::stoll(val),(long long) 1231.23);
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

  ConfiguratorManager::config.getValue("mm_manager_timeout", val);
  EXPECT_EQ(std::stoll(val), SourceManager::MM.refresh_timeout());

}
