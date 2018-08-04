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

  void MoveState(NetworkManager::Network_Command_ID id, Pod_State::E_States state, bool allow) {
    auto command = std::make_shared<NetworkManager::Network_Command>();
    command->id = id;
    command->value = 0;
    Pod_State::E_States start_state = pod->state_machine->get_current_state();
    pod->processing_command.reset();
    EXPECT_TRUE(simulator->send_command(command));
    pod->processing_command.wait();
    if(allow) {
      EXPECT_EQ(pod->state_machine->get_current_state(), state);
    } else {
      EXPECT_EQ(pod->state_machine->get_current_state(), start_state);
    }
  }

  std::shared_ptr<Pod> pod;
  std::thread pod_thread;
  std::shared_ptr<Simulator> simulator;
};



