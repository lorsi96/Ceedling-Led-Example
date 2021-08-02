#ifndef __LEDS_H__
#define __LEDS_H__

/* ****************************************************************************************************************** */
/*                                                     Inclusions                                                     */
/* ****************************************************************************************************************** */

#include <stdbool.h>
#include <stdint.h>

/* ****************************************************************************************************************** */
/*                                                  Public Constatns                                                  */
/* ****************************************************************************************************************** */
#define ALL_LEDS_OFF 0x0000
#define ALL_LEDS_ON 0xFFFF
#define DEFAULT_PORT_VALUE ALL_LEDS_OFF
#define N_LEDS 16
#define MIN_LED_IND 1
#define MAX_LED_IND N_LEDS

/* ****************************************************************************************************************** */
/*                                                    Public Types                                                    */
/* ****************************************************************************************************************** */
typedef uint16_t LedGroup_led_t;

typedef struct {
  uint16_t* port;
} LedGroup_obj_t;

/* ****************************************************************************************************************** */
/*                                                  Public Functions                                                  */
/* ****************************************************************************************************************** */
void LedGroup_init(LedGroup_obj_t* ledGroup, uint64_t portAddress);

void LedGroup_turnOn(LedGroup_obj_t* ledGroup, LedGroup_led_t led);

void LedGroup_turnOff(LedGroup_obj_t* ledGroup, LedGroup_led_t led);

void LedGroup_setAllOn(LedGroup_obj_t* ledGroup);

void LedGroup_setAllOff(LedGroup_obj_t* ledGroup);

bool LegGroup_getLedState(LedGroup_obj_t* ledGroup, LedGroup_led_t led);

/* ****************************************************************************************************************** */
/*                                                   End of Module                                                    */
/* ****************************************************************************************************************** */

#endif  // __LEDS_H__
