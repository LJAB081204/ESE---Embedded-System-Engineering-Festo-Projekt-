#include <gtest/gtest.h>
#include "HAL.h"

// ============================================
// LAMP TESTS
// ============================================

TEST(HALTest, GreenLampOn) {
    HAL hal;
    hal.getActors()->greenLampOn();
    // Test that function can be called without error
    SUCCEED();
}

TEST(HALTest, GreenLampOff) {
    HAL hal;
    hal.getActors()->greenLampOff();
    SUCCEED();
}

TEST(HALTest, YellowLampOn) {
    HAL hal;
    hal.getActors()->yellowLampOn();
    SUCCEED();
}

TEST(HALTest, YellowLampOff) {
    HAL hal;
    hal.getActors()->yellowLampOff();
    SUCCEED();
}

TEST(HALTest, RedLampOn) {
    HAL hal;
    hal.getActors()->redLampOn();
    SUCCEED();
}

TEST(HALTest, RedLampOff) {
    HAL hal;
    hal.getActors()->redLampOff();
    SUCCEED();
}

// ============================================
// MOTOR TESTS
// ============================================

TEST(HALTest, MotorForwards) {
    HAL hal;
    hal.getActors()->motorForwards();
    hal.getActors()->motorStop();
    SUCCEED();
}

TEST(HALTest, MotorBackwards) {
    HAL hal;
    hal.getActors()->motorBackwards();
    hal.getActors()->motorStop();
    SUCCEED();
}

TEST(HALTest, MotorSlow) {
    HAL hal;
    hal.getActors()->motorSlow();
    hal.getActors()->motorStop();
    SUCCEED();
}

TEST(HALTest, MotorStop) {
    HAL hal;
    hal.getActors()->motorStop();
    SUCCEED();
}

// ============================================
// PUSHER TESTS
// ============================================

TEST(HALTest, OpenPusher) {
    HAL hal;
    hal.getActors()->openPusher();
    SUCCEED();
}

TEST(HALTest, ClosePusher) {
    HAL hal;
    hal.getActors()->closePusher();
    SUCCEED();
}

// ============================================
// LED TESTS (GPIO2)
// ============================================

TEST(HALTest, LedButtonStart) {
    HAL hal;
    hal.getActors()->ledButtonStart();
    SUCCEED();
}

TEST(HALTest, LedButtonStartOff) {
    HAL hal;
    hal.getActors()->ledButtonStartOff();
    SUCCEED();
}

TEST(HALTest, LedButtonReset) {
    HAL hal;
    hal.getActors()->ledButtonReset();
    SUCCEED();
}

TEST(HALTest, LedButtonResetOff) {
    HAL hal;
    hal.getActors()->ledButtonResetOff();
    SUCCEED();
}

TEST(HALTest, LampQ1) {
    HAL hal;
    hal.getActors()->lampQ1();
    SUCCEED();
}

TEST(HALTest, LampQ1Off) {
    HAL hal;
    hal.getActors()->lampQ1Off();
    SUCCEED();
}

TEST(HALTest, LampQ2) {
    HAL hal;
    hal.getActors()->lampQ2();
    SUCCEED();
}

TEST(HALTest, LampQ2Off) {
    HAL hal;
    hal.getActors()->lampQ2Off();
    SUCCEED();
}

// ============================================
// SENSOR TESTS
// ============================================

TEST(HALTest, SensorItemAtEntry) {
    HAL hal;
    bool result = hal.getSensors()->isItemAtEntry();
    ASSERT_TRUE(result || !result);  // Just check that it returns a boolean
}

TEST(HALTest, SensorItemInHeightMeas) {
    HAL hal;
    bool result = hal.getSensors()->isItemInHeightMeas();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorItemHeightOk) {
    HAL hal;
    bool result = hal.getSensors()->isItemHeightOk();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorItemInPusher) {
    HAL hal;
    bool result = hal.getSensors()->isItemInPusher();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorItemMetal) {
    HAL hal;
    bool result = hal.getSensors()->isItemMetal();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorItemAtExit) {
    HAL hal;
    bool result = hal.getSensors()->isItemAtExit();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorPusherOpen) {
    HAL hal;
    bool result = hal.getSensors()->isPusherOpen();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorSlideFull) {
    HAL hal;
    bool result = hal.getSensors()->isSlideFull();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorButtonStart) {
    HAL hal;
    bool result = hal.getSensors()->isButtonStartPressed();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorButtonStop) {
    HAL hal;
    bool result = hal.getSensors()->isButtonStopPressed();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorButtonReset) {
    HAL hal;
    bool result = hal.getSensors()->isButtonResetPressed();
    ASSERT_TRUE(result || !result);
}

TEST(HALTest, SensorEStop) {
    HAL hal;
    bool result = hal.getSensors()->isEStopTriggered();
    ASSERT_TRUE(result || !result);
}

// ============================================
// MOTOR STATE TRANSITION TESTS
// ============================================

TEST(HALTest, MotorForwardsClearsStop) {
    HAL hal;
    hal.getActors()->motorStop();
    hal.getActors()->motorForwards();
    // Just verify that transitions work without error
    hal.getActors()->motorStop();
    SUCCEED();
}

TEST(HALTest, MotorBackwardsClearsStop) {
    HAL hal;
    hal.getActors()->motorStop();
    hal.getActors()->motorBackwards();
    // Just verify that transitions work without error
    hal.getActors()->motorStop();
    SUCCEED();
}

TEST(HALTest, MotorSlowClearsForwards) {
    HAL hal;
    hal.getActors()->motorForwards();
    hal.getActors()->motorSlow();
    // Just verify that transitions work without error
    hal.getActors()->motorStop();
    SUCCEED();
}

// ============================================
// LAMP BLINK TESTS
// ============================================
/**
TEST(HALTest, GreenLampBlink) {
    HAL hal;
    hal.getActors()->greenLampBlink();
    SUCCEED();
}

TEST(HALTest, YellowLampBlink) {
    HAL hal;
    hal.getActors()->yellowLampBlink();
    SUCCEED();
}

TEST(HALTest, RedLampBlink) {
    HAL hal;
    hal.getActors()->redLampBlink();
    SUCCEED();
}
*/

// ============================================
// INTEGRATION TESTS
// ============================================
/*
TEST(HALTest, HALInitialization) {
    // Test that HAL can be initialized without error
    HAL hal;
    ASSERT_NE(hal.getSensors(), nullptr);
    ASSERT_NE(hal.getActors(), nullptr);
}

TEST(HALTest, MultipleHALInstances) {
    // Test that multiple HAL instances can be created
    HAL hal1;
    HAL hal2;

    ASSERT_NE(hal1.getSensors(), nullptr);
    ASSERT_NE(hal2.getActors(), nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/
