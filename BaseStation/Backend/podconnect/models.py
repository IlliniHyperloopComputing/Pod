from django.db import models

# Create your models here.

class MotionData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    position = models.IntegerField(default=0)
    velocity = models.IntegerField(default=0)
    acceleration = models.IntegerField(default=0)
    motor_state = models.IntegerField(default=0)
    brake_state = models.IntegerField(default=0)
    motor_target_torque = models.IntegerField(default=0)
    relay_state_buff_0 = models.CharField(max_length=1, default='')
    relay_state_buff_1 = models.CharField(max_length=1, default='')
    relay_state_buff_2 = models.CharField(max_length=1, default='')
    relay_state_buff_3 = models.CharField(max_length=1, default='')

class ADCData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    data_0 = models.IntegerField(default=0)
    data_1 = models.IntegerField(default=0)
    data_2 = models.IntegerField(default=0)
    data_3 = models.IntegerField(default=0)
    data_4 = models.IntegerField(default=0)
    data_5 = models.IntegerField(default=0)
    data_6 = models.IntegerField(default=0)

class CANData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)

    # Motor Controller
    status_word = models.IntegerField(default=0)
    position_val = models.IntegerField(default=0)
    torque_val  = models.IntegerField(default=0)
    controller_temp = models.IntegerField(default=0)
    motor_temp = models.IntegerField(default=0)
    dc_link_voltage = models.IntegerField(default=0)
    logic_power_supply_voltage = models.IntegerField(default=0)
    current_demand = models.IntegerField(default=0)
    motor_current_val = models.IntegerField(default=0)
    electrical_angle = models.IntegerField(default=0)
    phase_a_current = models.IntegerField(default=0)
    phase_b_current = models.IntegerField(default=0)

    # BMS
    internal_relay_state = models.IntegerField(default=0)  # Used within the CANManager to set BMS relay states
    relay_state = models.IntegerField(default=0)           # This should agree with the above (given a small delay)
    rolling_counter = models.IntegerField(default=0)
    fail_safe_state = models.IntegerField(default=0)
    peak_current = models.IntegerField(default=0)
    pack_voltage_inst = models.IntegerField(default=0)
    pack_voltage_open = models.IntegerField(default=0)
    pack_soc = models.IntegerField(default=0)
    pack_amphours = models.IntegerField(default=0)
    pack_resistance = models.IntegerField(default=0)
    pack_dod = models.IntegerField(default=0)
    pack_soh = models.IntegerField(default=0)
    current_limit_status = models.IntegerField(default=0)
    max_pack_dcl = models.IntegerField(default=0)
    avg_pack_current = models.IntegerField(default=0)
    highest_temp = models.IntegerField(default=0)
    highest_temp_id = models.IntegerField(default=0)

    avg_temp = models.IntegerField(default=0)
    internal_temp = models.IntegerField(default=0)
    low_cell_voltage = models.IntegerField(default=0)
    low_cell_voltage_id = models.IntegerField(default=0)
    high_cell_voltage = models.IntegerField(default=0)
    high_cell_voltage_id = models.IntegerField(default=0)

    low_cell_internalR = models.IntegerField(default=0)
    low_cell_internalR_id = models.IntegerField(default=0)
    high_cell_internalR = models.IntegerField(default=0)
    high_cell_internalR_id = models.IntegerField(default=0)
    power_voltage_input = models.IntegerField(default=0)
    dtc_status_one = models.IntegerField(default=0)
    dtc_status_two = models.IntegerField(default=0)
    adaptive_total_cap = models.IntegerField(default=0)
    adaptive_amphours = models.IntegerField(default=0)
    adaptive_soc = models.IntegerField(default=0)

class I2CData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    sensor_0x48_0 = models.IntegerField(default=0)
    sensor_0x48_1 = models.IntegerField(default=0)
    sensor_0x48_2 = models.IntegerField(default=0)
    sensor_0x48_3 = models.IntegerField(default=0)
    sensor_0x49_0 = models.IntegerField(default=0)
    sensor_0x49_1 = models.IntegerField(default=0)
    sensor_0x49_2 = models.IntegerField(default=0)
    sensor_0x49_3 = models.IntegerField(default=0)
    sensor_0x77_0 = models.IntegerField(default=0)
    sensor_0x77_1 = models.IntegerField(default=0)
    sensor_0x77_2 = models.IntegerField(default=0)
    sensor_0x77_3 = models.IntegerField(default=0)

class PRUData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    orange_distance = models.IntegerField(default=0)
    orange_velocity = models.IntegerField(default=0)
    wheel_distance = models.IntegerField(default=0)
    wheel_velocity = models.IntegerField(default=0)

class Errors(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    ADCError = models.IntegerField(default=0)
    CANError = models.IntegerField(default=0)
    I2CError = models.IntegerField(default=0)
    PRUError = models.IntegerField(default=0)
    NetworkError = models.IntegerField(default=0)
    OtherError = models.IntegerField(default=0)

class State(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    state = models.IntegerField(default=0)
