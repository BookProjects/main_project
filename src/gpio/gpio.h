#ifndef GPIO_H
#define GPIO_H
/*
 * gpio.h Abstract GPIO Definitions
 */
#include <stdint.h>
#include <stddef.h>

// Forward declaration of struct to maintain hidden data
typedef struct GPIOStruct *GPIO;

/*
 * gpio_port - number corresponding to a particular gpio_port. eg)
 *             0 - GPIO_A
 *             1 - GPIO_B
 *             2 - GPIO_C
 *             3 - GPIO_D
 *             4 - GPIO_E
 *             5 - GPIO_F
 *
 * It is the caller's responsibility to use the right port number for the
 * particular hardware. If an invalid port is used, a NULL pointer will be
 * returned.
 */
GPIO gpio_create(uint32_t gpio_port);
/*
int32_t gpio_destroy(GPIO gpio_port);

int32_t gpio_configure(GPIO gpio_port, uint32_t configuration);
int32_t gpio_configure_pin(GPIO gpio_port, uint32_t pin, uint32_t configuration);

uint32_t gpio_digital_read(GPIO gpio_port);
uint32_t gpio_digital_read_pin(GPIO gpio_port, uint32_t pin);
*/

#endif  // GPIO_H
