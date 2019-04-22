#ifdef SIM // Only compile if building test executable
#include "Pod.h"
#include "Simulator.h"

using namespace Utils;

class PodTest : public ::testing::Test
{
  protected:
  virtual void SetUp() {
    // Reset the condition we use to determine when the Pod is connected
    TCPManager::connected.reset();
    UDPManager::setup.reset();

    // Startup our server
    EXPECT_TRUE(SimulatorManager::sim.start_server("127.0.0.1", "8001") >= 0);

    // Set the Simulator server running in its own thread
    sim_thread = std::thread([&](){ SimulatorManager::sim.sim_connect();});

    // Create the Pod object
    pod = std::make_shared<Pod>(podtest_global::config_to_open);

    // Set the Pod running in its own thread
    pod_thread = std::thread([&](){ pod->run();});

    // We wait until pod->startup() is done
    pod->ready.wait();

    // Reset error timeout flags
    for (int i = 0; i < 8*6; i++) {
        Command::error_flag_timers[i] = -1000000;  // negative 1 second. 
    }
   
    EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_SAFE_MODE);

    // We wait until the Pod has connected, and we see connected on the Simulator side
    // Need both of these because otherwise there were occasionally problems -- 
    // the pod must have connected by it wasn't registered by the simulation. 
    // then when the simulation tried to send data it failed.
    UDPManager::setup.wait();
    TCPManager::connected.wait();
    SimulatorManager::sim.connected.wait();

    print(LogLevel::LOG_DEBUG, "Sim - Setup finished, running test\n");
  }

  virtual void TearDown() {
    print(LogLevel::LOG_DEBUG, "Test finished, begin teardown\n");
    SimulatorManager::sim.stop();
    sim_thread.join();
    pod->trigger_shutdown();
    pod_thread.join();
    print(LogLevel::LOG_DEBUG, "Test teardown complete\n");
  }

  
  /*
   * Helper function to send a command using the simulator
   * @param id the id of the command to run
   * @param value the value for the command
   */
  void SendCommand(Command::Network_Command_ID id, uint8_t value) {

    auto command = std::make_shared<Command::Network_Command>();
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
  void MoveState(Command::Network_Command_ID id, E_States state, bool allow) {

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
  std::thread sim_thread;
};
#endif
