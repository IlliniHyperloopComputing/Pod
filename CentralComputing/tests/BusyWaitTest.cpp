#include "Utils.h"
#include "Event.hpp"
#include "Pod.h"
#include "Simulator.hpp"
#include <iostream>
#include <cstdio>
#include <sched.h>

//Test to make sure the busy wait takes as long as it should, given a small margin of error
TEST(BusyWaitTest, testWaiting) {

  int waitTime = 500;
  double marginOfError = 1.2;

  auto start = microseconds();

  //busWait for 100 microseconds
  BusyWait(waitTime);

  auto stop = microseconds();

  auto duration = (stop - start);

  print(LogLevel::LOG_DEBUG, "Busy Wait Time: %d\n", waitTime);
  print(LogLevel::LOG_DEBUG, "Duration: %lu\n", duration);
  //check to make sure that our busy wait of 100 microseconds took between 80 and 120 microseconds
  EXPECT_LE(duration, marginOfError * waitTime);
  EXPECT_GE(duration, waitTime);
}

//testing to make sure that busy wait behaves correctly by forcing non-parallelism (1 CPU and multipe threads)
TEST(BusyWaitTest, testNonParallel){
  int waitTime = 500;
  double marginOfError = 1.2;
  auto start = microseconds();
  constexpr unsigned num_threads = 4;
  int thread_count = 4;
  //saves our cpu affinity so that we can revert back after the test
  cpu_set_t original;
  sched_getaffinity(0, sizeof(original), &original);

  //creates a cpu mask that we will use for the test
  cpu_set_t mask;
  int status;

  //removes all CPUs from our current CPU set and then adds CPU 0 to our mask
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  ////changes the scheduler's affinity based on the mask we've created
  status = sched_setaffinity(0, sizeof(mask), &mask);

  //expect no error
  EXPECT_EQ(status, 0);

  //create our threads, calling busy wait each time
  std::thread t[num_threads];
  for (unsigned int i = 0; i < num_threads; ++i) {
      t[i] = std::thread(BusyWait, waitTime);
  }

  for (auto& thread : t) {
    thread.join();
  }

  auto stop = microseconds();

  auto duration = (stop - start);

  print(LogLevel::LOG_DEBUG, "Set Busy Wait Time: %d\n", waitTime * thread_count);
  print(LogLevel::LOG_DEBUG, "Duration: %lu\n", duration);

  //expect that this process took AT LEAST as long as all of the busy BusyWaiting
  //we can assume it didn't take longer than it should've from our first test
  EXPECT_GE(duration, waitTime * thread_count);
  EXPECT_LE(duration, marginOfError * (waitTime * thread_count));

  //reverts scheduler affinity back to normal, expecting no error
  status = sched_setaffinity(0, sizeof(original), &original);
  EXPECT_EQ(status, 0);
}
