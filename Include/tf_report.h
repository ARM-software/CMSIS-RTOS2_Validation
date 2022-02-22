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

#ifndef TF_REPORT_H__
#define TF_REPORT_H__

#include <stdbool.h>
#include <stdarg.h>
#include "tf_assert.h"

/*-----------------------------------------------------------------------------
 * Test Report Configuration
 *----------------------------------------------------------------------------*/

/*
  Print Output Format <0=> Plain Text <1=> XML
  Set the test results output format to plain text or XML
*/
#ifndef PRINT_XML_REPORT
#define PRINT_XML_REPORT            0
#endif

/*
  Buffer size for assertions results
  Set the buffer size for assertions results buffer
*/
#ifndef BUFFER_ASSERTIONS
#define BUFFER_ASSERTIONS           128
#endif

/*-----------------------------------------------------------------------------
 * Test report global definitions
 *----------------------------------------------------------------------------*/
#define REP_TC_FAIL 0
#define REP_TC_WARN 1
#define REP_TC_PASS 2
#define REP_TC_NOEX 3

/* Test case result definition */
typedef enum {
  PASSED = 0,
  WARNING,
  FAILED,
  NOT_EXECUTED
} TC_RES;

/* Assertion result info */
typedef struct {
  const char    *module;                  /* Module name                        */
  uint32_t       line;                    /* Assertion line                     */
} AS_INFO;

/* Test case callback interface definition */
typedef struct {
  bool (* Result) (TC_RES res);
  bool (* Dbgi)   (TC_RES res, const char *fn, uint32_t ln, char *desc);
} TC_ITF;

/* Assert interface to the report */
extern TC_ITF tcitf;

/* Assertion result buffer */
typedef struct {
  AS_INFO passed[BUFFER_ASSERTIONS];
  AS_INFO failed[BUFFER_ASSERTIONS];
  AS_INFO warnings[BUFFER_ASSERTIONS];
} AS_T_INFO;

/* Assertion statistics */
typedef struct {
  uint32_t passed;           /* Total assertions passed                  */
  uint32_t failed;           /* Total assertions failed                  */
  uint32_t warnings;         /* Total assertions warnings                */
  AS_T_INFO info;            /* Detailed assertion info                  */
} AS_STAT;

/* Test global statistics */
typedef struct {
  uint32_t  tests;           /* Total test cases count                   */
  uint32_t  executed;        /* Total test cases executed                */
  uint32_t  passed;          /* Total test cases passed                  */
  uint32_t  failed;          /* Total test cases failed                  */
  uint32_t  warnings;        /* Total test cases warnings                */
  AS_STAT   assertions;      /* Total assertions statistics              */
} TEST_REPORT;

/* Test report interface */
typedef struct {
  bool (* Init)     (void);
  bool (* Open)     (const char *title, const char *date, const char *time, const char *fn);
  bool (* Close)    (void);
  bool (* Open_TC)  (uint32_t num, const char *fn);
  bool (* Close_TC) (void);
} REPORT_ITF;

/* Test report statistics */
extern TEST_REPORT  test_report;

/* Test report interface */
extern REPORT_ITF   ritf;

/* Assertions and test results */
extern uint32_t __assert_true (const char *fn, uint32_t ln, uint32_t cond);

#endif /* TF_REPORT_H__ */
