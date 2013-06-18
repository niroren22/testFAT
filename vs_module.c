/*
 * vs_module.c
 *
 *  Created on: Jun 18, 2013
 *      Author: niroren
 */

#include <stdint.h>
#include <stdlib.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/ssi.h"
#include "fatfs/ff.h"
#include "utils.h"
#include "io.h"
#include "vs_module.h"

//Decoder Global Data
VSBUFFER vs_buffers[DEFAULT_NUM_UNITS];
VSSTATUS vs_status[DEFAULT_NUM_UNITS];

//Decoder Local Data
volatile unsigned int vs_bufhead[DEFAULT_NUM_UNIT] = {0,0};
volatile unsigned int vs_buftail[DEFAULT_NUM_UNIT] = {0,0};

//Functions implementation

void vs_init(int unit)
{
  vs_status[unit].vs_playing = 0;

  //reset vs buffer
  vs_bufreset(unit);

  //reset vs
  vs_reset();

  //set volume, bass, treble
  vs_setvolume(unit, DEFAULT_VOLUME);
  vs_setbassfreq(unit, DEFAULT_BASSFREQ);
  vs_setbassamp(unit, DEFAULT_BASSAMP);
  vs_settreblefreq(unit, DEFAULT_TREBLEFREQ);
  vs_settrebleamp(unit, DEFAULT_TREBLEAMP);

  //init pin interrupt

  //TODO: needs change - init interrupts by unit
  //GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_1, GPIO_HIGH_LEVEL);
  //GPIOPortIntRegister(GPIO_PORTA_BASE, vs_requesthandler);

  return;
}

void vs_bufreset(int unit)
{
  vs_bufhead[unit] = 0;
  vs_buftail[unit] = 0;

  return;
}

void vs_bufsethead(int unit, unsigned int head)
{
  vs_bufhead[unit] = head;

  return;
}

void vs_reset(int unit)
{
  unsigned long i;

  //ssi speed down
  vs_ssi_wait(); //wait for transfer complete
  vs_ssi_speed(2000000); //2 MHz

  //hard reset
  VS_CS_DISABLE(unit);
  VS_DCS_DISABLE(unit);
  VS_RST_ENABLE(unit);
  delay_ms(5);
  VS_RST_DISABLE(unit);
  delay_ms(10);

  //set registers
  vs_write_reg(unit, VS_MODE, SM_SDINEW);
  //set clock multiplier and load patch - VS1033
  vs_write_reg(unit, VS_CLOCKF, 0x1800|VS1033_SC_MUL_4X);
  //Load Patch disabled
  //vs_write_plugin(vs1033d_patch, VS1033D_PATCHLEN);

  //ssi speed up
  vs_ssi_speed(0); //0 = default speed

  return;
}

void vs_write_reg(int unit, unsigned int reg, unsigned int data)
{
  unsigned int timeout;

  IntMasterDisable();

  vs_ssi_wait(); //wait for transfer complete
  VS_DCS_DISABLE(unit);
  VS_CS_ENABLE(unit);
  vs_ssi_readwrite(VS_WRITE);
  vs_ssi_readwrite(reg);
  vs_ssi_readwrite(data>>8);
  vs_ssi_readwrite(data);
  VS_CS_DISABLE(unit);

  //execution -> DREQ low
  switch(reg)
  {
    case VS_MODE:     timeout = 20000; break;
    case VS_STATUS:   timeout =   100; break;
    case VS_BASS:     timeout =  1000; break;
    case VS_CLOCKF:   timeout = 20000; break;
    case VS_WRAM:     timeout =   100; break;
    case VS_WRAMADDR: timeout =   100; break;
    case VS_VOL:      timeout =  1000; break;
    default:          timeout =  1000; break;
  }
  for(; timeout!=0; timeout--)
  {
    if(vs_request(unit))
    {
      break;
    }
  }

  IntMasterEnable();

  return;
}


unsigned int vs_read_reg(int unit, unsigned int reg)
{
  unsigned int ret, timeout;

  IntMasterDisable();

  vs_ssi_wait(); //wait for transfer complete
  VS_DCS_DISABLE(unit);
  VS_CS_ENABLE(unit);
  vs_ssi_readwrite(VS_READ);
  vs_ssi_readwrite(reg);
  ret  = vs_ssi_readwrite(0xff)<<8;
  ret |= vs_ssi_readwrite(0xff);
  VS_CS_DISABLE(unit);

  //execution -> DREQ low
  switch(reg)
  {
    case VS_MODE:     timeout = 20000; break;
    case VS_STATUS:   timeout =   100; break;
    case VS_BASS:     timeout =  1000; break;
    case VS_CLOCKF:   timeout = 20000; break;
    case VS_WRAM:     timeout =   100; break;
    case VS_WRAMADDR: timeout =   100; break;
    case VS_VOL:      timeout =  1000; break;
    default:          timeout =  1000; break;
  }
  for(; timeout!=0; timeout--)
  {
    if(vs_request(unit))
    {
      break;
    }
  }

  IntMasterEnable();

  return ret;
}

unsigned int vs_request(int unit)
{
  return VS_DREQ_READ(unit); //1=ready, 0=buf full
}

void vs_setvolume(int unit, int vol) //0 - 100%
{
  if(vol <= 0) //<= 0
  {
    vs_write_reg(unit, VS_VOL, 0xFFFF); //analog power off
  }
  else if(vol > 100) //> 100
  {
    vs_vol = 100;
    if(vs_status[unit].vs_playing)
    {
      vs_write_volume(unit);
    }
  }
  else //1 - 99
  {
	vs_status[unit].vs_vol = vol;
    if(vs_status[unit].vs_playing)
    {
      vs_write_volume(unit);
    }
  }

  return;
}

void vs_write_volume(int unit)
{
  int vol;

  vol = 100-vs_status[unit].vs_vol;
  vs_write_reg(unit, VS_VOL, (vol<<8)|(vol<<0));

  return;
}

void vs_setbassfreq(int unit, int freq) //20 - 150Hz
{
  freq /= 10;

  if(freq < 2) //< 2
  {
    freq = 2;
  }
  else if(freq > 15) //> 15
  {
    freq = 15;
  }
  vs_status[unit].vs_bassFreq = freq;
  vs_write_bass(unit);

  return;
}

void vs_write_bass(int unit)
{
  vs_write_reg(unit, VS_BASS, ((vs_status[unit].vs_trebAmp&0x0f)<<12)|
		  	  	  	  	  	  ((vs_status[unit].vs_trebFreq&0x0f)<<8)|
		  	  	  	  	  	  ((vs_status[unit].vs_bassAmp&0x0f)<<4)|
		  	  	  	  	  	  ((vs_status[unit].vs_bassFreq&0x0f)<<0));
  return;
}

void vs_setbassamp(int unit, int amp) //0 - 15dB
{
  if(amp < 0) //< 0
  {
    amp = 0;
  }
  else if(amp > 15) //> 15
  {
    amp = 15;
  }
  vs_status[unit].vs_bassAmp = amp;
  vs_write_bass(unit);

  return;
}

void vs_settreblefreq(int unit, int freq) //1000 - 15000Hz
{
  freq /= 1000;

  if(freq < 1) //< 1
  {
    freq = 1;
  }
  else if(freq > 15) //> 15
  {
    freq = 15;
  }
  vs_status[unit].vs_trebFreq = freq;
  vs_write_bass(unit);

  return;
}

void vs_settrebleamp(int unit, int amp) //-8 - 7dB
{
  if(amp < -8) //< -8
  {
    amp = -8;
  }
  else if(amp > 7) //> 7
  {
    amp = 7;
  }
  vs_status[unit].vs_trebAmp = amp;
  vs_write_bass(unit);

  return;
}
