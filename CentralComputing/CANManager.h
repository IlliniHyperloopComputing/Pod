#ifndef CANMANAGER_H_
#define CANMANAGER_H_

#include "SourceManagerBase.hpp"
#include "Defines.hpp"

class CANManager : public SourceManagerBase<CANData> {
 private:
  bool initialize_source();
  void stop_source();
  std::shared_ptr<CANData> refresh();
  std::shared_ptr<CANData> refresh_sim();
  void initialize_sensor_error_configs();
  void check_for_sensor_error(const std::shared_ptr<CANData> &);

  int32_t error_motor_ctrl_over_temp;
  int32_t error_motor_over_temp;
  int32_t error_dc_link_over_voltage; 
  int32_t error_dc_link_under_voltage; 
  int32_t error_motor_ctrl_logic_over_voltage; 
  int32_t error_motor_ctrl_logic_under_voltage; 

  int32_t error_cell_over_voltage;
  int32_t error_cell_under_voltage;
  int32_t error_cell_over_temp;
  int32_t error_battery_over_voltage;
  int32_t error_battery_under_voltage;
  int32_t error_battery_over_current;

  std::string name() {
    return "can";
  }

  // iterator for testing purposes, remove
  int i = 5;
};

#endif  // CANMANAGER_H_
