/* ****************************************************************************************************************** */
/*                                                     Inclusions                                                     */
/* ****************************************************************************************************************** */
#include "leds.h"

#include "exceptions.h"

/* ****************************************************************************************************************** */
/*                                                  Private Constants                                                 */
/* ****************************************************************************************************************** */
#define MASK_BIT 0b1
#define LED_INDEX_OFFSET 1

/* ****************************************************************************************************************** */
/*                                                  Private Functions                                                 */
/* ****************************************************************************************************************** */
static uint16_t LedGroup_ledToPortFlag(LedGroup_led_t led) { return MASK_BIT << (led - LED_INDEX_OFFSET); }

static bool LedGroup_isLedValid(LedGroup_led_t led) { return led >= MIN_LED_IND && led <= MAX_LED_IND; }

/* ****************************************************************************************************************** */
/*                                                  Public Functions                                                  */
/* ****************************************************************************************************************** */

void LedGroup_init(LedGroup_obj_t* ledGroup, uint64_t portAddress) {
  ledGroup->port = (uint16_t*)portAddress;
  *ledGroup->port = DEFAULT_PORT_VALUE;
}

void LedGroup_turnOn(LedGroup_obj_t* ledGroup, LedGroup_led_t led) {
  if (LedGroup_isLedValid(led)) {
    *ledGroup->port |= LedGroup_ledToPortFlag(led);
  } else {
    Exceptions_throw(LED_OUT_OF_BOUNDS, "Attempted to turn on an invalid LED.");
  }
}

void LedGroup_turnOff(LedGroup_obj_t* ledGroup, LedGroup_led_t led) {
  if (LedGroup_isLedValid(led)) {
    *ledGroup->port &= ~LedGroup_ledToPortFlag(led);
  } else {
    Exceptions_throw(LED_OUT_OF_BOUNDS, "Attempted to turn off an invalid LED.");
  }
}

bool LegGroup_getLedState(LedGroup_obj_t* ledGroup, LedGroup_led_t led) {
  return (*ledGroup->port >> (led - LED_INDEX_OFFSET)) & MASK_BIT;
}

void LedGroup_setAllOn(LedGroup_obj_t* ledGroup) { *ledGroup->port = ALL_LEDS_ON; }

void LedGroup_setAllOff(LedGroup_obj_t* ledGroup) { *ledGroup->port = ALL_LEDS_OFF; }
