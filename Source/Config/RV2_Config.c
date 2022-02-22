/*
 * Copyright (C) 2022 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cmsis_rv2.h"

#include "RV2_Config_Device.h"

#if defined(__CORTEX_A)
#include "irq_ctrl.h"
#endif

/* Prototypes */
void TST_IRQ_HANDLER_A (void);
void TST_IRQ_HANDLER_B (void);
void (*TST_IRQHandler_A)(void);
void (*TST_IRQHandler_B)(void);


/*
  Primary interrupt handler
*/
void TST_IRQ_HANDLER_A (void) {

  if (TST_IRQHandler_A != NULL) {
    TST_IRQHandler_A();
  }
}

/*
  Secondary interrupt handler
*/
void TST_IRQ_HANDLER_B (void) {

  if (TST_IRQHandler_B != NULL) {
    TST_IRQHandler_B();
  }
}

/*
  Test suite initialization
*/
void TS_Init (void) {
#if defined(__ARM_ARCH_7A__) && defined(__CORTEX_A)
  IRQn_ID_t irq = (IRQn_ID_t)TST_IRQ_NUM_A;

  // Disable corresponding IRQ first
  IRQ_Disable(irq);
  IRQ_ClearPending(irq);

  // Set edge-triggered IRQ
  IRQ_SetMode (irq, IRQ_MODE_TRIG_EDGE);

  // Register interrupt handler
  IRQ_SetHandler(irq, TST_IRQ_HANDLER_A);
#endif

  EnableIRQ();
}

/*
  Test suite de-initialization
*/
void TS_Uninit (void) {
  /* Close debug session here */
  __BKPT();
}

/*
  Enable interrupt trigger in the IRQ controller.
*/
void EnableIRQ(void) {
#if defined(__CORTEX_A)
  IRQ_Enable((IRQn_ID_t)TST_IRQ_NUM_A);
#elif defined(__CORTEX_M)
  NVIC_EnableIRQ((IRQn_Type)TST_IRQ_NUM_A);
#endif
}

/*
  Disable interrupt trigger in the IRQ controller.
*/
void DisableIRQ(void) {
#if defined(__CORTEX_A)
  IRQ_Disable((IRQn_ID_t)TST_IRQ_NUM_A);
#elif defined(__CORTEX_M)
  NVIC_DisableIRQ((IRQn_Type)TST_IRQ_NUM_A);
#endif
}

/*
  Set pending interrupt in the IRQ controller.
*/
void SetPendingIRQ(void) {
#if defined(__CORTEX_A)
  __disable_irq();
  IRQ_SetPending((IRQn_ID_t)TST_IRQ_NUM_A);
  __enable_irq();
#elif defined(__CORTEX_M)
  NVIC_SetPendingIRQ((IRQn_Type)TST_IRQ_NUM_A);
#endif

  /* Data/Instruction Synchronization and Memory Barrier */
  __DSB();
  __ISB();
  __DMB();

#if defined(__CORTEX_A)
  while (IRQ_GetPending((IRQn_ID_t)TST_IRQ_NUM_A) != 0);
#elif defined(__CORTEX_M)
  while (NVIC_GetPendingIRQ((IRQn_Type)TST_IRQ_NUM_A) != 0);
#endif
}
