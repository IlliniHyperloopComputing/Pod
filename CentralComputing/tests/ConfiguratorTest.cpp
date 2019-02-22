#include "PodTest.cpp"

//Test Configurator
TEST(ConfiguratorTest, LoadOutputVariables) {
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/test.txt"), true);
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/notrealfile.txt"), false);

    double val;

    ConfiguratorManager::config.getValue("Variable", val);
    EXPECT_EQ(val, 12);

    ConfiguratorManager::config.getValue("Number", val);
    EXPECT_EQ(val, 2342);

    ConfiguratorManager::config.getValue("MaxDecel", val);
    EXPECT_EQ(val, 231);

    ConfiguratorManager::config.getValue("MaxAccel", val);
    EXPECT_EQ(val, 1231.23);

    EXPECT_EQ(ConfiguratorManager::config.getValue("FakeName", val), false);

}
