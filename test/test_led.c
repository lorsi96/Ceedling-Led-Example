/* ****************************************************************************************************************** */
/*                                                     Inclusions                                                     */
/* ****************************************************************************************************************** */
#include "leds.h"
#include "mock_exceptions.h"
#include "unity.c"

/* ****************************************************************************************************************** */
/*                                                  Private Variables                                                 */
/* ****************************************************************************************************************** */
static uint16_t port;
static LedGroup_obj_t ledGroup;

/* ****************************************************************************************************************** */
/*                                                 Set Up & Tear Down                                                 */
/* ****************************************************************************************************************** */
void setUp() { LedGroup_init(&ledGroup, (uint64_t)&port); }

void tearDown() {}

/* ****************************************************************************************************************** */
/*                                                   Test Functions                                                   */
/* ****************************************************************************************************************** */

void test_LedsOffAfterCreate() {
  port = ALL_LEDS_ON;
  LedGroup_init(&ledGroup, (uint64_t)&port);
  TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF, port);
}

void test_TurnOnLed() {
  LedGroup_led_t led = 11;
  LedGroup_turnOn(&ledGroup, led);
  TEST_ASSERT_EQUAL_HEX16(1 << (led - 1), port);
}

void test_TurnOffLed() {
  LedGroup_led_t led = 3;
  LedGroup_turnOn(&ledGroup, led);
  LedGroup_turnOff(&ledGroup, led);
  TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF, port);
}

void test_TurnOnAllLeds() {
  LedGroup_setAllOn(&ledGroup);
  TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_ON, port);
}

void test_TurnOffAllLeds() {
  LedGroup_setAllOn(&ledGroup);
  LedGroup_setAllOff(&ledGroup);
  TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF, port);
}

void test_FullLedSequence() {
  uint16_t expected = ALL_LEDS_OFF;
  for (uint8_t i = MIN_LED_IND; i <= MAX_LED_IND; i++) {
    LedGroup_turnOn(&ledGroup, i);
    expected |= 1 << (i - 1);
    TEST_ASSERT_EQUAL_HEX16(expected, port);
  }
  for (uint8_t i = MAX_LED_IND; i >= MIN_LED_IND; i--) {
    LedGroup_turnOff(&ledGroup, i);
    expected &= ~(1 << (i - 1));
    TEST_ASSERT_EQUAL_HEX16(expected, port);
  }
}

void test_TurnLedsWithoutOverlap() {
  LedGroup_led_t led1 = 11, led2 = 13, led3 = 5;
  LedGroup_turnOn(&ledGroup, led1);
  LedGroup_turnOn(&ledGroup, led2);
  LedGroup_turnOn(&ledGroup, led3);
  LedGroup_turnOff(&ledGroup, led2);
  TEST_ASSERT_EQUAL_HEX16(0b0000010000010000, port);
}

void test_InvalidLedIndexes() {
  Exceptions_throw_Expect(LED_OUT_OF_BOUNDS, "Attempted to turn on an invalid LED.");
  LedGroup_turnOn(&ledGroup, 17);

  Exceptions_throw_Expect(LED_OUT_OF_BOUNDS, "Attempted to turn off an invalid LED.");
  LedGroup_turnOff(&ledGroup, 0);

  Exceptions_throw_Expect(LED_OUT_OF_BOUNDS, "Attempted to turn on an invalid LED.");
  LedGroup_turnOn(&ledGroup, 51);
}

void test_GetLedState() {
  LedGroup_turnOn(&ledGroup, 5);
  bool state1 = LegGroup_getLedState(&ledGroup, 1);
  bool state5 = LegGroup_getLedState(&ledGroup, 5);
  TEST_ASSERT_FALSE(state1);
  TEST_ASSERT_TRUE(state5);
}

void test_TwoInstancesDontOverlap() {
  uint16_t portA;
  uint16_t portB;
  LedGroup_obj_t groupA;
  LedGroup_obj_t groupB;

  LedGroup_init(&groupA, (uint64_t)&portA);
  LedGroup_init(&groupB, (uint64_t)&portB);

  LedGroup_turnOn(&groupA, 5);
  LedGroup_turnOn(&groupB, 6);

  TEST_ASSERT_TRUE(LegGroup_getLedState(&groupA, 5));
  TEST_ASSERT_TRUE(LegGroup_getLedState(&groupB, 6));
  TEST_ASSERT_FALSE(LegGroup_getLedState(&groupB, 5));
  TEST_ASSERT_FALSE(LegGroup_getLedState(&groupA, 6));
}