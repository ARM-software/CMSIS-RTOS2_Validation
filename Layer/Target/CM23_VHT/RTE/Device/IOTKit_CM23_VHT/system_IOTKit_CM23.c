/******************************************************************************
 * @file     system_IOTKit_CM23.c
 * @brief    CMSIS Device System Source File for IOTKit_CM23 Device
 ******************************************************************************/
/* Copyright (c) 2015 - 2022 ARM LIMITED

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

#if defined (IOTKit_CM23) || defined (IOTKit_CM23_VHT)
  #include "IOTKit_CM23.h"
#else
  #error device not specified!
#endif

#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
  #include "partition_IOTKit_CM23.h"
#endif

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define  XTAL            (50000000UL)     /* Oscillator frequency */

#define  SYSTEM_CLOCK    (XTAL / 2U)

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
extern const VECTOR_TABLE_Type __VECTOR_TABLE[256];

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;  /* System Core Clock Frequency */

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
  SystemCoreClock = SYSTEM_CLOCK;
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
//  uint32_t blk_cfg, blk_max, blk_size, blk_cnt;
#endif

#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
  SCB->VTOR = (uint32_t) &(__VECTOR_TABLE[0]);
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
  SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* start IOT Green configuration ------------------------- */

  /* configure MPC --------------- */

  /* configure unsecure code area MPSSSRAM1 (0x00200000 - 0x003FFFFF) */
//  blk_max = IOTKIT_MPCSSRAM1->BLK_MAX;   /* = 0x1     */
//  blk_cfg = IOTKIT_MPCSSRAM1->BLK_CFG;   /* = 0xC     */
//  blk_size = 1UL << (blk_cfg + 5U);      /* = 0x20000 */
//  blk_cnt  = 0x200000U / blk_size;       /* = 0x10    */

  IOTKIT_MPCSSRAM1->CTRL    &= ~(1UL << 8U);            /* clear auto increment */
  IOTKIT_MPCSSRAM1->BLK_IDX  = 0;                       /* write LUT index */
  IOTKIT_MPCSSRAM1->BLK_LUT  = 0xFFFF0000UL;            /* configure blocks */


  /* configure unsecure data area MPSSSRAM3 (0x28200000 - 0x283FFFFF) */
//  blk_max = IOTKIT_MPCSSRAM3->BLK_MAX;   /* = 0x1     */
//  blk_cfg = IOTKIT_MPCSSRAM3->BLK_CFG;   /* = 0xB     */
//  blk_size = 1UL << (blk_cfg + 5U);      /* = 0x10000 */
//  blk_cnt  = 0x200000U / blk_size;       /* = 0x20    */

  IOTKIT_MPCSSRAM3->CTRL &= ~(1UL << 8U);              /* clear auto increment */
  IOTKIT_MPCSSRAM3->BLK_IDX = 1;                       /* write LUT index */
  IOTKIT_MPCSSRAM3->BLK_LUT = 0xFFFFFFFFUL;            /* configure blocks */



  /* enable the Non Secure Callable Configuration for IDAU (NSCCFG register) */
  IOTKIT_SPC->NSCCFG |= 1U;


  /* configure PPC --------------- */
#if !defined (__USE_SECURE)
  /* Allow Non-secure access for SCC/FPGAIO registers */
  IOTKIT_SPC->APBNSPPCEXP[2U] |= ((1UL << 0U) |
                                  (1UL << 2U)  );
  /* Allow Non-secure access for SPI1/UART0 registers */
  IOTKIT_SPC->APBNSPPCEXP[1U] |= ((1UL << 1U) |
                                  (1UL << 5U)  );
#endif

/* end IOT Green configuration --------------------------- */

  TZ_SAU_Setup();
#endif

  SystemCoreClock = SYSTEM_CLOCK;
}
