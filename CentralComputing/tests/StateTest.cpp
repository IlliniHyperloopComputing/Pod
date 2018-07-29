#include <gtest/gtest.h>
#include <../Pod.h>
#include <../Simulator.hpp>

using namespace Utils;

TEST(StateTest, Startup) {
  std::shared_ptr<Pod> pod = make_shared<Pod>();
  std::thread pod_thread([&](){ pod->startup(); });
  sem_wait(&pod->ready_semaphore);
 
  EXPECT_EQ(pod->state_machine->get_current_state(), Pod_State::E_States::ST_SAFE_MODE);

  Simulator simulator(nullptr);
  pod->stop();
  //EXPECT_TRUE(simulator.sim_connect("127.0.0.1", "8800"));
    
  //usleep(10000);
  pod_thread.join();
}
