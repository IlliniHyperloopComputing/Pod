from . import models

###############################################################
# Methods for saving data                                     #
###############################################################

def saveStateData(data):
    if len(data) != 1:
        return -1
    state_model = models.State(
        state=data[0]
    )
    state_model.save()
    return 1

def saveADCData(data):
    if len(data) != 7:
        return -1
    error_model = models.ADCData(
        data_0=data[0],
        data_1=data[1],
        data_2=data[2],
        data_3=data[3],
        data_4=data[4],
        data_5=data[5],
        data_6=data[6]
    )
    error_model.save()
    return 1

# Input: data array
# Function: saves data as a error data model
# Returns: -1 if the array is too small
# Returns: 1 on success
def saveErrorData(data):
    if len(data) != 6:
        return -1
    error_model = models.Errors(
        ADCError=data[0],
        CANError=data[1],
        I2CError=data[2],
        PRUError=data[3],
        NetworkError=data[4],
        OtherError=data[5],
    )
    error_model.save()
    return 1

def saveI2CData(data):
    if len(data) != 12:
        return -1
    i2c_model = models.I2CData(
        sensor_0x48_0 = data[0],
        sensor_0x48_1 = data[1],
        sensor_0x48_2 = data[2],
        sensor_0x48_3 = data[3],
        sensor_0x49_0 = data[4],
        sensor_0x49_1 = data[5],
        sensor_0x49_2 = data[6],
        sensor_0x49_3 = data[7],
        sensor_0x77_0 = data[8],
        sensor_0x77_1 = data[9],
        sensor_0x77_2 = data[10],
        sensor_0x77_3 = data[11],
    )
    i2c_model.save()
    return 1

def savePRUData(data):
    if len(data) != 4:
        return -1
    pru_model = models.PRUData(
        orange_distance = data[0],
        orange_velocity = data[1],
        wheel_distance = data[2],
        wheel_velocity = data[3]
    )
    pru_model.save()
    return 1

def saveMotionData(data):
    if len(data) != 3:
        return -1
    motion_model = models.MotionData(
        position = data[0],
        velocity = data[1],
        acceleration = data[2]
    )
    motion_model.save()
    return 1

# There has to be a better way to do this
# Input: data array
# Function: saves data as a can data model
# Returns: -1 if the array is too small
# Returns: 1 on success
def saveCANData(data):
    # print("len data:")
    # print(len(data))
    if len(data) < 45:
        return -1
    can_model = models.CANData(
        # Motor Controller
        status_word=data[0],
        position_val=data[1],
        torque_val=data[2],
        controller_temp=data[3],
        motor_temp=data[4],
        dc_link_voltage=data[5],
        logic_power_supply_voltage=data[6],
        current_demand=data[7],
        motor_current_val=data[8],
        electrical_angle=data[9],
        phase_a_current=data[10],
        phase_b_current=data[11],

        ## BMS
        # Used witdatain tdatae CANManager to set BMS relay states
        internal_relay_state=data[12],
        # Tdatais sdataould agree witdata tdatae above (given a small delay)
        relay_state=data[13],
        rolling_counter=data[14],
        fail_safe_state=data[15],
        peak_current=data[16],
        pack_voltage_inst=data[17],
        pack_voltage_open=data[18],
        pack_soc=data[19],
        pack_amphours=data[20],
        pack_resistance=data[21],
        pack_dod=data[22],
        pack_soh=data[23],
        current_limit_status=data[24],

        max_pack_dcl=data[25],
        avg_pack_current=data[26],
        highest_temp=data[27],
        highest_temp_id=data[28],

        avg_temp=data[29],
        internal_temp=data[30],
        low_cell_voltage=data[31],
        low_cell_voltage_id=data[32],
        high_cell_voltage=data[33],
        high_cell_voltage_id=data[34],

        low_cell_internalR=data[35],
        low_cell_internalR_id=data[36],
        high_cell_internalR=data[37],
        high_cell_internalR_id=data[38],
        power_voltage_input=data[39],
        dtc_status_one=data[40],
        dtc_status_two=data[41],
        adaptive_total_cap=data[42],
        adaptive_amphours=data[43],
        adaptive_soc=data[44]
    )

    can_model.save()
    return 1
