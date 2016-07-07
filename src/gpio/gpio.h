#ifndef GPIO_H
#define GPIO_H
/*
 * gpio.h Abstract GPIO Definitions
 */
#include <stdint.h>
#include <stddef.h>

#include "utils/errors.h"

// Forward declaration of struct to maintain hidden data
typedef struct GPIOStruct *GPIO;

typedef uint8_t GPIOPin;

typedef enum {
    INPUT  = 0,
    OUTPUT = 1,
    ALT    = 2,
    ANALOG = 3
} GPIOType;

typedef enum {
    PUSH_PULL = 0,
    OPEN_DRAIN = 1
} GPIOOutputType;

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
    GPIOType type;
    GPIOOutputType output_type;
    GPIOSpeed speed;
    GPIOPull pull;
} GPIOConfig;

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
err_t gpio_port_configure(GPIO gpio, GPIOConfig config);
err_t gpio_pin_configure(GPIO gpio, GPIOConfig config, GPIOPin pin);
err_t gpio_destroy(GPIO gpio);

/*

int32_t gpio_configure(GPIO gpio_port, uint32_t configuration);
int32_t gpio_configure_pin(GPIO gpio_port, uint32_t pin, uint32_t configuration);

uint32_t gpio_digital_read(GPIO gpio_port);
uint32_t gpio_digital_read_pin(GPIO gpio_port, uint32_t pin);
*/

#endif  // GPIO_H
