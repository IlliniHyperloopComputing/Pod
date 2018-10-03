#include "Pod.h"
#include "Event.hpp"
#include "Simulator.hpp"

using namespace Utils;
class PodTest : public ::testing::Test
{
  protected:
  virtual void SetUp() {
    // Create the Pod object
    pod = make_shared<Pod>();

    // Set the Pod running in its own thread
    pod_thread = std::thread([&](){ pod->startup();});

    // We wait until pod->startup() is done
    pod->ready.wait();
   
    EXPECT_EQ(pod->state_machine->get_current_state(), Pod_State::E_States::ST_SAFE_MODE);
    simulator = std::make_shared<Simulator>(nullptr);
    EXPECT_TRUE(simulator->sim_connect("127.0.0.1", "8800"));//TODO make these variables somewhere
    NetworkManager::connected.wait();
    print(LogLevel::LOG_INFO, "Done setting up test, running test\n");
  }

  virtual void TearDown() {
    print(LogLevel::LOG_INFO, "Test finished, tearing down\n");
    simulator->disconnect();
    pod->stop();
    pod_thread.join();
  }

  /*
   * Handy helper function for testing state transitions
   * @param id the command to run
   * @param state the target state the command will bring you to
   * @param allow true if this transition should be allowed, false otherwises
   */
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

  void SendCommand(NetworkManager::Network_Command_ID id, uint8_t value) {

    auto command = std::make_shared<NetworkManager::Network_Command>();
    command->id = id;
    command->value = value;
    pod->processing_command.reset();
    EXPECT_TRUE(simulator->send_command(command));
    pod->processing_command.wait();
  }

  std::shared_ptr<Pod> pod;
  std::thread pod_thread;
  std::shared_ptr<Simulator> simulator;
};



