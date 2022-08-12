/*
 * debugger.c
 *
 *  Created on: Jan 4, 2022
 *      Author: janoko
 */

#include "debugger.h"
#include <os.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef USE_FREERTOS
#include "cmsis_os2.h"
#endif

/* External variables
 * --------------------------------------------*/
#if USE_FREERTOS
extern osMutexId_t LogMutexHandle;
#endif

/* Private constants
 * --------------------------------------------*/
#define LOG_TIMEOUT_MS 2U

/* Private function prototypes
 * --------------------------------------------*/
static void Lock(void);
static void UnLock(void);
static void SendITM(char ch);
static void printTmpWordText(uint8_t len);

static char *DBG_Breakline = "\r\n";
static uint8_t tmpWord[16];

/* Public function implementations
 * --------------------------------------------*/
void DBG_Init(void)
{
  Lock();
  setvbuf(stdout, NULL, _IONBF, 0);
  UnLock();
}


void DBG_Printf(const char *format, ...)
{
  va_list args;

  Lock();
  va_start (args, format);
  vprintf(format, args);
  va_end (args);
  UnLock();
}


void DBG_Println(const char *format, ...)
{
  va_list args;

  Lock();
  va_start (args, format);
  vprintf (format, args);
  printf(DBG_Breakline);
  va_end (args);
  UnLock();
}


void DBG_PrintB(const uint8_t* bytes, uint16_t len)
{
  uint8_t j = 0;

  Lock();
  for (uint16_t i = 0; i < len; i++) {
    printf("%02X ", *bytes);
    if ((i+1) % 16 == 0 || i == len-1) {
      for (uint8_t k = j+1; k < 16; k++) {
        printf("-- ");
      }
      printf(" |  ");
      printTmpWordText(j+1);
      printf(DBG_Breakline);
    } else if ((i+1) % 8 == 0) {
      printf(" ");
    }

    tmpWord[j] = *bytes;
    bytes++;
    j++;
    if (j >= 16) j = 0;
  }
  printf(DBG_Breakline);
  UnLock();
}

void DBG_vPrintf(const char *format, va_list args)
{
  Lock();
  vprintf (format, args);
  UnLock();
}

void DBG_vPrintln(const char *format, va_list args)
{
  Lock();
  vprintf (format, args);
  printf(DBG_Breakline);
  UnLock();
}

int __io_putchar(int ch)
{
  SendITM(ch);

  return (ch);
}

int _write(int file, char *data, int len)
{
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
    __io_putchar(*data++);

  return (len);
}

/* Private function implementations
 * --------------------------------------------*/
static void Lock(void)
{
}

static void UnLock(void)
{
}

static void SendITM(char ch)
{
  uint8_t retry = 2;

#ifdef DEBUG
  /* wait if busy */
  while (retry--)
  {
    if (ITM->PORT[0].u32 != 0)
      break;
    OS_Delay(1);
  }

  ITM->PORT[0].u8 = (uint8_t)ch;
#endif
}


static void printTmpWordText(uint8_t len)
{
  for (uint16_t i = 0; i < len; i++) {
    if (tmpWord[i] > 0x20 && tmpWord[i] < 0x7A)
      printf("%c ", tmpWord[i]);
    else
      printf("- ");

    if ((i+1) % 8 == 0)
      printf(" ");
  }
}
