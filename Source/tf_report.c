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

#include <stdio.h>
#include <string.h>

#include "tf_report.h"

/*
  Select test report print output
  0: Standard C library STDOUT
  1: Private printf implementation calling extern function stdout_putchar(ch)
*/
#ifndef TF_OUTPUT
#define TF_OUTPUT           1
#endif

/*
  Select print statement EOL style
  0: End Of Line: \n
  1: End Of Line: \r\n
*/
#ifndef TF_OUTPUT_CRLF
#define TF_OUTPUT_CRLF      1
#endif

TEST_REPORT test_report;
static AS_STAT     current_assertions;   /* Current test case assertions statistics */
#define TAS (&test_report.assertions)    /* Total assertions                        */
#define CAS (&current_assertions)        /* Current assertions                      */

/* Select test report print output */
#if (TF_OUTPUT == 0)
  /* Use standard C library functions */
  #define PRINT(x) print_stdio x
  #define FLUSH()  flush_stdio()
#elif (TF_OUTPUT == 1)
  /* Use static printf implementation and out_putchar() */
  #define PRINT(x) printf_lim x
  #define FLUSH()
#else
  #error "Unknown TF_OUTPUT setting."
#endif

/* Select print statement EOL style */
#if (TF_OUTPUT_CRLF == 0)
  /* End Of Line: \n */
  #define TF_EOL "\n"
#elif (TF_OUTPUT_CRLF == 1)
  /* End Of Line: \r\n */
  #define TF_EOL "\r\n"
#else
  #error "Invalid TF_OUTPUT_CRLF setting."
#endif

static uint8_t Passed[] = "PASSED";
static uint8_t Warning[] = "WARNING";
static uint8_t Failed[] = "FAILED";
static uint8_t NotExe[] = "NOT EXECUTED";

/*-----------------------------------------------------------------------------
 * Strip path info
 *----------------------------------------------------------------------------*/
static const char *fn_strip (const char *fn) {
  const char *p;

  p = fn;

  while (*fn != '\0') {
    if ((*fn == '\\') || (*fn == '/')) {
      p = fn + 1;
    }
    fn++;
  }
  return (p);
}

/*-----------------------------------------------------------------------------
 * Test report function prototypes
 *----------------------------------------------------------------------------*/
static bool     tr_Init   (void);
static bool     tc_Init   (void);
static uint8_t *tr_Eval   (void);
static uint8_t *tc_Eval   (void);
static bool     StatCount (TC_RES res);

/*-----------------------------------------------------------------------------
 * Printer function prototypes
 *----------------------------------------------------------------------------*/
#if (TF_OUTPUT == 0)
static void print_stdio (const char *msg, ...);
static void flush_stdio (void);
#elif (TF_OUTPUT == 1)
extern int stdout_putchar (int ch);
static int out_putchar    (int ch);
static int printf_lim (const char *fmt, ...);
#endif


/*-----------------------------------------------------------------------------
 * Assert interface function prototypes
 *----------------------------------------------------------------------------*/
static bool As_File_Result (TC_RES res);
static bool As_File_Dbgi   (TC_RES res, const char *fn, uint32_t ln, char *desc);

TC_ITF tcitf = {
  As_File_Result,
  As_File_Dbgi,
};


/*-----------------------------------------------------------------------------
 * Test report interface function prototypes
 *----------------------------------------------------------------------------*/
bool tr_File_Init  (void);
bool tr_File_Open  (const char *title, const char *date, const char *time, const char *fn);
bool tr_File_Close (void);
bool tc_File_Open  (uint32_t num, const char *fn);
bool tc_File_Close (void);

REPORT_ITF ritf = {
  tr_File_Init,
  tr_File_Open,
  tr_File_Close,
  tc_File_Open,
  tc_File_Close
};


/*-----------------------------------------------------------------------------
 * Init test report
 *----------------------------------------------------------------------------*/
bool tr_File_Init (void) {
  return (tr_Init());
}


/*-----------------------------------------------------------------------------
 * Open test report
 *----------------------------------------------------------------------------*/
#if (PRINT_XML_REPORT == 1)
bool tr_File_Open (const char *title, const char *date, const char *time, const char *fn) {
  PRINT(("<?xml version=\"1.0\"?>%s", TF_EOL));
  PRINT(("<?xml-stylesheet href=\"TR_Style.xsl\" type=\"text/xsl\" ?>%s", TF_EOL));
  PRINT(("<report>%s", TF_EOL));
  PRINT(("<test>%s", TF_EOL));
  PRINT(("<title>%s</title>%s", title, TF_EOL));
  PRINT(("<date>%s</date>%s",   date,  TF_EOL));
  PRINT(("<time>%s</time>%s",   time,  TF_EOL));
  PRINT(("<file>%s</file>%s",   fn,    TF_EOL));
  PRINT(("<test_cases>%s", TF_EOL));
#else
bool tr_File_Open (const char *title, const char *date, const char *time, const char __attribute__((unused)) *fn) {
  PRINT(("%s   %s   %s %s%s", title, date, time, TF_EOL));
#endif
  FLUSH();

  return (true);
}


/*-----------------------------------------------------------------------------
 * Open test case
 *----------------------------------------------------------------------------*/
bool tc_File_Open (uint32_t num, const char *fn) {
  tc_Init ();
#if (PRINT_XML_REPORT == 1)
  PRINT(("<tc>%s", TF_EOL));
  PRINT(("<no>%d</no>%s",     num, TF_EOL));
  PRINT(("<func>%s</func>%s", fn,  TF_EOL));
  PRINT(("<req></req>"));
  PRINT(("<meth></meth>"));
  PRINT(("<dbgi>%s", TF_EOL));
#else
  PRINT(("TEST %02d: %-32s ", num, fn));
#endif
  FLUSH();

  return (true);
}


/*-----------------------------------------------------------------------------
 * Close test case
 *----------------------------------------------------------------------------*/
bool tc_File_Close (void) {
  uint8_t *res = tc_Eval();
#if (PRINT_XML_REPORT == 1)
  PRINT(("</dbgi>%s", TF_EOL));
  PRINT(("<res>%s</res>%s", res, TF_EOL));
  PRINT(("</tc>%s", TF_EOL));
#else
  if ((res==Passed)||(res==NotExe))
    PRINT(("%s%s", res, TF_EOL));
  else
    PRINT(("%s", TF_EOL));
#endif
  FLUSH();

  return (true);
}


/*-----------------------------------------------------------------------------
 * Close test report
 *----------------------------------------------------------------------------*/
bool tr_File_Close (void) {
#if (PRINT_XML_REPORT == 1)
  PRINT(("</test_cases>%s", TF_EOL));
  PRINT(("<summary>%s", TF_EOL));
  PRINT(("<tcnt>%d</tcnt>%s", test_report.tests,    TF_EOL));
  PRINT(("<exec>%d</exec>%s", test_report.executed, TF_EOL));
  PRINT(("<pass>%d</pass>%s", test_report.passed,   TF_EOL));
  PRINT(("<fail>%d</fail>%s", test_report.failed,   TF_EOL));
  PRINT(("<warn>%d</warn>%s", test_report.warnings, TF_EOL));
  PRINT(("<tres>%s</tres>%s", tr_Eval(),            TF_EOL));
  PRINT(("</summary>%s", TF_EOL));
  PRINT(("</test>%s", TF_EOL));
  PRINT(("</report>%s", TF_EOL));
#else
  PRINT(("\nTest Summary: %d Tests, %d Executed, %d Passed, %d Failed, %d Warnings.%s",
         test_report.tests,
         test_report.executed,
         test_report.passed,
         test_report.failed,
         test_report.warnings,
         TF_EOL));
  PRINT(("Test Result: %s%s", tr_Eval(), TF_EOL));
#endif
  FLUSH();

  return (true);
}


/*-----------------------------------------------------------------------------
 * Assertion result counter
 *----------------------------------------------------------------------------*/
bool As_File_Result (TC_RES res) {
  return (StatCount (res));
}


/*-----------------------------------------------------------------------------
 * Set debug information state
 *----------------------------------------------------------------------------*/
#if (PRINT_XML_REPORT == 1)
bool As_File_Dbgi (TC_RES __attribute__((unused)) res, const char *fn, uint32_t ln, char *desc) {
  PRINT(("<detail>%s", TF_EOL));
  if (desc!=NULL) PRINT(("<desc>%s</desc>%s", desc, TF_EOL));
  PRINT(("<module>%s</module>%s", fn, TF_EOL));
  PRINT(("<line>%d</line>%s", ln, TF_EOL));
  PRINT(("</detail>%s", TF_EOL));
#else
bool As_File_Dbgi (TC_RES res, const char *fn, uint32_t ln, char *desc) {
  PRINT(("\n  %s (%d)", fn, ln));
  if (res==WARNING) PRINT((" [WARNING]"));
  if (res==FAILED) PRINT((" [FAILED]"));
  if (desc!=NULL) PRINT((" %s", desc));
#endif
  FLUSH();

  return (true);
}


/*-----------------------------------------------------------------------------
 * Init test report
 *----------------------------------------------------------------------------*/
bool tr_Init (void) {
  TAS->passed = 0;
  TAS->failed = 0;
  TAS->warnings = 0;
  return (true);
}


/*-----------------------------------------------------------------------------
 * Init test case
 *----------------------------------------------------------------------------*/
bool tc_Init (void) {
  CAS->passed = 0;
  CAS->failed = 0;
  CAS->warnings = 0;
  return (true);
}


/*-----------------------------------------------------------------------------
 * Evaluate test report results
 *----------------------------------------------------------------------------*/
uint8_t *tr_Eval (void) {
  if (test_report.failed > 0) {
    /* Test fails if any test case failed */
    return (Failed);
  }
  else if (test_report.warnings > 0) {
    /* Test warns if any test case warnings */
    return (Warning);
  }
  else if (test_report.passed > 0) {
    /* Test passes if at least one test case passed */
    return (Passed);
  }
  else {
    /* No test cases were executed */
    return (NotExe);
  }
}


/*-----------------------------------------------------------------------------
 * Evaluate test case results
 *----------------------------------------------------------------------------*/
uint8_t *tc_Eval (void) {
  test_report.tests++;
  test_report.executed++;

  if (CAS->failed > 0) {
    /* Test case fails if any failed assertion recorded */
    test_report.failed++;
    return Failed;
  }
  else if (CAS->warnings > 0) {
    /* Test case warns if any warnings assertion recorded */
    test_report.warnings++;
    return Warning;
  }
  else if (CAS->passed > 0) {
    /* Test case passes if at least one assertion passed */
    test_report.passed++;
    return Passed;
  }
  else {
    /* Assert was not invoked - nothing to evaluate */
    test_report.executed--;
    return NotExe;
  }
}


/*-----------------------------------------------------------------------------
 * Statistics result counter
 *----------------------------------------------------------------------------*/
bool StatCount (TC_RES res) {
  switch (res) {
    case PASSED:
      CAS->passed++;
      TAS->passed++;
      break;

    case WARNING:
      CAS->warnings++;
      TAS->warnings++;
      break;

    case FAILED:
      CAS->failed++;
      TAS->failed++;
      break;

    case NOT_EXECUTED:
      return (false);
  }
  return (true);
}


/*-----------------------------------------------------------------------------
 * Set result
 *----------------------------------------------------------------------------*/
static uint32_t __set_result (const char *fn, uint32_t ln, TC_RES res, char* desc) {

  fn = fn_strip (fn);

  // save assertion result
  switch (res) {
    case PASSED:
      if (TAS->passed < BUFFER_ASSERTIONS) {
        test_report.assertions.info.passed[TAS->passed].module = fn;
        test_report.assertions.info.passed[TAS->passed].line = ln;
      }
      break;
    case FAILED:
      if (TAS->failed < BUFFER_ASSERTIONS) {
        test_report.assertions.info.failed[TAS->failed].module = fn;
        test_report.assertions.info.failed[TAS->failed].line = ln;
      }
      break;
    case WARNING:
      if (TAS->warnings < BUFFER_ASSERTIONS) {
        test_report.assertions.info.warnings[TAS->warnings].module = fn;
        test_report.assertions.info.warnings[TAS->warnings].line = ln;
      }
      break;
    case NOT_EXECUTED:
      break;
  }

  // set debug info (if the test case didn't pass)
  if (res != PASSED) tcitf.Dbgi (res, fn, ln, desc);
  // set result
  tcitf.Result (res);
  return (res);
}

/*-----------------------------------------------------------------------------
 * Assert true
 *----------------------------------------------------------------------------*/
uint32_t __assert_true (const char *fn, uint32_t ln, uint32_t cond) {
  TC_RES res;

  if (cond == 0U) {
    /* Condition is not true, test failed */
    res = FAILED;
  } else {
    /* Condition is true, test passed */
    res = PASSED;
  }

  __set_result(fn, ln, res, NULL);

  return (cond);
}


#if (TF_OUTPUT == 0)
/*-----------------------------------------------------------------------------
 *       print_stdio:  Print a message to the standard output
 *----------------------------------------------------------------------------*/
void print_stdio (const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  vprintf(msg, args);
  va_end(args);
}

/*-----------------------------------------------------------------------------
 *       flush_stdio:  Flush the standard output
 *----------------------------------------------------------------------------*/
void flush_stdio (void) {
  fflush(stdout);
}

#elif (TF_OUTPUT == 1)

/**
  Reverse string str
*/
static void rev (char *str) {
  unsigned int i, j;
  char c;

  i = 0;
  j = strlen(str) - 1;

  while (i < j) {
    c = str[i];

    str[i] = str[j];
    str[j] = c;
    
    i++;
    j--;
  }
}

/**
  Convert integer n to string and store it to str buffer
*/
static char *itoa (int n, char *str, int base) {
  int s;
  int i;

  /* Limit to base 10 */
  if (base != 10) {
    return str;
  }

  /* Store sign */
  s = n;

  if (n < 0) {
    /* Convert to positive value */
    n = -n;
  }

  /* Convert to digits in reverse order */
  i = 0;

  do {
    str[i] = (char)(n % 10) + '0';
    i++;

    n = n / 10;
  }
  while (n > 0);
  
  /* Add sign */
  if (s < 0) {
    str[i++] = '-';
  }

  /* Terminate the string */
  str[i] = '\0';

  /* Reverse value before we return */
  rev(str);

  return str;
}

/**
  Convert string to integer, return pointer to first character after number in end
*/
static int atoi_ex (const char *str, const char **end) {
  int s;
  int i;
  int r;

  /* Init sign, result and index */
  s = 1;
  r = 0;
  i = 0;

  if (str[i] == '-') {
    /* Negative sign */
    s = -1;
    i++;
  }
  
  while ((str[i] >= '0') && (str[i] <= '9')) {
    r  = r * 10;
    r += str[i] - '0';

    i++;
  }

  /* Add correct sign */
  r *= s;
  
  *end = &str[i];

  return (r);
}


/*
  Limited vfprintf function

  Supports:
  - flags tags: %-
  - width field: supported, * (asterisk) not supported
  - format tags: %d, %s
*/
static int vprintf_lim (char const *fmt, va_list arg) {
  char  ch, *pch;
  char *a_string;
  int   a_int;
  int   len;
  unsigned int slen;
  int   m, p;

  /* Stacked vfprintf buffer */
  char  buf[512];

  len = 0;

  while (*fmt) {
    ch = *fmt++;

    if (ch == '%') {
      /* Process a format specifier */
      ch = *fmt++;
      m = 0;
      p = 0;

      /* Check flags */
      if (ch == '-') {
        /* %-xxX: left-justify within given width */
        m  = atoi_ex (fmt, &fmt);
        ch = *fmt++;
      }
      else if ((ch >= '0') && (ch <= '9')) {
        /* %0xX: prepend zero or space, ensure minimum num of characters to be printed */
        p = atoi_ex (fmt, &fmt);
        ch = *fmt++;
      }

      /* Print argument value */
      switch (ch) {
        case 'd': /* %d: print an integer */
          a_int = va_arg(arg, int);

          /* Convert value */
          itoa (a_int, buf, 10);

          slen = strlen(buf);

          /* Left-pad the number with spaces */
          p -= slen;
          while (p > 0) {
            out_putchar(' ');
            len++;
            p--;
          }

          /* Send to output */
          
          pch = buf;
          while (*pch) {
            out_putchar(*pch++);
          }
          len += slen;

          m -= slen;

          while (m > 0) {
            out_putchar(' ');
            len++;
            m--;
          }
          break;

        case 's': /* %s: print a string */
          a_string = va_arg(arg, char *);

          slen = strlen (a_string);

          /* Send to output */
          pch = a_string;
          while (*pch) {
            out_putchar(*pch++);
          }
          len += slen;

          m -= slen;

          while (m > 0) {
            out_putchar(' ');
            len++;
            m--;
          }
          break;

        default:
          /* Everything else is unsupported */
          out_putchar(ch);
          len++;
          break;
      }
    }
    else {
      /* Send directly to output */
      out_putchar(ch);
      len++;
    }
  }

  return len;
}


static int printf_lim (const char *fmt, ...) {
  va_list arg;
  int len;

  va_start(arg, fmt);
  len = vprintf_lim (fmt, arg);
  va_end(arg);

  return len;
}

static int out_putchar (int ch) {
  return stdout_putchar(ch);
}
#endif /* TF_OUTPUT */
