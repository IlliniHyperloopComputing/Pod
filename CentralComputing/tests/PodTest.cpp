#ifdef SIM // Only compile if building test executable
#include "Pod.h"
#include "Simulator.h"

using namespace Utils;
using std::string;

class PodTest : public ::testing::Test
{
  protected:
  virtual void SetUp() {
    // Reset the condition we use to determine when the Pod is connected
    TCPManager::connected.reset();
    UDPManager::setup.reset();

    // Create the Pod object
    // This will load the configuration manager, so we can use those config values for
    //    starting up our simulation tcp and udp servers
    pod = std::make_shared<Pod>(podtest_global::config_to_open);

    // Grab these configuration values for the network setup
    string tcp_port, tcp_addr, udp_send, udp_recv, udp_addr;
    EXPECT_TRUE(ConfiguratorManager::config.getValue("tcp_port", tcp_port));
    EXPECT_TRUE(ConfiguratorManager::config.getValue("tcp_addr", tcp_addr));
    EXPECT_TRUE(ConfiguratorManager::config.getValue("udp_send_port", udp_send));
    EXPECT_TRUE(ConfiguratorManager::config.getValue("udp_recv_port", udp_recv));
    EXPECT_TRUE(ConfiguratorManager::config.getValue("udp_addr", udp_addr));

    // Startup our simulation TCP server
    EXPECT_TRUE(SimulatorManager::sim.start_server_tcp(tcp_addr.c_str(), tcp_port.c_str()) >= 0);

    // Startup our simulation UDP heartbeat 
    // send and recv ports are flipped 
    EXPECT_TRUE(SimulatorManager::sim.start_udp(udp_addr.c_str(), udp_recv.c_str(), udp_send.c_str()) >= 0);

    // Set the Simulator server running in its own thread
    sim_tcp_thread = std::thread([&](){ SimulatorManager::sim.sim_connect_tcp();});

    // Reset these events before we have the pod run
    pod->processing_error.reset();
    pod->tcp_fully_setup.reset(); 
    pod->udp_fully_setup.reset(); 

    // Set the Pod running in its own thread
    pod_thread = std::thread([&](){ pod->run();});

    // TCP
    // We wait until the Pod has connected, and we see connected on the Simulator side
    // Need both of these because otherwise there were occasionally problems -- 
    // the pod must have connected by it wasn't registered by the simulation. 
    // then when the simulation tried to send data it failed.
    TCPManager::connected.wait();
    pod->processing_error.wait();  // Wait for signal that we have processed the CLR_NETWORK_ERROR 
    SimulatorManager::sim.connected_tcp.wait();
    pod->processing_error.reset();  // We are going to process another event.
    pod->tcp_fully_setup.invoke();  // Tell the pod to go-ahead and start UDP

    // UDP
    // Wait for the Pod's UDP manager to finish its setup. 
    // Then set our simulator UDP thread running, which will start the heartbeat
    // Then wait for the logic_loop() to process the CLR_NETWORK_ERROR issued once the pod's udpmanager is connected
    // Then wait for our simulator to inicate that it thinks we are connected
    UDPManager::setup.wait();
    sim_udp_thread = std::thread([&](){ SimulatorManager::sim.sim_connect_udp();});
    pod->processing_error.wait();  // Wait for signal that we have processed the CLR_NETWORK_ERROR 
    SimulatorManager::sim.connected_udp.wait();
    pod->udp_fully_setup.invoke();  // Tell the pod to go-ahead 

    EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_SAFE_MODE);

    // We wait until pod->startup() is done
    pod->ready.wait();

    // Reset error timeout flags
    for (int i = 0; i < 8*6; i++) {
        Command::error_flag_timers[i] = -1000000;  // negative 1 second. 
    }

    print(LogLevel::LOG_DEBUG, "Sim - Setup finished, running test\n");
  }

  virtual void TearDown() {
    print(LogLevel::LOG_DEBUG, "Sim - Test finished, begin teardown\n");
    SimulatorManager::sim.stop();
    sim_tcp_thread.join();
    sim_udp_thread.join();
    pod->trigger_shutdown();
    pod_thread.join();
    Command::flush();  // Flush command queue
    print(LogLevel::LOG_DEBUG, "Sim - Test teardown complete\n");
  }

  
  /*
   * Helper function to send a command using the simulator
   * @param id the id of the command to run
   * @param value the value for the command
   */
  void SendCommand(Command::Network_Command_ID id, uint32_t value) {

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
  std::thread sim_tcp_thread;
  std::thread sim_udp_thread;
};
#endif
