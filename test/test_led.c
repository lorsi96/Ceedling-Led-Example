/* ****************************************************************************************************************** */
/*                                                     Inclusions                                                     */
/* ****************************************************************************************************************** */
#include "leds.h"
#include "mock_exceptions.h"
#include "unity.c"

/* ****************************************************************************************************************** */
/*                                                  Private Variables                                                 */
/* ****************************************************************************************************************** */
static LedGroup_obj_t ledGroup;

/* ****************************************************************************************************************** */
/*                                                 Set Up & Tear Down                                                 */
/* ****************************************************************************************************************** */
void setUp() { LedGroup_init(&ledGroup); }

void tearDown() {}

/* ****************************************************************************************************************** */
/*                                                   Test Functions                                                   */
/* ****************************************************************************************************************** */

void test_LedsOffAfterCreate() {
  ledGroup.port = ALL_LEDS_ON;
  LedGroup_init(&ledGroup);
  TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF, ledGroup.port);
}

void test_TurnOnLed() {
  LedGroup_led_t led = 11;
  LedGroup_turnOn(&ledGroup, led);
  TEST_ASSERT_EQUAL_HEX16(1 << (led - 1), ledGroup.port);
}

void test_TurnOffLed() {
  LedGroup_led_t led = 3;
  LedGroup_turnOn(&ledGroup, led);
  LedGroup_turnOff(&ledGroup, led);
  TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF, ledGroup.port);
}

void test_TurnOnAllLeds() {
  LedGroup_setAllOn(&ledGroup);
  TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_ON, ledGroup.port);
}

void test_TurnOffAllLeds() {
  LedGroup_setAllOn(&ledGroup);
  LedGroup_setAllOff(&ledGroup);
  TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF, ledGroup.port);
}

void test_FullLedSequence() {
  uint16_t expected = ALL_LEDS_OFF;
  for (uint8_t i = MIN_LED_IND; i <= MAX_LED_IND; i++) {
    LedGroup_turnOn(&ledGroup, i);
    expected |= 1 << (i - 1);
    TEST_ASSERT_EQUAL_HEX16(expected, ledGroup.port);
  }
  for (uint8_t i = MAX_LED_IND; i >= MIN_LED_IND; i--) {
    LedGroup_turnOff(&ledGroup, i);
    expected &= ~(1 << (i - 1));
    TEST_ASSERT_EQUAL_HEX16(expected, ledGroup.port);
  }
}

void test_TurnLedsWithoutOverlap() {
  LedGroup_led_t led1 = 11, led2 = 13, led3 = 5;
  LedGroup_turnOn(&ledGroup, led1);
  LedGroup_turnOn(&ledGroup, led2);
  LedGroup_turnOn(&ledGroup, led3);
  LedGroup_turnOff(&ledGroup, led2);
  TEST_ASSERT_EQUAL_HEX16(0b0000010000010000, ledGroup.port);
}

void test_InvalidLedIndexes() {
  Exceptions_throw_Expect(LED_OUT_OF_BOUNDS, "Attempted to turn on an invalid LED.");
  LedGroup_turnOn(&ledGroup, 17);

  Exceptions_throw_Expect(LED_OUT_OF_BOUNDS, "Attempted to turn off an invalid LED.");
  LedGroup_turnOff(&ledGroup, 0);

  Exceptions_throw_Expect(LED_OUT_OF_BOUNDS, "Attempted to turn on an invalid LED.");
  LedGroup_turnOn(&ledGroup, 51);
}
