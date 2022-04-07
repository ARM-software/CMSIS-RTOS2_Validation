/******************************************************************************
 * @file     startup_CMSDK_CM0plus.c
 * @brief    CMSIS Startup File for CMSDK_M0plus Device
 ******************************************************************************/
/* Copyright (c) 2022 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#if defined (CMSDK_CM0plus) || defined (CMSDK_CM0plus_VHT)
  #include "CMSDK_CM0plus.h"
#else
  #error device not specified!
#endif

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
            void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler      (void) __attribute__ ((weak));
void SVC_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));

/* Interrupts */
void UART0RX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART0TX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART1RX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART1TX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART2RX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART2TX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO0ALL_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO1ALL_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER0_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER1_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void DUALTIMER_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI_0_1_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART_0_1_2_OVF_Handler (void) __attribute__ ((weak, alias("Default_Handler")));
void ETHERNET_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void I2S_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void TOUCHSCREEN_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO2_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO3_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void UART3RX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART3TX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART4RX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UART4TX_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI_2_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI_3_4_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
#if defined CMSDK_CM0plus_VHT   /* VSI Interrupts */
void ARM_VSI0_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void ARM_VSI1_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void ARM_VSI2_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void ARM_VSI3_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void ARM_VSI4_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void ARM_VSI5_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void ARM_VSI6_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void ARM_VSI7_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
#else
void GPIO0_0_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO0_1_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO0_2_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO0_3_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO0_4_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO0_5_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO0_6_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO0_7_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
#endif


/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[48];
       const VECTOR_TABLE_Type __VECTOR_TABLE[48] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVC Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  UART0RX_Handler,                          /*   0 UART 0 receive interrupt */
  UART0TX_Handler,                          /*   1 UART 0 transmit interrupt */
  UART1RX_Handler,                          /*   2 UART 1 receive interrupt */
  UART1TX_Handler,                          /*   3 UART 1 transmit interrupt */
  UART2RX_Handler,                          /*   4 UART 2 receive interrupt */
  UART2TX_Handler,                          /*   5 UART 2 transmit interrupt */
  GPIO0ALL_Handler,                         /*   6 GPIO 0 combined interrupt */
  GPIO1ALL_Handler,                         /*   7 GPIO 1 combined interrupt */
  TIMER0_Handler,                           /*   8 Timer 0 interrupt */
  TIMER1_Handler,                           /*   9 Timer 1 interrupt */
  DUALTIMER_Handler,                        /*  10 Dual Timer interrupt */
  SPI_0_1_Handler,                          /*  11 SPI 0, SPI 1 interrupt */
  UART_0_1_2_OVF_Handler,                   /*  12 UART overflow (0, 1 & 2) interrupt */
  ETHERNET_Handler,                         /*  13 Ethernet interrupt */
  I2S_Handler,                              /*  14 Audio I2S interrupt */
  TOUCHSCREEN_Handler,                      /*  15 Touch Screen interrupt */
  GPIO2_Handler,                            /*  16 GPIO 2 combined interrupt */
  GPIO3_Handler,                            /*  17 GPIO 3 combined interrupt */
  UART3RX_Handler,                          /*  18 UART 3 receive interrupt */
  UART3TX_Handler,                          /*  19 UART 3 transmit interrupt */
  UART4RX_Handler,                          /*  20 UART 4 receive interrupt */
  UART4TX_Handler,                          /*  21 UART 4 transmit interrupt */
  SPI_2_Handler,                            /*  22 SPI 2 interrupt */
  SPI_3_4_Handler,                          /*  23 SPI 3, SPI 4 interrupt */
#if defined CMSDK_CM0plus_VHT
  ARM_VSI0_Handler,                         /*  24 VSI 0 interrupt */
  ARM_VSI1_Handler,                         /*  25 VSI 1 interrupt */
  ARM_VSI2_Handler,                         /*  26 VSI 2 interrupt */
  ARM_VSI3_Handler,                         /*  27 VSI 3 interrupt */
  ARM_VSI4_Handler,                         /*  28 VSI 4 interrupt */
  ARM_VSI5_Handler,                         /*  29 VSI 5 interrupt */
  ARM_VSI6_Handler,                         /*  30 VSI 6 interrupt */
  ARM_VSI7_Handler                          /*  31 VSI 7 interrupt */
#else
  GPIO0_0_Handler,                          /*  24 GPIO 0 individual interrupt ( 0) */
  GPIO0_1_Handler,                          /*  25 GPIO 0 individual interrupt ( 1) */
  GPIO0_2_Handler,                          /*  26 GPIO 0 individual interrupt ( 2) */
  GPIO0_3_Handler,                          /*  27 GPIO 0 individual interrupt ( 3) */
  GPIO0_4_Handler,                          /*  28 GPIO 0 individual interrupt ( 4) */
  GPIO0_5_Handler,                          /*  29 GPIO 0 individual interrupt ( 5) */
  GPIO0_6_Handler,                          /*  30 GPIO 0 individual interrupt ( 6) */
  GPIO0_7_Handler                           /*  31 GPIO 0 individual interrupt ( 7) */
#endif
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
  Hard Fault Handler
 *----------------------------------------------------------------------------*/
void HardFault_Handler(void)
{
  while(1);
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
  while(1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif
