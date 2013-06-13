/*
 * main.c
 *
 *  Created on: Jun 6, 2013
 *      Author: niroren
 */


#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#include "io.h"
#include "mmc.h"


int main(void)
{
	unsigned long ulPeriod;
	unsigned long ulDelay;

	unsigned long recvData = 11;

	volatile int status = 0;

	char buffer[40];
	unsigned int bytesdRead;

	FRESULT res;
	FIL fileobj;

	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	//Initialize pins
	init_pins();

	//Initialize peripherals
	init_periph();

	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);//false for 100kHz mode
	    //0x68 is the 7-bit address of the DS1307
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x68, false);//true for a read action
	I2CMasterDataPut(I2C0_MASTER_BASE, 0x10);      //first Date/Time Register
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}
	recvData = I2CMasterDataGet(I2C0_MASTER_BASE);
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}

	fs_mount();

	if(disk_status(0) == 0)
	{
		status++;
	}
	else
	{
	    status--;
	}

	if (status == 1) {
		res = f_open(&fileobj, "config.txt", FA_OPEN_EXISTING | FA_READ);
		res = f_read(&fileobj, buffer, 6, &bytesdRead);
	}

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);

	ulPeriod = SysCtlClockGet() / 10;
	ulDelay = ((ulPeriod / 2) / 3) - 4 ;

	while(1)
	{
		// Turn on the LED
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x01);

		// Delay for a bit
		SysCtlDelay(ulDelay);

		// Turn off the LED
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);

		// Delay for a bit
		SysCtlDelay(ulDelay);
	}
}
