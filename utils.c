/*
 * utils.c
 *
 *  Created on: Jun 7, 2013
 *      Author: niroren
 */

#include <ctype.h>
#include <string.h>
#include "utils.h"

TIME timedate;

void gettime(TIME* t)
{
  IntMasterDisable();
  t->year  = timedate.year;
  t->month = timedate.month;
  t->day   = timedate.day;
  t->wday  = timedate.wday;
  t->h     = timedate.h;
  t->m     = timedate.m;
  t->s     = timedate.s;
  IntMasterEnable();

  return;
}

//removes the spaces at the beginning and at the end of the string
char* strrmvspace(char *dst, const char *src)
{
  unsigned int i;

  if(*src == 0)
  {
    *dst = 0;
  }
  else
  {
    //at start
    for(i=0; isspace(src[i]); i++);
    strcpy(dst, &src[i]);

    //at end
    i=strlen(dst)-1;
    for(i=strlen(dst)-1; isspace(dst[i]); i--)
    {
      dst[i] = 0;
    }
  }

  return dst;
}
