from . import models

###############################################################
# Methods for saving data                                     #
###############################################################

def saveStateData(data):
    if len(data) < 1:
        return -1
    state_model = models.State(
        state=data[0]
    )
    state_model.save()

# Input: data array
# Function: saves data as a error data model
# Returns: -1 if the array is too small
# Returns: 1 on success
def saveErrorData(data):
    if len(data) < 6:
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
        data=data[0],
        status_word=data[1],
        position_val=data[2],
        torque_val=data[3],
        controller_temp=data[4],
        motor_temp=data[5],
        dc_link_voltage=data[6],
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
