/*
 * utils.h
 *
 *  Created on: Jun 7, 2013
 *      Author: niroren
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "inc/hw_types.h"
#include "driverlib/interrupt.h"

#define MAX_ADDR (260)


//----- DEFINES -----
typedef struct
{
  unsigned int s;     //0-59
  unsigned int m;     //0-59
  unsigned int h;     //0-23
  unsigned int day;   //1-31
  unsigned int month; //1-12
  unsigned int year;  //1970-...
  unsigned int wday;  //0-6 (0=Sunday)
} TIME;

void    gettime(TIME* t);

//removes the spaces at the beginning and at the end of the string
char*   strrmvspace(char *dst, const char *src);

#endif /* UTILS_H_ */
