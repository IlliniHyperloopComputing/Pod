from django.db import models

# Create your models here.

class MotionData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    data = models.IntegerField()

class ADCData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    data = models.IntegerField()

class CANData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)

    # Motor Controller
    data = models.IntegerField()
    status_word = models.IntegerField()
    position_val = models.IntegerField()
    torque_val  = models.IntegerField()
    controller_temp = models.IntegerField()
    motor_temp = models.IntegerField()
    dc_link_voltage = models.IntegerField()
    logic_power_supply_voltage = models.IntegerField()
    current_demand = models.IntegerField()
    motor_current_val = models.IntegerField()
    electrical_angle = models.IntegerField()
    phase_a_current = models.IntegerField()
    phase_b_current = models.IntegerField()

    # BMS
    internal_relay_state = models.IntegerField()  # Used within the CANManager to set BMS relay states
    relay_state = models.IntegerField()           # This should agree with the above (given a small delay)
    rolling_counter = models.IntegerField()
    fail_safe_state = models.IntegerField()
    peak_current = models.IntegerField()
    pack_voltage_inst = models.IntegerField()
    pack_voltage_open = models.IntegerField()
    pack_soc = models.IntegerField()
    pack_amphours = models.IntegerField()
    pack_resistance = models.IntegerField()
    pack_dod = models.IntegerField()
    pack_soh = models.IntegerField()
    current_limit_status = models.IntegerField()
    max_pack_dcl = models.IntegerField()
    avg_pack_current = models.IntegerField()
    highest_temp = models.IntegerField()
    highest_temp_id = models.IntegerField()
    avg_temp = models.IntegerField()
    internal_temp = models.IntegerField()
    low_cell_voltage = models.IntegerField()
    low_cell_voltage_id = models.IntegerField()
    high_cell_voltage = models.IntegerField()
    high_cell_voltage_id = models.IntegerField()
    low_cell_internalR = models.IntegerField()
    low_cell_internalR_id = models.IntegerField()
    high_cell_internalR = models.IntegerField()
    high_cell_internalR_id = models.IntegerField()
    power_voltage_input = models.IntegerField()
    dtc_status_one = models.IntegerField()
    dtc_status_two = models.IntegerField()
    adaptive_total_cap = models.IntegerField()
    adaptive_amphours = models.IntegerField()
    adaptive_soc = models.IntegerField()

class I2CData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    data = models.IntegerField()

class PRUData(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    data = models.IntegerField()

class Errors(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    ADCError = models.IntegerField()
    CANError = models.IntegerField()
    I2CError = models.IntegerField()
    PRUError = models.IntegerField()
    NetworkError = models.IntegerField()
    OtherError = models.IntegerField()

class State(models.Model):
    date_time = models.DateTimeField(auto_now=True,primary_key=True)
    state = models.IntegerField()