#include <gtest/gtest.h>
#include <../Pod.h>

TEST(StateTest, Startup) {
  std::shared_ptr<Pod> pod = make_shared<Pod>();
  std::thread pod_thread([&](){ pod->startup(); });
  EXPECT_EQ(pod->state_machine->get_current_state(), Pod_State::E_States::ST_SAFE_MODE);
  pod_thread.join();
}
