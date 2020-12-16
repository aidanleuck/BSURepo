#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOC_BASE 0x40020800
#define GPIOD_BASE 0x40020C00
#define GPIOE_BASE 0x40021000

struct GPIO_REGS
{
    int GPIO_MODER;
    int GPIO_OTYPER;
    int GPIO_OSPEEDR;
    int GPIO_PUPDR;
    int GPIO_IDR;
    int GPIO_ODR;
    int GPIO_BSRR;
    int GPIO_LCKR;
    int GPIO_AFRL;
    int GPIO_AFRH;
};