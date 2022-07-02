#include <stdint.h>
#include <stdio.h>

#include "stm32f446/stm32f446re.h"
// #include "stm32f446/flash.h"
// #include "stm32f446/gpio.h"
// #include "stm32f446/pwr.h"
// #include "stm32f446/rcc.h"

#define PLL_M 4
#define PLL_N 180
#define PLL_P 0  // PLLP = 2

uint32_t my_global = 10;
uint32_t my_global2 = 210;
uint32_t my_global3 = 140;
uint32_t my_global4 = 107;
uint32_t my_global5 = 610;

void SysClockConfig(void) {
  /*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
    1. ENABLE HSE and wait for the HSE to become Ready
    2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
    3. Configure the FLASH PREFETCH and the LATENCY Related Settings
    4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
    5. Configure the MAIN PLL
    6. Enable the PLL and wait for it to become ready
    7. Select the Clock Source and wait for it to be set
  ********************************************************/

  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  PWR->CR |= PWR_CR_VOS;

  FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN |
               FLASH_ACR_LATENCY_5WS;
  RCC->CFGR &= ~(1 << 4);

  RCC->CFGR |= (5 << 10);

  RCC->CFGR |= (4 << 13);

  RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16) | (RCC_PLLCFGR_PLLSRC_HSE);
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;

  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while (!(RCC->CFGR & (2 << 2)))
    ;
}

void GPIO_Config(void) {
  /************ STEPS FOLLOWED ***********
  1. Enable the GPIO CLOCK
  2. Set the Pin as OUTPUT/INPUT
  3. Configure the OUTPUT MODE
  ***************************************/
  RCC->AHB1ENR |= (1 << 0);
  GPIOA->MODER &= ~((1 << 2) | (1 << 3));  // pin PA1(bits 3:2) as input (00)
  GPIOA->MODER |= 1 << 8;
}

void delay(uint32_t time) {
  while (time--)
    ;
}

int main(void) {
  SysClockConfig();
  GPIO_Config();

  while (1) {
    if (GPIOA->IDR & (1 << 1)) {
      GPIOA->BSRR |= (1 << 4);
    } else {
      GPIOA->BSRR |= (1 << (4 + 16));
    }
  }
}

// 2. implement the fault handlers
void HardFault_Handler(void) {
  printf("Exception : Hardfault\n");
  while (1)
    ;
}

void MemManage_Handler(void) {
  printf("Exception : MemManage\n");
  while (1)
    ;
}

void BusFault_Handler(void) {
  printf("Exception : BusFault\n");
  while (1) {
  }
  if (1 == 1) {
  } else {
  }
}
