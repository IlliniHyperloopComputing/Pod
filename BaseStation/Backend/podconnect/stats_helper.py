from . import models

def getStats():
    state_data = models.State.objects.latest("date_time")
    can_data = models.CANData.objects.latest("date_time")
    error_data = models.Errors.objects.latest("date_time")
    i2c_data = models.I2CData.objects.latest("date_time")
    pru_data = models.PRUData.objects.latest("date_time")
    motion_data = models.MotionData.objects.latest("date_time")
    adc_data = models.ADCData.objects.latest("date_time")

    can_data_motor = {
        "stats": [
            {
                "name": "State_Number",
                "value": str(state_data.state),
                "color": "limegreen"
            },
            {
                "name": "Status_Word",
                "value": str(hex(can_data.status_word)),
                "color": "limegreen"
            },
            {
                "name": "Position_Val",
                "value": str(can_data.position_val),
                "color": "limegreen"
            },
            {
                "name": "Torque_Val",
                "value": str(can_data.torque_val),
                "color": "limegreen"
            },
            {
                "name": "Controller_Temp",
                "value": str(can_data.controller_temp),
                "color": "limegreen"
            },
            {
                "name": "Motor_Temp",
                "value": str(can_data.motor_temp),
                "color": "limegreen"
            },
            {
                "name": "DC_Link_Voltage",
                "value": str(can_data.dc_link_voltage),
                "color": "limegreen"
            },
            {
                "name": "Logic_Power_Supply_Voltage",
                "value": str(can_data.logic_power_supply_voltage),
                "color": "limegreen"
            },
            {
                "name": "Current_Demand",
                "value": str(can_data.current_demand),
                "color": "limegreen"
            },
            {
                "name": "Motor_Current_Val",
                "value": str(can_data.motor_current_val),
                "color": "limegreen"
            },
            {
                "name": "Electrical_Angle",
                "value": str(can_data.electrical_angle),
                "color": "limegreen"
            },
            {
                "name": "Phase_A_Current",
                "value": str(can_data.phase_a_current),
                "color": "limegreen"
            },
            {
                "name": "Phase_B_Current",
                "value": str(can_data.phase_b_current),
                "color": "limegreen"
            },
            {
                "name": "Accel_1 x",
                "value": str(adc_data.data_5),
                "color": "limegreen"
            },
            {
                "name": "Accel_1 y",
                "value": str(adc_data.data_6),
                "color": "limegreen"
            },
            {
                "name": "Accel_1 z",
                "value": str(adc_data.data_4),
                "color": "limegreen"
            },
            {
                "name": "Accel_2 x",
                "value": str(adc_data.data_1),
                "color": "limegreen"
            },
            {
                "name": "Accel_2 y",
                "value": str(adc_data.data_3),
                "color": "limegreen"
            },
            {
                "name": "Accel_2 z",
                "value": str(adc_data.data_2),
                "color": "limegreen"
            }
        ]
    }
    can_data_bms1 = {
        "stats": [
            {
                "name": "Internal_Relay_State",
                "value": str(hex(can_data.internal_relay_state)),
                "color": "limegreen"
            },
            {
                "name": "Relay_State",
                "value": str(hex(can_data.relay_state)),
                "color": "limegreen"
            },
            {
                "name": "Rolling_Counter",
                "value": str(can_data.rolling_counter),
                "color": "limegreen"
            },
            {
                "name": "Fail_Safe_State",
                "value": str(hex(can_data.fail_safe_state)),
                "color": "limegreen"
            },
            {
                "name": "Peak_Current",
                "value": str(can_data.peak_current),
                "color": "limegreen"
            },
            {
                "name": "Pack_Voltage_Inst",
                "value": str(can_data.pack_voltage_inst),
                "color": "limegreen"
            },
            {
                "name": "Pack_Voltage_Open",
                "value": str(can_data.pack_voltage_open),
                "color": "limegreen"
            },
            {
                "name": "Pack_SOC",
                "value": str(can_data.pack_soc),
                "color": "limegreen"
            },
            {
                "name": "Pack_Amphours",
                "value": str(can_data.pack_amphours),
                "color": "limegreen"
            },
            {
                "name": "Pack_Resistance",
                "value": str(can_data.pack_resistance),
                "color": "limegreen"
            },
            {
                "name": "Pack_DOD",
                "value": str(can_data.pack_dod),
                "color": "limegreen"
            },
            {
                "name": "Pack_SOH",
                "value": str(can_data.pack_soh),
                "color": "limegreen"
            },
            {
                "name": "Current_Limit_Status",
                "value": str(can_data.current_limit_status),
                "color": "limegreen"
            },
            {
                "name": "Max_Pack_DCL",
                "value": str(can_data.max_pack_dcl),
                "color": "limegreen"
            },
            {
                "name": "Motor_State",
                "value": str(motion_data.motor_state),
                "color": "limegreen"
            },
            {
                "name": "Brake_State",
                "value": str(motion_data.brake_state),
                "color": "limegreen"
            },
            {
                "name": "Motor_Target_Torque",
                "value": str(motion_data.motor_target_torque),
                "color": "limegreen"
            },
            {
                "name": "Pre-Charge Relay Status",
                "value": motion_data.relay_state_buff_0,
                "color": "limegreen"
            },
            {
                "name": "LV Relay Status",
                "value": motion_data.relay_state_buff_1,
                "color": "limegreen"
            },
            {
                "name": "HV Relay Status",
                "value": motion_data.relay_state_buff_2,
                "color": "limegreen"
            },
            {
                "name": "Relay State Buff 3",
                "value": motion_data.relay_state_buff_3,
                "color": "limegreen"
            }
        ]
    }
    can_data_bms2 = {
        "stats": [
            {
                "name": "Avg_Pack_Current",
                "value": str(can_data.avg_pack_current),
                "color": "limegreen"
            },
            {
                "name": "Highest_Temp",
                "value": str(can_data.highest_temp),
                "color": "limegreen"
            },
            {
                "name": "Highest_Temp_ID",
                "value": str(can_data.highest_temp_id),
                "color": "limegreen"
            },
            {
                "name": "Avg_Temp",
                "value": str(can_data.avg_temp),
                "color": "limegreen"
            },
            {
                "name": "Internal_Temp",
                "value": str(can_data.internal_temp),
                "color": "limegreen"
            },
            {
                "name": "Low_Cell_Voltage",
                "value": str(can_data.low_cell_voltage),
                "color": "limegreen"
            },
            {
                "name": "Low_Cell_Voltage_ID",
                "value": str(can_data.low_cell_voltage_id),
                "color": "limegreen"
            },
            {
                "name": "High_Cell_Voltage",
                "value": str(can_data.high_cell_voltage),
                "color": "limegreen"
            },
            {
                "name": "High_Cell_Voltage_ID",
                "value": str(can_data.high_cell_voltage_id),
                "color": "limegreen"
            },
            {
                "name": "Low_Cell_InternalR",
                "value": str(can_data.low_cell_internalR),
                "color": "limegreen"
            },
            {
                "name": "Low_Cell_InternalR_ID",
                "value": str(can_data.low_cell_internalR_id),
                "color": "limegreen"
            },
            {
                "name": "High_Cell_InternalR",
                "value": str(can_data.high_cell_internalR),
                "color": "limegreen"
            },
            {
                "name": "High_Cell_InternalR_ID",
                "value": str(can_data.high_cell_internalR_id),
                "color": "limegreen"
            },
            {
                "name": "Power_Voltage_Input",
                "value": str(can_data.power_voltage_input),
                "color": "limegreen"
            },
            {
                "name": "DTC_Status_One",
                "value": str(hex(can_data.dtc_status_one)),
                "color": "limegreen"
            },
            {
                "name": "DTC_Status_Two",
                "value": str(hex(can_data.dtc_status_two)),
                "color": "limegreen"
            },
            {
                "name": "Adaptive_Total_Cap",
                "value": str(can_data.adaptive_total_cap),
                "color": "limegreen"
            },
            {
                "name": "Adaptive_Amphours",
                "value": str(can_data.adaptive_amphours),
                "color": "limegreen"
            },
            {
                "name": "Adaptive_SOC",
                "value": str(can_data.adaptive_soc),
                "color": "limegreen"
            }
        ]
    }

    error_colors = errorColors(error_data)
    
    error_data_frame = {
        "stats": [
            {
                "name": "ADCError",
                "value": str(hex(error_data.ADCError)),
                "color": error_colors["adc_color"]
            },
            {
                "name": "CANError",
                "value": str(hex(error_data.CANError)),
                "color": error_colors["can_color"]
            },
            {
                "name": "I2CError",
                "value": str(hex(error_data.I2CError)),
                "color": error_colors["i2c_color"]
            },
            {
                "name": "PRUError",
                "value": str(hex(error_data.PRUError)),
                "color": error_colors["pru_color"]
            },
            {
                "name": "NetworkError",
                "value": str(hex(error_data.NetworkError)),
                "color": error_colors["net_color"]
            },
            {
                "name": "OtherError",
                "value": str(hex(error_data.OtherError)),
                "color": error_colors["other_color"]
            },
            {
                "name": "I2C 0x48 0",
                "value": str(i2c_data.sensor_0x48_0),
                "color": "limegreen"
            },
            {
                "name": "I2C 0x48 1",
                "value": str(i2c_data.sensor_0x48_1),
                "color": "limegreen"
            },
            {
                "name": "I2C 0x48 2",
                "value": str(i2c_data.sensor_0x48_2),
                "color": "limegreen"
            },
            {
                "name": "I2C 0x48 3",
                "value": str(i2c_data.sensor_0x48_3),
                "color": "limegreen"
            },
            {
                "name": "Orange Distance",
                "value": str(pru_data.orange_distance),
                "color": "limegreen"
            },
            {
                "name": "Orange Velocity",
                "value": str(pru_data.orange_velocity),
                "color": "limegreen"
            },
            {
                "name": "Wheel Distance",
                "value": str(pru_data.wheel_distance),
                "color": "limegreen"
            },
            {
                "name": "Wheel Velocity",
                "value": str(pru_data.wheel_velocity),
                "color": "limegreen"
            },
            {
                "name": "Acceleration",
                "value": str(motion_data.acceleration),
                "color": "limegreen"
            },
            {
                "name": "Pre-Charge counter",
                "value": motion_data.p_counter,
                "color": "limegreen"
            },
            {
                "name": "Pre-Charge timeout",
                "value": motion_data.p_timeout,
                "color": "limegreen"
            },
            {
                "name": "Acceleration counter",
                "value": motion_data.a_counter,
                "color": "limegreen"
            },
            {
                "name": "Acceleration timeout",
                "value": motion_data.a_timeout,
                "color": "limegreen"
            },
            {
                "name": "Coast counter",
                "value": motion_data.c_counter,
                "color": "limegreen"
            },
            {
                "name": "Coast timeout",
                "value": motion_data.c_timeout,
                "color": "limegreen"
            },
            {
                "name": "Brake counter",
                "value": motion_data.b_counter,
                "color": "limegreen"
            },
            {
                "name": "Brake timeout",
                "value": motion_data.b_timeout,
                "color": "limegreen"
            }
        ]
    }
    toReturn = [can_data_motor, can_data_bms1, can_data_bms2, error_data_frame]
    return toReturn

def getRanges():
    toRet = {}
    toRet["State_Number"] = [0, 7, "N/A"]
    toRet["Status_Word"] = [0, 1, "m"]  # ??????
    toRet["Position_Val"] = [-3, 1250, "m"]
    toRet["Torque_Val"] = [0, 230, "Nm"]
    toRet["Controller_Temp"] = [0, 80, "degC"]
    toRet["Motor_Temp"] = [0, 80, "degC"]
    toRet["DC_Link_Voltage"] = [0, 120, "V"]
    toRet["Logic_Power_Supply_Voltage"] = [12.8, 16.8, "V"]
    toRet["Current_Demand"] = [0, 544, "A"]
    toRet["Motor_Current_Val"] = [0, 520, "A"]
    toRet["Electrical_Angle"] = [0, 1, "m"]  # ??????
    toRet["Phase_A_Current"] = [0, 1, "m"]  # ??????
    toRet["Phase_B_Current"] = [0, 1, "m"]  # ??????
    toRet["Accel_1 x"] = [-98.1, 29.5, "m/s2"]
    toRet["Accel_1 y"] = [-1.5, 1.5, "m/s2"]
    toRet["Accel_1 z"] = [-1.5, 1.5, "m/s2"]
    toRet["Accel_2 x"] = [-98.1, 29.5, "m/s2"]
    toRet["Accel_2 y"] = [-1.5, 1.5, "m/s2"]
    toRet["Accel_2 z"] = [-1.5, 1.5, "m/s2"]
    toRet["Internal_Relay_State"] = [0, 1, "m"]
    toRet["Relay_State"] = [0, 1, "N/A"]
    toRet["Rolling_Counter"] = [0, 1, "m"]  # ??????
    toRet["Fail_Safe_State"] = [0, 0, "N/A"]
    toRet["Peak_Current"] = [0, 1, "m"]  # ??????
    toRet["Pack_Voltage_Inst"] = [100, 120, "V"]  # ??????
    toRet["Pack_Voltage_Open"] = [0, 1, "m"]  # ??????
    toRet["Pack_SOC"] = [20, 100, "%"]
    toRet["Pack_Amphours"] = [0, 544, "A"]  # ??????
    toRet["Pack_Resistance"] = [0, 1, "m"]  # ??????
    toRet["Pack_DOD"] = [0, 1, "m"]  # ??????
    toRet["Pack_SOH"] = [0, 1, "m"]  # ??????
    toRet["Current_Limit_Status"] = [0, 544, "A"] # HV Discharge Limit?
    toRet["Max_Pack_DCL"] = [0, 1, "m"]  # ??????
    toRet["Motor_State"] = [0, 1, "N/A"]
    toRet["Brake_State"] = [0, 1, "N/A"]
    toRet["Motor_Target_Torque"] = [0, 230, "Nm"]  # what is this? is this the same is torque val?
    toRet["Pre-Charge Relay Status"] = [0, 1, "m"]  # Relay_State includes this 
    toRet["LV Relay Status"] = [0, 1, "m"]  # Relay_State includes this 
    toRet["HV Relay Status"] = [0, 1, "m"]  # Relay_State includes this 
    toRet["Relay State Buff 3"] = [0, 1, "m"]  # ???
    toRet["Avg_Pack_Current"] = [0, 544, "A"]
    toRet["Highest_Temp"] = [0, 65, "degC"]  # i'm guessing this is for a cell. NOT on health sheet
    toRet["Highest_Temp_ID"] = [0, 30, "Cell"]
    toRet["Avg_Temp"] = [0, 1, "m"]  # i'm guessing this is for the battery pack. NOT on health sheet
    toRet["Internal_Temp"] = [0, 65, "degC"]  
    toRet["Low_Cell_Voltage"] = [3.2, 4.2, "V"]
    toRet["Low_Cell_Voltage_ID"] = [0, 30, "Cell"]
    toRet["High_Cell_Voltage"] = [3.2, 4.2, "V"]
    toRet["High_Cell_Voltage_ID"] = [0, 30, "Cell"]
    toRet["Low_Cell_InternalR"] = [0, 1, "m"]  # ??????
    toRet["Low_Cell_InternalR_ID"] = [0, 1, "m"]  # ??????
    toRet["High_Cell_InternalR"] = [0, 1, "m"] # ??????
    toRet["High_Cell_InternalR_ID"] = [0, 1, "m"] # ??????
    toRet["Power_Voltage_Input"] = [12.8, 16.8, "V"]
    toRet["DTC_Status_One"] = [0, 0, "N/A"]  # guesssing this for BMS
    toRet["DTC_Status_Two"] = [0, 0, "N/A"]  # guesssing this for BMS
    toRet["Adaptive_Total_Cap"] = [0, 1, "m"]  # ??????
    toRet["Adaptive_Amphours"] = [0, 1, "m"] # ???????
    toRet["Adaptive_SOC"] = [0, 1, "m"] # ??????
    toRet["ADCError"] = [0, 1, "m"]  # FILLLL INNNNNNNNNNNNNNNNNNNNNNN 
    toRet["CANError"] = [0, 1, "m"]
    toRet["I2CError"] = [0, 1, "m"]
    toRet["PRUError"] = [0, 1, "m"]
    toRet["NetworkError"] = [0, 1, "m"]
    toRet["OtherError"] = [0, 1, "m"]
    toRet["I2C 0x48 0"] = [0, 1, "m"]
    toRet["I2C 0x48 1"] = [0, 1, "m"]
    toRet["I2C 0x48 2"] = [0, 1, "m"]
    toRet["I2C 0x48 3"] = [0, 1, "m"]
    toRet["Orange Distance"] = [-3, 1250, "m"]
    toRet["Orange Velocity"] = [-1, 110, "m/s"]
    toRet["Wheel Distance"] = [-3, 1250, "m"]
    toRet["Wheel Velocity"] = [-1, 110, "m/s"]
    toRet["Acceleration"] = [0, 1, "m"]  # ??????
    toRet["Pre-Charge counter"] = [0, 1, "m"]  # ??????
    toRet["Pre-Charge timeout"] = [0, 30, "s"]  
    toRet["Acceleration counter"] = [0, 10, "N/A"]  # ??????
    toRet["Acceleration timeout"] = [0, 15, "s"]  
    toRet["Coast counter"] = [0, 1, "m"]  # ??????
    toRet["Coast timeout"] = [0, 0.2, "s"]
    toRet["Brake counter"] = [0, 1, "m"]  # ??????
    toRet["Brake timeout"] = [0, 10, "s"]

    return toRet

def getColor(low, val, high):
    if low <= val and val <= high:
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