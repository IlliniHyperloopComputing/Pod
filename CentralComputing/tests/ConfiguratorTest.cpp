#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"
#include <string>
using namespace std;

//Test Configurator without Pod
TEST(ConfiguratorTest, LoadOutputVariables) {
  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/test.txt", false), true);
  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/notrealfile.txt", false), false);

  int64_t val;
  ConfiguratorManager::config.getValue("Variable", val);
  EXPECT_EQ(val, (int64_t) 12);

  ConfiguratorManager::config.getValue("Variable1", val);
  EXPECT_EQ(val, (int64_t) -12);

  ConfiguratorManager::config.getValue("Number", val);
  EXPECT_EQ(val, (int64_t) 2342);

  ConfiguratorManager::config.getValue("MaxDecel", val);
  EXPECT_EQ(val, (int64_t) 231);

  float val2;
  ConfiguratorManager::config.getValue("MaxAccel", val2);
  EXPECT_EQ(val2, (float) 1231.23);

  ConfiguratorManager::config.getValue("MaxAccel1", val2);
  EXPECT_EQ(val2, (float) -1231.23);


  EXPECT_EQ(ConfiguratorManager::config.getValue("FakeName", val), false);

}

//Test Flight Plan Configurator without Pod
TEST(ConfiguratorTest, FlightPlanTest) {
  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/fakeFlightPlan.txt", true), false);
  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/basicFlightPlan.txt", true), true);

  unsigned int idx = 0;

  int16_t val = ConfiguratorManager::config.getFlightPlan(0, &idx);
  EXPECT_EQ(val, 100);
  EXPECT_EQ(idx,(unsigned int) 0);

  idx = 1;
  val = ConfiguratorManager::config.getFlightPlan(0, &idx); // test on a higher index, should return 0
  EXPECT_EQ(val, 0);
  EXPECT_EQ(idx, (unsigned int) 1);


  EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/basicFlightPlan1.txt", true), true);

  idx = 0;
  val = ConfiguratorManager::config.getFlightPlan(0,   &idx);
  EXPECT_EQ(val, 105);
  EXPECT_EQ(idx, (unsigned int)0);
  idx = 0;

  val = ConfiguratorManager::config.getFlightPlan(200, &idx); 
  EXPECT_EQ(val, 200);
  EXPECT_EQ(idx, (unsigned int)1);
  idx = 1;
  val = ConfiguratorManager::config.getFlightPlan(200, &idx); 
  EXPECT_EQ(val, 200);
  EXPECT_EQ(idx, (unsigned int)1);
  idx = 0;

  val = ConfiguratorManager::config.getFlightPlan(300, &idx); 
  EXPECT_EQ(val, 400);
  EXPECT_EQ(idx, (unsigned int)2);
  idx = 1;
  val = ConfiguratorManager::config.getFlightPlan(300, &idx); 
  EXPECT_EQ(idx, (unsigned int)2);
  idx = 2;
  EXPECT_EQ(val, 400);
  val = ConfiguratorManager::config.getFlightPlan(300, &idx); 
  EXPECT_EQ(val, 400);
  EXPECT_EQ(idx, (unsigned int)2);
  idx = 0;

  val = ConfiguratorManager::config.getFlightPlan(400, &idx); 
  EXPECT_EQ(val, 300);
  EXPECT_EQ(idx, (unsigned int)3);
  idx = 1;
  val = ConfiguratorManager::config.getFlightPlan(400, &idx); 
  EXPECT_EQ(val, 300);
  EXPECT_EQ(idx, (unsigned int)3);
  idx = 2;
  val = ConfiguratorManager::config.getFlightPlan(400, &idx); 
  EXPECT_EQ(val, 300);
  EXPECT_EQ(idx,(unsigned int) 3);
  idx = 3;
  val = ConfiguratorManager::config.getFlightPlan(400, &idx); 
  EXPECT_EQ(val, 300);
  EXPECT_EQ(idx, (unsigned int)3);
  idx = 0;

  val = ConfiguratorManager::config.getFlightPlan(500, &idx); 
  EXPECT_EQ(val, 900);
  EXPECT_EQ(idx, (unsigned int)4);
  idx = 1;
  val = ConfiguratorManager::config.getFlightPlan(500, &idx); 
  EXPECT_EQ(val, 900);
  EXPECT_EQ(idx, (unsigned int)4);
  idx = 2;
  val = ConfiguratorManager::config.getFlightPlan(500, &idx); 
  EXPECT_EQ(val, 900);
  EXPECT_EQ(idx, (unsigned int)4);
  idx = 3;
  val = ConfiguratorManager::config.getFlightPlan(500, &idx); 
  EXPECT_EQ(val, 900);
  EXPECT_EQ(idx, (unsigned int)4);
  idx = 4;
  val = ConfiguratorManager::config.getFlightPlan(500, &idx); 
  EXPECT_EQ(val, 900);
  EXPECT_EQ(idx, (unsigned int)4);

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
