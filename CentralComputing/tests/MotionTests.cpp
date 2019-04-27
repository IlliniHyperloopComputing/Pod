#ifdef SIM  // Only compile if building test executable
#include "PodTest.cpp"
#include "MotionModel.h"
#include "Defines.hpp"
#include "Utils.h"
#include <memory>
#include <random>
using std::make_shared;
using Utils::print;
using Utils::LogLevel;

#define MOTION_TEST_SETUP \
        MotionModel mm; \
        std::shared_ptr<UnifiedState> unified_state = make_shared<UnifiedState>(); \
        unified_state->motion_data = make_shared<MotionData>(); \
        unified_state->adc_data = make_shared<ADCData>(); \
        unified_state->adc_data->accel[0] = 0; \
        unified_state->adc_data->accel[1] = 0; \
        unified_state->adc_data->accel[2] = 0; \
        unified_state->can_data = make_shared<CANData>(); \
        unified_state->can_data->wheel_distance = 0; \
        unified_state->i2c_data = make_shared<I2CData>(); \
        unified_state->pru_data = make_shared<PRUData>(); \
        unified_state->pru_data->orange_distance[0] = 0; \
        unified_state->pru_data->orange_distance[1] = 0; \
        unified_state->pru_data->wheel_distance[0] = 0; \
        unified_state->pru_data->wheel_distance[1] = 0; \
        unified_state->pru_data->wheel_velocity[0] = 0; \
        unified_state->pru_data->wheel_velocity[1] = 0; 


TEST(MotionTests, distance_no_faults) {
    // Create state with all zeros
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("defaultConfig.txt"), true);
    MOTION_TEST_SETUP;

    // Test that with no input, nothing happens
    mm.calculate(unified_state);
    EXPECT_EQ(unified_state->motion_data->x[0], 0);
    EXPECT_EQ(unified_state->motion_data->x[1], 0);
    EXPECT_EQ(unified_state->motion_data->x[2], 0);

    // Iterate over 1000, test very basic distance calculation.
    for(int i = 0; i< 1000; i++){
        if (i%50 == 0){
            unified_state->pru_data->orange_distance[0] = i;
            unified_state->pru_data->orange_distance[1] = i;
        }
        unified_state->pru_data->wheel_distance[0] = i;
        unified_state->pru_data->wheel_distance[1] = i;
        unified_state->can_data->wheel_distance = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], i);
        EXPECT_EQ(unified_state->motion_data->x[1], 0);
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, distance_orange_fault) {
    // Create state with all zeros
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("defaultConfig.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic distance calculation.
    for(int i = 0; i< 1000; i++){
        if (i%50 == 0){
            unified_state->pru_data->orange_distance[0] = 0;  // FAULT
            unified_state->pru_data->orange_distance[1] = i;
        }
        unified_state->pru_data->wheel_distance[0] = i;
        unified_state->pru_data->wheel_distance[1] = i;
        unified_state->can_data->wheel_distance = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], i);
        EXPECT_EQ(unified_state->motion_data->x[1], 0);
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, distance_wheel_orange_fault) {
    // Create state with all zeros
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("defaultConfig.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic distance calculation.
    for(int i = 0; i< 1000; i++){
        if (i%50 == 0){
            unified_state->pru_data->orange_distance[0] = 0;  // FAULT
            unified_state->pru_data->orange_distance[1] = i;
        }
        unified_state->pru_data->wheel_distance[0] = i;
        unified_state->pru_data->wheel_distance[1] = 0; // FAULT
        unified_state->can_data->wheel_distance = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], i);
        EXPECT_EQ(unified_state->motion_data->x[1], 0);
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, distance_wheel_diverge) {
    // Create state with all zeros
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("defaultConfig.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic distance calculation.
    for(int i = 0; i< 1000; i++){
        if (i%50 == 0){
            unified_state->pru_data->orange_distance[0] = i;
            unified_state->pru_data->orange_distance[1] = i;
            unified_state->pru_data->wheel_distance[0] += 2;  // DIVERGE UP
            unified_state->pru_data->wheel_distance[1] -= 1;  // DIVERGE DOWN
        }
        unified_state->pru_data->wheel_distance[0] += 1;
        unified_state->pru_data->wheel_distance[1] += 1; 
        unified_state->can_data->wheel_distance = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], unified_state->pru_data->wheel_distance[0]);
        EXPECT_EQ(unified_state->motion_data->x[1], 0);
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, distance_wheel_diverge_orange_fault) {
    // Create state with all zeros
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("defaultConfig.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic distance calculation.
    for(int i = 0; i< 1000; i++){
        if (i%50 == 0){
            unified_state->pru_data->orange_distance[0] = i;
            unified_state->pru_data->orange_distance[1] = i - 50; // FAULT
            unified_state->pru_data->wheel_distance[0] -= 8;  // DIVERGE DOWN
            unified_state->pru_data->wheel_distance[1] += 5;  // DIVERGE UP
        }
        unified_state->pru_data->wheel_distance[0] += 1;
        unified_state->pru_data->wheel_distance[1] += 1; 
        unified_state->can_data->wheel_distance = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], unified_state->pru_data->wheel_distance[1]);
        EXPECT_EQ(unified_state->motion_data->x[1], 0);
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, distance_wheel_diverge_orange_double_fault) {
    // Create state with all zeros
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("defaultConfig.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic distance calculation.
    for(int i = 0; i< 1000; i++){
        if (i%50 == 0){
            unified_state->pru_data->orange_distance[0] = i - 100; // DOUBLE FAULT
            unified_state->pru_data->orange_distance[1] = i - 50; // DOUBLE FAULT
            unified_state->pru_data->wheel_distance[0] -= 2;  // DIVERGE DOWN
            unified_state->pru_data->wheel_distance[1] += 10;  // DIVERGE UP
        }
        unified_state->pru_data->wheel_distance[0] += 1;
        unified_state->pru_data->wheel_distance[1] += 1; 
        unified_state->can_data->wheel_distance = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], unified_state->pru_data->wheel_distance[1]);
        EXPECT_EQ(unified_state->motion_data->x[1], 0);
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, distance_motor_slip) {
    // Create state with all zeros
    // Load test.txt, it defines motor_distance_clamp as 10
    ConfiguratorManager::config.clear();
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/test.txt"), true);
    MOTION_TEST_SETUP;


    // Iterate over 1000, test very basic distance calculation.
    for(int i = 0; i< 1000; i++){
        if (i%50 == 0){
            unified_state->pru_data->orange_distance[0] = i;
            unified_state->pru_data->orange_distance[1] = i;
        }
        unified_state->pru_data->wheel_distance[0] = i;
        unified_state->pru_data->wheel_distance[1] = i;
        unified_state->can_data->wheel_distance = i+1;

        mm.calculate(unified_state);
        EXPECT_GE(unified_state->motion_data->x[0], i);
        EXPECT_LE(unified_state->motion_data->x[0], i+10);
        EXPECT_EQ(unified_state->motion_data->x[1], 0);
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, velocity_basic){
    // Create state with all zeros
    // Load test.txt, it defines motor_distance_clamp as 10
    ConfiguratorManager::config.clear();
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/velocity_basic.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 10000; i++){
        unified_state->pru_data->wheel_velocity[0] = i;
        unified_state->pru_data->wheel_velocity[1] = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_EQ(unified_state->motion_data->x[1], i);
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, velocity_low_pass){
    // Create state with all zeros
    // Load test.txt, it defines motor_distance_clamp as 10
    ConfiguratorManager::config.clear();
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/velocity_low_pass.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 1000; i++){
        unified_state->pru_data->wheel_velocity[0] = 50;
        unified_state->pru_data->wheel_velocity[1] = 50;

        if (i%50 == 0){
            unified_state->pru_data->wheel_velocity[0] += 50; //High frequency event
            unified_state->pru_data->wheel_velocity[1] += 40;
        }

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_LE(unified_state->motion_data->x[1], 50+13); // filter keeps it under this value
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, velocity_random_noise){
    // Create state with all zeros
    // Load test.txt, it defines motor_distance_clamp as 10
    ConfiguratorManager::config.clear();
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/velocity_low_pass.txt"), true);
    MOTION_TEST_SETUP;
    // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(-3, 3);

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 10000; i++){
        unified_state->pru_data->wheel_velocity[0] = i/40 + dis(gen);
        unified_state->pru_data->wheel_velocity[1] = i/40 + dis(gen);

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_LE(unified_state->motion_data->x[1], i/40 + 4); // filter keeps it under this value
        EXPECT_GE(unified_state->motion_data->x[1], i/40 - 4); // filter keeps it under this value
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, velocity_diverge){
    // Create state with all zeros
    // Load test.txt, it defines motor_distance_clamp as 10
    ConfiguratorManager::config.clear();
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/velocity_low_pass.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 1000; i++){
        unified_state->pru_data->wheel_velocity[0] = i + 7;
        unified_state->pru_data->wheel_velocity[1] = i - 2;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_LE(unified_state->motion_data->x[1], i + 4); 
        EXPECT_GE(unified_state->motion_data->x[1], i - 4); 
        EXPECT_EQ(unified_state->motion_data->x[2], 0);
    }
}

TEST(MotionTests, acceleration_basic){
    // Create state with all zeros
    // Load test.txt, it defines motor_distance_clamp as 10
    ConfiguratorManager::config.clear();
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/velocity_basic.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 1000; i++){
        unified_state->adc_data->accel[0] = i;
        unified_state->adc_data->accel[1] = i;
        unified_state->adc_data->accel[2] = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_EQ(unified_state->motion_data->x[1], 0); 
        EXPECT_EQ(unified_state->motion_data->x[2], i);
    }
}

TEST(MotionTests, acceleration_one_failure){
    // Create state with all zeros
    // Load test.txt, it defines motor_distance_clamp as 10
    ConfiguratorManager::config.clear();
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/velocity_basic.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 1000; i++){
        unified_state->adc_data->accel[0] = 0;
        unified_state->adc_data->accel[1] = i;
        unified_state->adc_data->accel[2] = i;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_EQ(unified_state->motion_data->x[1], 0); 
        EXPECT_EQ(unified_state->motion_data->x[2], i);
    }
}

TEST(MotionTests, acceleration_select_median){
    // Create state with all zeros
    ConfiguratorManager::config.clear();
    EXPECT_EQ(ConfiguratorManager::config.openConfigFile("tests/velocity_basic.txt"), true);
    MOTION_TEST_SETUP;

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 1000; i++){
        unified_state->adc_data->accel[0] = i+7;
        unified_state->adc_data->accel[1] = i+2;
        unified_state->adc_data->accel[2] = i+11;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_EQ(unified_state->motion_data->x[1], 0); 
        EXPECT_EQ(unified_state->motion_data->x[2], i+7);
    }

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 1000; i++){
        unified_state->adc_data->accel[0] = i+2;
        unified_state->adc_data->accel[1] = i+7;
        unified_state->adc_data->accel[2] = i+8;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_EQ(unified_state->motion_data->x[1], 0); 
        EXPECT_EQ(unified_state->motion_data->x[2], i+7);
    }

    // Iterate over 1000, test very basic velocity calculation.
    for(int i = 0; i< 1000; i++){
        unified_state->adc_data->accel[0] = i+1;
        unified_state->adc_data->accel[1] = i+9;
        unified_state->adc_data->accel[2] = i+7;

        mm.calculate(unified_state);
        EXPECT_EQ(unified_state->motion_data->x[0], 0);
        EXPECT_EQ(unified_state->motion_data->x[1], 0); 
        EXPECT_EQ(unified_state->motion_data->x[2], i+7);
    }
}

// case where motor distance is way ahead of all other distance measurements
// case where one wheel optical encoder failes
// case where one orange tape sensor fails
// case where our distance is at a MINIMUM the orange tape distance
// case where you stop, and orange tape velocity still thinks you are going fast


#endif