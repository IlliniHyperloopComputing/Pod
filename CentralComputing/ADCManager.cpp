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
  accel_diff_counter = 0;
  calculate_zero_g_time = 0;
  accel1_zero_g = default_zero_g;  // Set the defaults
  accel2_zero_g = default_zero_g;
  adc0_san_positive_counter = 0;
  adc0_san_negative_counter = 0;
  adc1_san_positive_counter = 0;
  adc1_san_negative_counter = 0;

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
      zero_g_sum[0] += new_data->data[adc_axis_0];  
      zero_g_sum[1] += new_data->data[adc_axis_1];
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
  new_data -> data[adc_axis_0] = adc_dir_flip * (new_data -> data[adc_axis_0] - accel1_zero_g);
  new_data -> data[adc_axis_1] = adc_dir_flip * (new_data -> data[adc_axis_1] - accel2_zero_g);

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
      ConfiguratorManager::config.getValue("error_accel_x_over", error_accel_x_over) &&
      ConfiguratorManager::config.getValue("error_accel_x_under", error_accel_x_under) &&
      ConfiguratorManager::config.getValue("error_accel_y_over", error_accel_y_over) &&
      ConfiguratorManager::config.getValue("error_accel_y_under", error_accel_y_under) &&
      ConfiguratorManager::config.getValue("error_accel_z_over", error_accel_z_over) &&
      ConfiguratorManager::config.getValue("error_accel_z_under", error_accel_z_under) &&
      ConfiguratorManager::config.getValue("adc_axis_0", adc_axis_0) &&
      ConfiguratorManager::config.getValue("adc_axis_1", adc_axis_1) &&
      ConfiguratorManager::config.getValue("adc_dir_flip", adc_dir_flip) &&
      ConfiguratorManager::config.getValue("adc_sanity_bound_positive", adc_san_positive) &&
      ConfiguratorManager::config.getValue("adc_sanity_bound_negative", adc_san_negative) &&
      ConfiguratorManager::config.getValue("adc_sanity_bound_counter_error", adc_san_counter_error) &&
      ConfiguratorManager::config.getValue("accel_diff_counter_error",accel_diff_counter_error))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: ADCManager Missing necessary configuration\n");
    exit(1);
  }
}

void ADCManager::check_for_sensor_error(const std::shared_ptr<ADCData> & check_data, E_States state) {
  //Just hardcoding and using difference for accelerometers for now

  int32_t* adc_data = check_data->data;
  // If we are in a flight state, check if accelerometers are in error
  if (state == E_States::ST_FLIGHT_ACCEL ||
      state == E_States::ST_FLIGHT_BRAKE || 
      state == E_States::ST_FLIGHT_COAST) {
    if (abs(adc_data[adc_axis_0] - adc_data[adc_axis_1]) >= error_accel_diff) {
      accel_diff_counter++;
      if (accel_diff_counter > accel_diff_counter_error) {
        Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR,ADCErrors::ADC_ACCEL_DIFF_ERROR);
      }
    } else {
      accel_diff_counter = 0;
    }
  }

  // POSITIVE SANITY ERRORS
  if (adc_data[adc_axis_0] > adc_san_positive) {
    adc0_san_positive_counter++;
    if(adc0_san_positive_counter > adc_san_counter_error) {
      Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR, ADCErrors::ADC_POSITIVE_SANITY_ERROR);
    }
  } else {
    adc0_san_positive_counter = 0;
  }
  if (adc_data[adc_axis_1] > adc_san_positive) {
    adc1_san_positive_counter++;
    if(adc1_san_positive_counter > adc_san_counter_error) {
      Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR, ADCErrors::ADC_POSITIVE_SANITY_ERROR);
    }
  } else {
    adc1_san_positive_counter = 0;
  }

  // NEGATIVE SANITY ERRORS
  if (adc_data[adc_axis_0] < adc_san_negative) {
    adc0_san_negative_counter++;
    if(adc0_san_negative_counter > adc_san_counter_error) {
      Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR, ADCErrors::ADC_NEGATIVE_SANITY_ERROR);
    }
  } else {
    adc0_san_negative_counter = 0;
  }
  if (adc_data[adc_axis_1] < adc_san_negative) {
    adc1_san_negative_counter++;
    if(adc1_san_negative_counter > adc_san_counter_error) {
      Command::set_error_flag(Command::Network_Command_ID::SET_ADC_ERROR, ADCErrors::ADC_NEGATIVE_SANITY_ERROR);
    }
  } else {
    adc1_san_negative_counter = 0;
  }

  /*
  // NO LONGER CHECKS PRESSURE
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
  */
}
