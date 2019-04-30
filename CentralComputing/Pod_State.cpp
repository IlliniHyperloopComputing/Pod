#include "Pod_State.h" 
#include "Pod.h"

using Utils::print;
using Utils::LogLevel;

Pod_State::Pod_State()
  : StateMachine(ST_MAX_STATES),
  motor(), brakes() {
  transition_map[Command::TRANS_SAFE_MODE] = &Pod_State::move_safe_mode;  
  transition_map[Command::TRANS_FUNCTIONAL_TEST] = &Pod_State::move_functional_tests;
  transition_map[Command::TRANS_LOADING] = &Pod_State::move_loading;
  transition_map[Command::TRANS_LAUNCH_READY] = &Pod_State::move_launch_ready;
  transition_map[Command::LAUNCH] = &Pod_State::accelerate;
  transition_map[Command::TRANS_FLIGHT_COAST] = &Pod_State::coast;
  transition_map[Command::TRANS_FLIGHT_BRAKE] = &Pod_State::brake;
  transition_map[Command::EMERGENCY_BRAKE] = &Pod_State::emergency_brake;
  transition_map[Command::TRANS_ERROR_STATE]= &Pod_State::abort;
  // non state transition commands
  transition_map[Command::ENABLE_MOTOR] = &Pod_State::no_transition;
  transition_map[Command::DISABLE_MOTOR] = &Pod_State::no_transition;
  transition_map[Command::SET_MOTOR_SPEED] = &Pod_State::no_transition;
  transition_map[Command::ENABLE_BRAKE] = &Pod_State::no_transition;
  transition_map[Command::DISABLE_BRAKE] = &Pod_State::no_transition;
  transition_map[Command::SET_ADC_ERROR] = &Pod_State::move_safe_mode_or_abort;
  transition_map[Command::SET_CAN_ERROR] = &Pod_State::move_safe_mode_or_abort;
  transition_map[Command::SET_I2C_ERROR] = &Pod_State::move_safe_mode_or_abort;
  transition_map[Command::SET_PRU_ERROR] = &Pod_State::move_safe_mode_or_abort;
  transition_map[Command::SET_NETWORK_ERROR] = &Pod_State::move_safe_mode_or_abort;
  transition_map[Command::SET_OTHER_ERROR] = &Pod_State::move_safe_mode_or_abort;
  transition_map[Command::CLR_ADC_ERROR] = &Pod_State::no_transition;
  transition_map[Command::CLR_CAN_ERROR] = &Pod_State::no_transition;
  transition_map[Command::CLR_I2C_ERROR] = &Pod_State::no_transition;
  transition_map[Command::CLR_PRU_ERROR] = &Pod_State::no_transition;
  transition_map[Command::CLR_NETWORK_ERROR] = &Pod_State::no_transition;
  transition_map[Command::CLR_OTHER_ERROR] = &Pod_State::no_transition;
  steady_state_map[ST_SAFE_MODE] = &Pod_State::steady_safe_mode;
  steady_state_map[ST_FUNCTIONAL_TEST] = &Pod_State::steady_functional;
  steady_state_map[ST_LOADING] = &Pod_State::steady_loading;
  steady_state_map[ST_LAUNCH_READY] = &Pod_State::steady_launch_ready;
  steady_state_map[ST_FLIGHT_ACCEL] = &Pod_State::steady_flight_accelerate;
  steady_state_map[ST_FLIGHT_COAST] = &Pod_State::steady_flight_coast;
  steady_state_map[ST_FLIGHT_BRAKE] = &Pod_State::steady_flight_brake;
  steady_state_map[ST_ERROR] = &Pod_State::steady_abort_state;

  if (!(ConfiguratorManager::config.getValue("acceleration_timeout", acceleration_timeout) && 
      ConfiguratorManager::config.getValue("coast_timeout", coast_timeout) &&
      ConfiguratorManager::config.getValue("brake_timeout", brake_timeout) &&
      ConfiguratorManager::config.getValue("estimated_brake_deceleration", estimated_brake_deceleration) &&
      ConfiguratorManager::config.getValue("length_of_track", length_of_track) &&
      ConfiguratorManager::config.getValue("brake_buffer_length", brake_buffer_length) &&
      ConfiguratorManager::config.getValue("not_moving_velocity", not_moving_velocity) &&
      ConfiguratorManager::config.getValue("not_moving_acceleration", not_moving_acceleration))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: POD_STATE: Missing necessary configuration\n");
    exit(1);
  }
}

// returns the current state as a E_States enum
E_States Pod_State::get_current_state() {
  return (E_States)StateMachine::getCurrentState();
}

/**
 * User controlled movement events
 * For each TRANSITION_MAP_ENTRY(_STATE_), 
 * We can enter the _STATE_ from the commemented state on the right
**/


/**
 * Can enter (transition into) FUNCTIONAL_TEST only from Safe Mode
 **/
void Pod_State::move_functional_tests() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(ST_FUNCTIONAL_TEST)      /* Safe Mode */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Functional test */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Loading */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Launch ready */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight accel */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight coast */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)
}

/**
 * Can enter (transition into) SAFE_MODE from: 
 * --Functional test
 * --Loading
 * --Launch Ready
 * --Flight Brake
 **/
void Pod_State::move_safe_mode() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Safe Mode */
    TRANSITION_MAP_ENTRY(ST_SAFE_MODE)      /* Functional test */
    TRANSITION_MAP_ENTRY(ST_SAFE_MODE)      /* Loading */
    TRANSITION_MAP_ENTRY(ST_SAFE_MODE)      /* Launch ready */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight accel */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight coast */
    TRANSITION_MAP_ENTRY(ST_SAFE_MODE)      /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)
}

void Pod_State::move_loading() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Safe Mode */
    TRANSITION_MAP_ENTRY(ST_LOADING)        /* Functional test */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Loading */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Launch ready */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight accel */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight coast */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)
}
void Pod_State::move_launch_ready() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Safe Mode */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Functional test */
    TRANSITION_MAP_ENTRY(ST_LAUNCH_READY)   /* Loading */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Launch ready */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight accel */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight coast */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)

  auto_transition_brake.reset();
  auto_transition_coast.reset();
}

// it is important all states should move to braking when this function is called, this is for emergencies
void Pod_State::emergency_brake() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)     /* Safe Mode */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)     /* Functional test */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)     /* Loading */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)     /* Launch ready */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)     /* Flight accel */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)     /* Flight coast */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)     /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)
}

/**
 * Software controlled events
 **/

void Pod_State::accelerate() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Safe Mode */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Functional test */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Loading */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_ACCEL)   /* Launch ready */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight accel */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight coast */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)
}

void Pod_State::coast() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Safe Mode */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Functional test */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Loading */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Launch ready */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_COAST)   /* Flight accel */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight coast */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)
}

void Pod_State::brake() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Safe Mode */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Functional test */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Loading */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Launch ready */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight accel */
    TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)   /* Flight coast */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)
}

void Pod_State::abort() {
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(ST_ERROR)        /* Safe Mode */
    TRANSITION_MAP_ENTRY(ST_ERROR)        /* Functional test */
    TRANSITION_MAP_ENTRY(ST_ERROR)        /* Loading */
    TRANSITION_MAP_ENTRY(ST_ERROR)        /* Launch ready */
    TRANSITION_MAP_ENTRY(ST_ERROR)        /* Flight accel */
    TRANSITION_MAP_ENTRY(ST_ERROR)        /* Flight coast */
    TRANSITION_MAP_ENTRY(ST_ERROR)        /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // Error State
  END_TRANSITION_MAP(NULL)  
}

void Pod_State::move_safe_mode_or_abort() {
  // Try moving to safemode first
  BEGIN_TRANSITION_MAP              /* Current state */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     /* Safe Mode */
    TRANSITION_MAP_ENTRY(ST_SAFE_MODE)      /* Functional test */
    TRANSITION_MAP_ENTRY(ST_SAFE_MODE)      /* Loading */
    TRANSITION_MAP_ENTRY(ST_SAFE_MODE)      /* Launch ready */
    TRANSITION_MAP_ENTRY(ST_ERROR)          /* Flight accel */
    TRANSITION_MAP_ENTRY(ST_ERROR)          /* Flight coast */
    TRANSITION_MAP_ENTRY(ST_ERROR)          /* Flight brake */
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // Error State
  END_TRANSITION_MAP(NULL)
}

void Pod_State::no_transition() {
  // do nothing
}

void Pod_State::ST_Safe_Mode() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
}

void Pod_State::ST_Functional_Test() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
}
void Pod_State::ST_Loading() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
}
void Pod_State::ST_Launch_Ready() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
}

void Pod_State::ST_Flight_Accel() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
  acceleration_start_time = microseconds();
  brakes.disable_brakes();
  motor.enable_motors();
  motor.set_throttle(100);
}

void Pod_State::ST_Flight_Coast() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
  coast_start_time = microseconds();
  motor.disable_motors();
}

void Pod_State::ST_Flight_Brake() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
  brake_start_time = microseconds();
  motor.disable_motors();
  brakes.enable_brakes();
}

void Pod_State::ST_Error() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
  motor.disable_motors();
  // enable brakes;
}

/////////////////////////////
// STEADY STATE FUNCTIONS //
///////////////////////////
void Pod_State::steady_safe_mode(Command::Network_Command * command, 
                                  std::shared_ptr<UnifiedState> state) {
  // not much special stuff to do here  
}

void Pod_State::steady_functional(Command::Network_Command * command, 
                                  std::shared_ptr<UnifiedState> state) {
  // process command, let manual commands go through
  switch (command->id) {
    case Command::ENABLE_MOTOR: 
      motor.enable_motors();
      break;
    case Command::DISABLE_MOTOR:
      motor.disable_motors();
      break;
    case Command::SET_MOTOR_SPEED:
      motor.set_throttle(command->value); 
      break;
    case Command::ENABLE_BRAKE:
      // activate brakes
      brakes.enable_brakes();
      break;
    case Command::DISABLE_BRAKE:
      // deactivate brakes
      brakes.disable_brakes();
      break;
    default:
      break;
  }
}

void Pod_State::steady_loading(Command::Network_Command * command, 
                                std::shared_ptr<UnifiedState> state) {
}

void Pod_State::steady_launch_ready(Command::Network_Command * command, 
                                    std::shared_ptr<UnifiedState> state) {
}

void Pod_State::steady_flight_accelerate(Command::Network_Command * command, 
                                        std::shared_ptr<UnifiedState> state) {
  // Access Pos, Vel, and Accel from Motion Model
  int32_t pos = state->motion_data->x[0];
  int32_t vel = state->motion_data->x[1];
  int32_t acc = state->motion_data->x[2];
  int64_t timeout_check = microseconds() - acceleration_start_time;
  
  // Transition if kinematics demand it, or we exceed our timeout
  if (shouldBrake(vel, pos) || timeout_check >= acceleration_timeout) {
    Command::put(Command::Network_Command_ID::TRANS_FLIGHT_COAST, 0);
    auto_transition_coast.invoke();
  }
}

void Pod_State::steady_flight_coast(Command::Network_Command * command, 
                                    std::shared_ptr<UnifiedState> state) {
  // Transition after we exceed our timeout
  int64_t timeout_check = microseconds() - coast_start_time;
  if (timeout_check >= coast_timeout) {
    Command::put(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, 0);
    auto_transition_brake.invoke();
  }
}

void Pod_State::steady_flight_brake(Command::Network_Command * command, 
                                    std::shared_ptr<UnifiedState> state) {
  int32_t acc = state->motion_data->x[2];
  int32_t vel = state->motion_data->x[1];
  int64_t timeout_check = microseconds() - brake_start_time;

  // Transition after we exceed our timeout AND acceleration AND Velocity are under a configurable value.
  if (std::abs(acc) < not_moving_velocity 
      && std::abs(vel) < not_moving_velocity 
      && timeout_check >= brake_timeout) {
    Command::put(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, 0);
    auto_transition_brake.invoke();
  }
}

bool Pod_State::shouldBrake(int64_t vel, int64_t pos) {
  int64_t target_distance = length_of_track - brake_buffer_length;
  int64_t stopping_distance = pos + (vel * vel) / (2*estimated_brake_deceleration);

  if (stopping_distance >= target_distance) {
    print(LogLevel::LOG_INFO, "Pod Should Brake, vel: %.2f pos: %.2f\n", vel, pos);
    return true;
  } else {
    return false;
  }
}

void Pod_State::steady_abort_state(Command::Network_Command * command, 
                                    std::shared_ptr<UnifiedState> state) {
}
