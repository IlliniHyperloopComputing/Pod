#include "Utils.h"
#include "Pod.h"
#include "Event.hpp"
#include "Simulator.hpp"
#include <ctime>
#include <iostream>
#include <cstdio>
#include <pthread.h>

TEST(UtilsTest, busyWaitTest) {
	//buffer based on how accurate busyWait needs to be while still passing the test
	long buffer = 100000;
	std::clock_t start = std::clock();
	//Set time according to how long the test should be for
	long time = 1000000;
	Utils::busyWait(time);
	double elapsedTime = (double) (((std::clock() - start) / ((double) CLOCKS_PER_SEC)) * 1000000);
	EXPECT_GE((long) elapsedTime, time - buffer);
	EXPECT_LE((long) elapsedTime, time + buffer);
}
//Expecting set and get commands to go through by checking if return values are zero

//tests the total wait time of threads created
TEST(UtilsTest, busyWaitThreads) {
	int threadCount = 10;
	long time = 100000;
	//buffer based on how accurate busyWait needs to be while still passing the test
	long buffer = 10000;
	std::clock_t start = std::clock();
	//need to save the first affinity and set it back to that at the end
	cpu_set_t first;
	EXPECT_EQ(sched_getaffinity(0, sizeof(first), &first), 0);

	cpu_set_t toRun;
	CPU_ZERO(&toRun);
	CPU_SET(0, &toRun);
	EXPECT_EQ(sched_setaffinity(0, sizeof(toRun), &toRun), 0);

	std::thread t[threadCount];
	for (int count = 0; count < threadCount; count++) {
		t[count] = std::thread(Utils::busyWait, time);
	}
	for (int count = 0; count < threadCount; count++) {
		t[count].join();
	}

	double elapsedTime = (double) (((std::clock() - start) / ((double) CLOCKS_PER_SEC)) * 1000000);
	EXPECT_GE((long) elapsedTime, (time * threadCount) - buffer);
	EXPECT_LE((long) elapsedTime, (time * threadCount) + buffer);

	EXPECT_EQ(sched_setaffinity(0, sizeof(first), &first), 0);
}

//tests the individual wait times of threads
TEST(UtilsTest, busyWaitIndivThreads) {
	int threadCount = 10;
	long time = 100000;
	//buffer based on how accurate busyWait needs to be while still passing the test
	long buffer = 10000;
	std::clock_t start = std::clock();
	//need to save the first affinity and set it back to that at the end
	cpu_set_t first;
	EXPECT_EQ(sched_getaffinity(0, sizeof(first), &first), 0);
	/
	cpu_set_t toRun;
	CPU_ZERO(&toRun);
	CPU_SET(0, &toRun);
	EXPECT_EQ(sched_setaffinity(0, sizeof(toRun), &toRun), 0);

	std::thread t[threadCount];
	
	for (int count = 0; count < threadCount; count++) {
		t[count] = std::thread(Utils::busyWait, time);
	}
	for (int count = 0; count < threadCount; count++) {
		std::clock_t start = std::clock();
		t[count].join();
		double elapsedTime = (double) (((std::clock() - start) / ((double) CLOCKS_PER_SEC)) * 1000000);
		EXPECT_GE((long) elapsedTime, time - buffer);
		EXPECT_LE((long) elapsedTime, time + buffer);
	}

	EXPECT_EQ(sched_setaffinity(0, sizeof(first), &first), 0);
}