#ifndef GPIO_H
#define GPIO_H
/*
 * gpio.h Abstract GPIO Definitions
 */
#include <stdint.h>
#include <stddef.h>

#include "utils/errors.h"
#include "utils/common.h"

// Forward declaration of struct to maintain hidden data
typedef struct GPIOStruct *GPIO;

typedef uint8_t GPIOPin;

typedef enum {
    INPUT  = 0,
    OUTPUT = 1,
    ALT    = 2,
    ANALOG = 3
} GPIOMode;

typedef enum {
    PUSH_PULL = 0,
    OPEN_DRAIN = 1  // Only LOW level is driven, when high pin is HI-Z
} GPIOType;

typedef enum {
    LOW_SPEED = 0,
    MED_SPEED = 1,
    HIGH_SPEED = 3
} GPIOSpeed;

typedef enum {
    NO_PULL = 0,
    PULL_UP = 1,
    PULL_DOWN = 2
} GPIOPull;

typedef struct {
    GPIOMode mode;
    GPIOType type;
    GPIOSpeed speed;
    GPIOPull pull;
} GPIOBaseConfig;

typedef enum {
    AF0  = 0,
    AF1  = 1,
    AF2  = 2,
    AF3  = 3,
    AF4  = 4,
    AF5  = 5,
    AF6  = 6,
    AF7  = 7,
    AF8  = 8,
    AF9  = 9,
    AF10 = 10,
    AF11 = 11,
    AF12 = 12,
    AF13 = 13,
    AF14 = 14,
    AF15 = 15
} AFVal;

typedef struct {
    GPIOBaseConfig base_config;
    AFVal fxn;
} GPIOPinConfig;


/*
 * Return a GPIO structure that the user can pass to other methods. Also,
 * initialize the GPIO port as a pull-down input.
 * gpio_port - number corresponding to a particular gpio_port. eg)
 *             0 - GPIO_A
 *             1 - GPIO_B
 *             2 - GPIO_C
 *             3 - GPIO_D
 *             4 - GPIO_E
 *             5 - GPIO_F
 *
 *             It is the caller's responsibility to use the right port number
 *             for the particular hardware. If an invalid port is used, a NULL
 *             pointer will be returned.
 */
GPIO gpio_create(uint32_t gpio_port);
err_t gpio_destroy(GPIO gpio);

err_t gpio_configure_port(GPIO gpio, GPIOBaseConfig config);
err_t gpio_configure_pin(GPIO gpio, GPIOPinConfig config, GPIOPin pin);

S_DATA gpio_read_port(GPIO gpio);

/*
 * Listing of desired functionality
 *
 * - Configure whole port or pin
 *
 * - Set output status of port or pin
 *   - Set and reset individual bits
 * - Get output status of port or pin
 *
 * - Get input status of port or pin
 *
 * ( STRETCH GOAL )
 * - Lock port
 *
 */

/*

int32_t gpio_configure(GPIO gpio_port, uint32_t configuration);
int32_t gpio_configure_pin(GPIO gpio_port, uint32_t pin, uint32_t configuration);

uint32_t gpio_digital_read(GPIO gpio_port);
uint32_t gpio_digital_read_pin(GPIO gpio_port, uint32_t pin);
*/

#endif  // GPIO_H
