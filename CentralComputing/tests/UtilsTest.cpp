#include "Utils.h"
#include "Pod.h"
#include "Event.hpp"
#include "Simulator.hpp"
#include <ctime>
#include <iostream>
#include <cstdio>

TEST(UtilsTest, busyWaitTest) {
	//buffer based on how accurate busyWait needs to be while still passing the test
	long buffer = 100;
	std::clock_t start = std::clock();
	//Set time according to how long the test should be for
	long time = 10000;
	Utils::busyWait(time);
	double elapsedTime = (double) (((std::clock() - start) / ((double) CLOCKS_PER_SEC)) * 1000);
	EXPECT_GE((long) elapsedTime, time - buffer);
	EXPECT_LE((long) elapsedTime, time + buffer);
}