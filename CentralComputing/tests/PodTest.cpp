#include <../Pod.h>
#include "../Event.hpp"
#include <../Simulator.hpp>

using namespace Utils;
class PodTest : public ::testing::Test
{
  protected:
  virtual void SetUp() {
    pod = make_shared<Pod>();
    pod_thread = std::thread([&](){ pod->startup();});
    pod->ready.wait();
   
    EXPECT_EQ(pod->state_machine->get_current_state(), Pod_State::E_States::ST_SAFE_MODE);
    simulator = std::make_shared<Simulator>(nullptr);
    EXPECT_TRUE(simulator->sim_connect("127.0.0.1", "8800"));//TODO make these variables somewhere
    NetworkManager::connected.wait();
  }

  virtual void TearDown() {
    simulator->disconnect();
    pod->stop();
    pod_thread.join();
  }

  std::shared_ptr<Pod> pod;
  std::thread pod_thread;
  std::shared_ptr<Simulator> simulator;
};



