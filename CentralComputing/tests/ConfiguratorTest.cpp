#include "Configurator.hpp"
#include "Pod.h"
#include "Event.hpp"
#include "Simulator.hpp"


//Test Configurator
TEST(ConfiguratorTest, LoadOutputVariables) {
    EXPECT_EQ(ConfiguratorManager::config.openVarFile("test.txt"), 1);
    EXPECT_EQ(ConfiguratorManager::config.openVarFile("notrealfile.txt"), 0);
    EXPECT_EQ(ConfiguratorManager::config.getValue("Variable"), 12);
    EXPECT_EQ(ConfiguratorManager::config.getValue("Number"), 2342);
    EXPECT_EQ(ConfiguratorManager::config.getValue("MaxDecel"), 231);
    EXPECT_EQ(ConfiguratorManager::config.getValue("MaxAccel"), 1231.23);
}
