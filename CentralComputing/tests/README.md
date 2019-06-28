# Write up of all important tests.

## Unit Tests verifying state diagram:

These tests automatically verify that the state machine cannot move into other states without having the correct transition requirements met.

### Proper mapping Unit Tests: 
  _Try to transition to invalid states_

1. Test that `Safe Mode` can only transition to:
    * `Functional Tests (outside)`
2. Test that `Functional Tests (outside)` can only transition to:
    * `Loading`
    * `Safe Mode`
3. Test that `Loading` can only transition to:
    * `Functional Tests (inside)`
    * `Safe Mode`
4. Test that `Functional Tests (inside)` can only transition to:
    * `Ready For Launch`
    * `Safe Mode`
5. Test that `Ready For Launch` can only transition to:
    * `Flight - Acceleration`
    * `Safe Mode`
6. Test that `Flight - Acceleration` can only transition to:
    * `Flight - Coast`
    * `Flight - Abort`
7. Test that `Flight - Coast` can only transition to:
    * `Flight - Brake`
    * `Flight - Abort`
8. Test that `Flight - Brake` can only transition to:
    * `Safe Mode`
    * `Flight - Abort`
9. Test that `Flight - Abort` can only transition to:
    * `Safe Mode`

### Automatic Transition Tests (Without Errors)
  _Ensure that automatic transitions work, assuming no Error conditions. These are larger scale scenarios that test the overall behavior of the software._

1. Test that `Flight - Acceleration` transitions to:
    * `Flight - Coast` 
    * when `accel_timeout` is exceeded53
2. Test that ` Flight - Acceleration ` transitions to:  
    * Flight - Coast 
    * when `d+(v^2) / (2*A_b) ≥ D_t − D_b` is satisfied
3. Test that ` Flight - Coast ` transitions to:
    * Flight - Brake
    * when `coast_timeout` is exceeded
4. Test that ` Flight - Brake ` transitions to:
    * Safe Mode
    * when `brake_timeout` is exceeded
    * AND
    * when `|accel| =< not_moving_accel` is satisfied
    * AND
    * when `|vel| =< not_moving_vel` is satisfied

### Error Generation Unit Tests (Safety Critical Errors)
  _Test that `Safety Critical Errors` generate the appropriate internal error when conditions are met._

1. For brevity, please see a list of `Safety Critical Errors` on the state Diagram
2. Every type of safety critical error will be tested in every different software state to verify behavior 

### Error Generation Unit Tests (Sensor Data Errors)
Test that `Sensor Data Errors` generate the appropriate internal error when conditions are met
1. For brevity, please see a list of `Sensor Data Errors` on the State Diagram
2. Every type of sensor data error will be tested in every different software state to verify behavior

### Automatic Transition Tests (With Errors)
  _Test behavior of automatic transitions triggered by Errors. This involves the pod moving into either its `Safe Mode` or `Flight - Abort` upon errors_ 
1. Test that `Safe Mode` , transitions to:
    * Does not transition upon `Safety Critical Error`
    * Does not transition upon `Sensor Data Error`
2. Test that `Functional Tests (outside)` transitions to:
    * `Safe Mode` upon `Safety Critical Error`
    * Does not transition upon `Sensor Data Error`
3. Test that `Loading` transitions to:
    * `Safe Mode` upon `Safety Critical Error`
    * Does not transition upon `Sensor Data Error`
4. Test that ` Functional Tests (inside) ` transitions to:
    * `Safe Mode` upon `Safety Critical Error`
    * Does not transition upon `Sensor Data Error`
5. Test that `Ready For Launch` transitions to:
    * `Safe Mode` upon `Safety Critical Error`
    * Does not transition upon `Sensor Data Error`
6. Test that `Flight - Acceleration` transitions to:
    * `Flight - Abort` upon `Safety Critical Error`
    * `Flight - Abort` upon `Sensor Data Error`
7. Test that `Flight - Coast` transitions to:
    * `Flight - Abort` upon `Safety Critical Error`
    * `Flight - Abort` upon `Sensor Data Error`
8. Test that `Flight - Brake` transitions to:
    * `Flight - Abort` upon `Safety Critical Error`
    * `Flight - Abort` upon `Sensor Data Error`
9. Test that `Flight - Abort` transitions to:
    * Does not transition upon `Safety Critical Error`
    * Does not transition upon `Sensor Data Error`

---

## Parking Lot / Vacuum Chamber / External Sub Track / Hyperloop Test Track Open Air
1. Sensor Tests. No Motor / No Brake
    * Ability to test all sensors without any motor or brake actuation. 
2. Automatic State Transitions with low thresholds. No Motor / No Brake
    * Lower the thresholds that trigger state transitions.
    * Push Pod on cart or rail, to test state transitions with accelerometer and optical encoders
3. Test Brakes. No Motor / Brake Only 
    * Pod is pushed up to speed manually, coasts, and brakes on its own
    * Automatic transition thresholds are lowered
4. Simulate Full Run. Motor / Brake 
    * Pod accelerates for a brief period under its own power, coasts, and brakes on its own
    * Automatic transition thresholds are lowered.
    * This test can be scaled for different lengths and motor powers