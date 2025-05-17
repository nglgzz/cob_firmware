#ifndef GPIO_H

#include <stdbool.h>
#include <stdint.h>

// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/gpio.html
#define GPIO0_BASE 0x50000000U
#define GPIO1_BASE 0x50000300U

struct gpio {
  //  (0x504) GPIO output register (1 sets pin to high, 0 to low)
  volatile uint32_t OUT;
  //  (0x508) GPIO output set register: set individual bits in GPIO port (1 sets
  //  pin to high, 0 has no effect)
  volatile uint32_t OUTSET;
  //  (0x50C) GPIO output clear register: clear individual bits in GPIO port (1
  //  sets pin to low, 0 has no effect)
  volatile uint32_t OUTCLR;
  // (0x510) Read GPIO port
  volatile uint32_t IN;
  // (0x514) Direction of GPIO pins
  volatile uint32_t DIR;
  // (0x518) DIR set register
  volatile uint32_t DIRSET;
  // (0x51C) DIR clear register
  volatile uint32_t DIRCLR;
  // (0x520) Latch register indicating what GPIO pins that have met the criteria
  // set in the PIN_CNF[n].SENSE registers. When reading 1 = criteria has been
  // met, 0 = criteria has not been met. When writing 1 = clear latch
  volatile uint32_t LATCH;
  // (0x524) Select between the default DETECT signal behavior and LDETECT mode
  volatile uint32_t DETECTMODE;
  volatile uint32_t __RESERVED[118];
  // (0x700) Configuration of GPIO pins
  volatile uint32_t PIN_CNF[32];
};

extern struct gpio *GPIO0;
extern struct gpio *GPIO1;

/* Register: GPIO_OUT */
/* Description: Write GPIO port */

#define GPIO_OUT_Low (0x0UL)  /*!< Pin driver is low */
#define GPIO_OUT_High (0x1UL) /*!< Pin driver is high */

/* Register: GPIO_OUTSET */
/* Description: Set individual bits in GPIO port */

#define GPIO_OUTSET_Low (0x0UL)  /*!< Read: pin driver is low */
#define GPIO_OUTSET_High (0x1UL) /*!< Read: pin driver is high */
#define GPIO_OUTSET_Set (0x1UL)  /*!< Write: a '1' sets the pin high; a '0' has no effect */

/* Register: GPIO_OUTCLR */
/* Description: Clear individual bits in GPIO port */

#define GPIO_OUTCLR_Low (0x0UL)   /*!< Read: pin driver is low */
#define GPIO_OUTCLR_High (0x1UL)  /*!< Read: pin driver is high */
#define GPIO_OUTCLR_Clear (0x1UL) /*!< Write: a '1' sets the pin low; a '0' has no effect */

/* Register: GPIO_IN */
/* Description: Read GPIO port */

#define GPIO_IN_Low (0x0UL)  /*!< Pin input is low */
#define GPIO_IN_High (0x1UL) /*!< Pin input is high */

/* Register: GPIO_DIR */
/* Description: Direction of GPIO pins */

#define GPIO_DIR_Input (0x0UL)  /*!< Pin set as input */
#define GPIO_DIR_Output (0x1UL) /*!< Pin set as output */

/* Register: GPIO_DIRSET */
/* Description: Set individual bits in GPIO port */

#define GPIO_DIRSET_Low (0x0UL)  /*!< Read: pin set as input */
#define GPIO_DIRSET_High (0x1UL) /*!< Read: pin set as output */
#define GPIO_DIRSET_Set (0x1UL)  /*!< Write: a '1' sets the pin output; a '0' has no effect */

/* Register: GPIO_DIRCLR */
/* Description: Clear individual bits in GPIO port */

#define GPIO_DIRCLR_Low (0x0UL)   /*!< Read: pin set as input */
#define GPIO_DIRCLR_High (0x1UL)  /*!< Read: pin set as output */
#define GPIO_DIRCLR_Clear (0x1UL) /*!< Write: a '1' sets the pin input; a '0' has no effect \
                                   */

/* Register: GPIO_PIN_CNF */
/* Description: Description collection: Configuration of GPIO pins */

/* Bits 17..16 : Pin sensing mechanism */
#define GPIO_PIN_CNF_SENSE_Pos (16UL) /*!< Position of SENSE field. */
#define GPIO_PIN_CNF_SENSE_Msk \
  (0x3UL << GPIO_PIN_CNF_SENSE_Pos)         /*!< Bit mask of SENSE field. */
#define GPIO_PIN_CNF_SENSE_Disabled (0x0UL) /*!< Disabled */
#define GPIO_PIN_CNF_SENSE_High (0x2UL)     /*!< Sense for high level */
#define GPIO_PIN_CNF_SENSE_Low (0x3UL)      /*!< Sense for low level */

/* Bits 10..8 : Drive configuration */
#define GPIO_PIN_CNF_DRIVE_Pos (8UL) /*!< Position of DRIVE field. */
#define GPIO_PIN_CNF_DRIVE_Msk \
  (0x7UL << GPIO_PIN_CNF_DRIVE_Pos)     /*!< Bit mask of DRIVE field. */
#define GPIO_PIN_CNF_DRIVE_S0S1 (0x0UL) /*!< Standard '0', standard '1' */
#define GPIO_PIN_CNF_DRIVE_H0S1 (0x1UL) /*!< High drive '0', standard '1' */
#define GPIO_PIN_CNF_DRIVE_S0H1 (0x2UL) /*!< Standard '0', high drive '1' */
#define GPIO_PIN_CNF_DRIVE_H0H1 (0x3UL) /*!< High drive '0', high 'drive '1'' */
#define GPIO_PIN_CNF_DRIVE_D0S1 \
  (0x4UL) /*!< Disconnect '0' standard '1' (normally used for wired-or connections) */
#define GPIO_PIN_CNF_DRIVE_D0H1 \
  (0x5UL) /*!< Disconnect '0', high drive '1' (normally used for wired-or connections) */
#define GPIO_PIN_CNF_DRIVE_S0D1 \
  (0x6UL) /*!< Standard '0'. disconnect '1' (normally used for wired-and connections) */
#define GPIO_PIN_CNF_DRIVE_H0D1 \
  (0x7UL) /*!< High drive '0', disconnect '1' (normally used for wired-and connections) */

/* Bits 3..2 : Pull configuration */
#define GPIO_PIN_CNF_PULL_Pos (2UL) /*!< Position of PULL field. */
#define GPIO_PIN_CNF_PULL_Msk                                           \
  (0x3UL << GPIO_PIN_CNF_PULL_Pos)         /*!< Bit mask of PULL field. \
                                            */
#define GPIO_PIN_CNF_PULL_Disabled (0x0UL) /*!< No pull */
#define GPIO_PIN_CNF_PULL_Pulldown (0x1UL) /*!< Pull down on pin */
#define GPIO_PIN_CNF_PULL_Pullup (0x3UL)   /*!< Pull up on pin */

/* Bit 1 : Connect or disconnect input buffer */
#define GPIO_PIN_CNF_INPUT_Pos (1UL) /*!< Position of INPUT field. */
#define GPIO_PIN_CNF_INPUT_Msk \
  (0x1UL << GPIO_PIN_CNF_INPUT_Pos)           /*!< Bit mask of INPUT field. */
#define GPIO_PIN_CNF_INPUT_Connect (0x0UL)    /*!< Connect input buffer */
#define GPIO_PIN_CNF_INPUT_Disconnect (0x1UL) /*!< Disconnect input buffer */

/* Bit 0 : Pin direction. Same physical register as DIR register */
#define GPIO_PIN_CNF_DIR_Pos (0UL)                           /*!< Position of DIR field. */
#define GPIO_PIN_CNF_DIR_Msk (0x1UL << GPIO_PIN_CNF_DIR_Pos) /*!< Bit mask of DIR field. */
#define GPIO_PIN_CNF_DIR_Input (0x0UL)  /*!< Configure pin as an input pin */
#define GPIO_PIN_CNF_DIR_Output (0x1UL) /*!< Configure pin as an output pin */

// ---------------------
// GPIO_CNF[n]
// ---------------------

struct gpio_pin_cnf {
  uint8_t DIR;    // Pin direction. Same physical register as DIR register
  uint8_t INPUT;  // Connect or disconnect input buffer
  uint8_t PULL;   // Pull configuration
  uint8_t SENSE;  // Pin sensing mechanism
};

void gpio_dir_pin_output(struct gpio *port, uint8_t pin);
void gpio_out_pin(struct gpio *port, uint8_t pin, bool value);

#define GPIO_H
#endif  // GPIO_H
