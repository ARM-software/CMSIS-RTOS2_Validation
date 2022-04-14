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

extern int stdio_init (void);

int main (void) {
  /* Initialize stdio */
  stdio_init();

  /* CMSIS-RTOS2 validation entry */
  cmsis_rv2();
}

#if defined(__GNUC__) && !defined(__CC_ARM) && !defined(__ARMCC_VERSION)
void _start (void) {
  main();

  for(;;);
}
#endif
