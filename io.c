/*
 * io.c
 *
 *  Created on: Jun 7, 2013
 *      Author: niroren
 */

#include "io.h"

void init_pins()
{
	//PORT A - SDHC Card
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//set output: SD_SCK, SD_CS, SD_MOSI
	GPIOSetOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5);
	//Set Input: SD_MISO
	GPIOSetInput(GPIO_PORTA_BASE, GPIO_PIN_4);
	//Initialize pins: SD_SCK <- LOW, SD_CS <- HIGH, SD_MOSI <- LOW
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5, GPIO_PIN_3);
	//Set SD_CLK, SD_MISO, SD_MOSI as SSI pins
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5);

	//Port B - RTC

	//Enable Port B
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	//Set pin type as I2C
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);

	//Port D - LCD

	//TODO: Initialize LCD pins

	//Port E - SSI1 , VS0_BSYNC, VS0_RESET, VS0_DREQ, VS0_CS
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	//Set Output: VS_CLK, VS0_CS, VS_MOSI, VS0_BSYNC, VS0_RESET
	GPIOSetOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6);
	//Set Input: VS_MISO, VS0_DREQ
	GPIOSetInput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_5);
	//Initialize Pins: VS_CLK <- LOW, VS0_CS <- HIGH, VS_MOSI <- LOW, VS0_BSYNC <- HIGH, VS0_RESET <- LOW,
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6, GPIO_PIN_1 | GPIO_PIN_4);
	//Set VS_CLK, VS_MOSI, VS_MISO as SSI pins
	GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);

	//Port F -VS1_BSYNC, VS1_RESET, VS1_DREQ, VS1_CS
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	//Set Output: VS1_RESET, VS1_BSYNC, VS1_CS
	GPIOSetOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
	//Set Input: VS1_DREQ
	GPIOSetInput(GPIO_PORTF_BASE, GPIO_PIN_1);
	//Initialize Pins: VS0_RESET <- LOW,  VS0_BSYNC <- HIGH, VS1_CS <- HIGH
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_2 | GPIO_PIN_3);

	return;

}

void init_periph()
{
	//initialize SSI0 - SD card
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	SysCtlPeripheralReset(SYSCTL_PERIPH_SSI0);

	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, SysCtlClockGet()/4, 8);
	SSIEnable(SSI0_BASE);

	SSIDataPut(SSI0_BASE, 0xff);
	SSIDataPut(SSI0_BASE, 0xff);
	SSIDataPut(SSI0_BASE, 0xff);

	//Initialize SSI1 - VS Decoders
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
	SysCtlPeripheralReset(SYSCTL_PERIPH_SSI1);

	SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, SysCtlClockGet()/4, 8);
	SSIEnable(SSI1_BASE);

	SSIDataPut(SSI1_BASE, 0xff);
	SSIDataPut(SSI1_BASE, 0xff);
	SSIDataPut(SSI1_BASE, 0xff);

	//Initialize I2C0 - RTC
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

	return;
}

void GPIOSetOutput(unsigned long port, unsigned char pins)
{
  GPIOPinTypeGPIOOutput(port, pins); //output
  GPIOPadConfigSet(port, pins, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD); //push-pull

  return;
}

void GPIOSetInput(unsigned long port, unsigned char pins)
{
  GPIOPinTypeGPIOInput(port, pins); //input
  GPIOPadConfigSet(port, pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); //pullup

  return;
}

void delay_ms(unsigned long ms)
{
  unsigned long t;

  t =  ms * (SysCtlClockGet() / 3000);
  SysCtlDelay(t); //3 cycles

  return;
}

//   ************************** SD card SSI functions implementation ****************************

void ssi_wait(void)
{
  unsigned long r;

  while(HWREG(SSI0_BASE + SSI_O_SR) & SSI_SR_BSY);  //busy?
  //while(!(HWREG(SSI1_BASE + SSI_O_SR) & SSI_SR_TFE));  //transmit fifo empty?

  while(SSIDataGetNonBlocking(SSI0_BASE, &r)); //clear receive fifo

  return;
}


void ssi_write(unsigned char c)
{
  SSIDataPut(SSI0_BASE, c);

  return;
}


unsigned char ssi_read(void)
{
  unsigned long r;

  SSIDataGet(SSI0_BASE, &r);

  return (unsigned char)r;
}


unsigned char ssi_readwrite(unsigned char c)
{
  unsigned long r;

  SSIDataPut(SSI0_BASE, c);
  SSIDataGet(SSI0_BASE, &r);

  return (unsigned char)r;
}


void ssi_speed(unsigned long speed)
{
  unsigned long clk;

  clk = SysCtlClockGet();

  if((speed == 0) ||
     (speed > (clk/2)))
  {
    speed = clk/2;
  }

  if(speed > SSI_SPEED)
  {
    speed = SSI_SPEED;
  }

  SSIDisable(SSI0_BASE);
  SSIConfigSetExpClk(SSI0_BASE, clk, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, speed, 8);
  SSIEnable(SSI0_BASE);

  return;
}


void ssi_off(void)
{
  ssi_wait();

  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_5); //SCK, SI = gpio
  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_5, 0); //SCK, SI = low

  return;
}


void ssi_on(void)
{
  GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_5); //SCK, SI = ssi

  return;
}


//   ************************** VS Decoders SSI functions implementation ****************************

void vs_ssi_wait(void)
{
  unsigned long r;

  while(HWREG(SSI1_BASE + SSI_O_SR) & SSI_SR_BSY);  //busy?
  //while(!(HWREG(SSI0_BASE + SSI_O_SR) & SSI_SR_TFE));  //transmit fifo empty?

  while(SSIDataGetNonBlocking(SSI1_BASE, &r)); //clear receive fifo

  return;
}

void vs_ssi_writewait(void)
{
  while(HWREG(SSI1_BASE + SSI_O_SR) & SSI_SR_BSY);  //busy?

  return;
}


void vs_ssi_write(unsigned char c)
{
  SSIDataPut(SSI1_BASE, c);

  return;
}


unsigned char vs_ssi_readwrite(unsigned char c)
{
  unsigned long r;

  SSIDataPut(SSI1_BASE, c);
  SSIDataGet(SSI1_BASE, &r);

  return (unsigned char)r;
}


void vs_ssi_speed(unsigned long speed)
{
  unsigned long clk;

  clk = SysCtlClockGet();

  if((speed == 0) ||
     (speed > (clk/2)))
  {
    speed = clk/2;
  }

  if(speed > SSI_SPEED)
  {
    speed = SSI_SPEED;
  }

  SSIDisable(SSI1_BASE);
  SSIConfigSetExpClk(SSI1_BASE, clk, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, speed, 8);
  SSIEnable(SSI1_BASE);

  return;
}
