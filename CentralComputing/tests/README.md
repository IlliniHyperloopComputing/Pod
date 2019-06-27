# Write up of all important tests.

## Unit Tests verifying state diagram:

These tests automatically verify that the state machine cannot move into other states without having the correct transition requirements met.

### Proper mapping Unit Tests: 
  _Try to transition to invalid states_

1. Test that `Safe Mode` can only transition to:
  * `Functional Tests (outside)`
2. Test that `Functional Tests (outside)` can only transition to:
  * Loading
  * Safe Mode
3. Test that `Loading` can only transition to:
  * Functional Tests (inside)
  * Safe Mode
4. Test that `Functional Tests (inside)` can only transition to:
  * Ready For Launch
  * Safe Mode
5. Test that `Ready For Launch ` can only transition to:
  * Flight - Acceleration
  * Safe Mode
6. Test that ` Flight - Acceleration ` can only transition to:
  * Flight - Coast
  * Flight - Abort
7. Test that ` Flight - Coast ` can only transition to:
  * Flight - Brake
  * Flight - Abort
8. Test that ` Flight - Brake ` can only transition to:
  * Safe Mode
  * Flight - Abort
9. Test that ` Flight - Abort ` can only transition to:
  * Safe Mode

### Automatic Transition Tests (Without Errors)
  _Ensure that automatic transitions work, assuming no Error conditions_
i. Test that ` Flight - Acceleration ` transitions to:
1. Flight - Coast
2. when `accel_timeout` is exceeded53
ii.
iii.
iv.
Test that ` Flight - Acceleration ` transitions to:
1. Flight - Coast
2. when `d+(v^2) / (2*A_b) ≥ D_t − D_b` is satisfied
Test that ` Flight - Coast ` transitions to:
1. Flight - Brake
2. when `coast_timeout` is exceeded
Test that ` Flight - Brake ` transitions to:
1. Safe Mode
2. when `brake_timeout` is exceeded
3. AND
4. when `|accel| =< not_moving_accel` is satisfied
5. AND
6. when `|vel| =< not_moving_vel` is satisfied
d. Error Generation Unit Tests
i. Test that `Safety Critical Errors` generate the appropriate
internal error when conditions are met
1. For brevity, please see a list of `Safety Critical Errors`
on the state Diagram
2. Every type of safety critical error will be tested in
every different software state to verify behavior
ii.
Test that `Sensor Data Errors` generate the appropriate
internal error when conditions are met
1. For brevity, please see a list of `Sensor Data Errors`
on the State Diagram
2. Every type of sensor data error will be tested in every
different software state to verify behavior
e. Automatic Transition Tests (With Errors)
i. Test that ` Safe Mode ` , transitions to:
1. Does not transition upon `Safety Critical Error`
2. Does not transition upon `Sensor Data Error`
ii.
Test that ` Functional Tests (outside) ` transitions to:
1. Safe Mode ` upon `Safety Critical Error`
2. Does not transition upon `Sensor Data Error`
iii. Test that ` Loading ` transitions to:54
iv.
v.
vi.
vii.
viii.
ix.
1. Safe Mode ` upon `Safety Critical Error`
2. Does not transition upon `Sensor Data Error`
Test that ` Functional Tests (inside) ` transitions to:
1. Safe Mode ` upon `Safety Critical Error`
2. Does not transition upon `Sensor Data Error`
Test that ` Ready For Launch ` transitions to:
1. Safe Mode ` upon `Safety Critical Error`
2. Does not transition upon `Sensor Data Error`
Test that ` Flight - Acceleration ` transitions to:
1. Flight - Abort u
pon `Safety Critical Error`
2. Flight - Abort u
pon `Sensor Data Error`
Test that ` Flight - Coast ` transitions to:
1. Flight - Abort u
pon `Safety Critical Error`
2. Flight - Abort u
pon `Sensor Data Error`
Test that ` Flight - Brake ` transitions to:
1. Flight - Abort u
pon `Safety Critical Error`
2. Flight - Abort u
pon `Sensor Data Error`
Test that ` Flight - Abort ` transitions to:
1. Does not transition upon `Safety Critical Error`
2. Does not transition upon `Sensor Data Error`
