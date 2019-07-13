#include "ADCManager.h"

bool ADCManager::initialize_source() {
  if (!(ConfiguratorManager::config.getValue("adc_filename", fileName) &&
        ConfiguratorManager::config.getValue("adc_calc_zero_g_timeout", calculate_zero_g_timeout) &&
        ConfiguratorManager::config.getValue("adc_default_zero_g", default_zero_g))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: ADC: Missing necessary configuration\n");
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_SETUP_FAILURE);
    exit(1);  // Crash hard on this error
  }

  do_calculate_zero_g = false;  // By default, just use the default values
  calculate_zero_g_time = 0;
  accel1_zero_g = default_zero_g;  // Set the defaults
  accel2_zero_g = default_zero_g;

  // Open the ADC file
  inFile.open(fileName, std::ifstream::in | std::ifstream::binary);
  if (!inFile) {
    print(LogLevel::LOG_DEBUG, "ADC Manager setup failed\n");
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_SETUP_FAILURE);
    return false;
  } else {
    print(LogLevel::LOG_DEBUG, "ADC Manager setup successful\n");
    return true;
  }
}

void ADCManager::stop_source() {
  if (inFile) {
    inFile.close();
    print(LogLevel::LOG_DEBUG, "ADC Manager stopped\n");
  } 
}

void ADCManager::calculate_zero_g() {
  do_calculate_zero_g = true;
  zero_g_sum[0] = 0;  // Zero these values used in calculating
  zero_g_sum[1] = 0;
  zero_g_num_samples = 0;
  calculate_zero_g_time = Utils::microseconds();
}

std::shared_ptr<ADCData> ADCManager::refresh() {
  std::shared_ptr<ADCData> new_data = std::make_shared<ADCData>();
  uint16_t buffer[NUM_ADC];
  inFile.read(reinterpret_cast<char *>(buffer), NUM_ADC * 2);
  if (!inFile) {
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_READ_ERROR);
  }
  for (int i = 0; i < NUM_ADC; i++) {
    uint16_t * val = (buffer + i);
    new_data -> data[i] = (*val);
  }
  //  print(Utils::LOG_ERROR, "%d \t%d\t%d\t %d\t%d\t%d\t%d\t\n", new_data->data[0], 
  //                          new_data->data[1], new_data->data[2], 
  //                          new_data->data[3], new_data->data[4],
  //                          new_data->data[5], new_data->data[6]);

  if (do_calculate_zero_g) { 
    if ((calculate_zero_g_time + calculate_zero_g_timeout) > Utils::microseconds()) {
      zero_g_sum[0] += new_data->data[0];  
      zero_g_sum[1] += new_data->data[1];
      zero_g_num_samples++;
    } else {  // Time is up, time to calculate
      accel1_zero_g = zero_g_sum[0] / zero_g_num_samples; 
      accel2_zero_g = zero_g_sum[1] / zero_g_num_samples;
      do_calculate_zero_g = false;
      print(Utils::LOG_DEBUG, "ADC - Accel1 zero g %d\n", accel1_zero_g);
      print(Utils::LOG_DEBUG, "ADC - Accel2 zero g %d\n", accel2_zero_g);
    }
  }

  // Apply the zero g location to each of the accelerometer's data
  new_data -> data[0] -= accel1_zero_g;
  new_data -> data[1] -= accel2_zero_g;

  return new_data;
}

std::shared_ptr<ADCData> ADCManager::refresh_sim() {
  #ifdef SIM
  return SimulatorManager::sim.sim_get_adc();
  #else
  return empty_data();
  #endif
}

void ADCManager::initialize_sensor_error_configs() {
  if (!(ConfiguratorManager::config.getValue("error_accel_diff", error_accel_diff) && 
      ConfiguratorManager::config.getValue("error_pneumatic_1_over_pressure", error_pneumatic_1_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_2_over_pressure", error_pneumatic_2_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_3_over_pressure", error_pneumatic_3_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_4_over_pressure", error_pneumatic_4_over_pressure) &&
      ConfiguratorManager::config.getValue("error_battery_box_over_pressure",  error_battery_box_over_pressure) &&
      ConfiguratorManager::config.getValue("error_battery_box_under_pressure", error_battery_box_under_pressure))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: ADCManager Missing necessary configuration\n");
    exit(1);
  }
}

void ADCManager::check_for_sensor_error(const std::shared_ptr<ADCData> & check_data, E_States state) {
  //Just hardcoding and using difference for accelerometers for now
  int32_t* adc_data = check_data->data;
  if (abs(adc_data[0] - adc_data[1]) > error_accel_diff) {
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_ACCEL_DIFF_ERROR);
  }
  if (adc_data[2] > error_pneumatic_1_over_pressure) {
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_PNEUMATIC_OVER_PRESSURE_ERROR_1);
  }
  if (adc_data[3] > error_pneumatic_2_over_pressure) {
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_PNEUMATIC_OVER_PRESSURE_ERROR_2);
  }
  if (adc_data[4] > error_pneumatic_3_over_pressure) {
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_PNEUMATIC_OVER_PRESSURE_ERROR_3);
  }
  if (adc_data[5] > error_pneumatic_4_over_pressure) {
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_PNEUMATIC_OVER_PRESSURE_ERROR_4);
  }
  if (adc_data[6] > error_battery_box_over_pressure) {
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_BATTERY_BOX_OVER_PRESSURE_ERROR);
  }
  if (adc_data[6] < error_battery_box_under_pressure) {
    Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_BATTERY_BOX_UNDER_PRESSURE_ERROR);
  }
}
