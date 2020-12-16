#include <stdio.h>
#include "gpio_regs.h"
#include "seg7.h"
#include <stdlib.h>
#include <stdint.h>

// Function Prototypes

void gpio_d_init(void);
void gpio_d_put(int);
int gpio_d_get_current(void);
void gpio_c_init(void);
int gpio_c_get(void);
void SW_Init(void);
void LEDR_Init(void);
void LEDR_Put(int, int);
void seg7_putControl(int, int);
int SW_Get_Manual(void);
int SW_Get_Control(void);
void SysTick_Handler(void);

struct GPIO_REGS *regSetD = (struct GPIO_REGS *)GPIOD_BASE;               // Use struct to establish addressability to port D as well as access register set.
struct GPIO_REGS *regSetC = (struct GPIO_REGS *)GPIOC_BASE;               // Establish addressability to port C and appropriate register set.
char seg7[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x3, 0x78, 0x0, 0x18}; // Hex values for 0-9

const double reload_value = ((double).0002 / 1000) / ((double)1 / 168000000);

// Values for various segment characters
const int L = 0x07;   // Value for seg7 L
const int R = 0x2F;   // value for seg7 R
const int U = 0x01;   // value for seg7 U
const int D = 0x21;   // value for seg7 d
const int b = 0x3;    // value for seg7 b
const int A = 0x8;    // value for seg7 A
const int OFF = 0xFF; // OFF

int ISR_COUNTER = 0; // Counter for ISR
int X_AXIS = 0;      // X_AXIS current adc value
int Y_AXIS = 0;      // Y_AXIS current adc value

int *DAC_RHHR1_VAL = 0x40007408; // Address of DAC_RHHR1_VAL register

int main()
{
    SW_Init();
    LEDR_Init();
    seg7_init();
    SetSysClock();
    Systick_Config(reload_value);
    while (1)
    {
        uint32_t q = 0;
        adc_start();

        int controlValue = SW_Get_Control();      // Gets control value from switch
        int modeValue = SW_Get_Manual();          // Gets mode value from switch
        LEDR_Put(modeValue, controlValue);        // Lights up appropriate LED's
        seg7_putControl(modeValue, controlValue); // Lights up seg7 display with mode and control values

        while (!adc_done())
        {
            q = q + 1;
        }
        uint32_t val = adc_get(); // In manual mode a resistor would change this value

        moveServo(val, modeValue, controlValue); // Moves servo
        seg7_putDegrees();                       // Displays degrees
    }
    return 0;
}

void SysTick_Handler(void)
{
    ISR_COUNTER = (ISR_COUNTER + 1) % 1000;
}

/**
 * Simply a helper function that calls gpio_c_init (gets the switches ready for input)
 */
void SW_Init()
{
    gpio_c_init();
}
int SW_Get_Manual()
{
    return ((gpio_c_get() & 0x1));
}
/**
 * Gets the sate of switch 1 (normalized)
 * IF Switch 1 is
 * OFF = LEFT/RIGHT
 * ON = UP/DOWN 
 */
int SW_Get_Control()
{
    return ((gpio_c_get() << 1) & 0x1);
}
/**
 * Abstraction that just calls our gpio_d_init
 */
void LEDR_Init()
{
    gpio_d_init();
}

/**
 * Sets LED's to on/off depending on switch state 
 */
void LEDR_Put(int mode, int control)
{
    int putVal = mode | (control << 1);
    int ODR = gpio_d_get_current(); // Gets the current value of the ODR
    int mask = (~0 << 2);           // Inverse mask (fffffffc)

    ODR = ODR & mask; // Clear bit 0 and 1 from ODR

    ODR = ODR | putVal; // Or in new value

    gpio_d_put(ODR); // Sets new value in register
}

/**
 * Initializes gpio port c as IDR 
 */
void gpio_c_init()
{
    rcc_gpio_enable(0x02); //Function provided in lab that enables the gpio_port
    int loadValue = 0x0;
    regSetC->GPIO_PUPDR = loadValue;  // SET GPIO_PUPDR to 0
    regSetC->GPIO_MODER = loadValue;  // Set GPIO_MODER to Input
    regSetC->GPIO_OTYPER = loadValue; // SET GPIO_OTYPER to 0

    loadValue = 0xffffffff;
    regSetC->GPIO_OSPEEDR = loadValue; // SET GPIO_OSPEEDR (output speed)
}
/**
 * Get value of the GPIO_IDR
 */
int gpio_c_get()
{
    return regSetC->GPIO_IDR;
}

/**
 * Initializes gpio_port d's registers (Our output port)
 */
void gpio_d_init()
{

    rcc_gpio_enable(0x01); //This was a function provided in lab that enables the gpio port

    int loadValue = 0x0; // Load value for three registers below

    regSetD->GPIO_PUPDR = loadValue;  // Set GPIO_PUPDR to initial value of 0
    regSetD->GPIO_OTYPER = loadValue; // Set GPIO_OTYPER to initial value of 0
    regSetD->GPIO_ODR = loadValue;    // Set GPIO_ODR to initial value of 0

    loadValue = 0x55555555;          // Load value for GPIO_MODER
    regSetD->GPIO_MODER = loadValue; // SET GPIO_MODER to 0x555.... (set to output mode)

    loadValue = 0xffffffff;            // Load value for GPIO_OSPEEDR
    regSetD->GPIO_OSPEEDR = loadValue; // Set output speed for the register
}
/**
 *  Sets value of the ODR to the value passed in to the function
 */
void gpio_d_put(int value)
{
    regSetD->GPIO_ODR = value;
}
/**
 * Returns the gpio_d ODR value
 */
int gpio_d_get_current()
{
    return regSetD->GPIO_ODR;
}
/**
 * Displays appropriate control and mode information on seg7 display
 * 
 * PARAMS
 * mode:
 * 0 = AUTOMATIC
 * 1 = Manual
 * 
 * control
 * 0 = Left/Right
 * 1 = Up/Down
 */
void seg7_putControl(int mode, int control)
{
    // If we are in automatic mode
    if (!mode)
    {
        seg7_put(7, A);   // Seg 7 = A
        seg7_put(5, b);   // Seg 5 = b for both
        seg7_put(4, OFF); // Turn 4 (in case it was on from switching to manual mode)
    }
    // if we are in manual mode
    else
    {
        seg7_put(7, OFF); // Turn off the A for automatic mode

        // If control is 1 then we are manipulating x y axis
        if (control)
        {
            seg7_put(5, U); // set 5 as U for up
            seg7_put(4, D); // set 4 as d for down
        }
        // If control is 0 we are manipulating L, R
        if (!control)
        {
            seg7_put(5, L); // set 5 as L
            seg7_put(4, R); // set 4 as R
        }
    }
}
void seg7_putDegrees()
{
    int degreeX = X_AXIS / ISR_COUNTER; // Calculates degrees
    int degreeY = Y_AXIS / ISR_COUNTER;

    int xTen = (degreeX / 10) % 10; // Get ten place of x degree
    int xOne = degreeX % 10;        // get one place of x degree

    int yTen = (degreeY / 10) % 10; // get ten place of y degree
    int yOne = degreeY % 10;        // get one place of y degree

    seg7_put(3, seg7[xTen]); // display degree value for x's degree ten place on segment
    seg7_put(2, seg7[xOne]); // display degree value for x's degree one's place on segment

    seg7_put(1, seg7[yTen]); // display degree value for y's degree ten place on segment
    seg7_put(0, seg7[yOne]); // display degree value for y's degree one place on segment
}
void moveServo(int adc, int mode, int control)
{
    if (!mode)
    {
        X_AXIS = adc; // Set x_axis to current adc
        Y_AXIS = adc; // set y_axis to current adc
    }
    else
    {
        // Only update x and y axis values depending on which switch is on.
        if (control)
        {
            Y_AXIS = adc;
        }
        else
        {
            X_AXIS = adc;
        }
    }
}
