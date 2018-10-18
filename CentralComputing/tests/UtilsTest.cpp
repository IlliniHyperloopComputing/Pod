#include "Utils.h"
#include "Pod.h"
#include "Event.hpp"
#include "Simulator.hpp"

#include <chrono>

TEST(UtilsTest, busyWaitTestShort) {
	double start = std::clock();
	int time = 1000;
	Utils::busyWait(time);
	EXPECT_GE(std::clock(), start + time);
}

TEST(UtilsTest, busyWaitTestMed) {
	double start = std::clock();
	int time = 200000;
	Utils::busyWait(time);
	EXPECT_GE(std::clock(), start + time);
}

TEST(UtilsTest, busyWaitTestLong) {
	double start = std::clock();
	int time = 20000000;
	Utils::busyWait(time);
	EXPECT_GE(std::clock(), start + time);
}
