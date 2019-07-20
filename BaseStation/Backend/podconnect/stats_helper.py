from . import models

def getStats():
    state_data = models.State.objects.latest("date_time")
    can_data = models.CANData.objects.latest("date_time")
    error_data = models.Errors.objects.latest("date_time")
    i2c_data = models.I2CData.objects.latest("date_time")
    pru_data = models.PRUData.objects.latest("date_time")
    motion_data = models.MotionData.objects.latest("date_time")
    adc_data = models.ADCData.objects.latest("date_time")

    ranges = getRanges()
    can_data_motor = {
        "stats": [
            {
                "name": "State_Number",
                "value": str(state_data.state),
                "color": getColor(ranges["State_Number"], state_data.state),
                "low":  ranges["State_Number"][0],
                "high": ranges["State_Number"][1],
                "units":ranges["State_Number"][2]
            },
            {
                "name": "Status_Word",
                "value": str(hex(can_data.status_word)),
                "color": getColor(ranges["Status_Word"], can_data.status_word),
                "low":  ranges["Status_Word"][0],
                "high": ranges["Status_Word"][1],
                "units":ranges["Status_Word"][2]
            },
            {
                "name": "Position_Val",
                "value": str(can_data.position_val),
                "color": getColor(ranges["Position_Val"], state_data.state),
                "low":  ranges["Position_Val"][0],
                "high": ranges["Position_Val"][1],
                "units":ranges["Position_Val"][2]
            },
            {
                "name": "Torque_Val",
                "value": str(can_data.torque_val),
                "color": getColor(ranges["Torque_Val"], state_data.state),
                "low":  ranges["Torque_Val"][0],
                "high": ranges["Torque_Val"][1],
                "units":ranges["Torque_Val"][2]
            },
            {
                "name": "Controller_Temp",
                "value": str(can_data.controller_temp),
                "color": getColor(ranges["Controller_Temp"], state_data.state),
                "low":  ranges["Controller_Temp"][0],
                "high": ranges["Controller_Temp"][1],
                "units":ranges["Controller_Temp"][2]
            },
            {
                "name": "Motor_Temp",
                "value": str(can_data.motor_temp),
                "color": getColor(ranges["Motor_Temp"], state_data.state),
                "low":  ranges["Motor_Temp"][0],
                "high": ranges["Motor_Temp"][1],
                "units":ranges["Motor_Temp"][2]
            },
            {
                "name": "DC_Link_Voltage",
                "value": str(can_data.dc_link_voltage),
                "color": getColor(ranges["DC_Link_Voltage"], state_data.state),
                "low":  ranges["DC_Link_Voltage"][0],
                "high": ranges["DC_Link_Voltage"][1],
                "units":ranges["DC_Link_Voltage"][2]
            },
            {
                "name": "Logic_Power_Supply_Voltage",
                "value": str(can_data.logic_power_supply_voltage),
                "color": getColor(ranges["Logic_Power_Supply_Voltage"], state_data.state),
                "low":  ranges["Logic_Power_Supply_Voltage"][0],
                "high": ranges["Logic_Power_Supply_Voltage"][1],
                "units":ranges["Logic_Power_Supply_Voltage"][2]
            },
            {
                "name": "Current_Demand",
                "value": str(can_data.current_demand),
                "color": getColor(ranges["Current_Demand"], state_data.state),
                "low":  ranges["Current_Demand"][0],
                "high": ranges["Current_Demand"][1],
                "units":ranges["Current_Demand"][2]
            },
            {
                "name": "Motor_Current_Val",
                "value": str(can_data.motor_current_val),
                "color": getColor(ranges["Motor_Current_Val"], state_data.state),
                "low":  ranges["Motor_Current_Val"][0],
                "high": ranges["Motor_Current_Val"][1],
                "units":ranges["Motor_Current_Val"][2]
            },
            {
                "name": "Electrical_Angle",
                "value": str(can_data.electrical_angle),
                "color": getColor(ranges["Electrical_Angle"], state_data.state),
                "low":  ranges["Electrical_Angle"][0],
                "high": ranges["Electrical_Angle"][1],
                "units":ranges["Electrical_Angle"][2]
            },
            {
                "name": "Phase_A_Current",
                "value": str(can_data.phase_a_current),
                "color": getColor(ranges["Phase_A_Current"], state_data.state),
                "low":  ranges["Phase_A_Current"][0],
                "high": ranges["Phase_A_Current"][1],
                "units":ranges["Phase_A_Current"][2]
            },
            {
                "name": "Phase_B_Current",
                "value": str(can_data.phase_b_current),
                "color": getColor(ranges["Phase_B_Current"], state_data.state),
                "low":  ranges["Phase_B_Current"][0],
                "high": ranges["Phase_B_Current"][1],
                "units":ranges["Phase_B_Current"][2]
            },
            {
                "name": "Accel_1 x",
                "value": str(adc_data.data_5),
                "color": getColor(ranges["Accel_1 x"], state_data.state),
                "low":  ranges["Accel_1 x"][0],
                "high": ranges["Accel_1 x"][1],
                "units":ranges["Accel_1 x"][2]
            },
            {
                "name": "Accel_1 y",
                "value": str(adc_data.data_6),
                "color": getColor(ranges["Accel_1 y"], state_data.state),
                "low":  ranges["Accel_1 y"][0],
                "high": ranges["Accel_1 y"][1],
                "units":ranges["Accel_1 y"][2]
            },
            {
                "name": "Accel_1 z",
                "value": str(adc_data.data_4),
                "color": getColor(ranges["Accel_1 z"], state_data.state),
                "low":  ranges["Accel_1 z"][0],
                "high": ranges["Accel_1 z"][1],
                "units":ranges["Accel_1 z"][2]
            },
            {
                "name": "Accel_2 x",
                "value": str(adc_data.data_1),
                "color": getColor(ranges["Accel_2 x"], state_data.state),
                "low":  ranges["Accel_2 x"][0],
                "high": ranges["Accel_2 x"][1],
                "units":ranges["Accel_2 x"][2]
            },
            {
                "name": "Accel_2 y",
                "value": str(adc_data.data_3),
                "color": getColor(ranges["Accel_2 y"], state_data.state),
                "low":  ranges["Accel_2 y"][0],
                "high": ranges["Accel_2 y"][1],
                "units":ranges["Accel_2 y"][2]
            },
            {
                "name": "Accel_2 z",
                "value": str(adc_data.data_2),
                "color": getColor(ranges["Accel_2 z"], state_data.state),
                "low":  ranges["Accel_2 z"][0],
                "high": ranges["Accel_2 z"][1],
                "units":ranges["Accel_2 z"][2]
            }
        ]
    }
    can_data_bms1 = {
        "stats": [
            {
                "name": "Internal_Relay_State",
                "value": str(hex(can_data.internal_relay_state)),
                "color": getColor(ranges["Internal_Relay_State"], state_data.state),
                "low":  ranges["Internal_Relay_State"][0],
                "high": ranges["Internal_Relay_State"][1],
                "units":ranges["Internal_Relay_State"][2]
            },
            {
                "name": "Relay_State",
                "value": str(hex(can_data.relay_state)),
                "color": getColor(ranges["Relay_State"], state_data.state),
                "low":  ranges["Relay_State"][0],
                "high": ranges["Relay_State"][1],
                "units":ranges["Relay_State"][2]
            },
            {
                "name": "Rolling_Counter",
                "value": str(can_data.rolling_counter),
                "color": getColor(ranges["Rolling_Counter"], state_data.state),
                "low":  ranges["Rolling_Counter"][0],
                "high": ranges["Rolling_Counter"][1],
                "units":ranges["Rolling_Counter"][2]
            },
            {
                "name": "Fail_Safe_State",
                "value": str(hex(can_data.fail_safe_state)),
                "color": getColor(ranges["Fail_Safe_State"], state_data.state),
                "low":  ranges["Fail_Safe_State"][0],
                "high": ranges["Fail_Safe_State"][1],
                "units":ranges["Fail_Safe_State"][2]
            },
            {
                "name": "Peak_Current",
                "value": str(can_data.peak_current),
                "color": getColor(ranges["Peak_Current"], state_data.state),
                "low":  ranges["Peak_Current"][0],
                "high": ranges["Peak_Current"][1],
                "units":ranges["Peak_Current"][2]
            },
            {
                "name": "Pack_Voltage_Inst",
                "value": str(can_data.pack_voltage_inst),
                "color": getColor(ranges["Pack_Voltage_Inst"], state_data.state),
                "low":  ranges["Pack_Voltage_Inst"][0],
                "high": ranges["Pack_Voltage_Inst"][1],
                "units":ranges["Pack_Voltage_Inst"][2]
            },
            {
                "name": "Pack_Voltage_Open",
                "value": str(can_data.pack_voltage_open),
                "color": getColor(ranges["Pack_Voltage_Open"], state_data.state),
                "low":  ranges["Pack_Voltage_Open"][0],
                "high": ranges["Pack_Voltage_Open"][1],
                "units":ranges["Pack_Voltage_Open"][2]
            },
            {
                "name": "Pack_SOC",
                "value": str(can_data.pack_soc),
                "color": getColor(ranges["Pack_SOC"], state_data.state),
                "low":  ranges["Pack_SOC"][0],
                "high": ranges["Pack_SOC"][1],
                "units":ranges["Pack_SOC"][2]
            },
            {
                "name": "Pack_Amphours",
                "value": str(can_data.pack_amphours),
                "color": getColor(ranges["Pack_Amphours"], state_data.state),
                "low":  ranges["Pack_Amphours"][0],
                "high": ranges["Pack_Amphours"][1],
                "units":ranges["Pack_Amphours"][2]
            },
            {
                "name": "Pack_Resistance",
                "value": str(can_data.pack_resistance),
                "color": getColor(ranges["Pack_Resistance"], state_data.state),
                "low":  ranges["Pack_Resistance"][0],
                "high": ranges["Pack_Resistance"][1],
                "units":ranges["Pack_Resistance"][2]
            },
            {
                "name": "Pack_DOD",
                "value": str(can_data.pack_dod),
                "color": getColor(ranges["Pack_DOD"], state_data.state),
                "low":  ranges["Pack_DOD"][0],
                "high": ranges["Pack_DOD"][1],
                "units":ranges["Pack_DOD"][2]
            },
            {
                "name": "Pack_SOH",
                "value": str(can_data.pack_soh),
                "color": getColor(ranges["Pack_SOH"], state_data.state),
                "low":  ranges["Pack_SOH"][0],
                "high": ranges["Pack_SOH"][1],
                "units":ranges["Pack_SOH"][2]
            },
            {
                "name": "Current_Limit_Status",
                "value": str(can_data.current_limit_status),
                "color": getColor(ranges["Current_Limit_Status"], state_data.state),
                "low":  ranges["Current_Limit_Status"][0],
                "high": ranges["Current_Limit_Status"][1],
                "units":ranges["Current_Limit_Status"][2]
            },
            {
                "name": "Max_Pack_DCL",
                "value": str(can_data.max_pack_dcl),
                "color": getColor(ranges["Max_Pack_DCL"], state_data.state),
                "low":  ranges["Max_Pack_DCL"][0],
                "high": ranges["Max_Pack_DCL"][1],
                "units":ranges["Max_Pack_DCL"][2]
            },
            {
                "name": "Motor_State",
                "value": str(motion_data.motor_state),
                "color": getColor(ranges["Motor_State"], state_data.state),
                "low":  ranges["Motor_State"][0],
                "high": ranges["Motor_State"][1],
                "units":ranges["Motor_State"][2]
            },
            {
                "name": "Brake_State",
                "value": str(motion_data.brake_state),
                "color": getColor(ranges["Brake_State"],
                "low": ranges["Brake_State"][0],
                "high": ranges["Brake_State"][1],
                "units":ranges["Brake_State"][2]
            },
            {
                "name": "Motor_Target_Torque",
                "value": str(motion_data.motor_target_torque),
                "color": getColor(ranges["Motor_Target_Torque"], state_data.state),
                "low":  ranges["Motor_Target_Torque"][0],
                "high": ranges["Motor_Target_Torque"][1],
                "units":ranges["Motor_Target_Torque"][2]
            },
            {
                "name": "Pre-Charge Relay Status",
                "value": motion_data.relay_state_buff_0,
                "color": getColor(ranges["Pre-Charge Relay Status"], state_data.state),
                "low":  ranges["Pre-Charge Relay Status"][0],
                "high": ranges["Pre-Charge Relay Status"][1],
                "units":ranges["Pre-Charge Relay Status"][2]
            },
            {
                "name": "LV Relay Status",
                "value": motion_data.relay_state_buff_1,
                "color": getColor(ranges["LV Relay Status"], state_data.state),
                "low":  ranges["LV Relay Status"][0],
                "high": ranges["LV Relay Status"][1],
                "units":ranges["LV Relay Status"][2]
            },
            {
                "name": "HV Relay Status",
                "value": motion_data.relay_state_buff_2,
                "color": getColor(ranges["HV Relay Status"], state_data.state),
                "low":  ranges["HV Relay Status"][0],
                "high": ranges["HV Relay Status"][1],
                "units":ranges["HV Relay Status"][2]
            },
            {
                "name": "Relay State Buff 3",
                "value": motion_data.relay_state_buff_3,
                "color": getColor(ranges["Relay State Buff 3"], state_data.state),
                "low":  ranges["Relay State Buff 3"][0],
                "high": ranges["Relay State Buff 3"][1],
                "units":ranges["Relay State Buff 3"][2]
            }
        ]
    }
    can_data_bms2 = {
        "stats": [
            {
                "name": "Avg_Pack_Current",
                "value": str(can_data.avg_pack_current),
                "color": getColor(ranges["Avg_Pack_Current"], state_data.state),
                "low":  ranges["Avg_Pack_Current"][0],
                "high": ranges["Avg_Pack_Current"][1],
                "units":ranges["Avg_Pack_Current"][2]
            },
            {
                "name": "Highest_Temp",
                "value": str(can_data.highest_temp),
                "color": getColor(ranges["Highest_Temp"], state_data.state),
                "low":  ranges["Highest_Temp"][0],
                "high": ranges["Highest_Temp"][1],
                "units":ranges["Highest_Temp"][2]
            },
            {
                "name": "Highest_Temp_ID",
                "value": str(can_data.highest_temp_id),
                "color": getColor(ranges["Highest_Temp_ID"], state_data.state),
                "low":  ranges["Highest_Temp_ID"][0],
                "high": ranges["Highest_Temp_ID"][1],
                "units":ranges["Highest_Temp_ID"][2]
            },
            {
                "name": "Avg_Temp",
                "value": str(can_data.avg_temp),
                "color": getColor(ranges["Avg_Temp"], state_data.state),
                "low":  ranges["Avg_Temp"][0],
                "high": ranges["Avg_Temp"][1],
                "units":ranges["Avg_Temp"][2]
            },
            {
                "name": "Internal_Temp",
                "value": str(can_data.internal_temp),
                "color": getColor(ranges["Internal_Temp"], state_data.state),
                "low":  ranges["Internal_Temp"][0],
                "high": ranges["Internal_Temp"][1],
                "units":ranges["Internal_Temp"][2]
            },
            {
                "name": "Low_Cell_Voltage",
                "value": str(can_data.low_cell_voltage),
                "color": getColor(ranges["Low_Cell_Voltage"], state_data.state),
                "low":  ranges["Low_Cell_Voltage"][0],
                "high": ranges["Low_Cell_Voltage"][1],
                "units":ranges["Low_Cell_Voltage"][2]
            },
            {
                "name": "Low_Cell_Voltage_ID",
                "value": str(can_data.low_cell_voltage_id),
                "color": getColor(ranges["Low_Cell_Voltage_ID"], state_data.state),
                "low":  ranges["Low_Cell_Voltage_ID"][0],
                "high": ranges["Low_Cell_Voltage_ID"][1],
                "units":ranges["Low_Cell_Voltage_ID"][2]
            },
            {
                "name": "High_Cell_Voltage",
                "value": str(can_data.high_cell_voltage),
                "color": getColor(ranges["High_Cell_Voltage"], state_data.state),
                "low":  ranges["High_Cell_Voltage"][0],
                "high": ranges["High_Cell_Voltage"][1],
                "units":ranges["High_Cell_Voltage"][2]
            },
            {
                "name": "High_Cell_Voltage_ID",
                "value": str(can_data.high_cell_voltage_id),
                "color": getColor(ranges["High_Cell_Voltage_ID"], state_data.state),
                "low":  ranges["High_Cell_Voltage_ID"][0],
                "high": ranges["High_Cell_Voltage_ID"][1],
                "units":ranges["High_Cell_Voltage_ID"][2]
            },
            {
                "name": "Low_Cell_InternalR",
                "value": str(can_data.low_cell_internalR),
                "color": getColor(ranges["Low_Cell_InternalR"], state_data.state),
                "low":  ranges["Low_Cell_InternalR"][0],
                "high": ranges["Low_Cell_InternalR"][1],
                "units":ranges["Low_Cell_InternalR"][2]
            },
            {
                "name": "Low_Cell_InternalR_ID",
                "value": str(can_data.low_cell_internalR_id),
                "color": getColor(ranges["Low_Cell_InternalR_ID"], state_data.state),
                "low":  ranges["Low_Cell_InternalR_ID"][0],
                "high": ranges["Low_Cell_InternalR_ID"][1],
                "units":ranges["Low_Cell_InternalR_ID"][2]
            },
            {
                "name": "High_Cell_InternalR",
                "value": str(can_data.high_cell_internalR),
                "color": getColor(ranges["High_Cell_InternalR"], state_data.state),
                "low":  ranges["High_Cell_InternalR"][0],
                "high": ranges["High_Cell_InternalR"][1],
                "units":ranges["High_Cell_InternalR"][2]
            },
            {
                "name": "High_Cell_InternalR_ID",
                "value": str(can_data.high_cell_internalR_id),
                "color": getColor(ranges["High_Cell_InternalR_ID"], state_data.state),
                "low":  ranges["High_Cell_InternalR_ID"][0],
                "high": ranges["High_Cell_InternalR_ID"][1],
                "units":ranges["High_Cell_InternalR_ID"][2]
            },
            {
                "name": "Power_Voltage_Input",
                "value": str(can_data.power_voltage_input),
                "color": getColor(ranges["Power_Voltage_Input"], state_data.state),
                "low":  ranges["Power_Voltage_Input"][0],
                "high": ranges["Power_Voltage_Input"][1],
                "units":ranges["Power_Voltage_Input"][2]
            },
            {
                "name": "DTC_Status_One",
                "value": str(hex(can_data.dtc_status_one)),
                "color": getColor(ranges["DTC_Status_One"], state_data.state),
                "low":  ranges["DTC_Status_One"][0],
                "high": ranges["DTC_Status_One"][1],
                "units":ranges["DTC_Status_One"][2]
            },
            {
                "name": "DTC_Status_Two",
                "value": str(hex(can_data.dtc_status_two)),
                "color": getColor(ranges["DTC_Status_Two"], state_data.state),
                "low":  ranges["DTC_Status_Two"][0],
                "high": ranges["DTC_Status_Two"][1],
                "units":ranges["DTC_Status_Two"][2]
            },
            {
                "name": "Adaptive_Total_Cap",
                "value": str(can_data.adaptive_total_cap),
                "color": getColor(ranges["Adaptive_Total_Cap"], state_data.state),
                "low":  ranges["Adaptive_Total_Cap"][0],
                "high": ranges["Adaptive_Total_Cap"][1],
                "units":ranges["Adaptive_Total_Cap"][2]
            },
            {
                "name": "Adaptive_Amphours",
                "value": str(can_data.adaptive_amphours),
                "color": getColor(ranges["Adaptive_Amphours"], state_data.state),
                "low":  ranges["Adaptive_Amphours"][0],
                "high": ranges["Adaptive_Amphours"][1],
                "units":ranges["Adaptive_Amphours"][2]
            },
            {
                "name": "Adaptive_SOC",
                "value": str(can_data.adaptive_soc),
                "color": getColor(ranges["Adaptive_SOC"], state_data.state),
                "low":  ranges["Adaptive_SOC"][0],
                "high": ranges["Adaptive_SOC"][1],
                "units":ranges["Adaptive_SOC"][2]
            }
        ]
    }

    error_colors = errorColors(error_data)
    
    error_data_frame = {
        "stats": [
            {
                "name": "ADCError",
                "value": str(hex(error_data.ADCError)),
                "color": getColor(ranges["ADCError"], state_data.state),
                "low":  ranges["ADCError"][0],
                "high": ranges["ADCError"][1],
                "units":ranges["ADCError"][2]
            },
            {
                "name": "CANError",
                "value": str(hex(error_data.CANError)),
                "color": getColor(ranges["CANError"], state_data.state),
                "low":  ranges["CANError"][0],
                "high": ranges["CANError"][1],
                "units":ranges["CANError"][2]
            },
            {
                "name": "I2CError",
                "value": str(hex(error_data.I2CError)),
                "color": getColor(ranges["I2CError"], state_data.state),
                "low":  ranges["I2CError"][0],
                "high": ranges["I2CError"][1],
                "units":ranges["I2CError"][2]
            },
            {
                "name": "PRUError",
                "value": str(hex(error_data.PRUError)),
                "color": getColor(ranges["PRUError"], state_data.state),
                "low":  ranges["PRUError"][0],
                "high": ranges["PRUError"][1],
                "units":ranges["PRUError"][2]
            },
            {
                "name": "NetworkError",
                "value": str(hex(error_data.NetworkError)),
                "color": getColor(ranges["NetworkError"], state_data.state),
                "low":  ranges["NetworkError"][0],
                "high": ranges["NetworkError"][1],
                "units":ranges["NetworkError"][2]
            },
            {
                "name": "OtherError",
                "value": str(hex(error_data.OtherError)),
                "color": getColor(ranges["OtherError"], state_data.state),
                "low":  ranges["OtherError"][0],
                "high": ranges["OtherError"][1],
                "units":ranges["OtherError"][2]
            },
            {
                "name": "Tank Pressure",
                "value": str(i2c_data.sensor_0x48_0),
                "color": getColor(ranges["Adaptive_SOC"], state_data.state),
                "low":  ranges["Adaptive_SOC"][0],
                "high": ranges["Adaptive_SOC"][1],
                "units":ranges["Adaptive_SOC"][2]
            },
            {
                "name": "Brake Pressure 1",
                "value": str(i2c_data.sensor_0x48_1),
                "color": getColor(ranges["Brake Pressure 1"], state_data.state),
                "low":  ranges["Brake Pressure 1"][0],
                "high": ranges["Brake Pressure 1"][1],
                "units":ranges["Brake Pressure 1"][2]
            },
            {
                "name": "Brake Pressure 2",
                "value": str(i2c_data.sensor_0x48_2),
                "color": getColor(ranges["Brake Pressure 2"], state_data.state),
                "low":  ranges["Brake Pressure 2"][0],
                "high": ranges["Brake Pressure 2"][1],
                "units":ranges["Brake Pressure 2"][2]
            },
            {
                "name": "Brake Pressure 3",
                "value": str(i2c_data.sensor_0x48_3),
                "color": getColor(ranges["Brake Pressure 3"], state_data.state),
                "low":  ranges["Brake Pressure 3"][0],
                "high": ranges["Brake Pressure 3"][1],
                "units":ranges["Brake Pressure 3"][2]
            },
            {
                "name": "Orange Distance",
                "value": str(pru_data.orange_distance),
                "color": getColor(ranges["Orange Distance"], state_data.state),
                "low":  ranges["Orange Distance"][0],
                "high": ranges["Orange Distance"][1],
                "units":ranges["Orange Distance"][2]
            },
            {
                "name": "Orange Velocity",
                "value": str(pru_data.orange_velocity),
                "color": getColor(ranges["Orange Velocity"], state_data.state),
                "low":  ranges["Orange Velocity"][0],
                "high": ranges["Orange Velocity"][1],
                "units":ranges["Orange Velocity"][2]
            },
            {
                "name": "Wheel Distance",
                "value": str(pru_data.wheel_distance),
                "color": getColor(ranges["Wheel Distance"], state_data.state),
                "low":  ranges["Wheel Distance"][0],
                "high": ranges["Wheel Distance"][1],
                "units":ranges["Wheel Distance"][2]
            },
            {
                "name": "Wheel Velocity",
                "value": str(pru_data.wheel_velocity),
                "color": getColor(ranges["Wheel Velocity"], state_data.state),
                "low":  ranges["Wheel Velocity"][0],
                "high": ranges["Wheel Velocity"][1],
                "units":ranges["Wheel Velocity"][2]
            },
            {
                "name": "Acceleration",
                "value": str(motion_data.acceleration),
                "color": getColor(ranges["Acceleration"], state_data.state),
                "low":  ranges["Acceleration"][0],
                "high": ranges["Acceleration"][1],
                "units":ranges["Acceleration"][2]
            },
            {
                "name": "Pre-Charge counter",
                "value": motion_data.p_counter,
                "color": getColor(ranges["Pre-Charge counter"], state_data.state),
                "low":  ranges["Pre-Charge counter"][0],
                "high": ranges["Pre-Charge counter"][1],
                "units":ranges["Pre-Charge counter"][2]
            },
            {
                "name": "Pre-Charge timeout",
                "value": motion_data.p_timeout,
                "color": getColor(ranges["Pre-Charge timeout"], state_data.state),
                "low":  ranges["Pre-Charge timeout"][0],
                "high": ranges["Pre-Charge timeout"][1],
                "units":ranges["Pre-Charge timeout"][2]
            },
            {
                "name": "Acceleration counter",
                "value": motion_data.a_counter,
                "color": getColor(ranges["Acceleration counter"], state_data.state),
                "low":  ranges["Acceleration counter"][0],
                "high": ranges["Acceleration counter"][1],
                "units":ranges["Acceleration counter"][2]
            },
            {
                "name": "Acceleration timeout",
                "value": motion_data.a_timeout,
                "color": getColor(ranges["Acceleration timeout"], state_data.state),
                "low":  ranges["Acceleration timeout"][0],
                "high": ranges["Acceleration timeout"][1],
                "units":ranges["Acceleration timeout"][2]
            },
            {
                "name": "Coast counter",
                "value": motion_data.c_counter,
                "color": getColor(ranges["Coast counter"], state_data.state),
                "low":  ranges["Coast counter"][0],
                "high": ranges["Coast counter"][1],
                "units":ranges["Coast counter"][2]
            },
            {
                "name": "Coast timeout",
                "value": motion_data.c_timeout,
                "color": getColor(ranges["Coast timeout"], state_data.state),
                "low":  ranges["Coast timeout"][0],
                "high": ranges["Coast timeout"][1],
                "units":ranges["Coast timeout"][2]
            },
            {
                "name": "Brake counter",
                "value": motion_data.b_counter,
                "color": getColor(ranges["Brake counter"], state_data.state),
                "low":  ranges["Brake counter"][0],
                "high": ranges["Brake counter"][1],
                "units":ranges["Brake counter"][2]
            },
            {
                "name": "Brake timeout",
                "value": motion_data.b_timeout,
                "color": getColor(ranges["Brake timeout"], state_data.state),
                "low":  ranges["Brake timeout"][0],
                "high": ranges["Brake timeout"][1],
                "units":ranges["Brake timeout"][2]
            }
        ]
    }
    toReturn = [can_data_motor, can_data_bms1, can_data_bms2, error_data_frame]
    return toReturn

def getRanges():
    toRet = {}
    toRet["State_Number"] = [0, 7, "N/A"]
    toRet["Status_Word"] = [0, 1, "m"]
    toRet["Position_Val"] = [0, 1, "m"]
    toRet["Torque_Val"] = [0, 1, "m"]
    toRet["Controller_Temp"] = [0, 1, "m"]
    toRet["Motor_Temp"] = [0, 1, "m"]
    toRet["DC_Link_Voltage"] = [0, 1, "m"]
    toRet["Logic_Power_Supply_Voltage"] = [0, 1, "m"]
    toRet["Current_Demand"] = [0, 1, "m"]
    toRet["Motor_Current_Val"] = [0, 1, "m"]
    toRet["Electrical_Angle"] = [0, 1, "m"]
    toRet["Phase_A_Current"] = [0, 1, "m"]
    toRet["Phase_B_Current"] = [0, 1, "m"]
    toRet["Accel_1 x"] = [-98.1, 29.5, "m/s^2"]
    toRet["Accel_1 y"] = [-1.5, 1.5, "m/s^2"]
    toRet["Accel_1 z"] = [-1.5, 1.5, "m/s^2"]
    toRet["Accel_2 x"] = [-98.1, 29.5, "m/s^2"]
    toRet["Accel_2 y"] = [-1.5, 1.5, "m/s^2"]
    toRet["Accel_2 z"] = [-1.5, 1.5, "m/s^2"]
    toRet["Internal_Relay_State"] = [0, 1, "m"]
    toRet["Relay_State"] = [0, 1, "m"]
    toRet["Rolling_Counter"] = [0, 1, "m"]
    toRet["Fail_Safe_State"] = [0, 1, "m"]
    toRet["Peak_Current"] = [0, 1, "m"]
    toRet["Pack_Voltage_Inst"] = [0, 1, "m"]
    toRet["Pack_Voltage_Open"] = [0, 1, "m"]
    toRet["Pack_SOC"] = [0, 1, "m"]
    toRet["Pack_Amphours"] = [0, 1, "m"]
    toRet["Pack_Resistance"] = [0, 1, "m"]
    toRet["Pack_DOD"] = [0, 1, "m"]
    toRet["Pack_SOH"] = [0, 1, "m"]
    toRet["Current_Limit_Status"] = [0, 1, "m"]
    toRet["Max_Pack_DCL"] = [0, 1, "m"]
    toRet["Motor_State"] = [0, 1, "m"]
    toRet["Brake_State"] = [0, 1, "m"]
    toRet["Motor_Target_Torque"] = [0, 1, "m"]
    toRet["Pre-Charge Relay Status"] = [0, 1, "m"]
    toRet["LV Relay Status"] = [0, 1, "m"]
    toRet["HV Relay Status"] = [0, 1, "m"]
    toRet["Relay State Buff 3"] = [0, 1, "m"]
    toRet["Avg_Pack_Current"] = [0, 1, "m"]
    toRet["Highest_Temp"] = [0, 1, "m"]
    toRet["Highest_Temp_ID"] = [0, 1, "m"]
    toRet["Avg_Temp"] = [0, 1, "m"]
    toRet["Internal_Temp"] = [0, 1, "m"]
    toRet["Low_Cell_Voltage"] = [0, 1, "m"]
    toRet["Low_Cell_Voltage_ID"] = [0, 1, "m"]
    toRet["High_Cell_Voltage"] = [0, 1, "m"]
    toRet["High_Cell_Voltage_ID"] = [0, 1, "m"]
    toRet["Low_Cell_InternalR"] = [0, 1, "m"]
    toRet["Low_Cell_InternalR_ID"] = [0, 1, "m"]
    toRet["High_Cell_InternalR"] = [0, 1, "m"]
    toRet["High_Cell_InternalR_ID"] = [0, 1, "m"]
    toRet["Power_Voltage_Input"] = [0, 1, "m"]
    toRet["DTC_Status_One"] = [0, 1, "m"]
    toRet["DTC_Status_Two"] = [0, 1, "m"]
    toRet["Adaptive_Total_Cap"] = [0, 1, "m"]
    toRet["Adaptive_Amphours"] = [0, 1, "m"]
    toRet["Adaptive_SOC"] = [0, 1, "m"]
    toRet["ADCError"] = [0, 1, "m"]
    toRet["CANError"] = [0, 1, "m"]
    toRet["I2CError"] = [0, 1, "m"]
    toRet["PRUError"] = [0, 1, "m"]
    toRet["NetworkError"] = [0, 1, "m"]
    toRet["OtherError"] = [0, 1, "m"]
    toRet["Tank Pressure"] = [200, 1000, "m"]
    toRet["Brake Pressure 1"] = [200, 1000, "m"]
    toRet["Brake Pressure 2"] = [200, 1000, "m"]
    toRet["Brake Pressure 3"] = [200, 1000, "m"]
    toRet["Orange Distance"] = [0, 1, "m"]
    toRet["Orange Velocity"] = [0, 1, "m"]
    toRet["Wheel Distance"] = [0, 1, "m"]
    toRet["Wheel Velocity"] = [0, 1, "m"]
    toRet["Acceleration"] = [0, 1, "m"]
    toRet["Pre-Charge counter"] = [0, 1, "m"]
    toRet["Pre-Charge timeout"] = [0, 1, "m"]
    toRet["Acceleration counter"] = [0, 1, "m"]
    toRet["Acceleration timeout"] = [0, 1, "m"]
    toRet["Coast counter"] = [0, 1, "m"]
    toRet["Coast timeout"] = [0, 1, "m"]
    toRet["Brake counter"] = [0, 1, "m"]
    toRet["Brake timeout"] = [0, 1, "m"]

    return toRet

def getColor(range, val):
    if range[0] <= val and val <= range[1]:
        return "limegreen"
    else:
        return "red"

def errorColors(error_data):
    toRet = {}
    if error_data.ADCError == 0:
        toRet["adc_color"] = "limegreen"
    else:
        toRet["adc_color"] =  "red"
    
    if error_data.CANError == 0:
        toRet["can_color"] = "limegreen"
    else:
        toRet["can_color"] =  "red"
    
    if error_data.I2CError == 0:
        toRet["i2c_color"] = "limegreen"
    else:
        toRet["i2c_color"] =  "red"
    
    if error_data.PRUError == 0:
        toRet["pru_color"] = "limegreen"
    else:
        toRet["pru_color"] =  "red"
    
    if error_data.NetworkError == 0:
        toRet["net_color"] = "limegreen"
    else:
        toRet["net_color"] =  "red"
    
    if error_data.OtherError == 0:
        toRet["other_color"] = "limegreen"
    else:
        toRet["other_color"] =  "red"
    
    return toRet