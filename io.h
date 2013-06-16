/*
 * io.h
 *
 *  Created on: Jun 7, 2013
 *      Author: niroren
 */

#ifndef IO_H_
#define IO_H_

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "inc/hw_ssi.h"

#define SSI_SPEED  (8000000UL) //8 MHz (max ssi speed)

//SD

//Currently not used
#define MMC_POWERON()             GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_0, 0); \
                                  GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD)
#define MMC_POWEROFF()            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_PIN_0); \
                                  GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU)

#define MMC_SELECT()              GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0)
#define MMC_DESELECT()            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3)
#define MMC_SCK_LOW()             GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0)
#define MMC_SCK_HIGH()            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2)
#define MMC_SI_LOW()              GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0)
#define MMC_SI_HIGH()             GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5)

//VS
#define VS_DREQ_READ(unit)        GPIOPinRead((unit == 0)  ? GPIO_PORTE_BASE : GPIO_PORTF_BASE, (unit == 0) ? GPIO_PIN_5 : GPIO_PIN_1)
#define VS_RST_DISABLE(unit)      GPIOPinWrite((unit == 0) ? GPIO_PORTE_BASE : GPIO_PORTF_BASE, (unit == 0) ? GPIO_PIN_6 : GPIO_PIN_0, \
											   (unit == 0) ? GPIO_PIN_6 : GPIO_PIN_0)
#define VS_RST_ENABLE(unit)       GPIOPinWrite((unit == 0) ? GPIO_PORTE_BASE : GPIO_PORTF_BASE, (unit == 0) ? GPIO_PIN_6 : GPIO_PIN_0, 0)
#define VS_CS_DISABLE(unit)       GPIOPinWrite((unit == 0) ? GPIO_PORTE_BASE : GPIO_PORTF_BASE, (unit == 0) ? GPIO_PIN_1 : GPIO_PIN_3, \
											   (unit == 0) ? GPIO_PIN_1 : GPIO_PIN_3)
#define VS_CS_ENABLE(unit)        GPIOPinWrite((unit == 0) ? GPIO_PORTE_BASE : GPIO_PORTF_BASE, (unit == 0) ? GPIO_PIN_1 : GPIO_PIN_3, 0)
#define VS_DCS_DISABLE(unit)      GPIOPinWrite((unit == 0) ? GPIO_PORTE_BASE : GPIO_PORTF_BASE, (unit == 0) ? GPIO_PIN_4 : GPIO_PIN_2, \
											   (unit == 0) ? GPIO_PIN_4 : GPIO_PIN_2)
#define VS_DCS_ENABLE(unit)       GPIOPinWrite((unit == 0) ? GPIO_PORTE_BASE : GPIO_PORTF_BASE, (unit == 0) ? GPIO_PIN_4 : GPIO_PIN_2, 0)


void init_pins();
void init_periph();

void GPIOSetOutput(unsigned long port, unsigned char pins);
void GPIOSetInput(unsigned long port, unsigned char pins);

void delay_ms(unsigned long ms);

void ssi_wait(void);
void ssi_write(unsigned char c);
unsigned char ssi_read(void);
unsigned char ssi_readwrite(unsigned char c);
void ssi_speed(unsigned long speed);
void ssi_off(void);
void ssi_on(void);

#endif /* IO_H_ */
