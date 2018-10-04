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
    EXPECT_TRUE(SimulatorManager::sim.sim_connect("127.0.0.1", "8800"));//TODO make these variables somewhere
    NetworkManager::connected.wait();
    print(LogLevel::LOG_INFO, "Done setting up test, running test\n");
  }

  virtual void TearDown() {
    print(LogLevel::LOG_INFO, "Test finished, tearing down\n");
    SimulatorManager::sim.disconnect();
    pod->stop();
    pod_thread.join();
  }

  
  /*
   * Helper function to send a command using the simulator
   * @param id the id of the command to run
   * @param value the value for the command
   */
  void SendCommand(NetworkManager::Network_Command_ID id, uint8_t value) {

    auto command = std::make_shared<NetworkManager::Network_Command>();
    command->id = id;
    command->value = value;
    pod->processing_command.reset();
    EXPECT_TRUE(SimulatorManager::sim.send_command(command));
    pod->processing_command.wait();
  }
  /*
   * Handy helper function for testing state transitions
   * @param id the command to run
   * @param state the target state the command will bring you to
   * @param allow true if this transition should be allowed, false otherwises
   */
  void MoveState(NetworkManager::Network_Command_ID id, Pod_State::E_States state, bool allow) {

    auto start_state = pod->state_machine->get_current_state();
    SendCommand(id, 0);

    if(allow) {
      EXPECT_EQ(pod->state_machine->get_current_state(), state);
    } else {
      EXPECT_EQ(pod->state_machine->get_current_state(), start_state);
    }
  }


  std::shared_ptr<Pod> pod;
  std::thread pod_thread;
};



