#ifdef SIM // Only compile if building test executable
#include "Utils.h"
#include "Pod.h"
#include "Event.h"
#include "Simulator.h"
#include <ctime>
#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <fstream>
#include <string>

using Utils::print;
using Utils::LogLevel;
using namespace Utils;

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
	constexpr int threadCount = 10;
	long time = 100000;
	//buffer based on how accurate busyWait needs to be while still passing the test
	long buffer = 20000;
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
	constexpr int threadCount = 10;
	long time = 100000;
	//buffer based on how accurate busyWait needs to be while still passing the test
	long buffer = 15000;
	//need to save the first affinity and set it back to that at the end
	cpu_set_t first;
	EXPECT_EQ(sched_getaffinity(0, sizeof(first), &first), 0);

	cpu_set_t toRun;
	CPU_ZERO(&toRun);
	CPU_SET(0, &toRun);
	EXPECT_EQ(sched_setaffinity(0, sizeof(toRun), &toRun), 0);

	std::thread t[threadCount];
	for (int count = 0; count < threadCount; count++) {
		std::clock_t start = std::clock();
		t[count] = std::thread(Utils::busyWait, time);
		t[count].join();
		double elapsedTime = (double) (((std::clock() - start) / ((double) CLOCKS_PER_SEC)) * 1000000);
		EXPECT_GE((long) elapsedTime, time - buffer);
		EXPECT_LE((long) elapsedTime, time + buffer);
	}

	EXPECT_EQ(sched_setaffinity(0, sizeof(first), &first), 0);
}


//Test should work on BBB. Otherwise returns true
TEST(UtilsTest, GPIOToggleTest) {   

  #ifdef BBB
	std::string zero = "0\n";
	std::string one = "1\n";
	std::string start = "/sys/class/gpio/gpio";
	std::string integer = std::to_string(HEARTBEAT_GPIO);
	std::string end = "/value";
	std::string path = start + integer + end;
	Utils::set_GPIO(HEARTBEAT_GPIO, false);
	std::ifstream ifs(path);
	std::string content;
	content.assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
	ifs.close();
	EXPECT_EQ(content, zero);
	Utils::set_GPIO(HEARTBEAT_GPIO, true);
	ifs.open(path);
	content.assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
	ifs.close();
	EXPECT_EQ(content, one);

  #else 
  print(LogLevel::LOG_ERROR, "Test only works on BBB. Run on BBB\n");
  EXPECT_EQ(true, true);
  #endif

}

#endif
