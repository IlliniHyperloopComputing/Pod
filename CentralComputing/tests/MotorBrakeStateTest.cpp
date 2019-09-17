#ifdef SIM
#include "PodTest.cpp"

TEST_F(PodTest, SafeMode) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, FunctionalTestOutside) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, FunctionalTestOutside) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, Loading) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, FunctionalTestInside) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, LaunchReady) {
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, FlightAccel) {
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, FlightCoast) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, FlightBrake) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, FlightAbort) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brakes.is_enabled());
}

TEST_F(PodTest, FlightBrake) {
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brakes.is_enabled());
}

#endif
