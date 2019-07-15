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
                "value": str(adc_data.data_5 - 2048),
                "color": "limegreen"
            },
            {
                "name": "Accel_1 y",
                "value": str(adc_data.data_6 - 2048),
                "color": "limegreen"
            },
            {
                "name": "Accel_1 z",
                "value": str(adc_data.data_4 - 2048),
                "color": "limegreen"
            },
            {
                "name": "Accel_2 x",
                "value": str(adc_data.data_1),
                "color": "limegreen"
            },
            {
                "name": "Accel_2 y",
                "value": str(adc_data.data_3 - 2048),
                "color": "limegreen"
            },
            {
                "name": "Accel_2 z",
                "value": str(adc_data.data_2 - 2048),
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
    if error_data.ADCError == 0:
        adc_color = "limegreen"
    else:
        adc_color =  "red"
    
    if error_data.CANError == 0:
        can_color = "limegreen"
    else:
        can_color =  "red"
    
    if error_data.I2CError == 0:
        i2c_color = "limegreen"
    else:
        i2c_color =  "red"
    
    if error_data.PRUError == 0:
        pru_color = "limegreen"
    else:
        pru_color =  "red"
    
    if error_data.NetworkError == 0:
        net_color = "limegreen"
    else:
        net_color =  "red"
    
    if error_data.OtherError == 0:
        other_color = "limegreen"
    else:
        other_color =  "red"
    
    error_data_frame = {
        "stats": [
            {
                "name": "ADCError",
                "value": str(hex(error_data.ADCError)),
                "color": adc_color
            },
            {
                "name": "CANError",
                "value": str(hex(error_data.CANError)),
                "color": can_color
            },
            {
                "name": "I2CError",
                "value": str(hex(error_data.I2CError)),
                "color": i2c_color
            },
            {
                "name": "PRUError",
                "value": str(hex(error_data.PRUError)),
                "color": pru_color
            },
            {
                "name": "NetworkError",
                "value": str(hex(error_data.NetworkError)),
                "color": net_color
            },
            {
                "name": "OtherError",
                "value": str(hex(error_data.OtherError)),
                "color": other_color
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
            }
        ]
    }
    toReturn = [can_data_motor, can_data_bms1, can_data_bms2, error_data_frame]
    return toReturn
