/*
 * sdcard.h
 *
 *  Created on: Jun 7, 2013
 *      Author: niroren
 */

#ifndef SDCARD_H_
#define SDCARD_H_

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#include "io.h"
#include "utils.h"

//----- DEFINES -----
//MMC/SD command
#define CMD0                           (0x40+ 0) //GO_IDLE_STATE
#define CMD1                           (0x40+ 1) //SEND_OP_COND
#define ACMD41                         (0xC0+41) //SEND_OP_COND (SDC)
#define CMD8                           (0x40+ 8) //SEND_IF_COND
#define CMD9                           (0x40+ 9) //SEND_CSD
#define CMD10                          (0x40+10) //SEND_CID
#define CMD12                          (0x40+12) //STOP_TRANSMISSION
#define ACMD13                         (0xC0+13) //SD_STATUS (SDC)
#define CMD16                          (0x40+16) //SET_BLOCKLEN
#define CMD17                          (0x40+17) //READ_SINGLE_BLOCK
#define CMD18                          (0x40+18) //READ_MULTIPLE_BLOCK
#define CMD23                          (0x40+23) //ET_BLOCK_COUNT
#define ACMD23                         (0xC0+23) //SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24                          (0x40+24) //WRITE_BLOCK
#define CMD25                          (0x40+25) //WRITE_MULTIPLE_BLOCK
#define CMD41                          (0x40+41) //SEND_OP_COND (ACMD)
#define CMD55                          (0x40+55) //APP_CMD
#define CMD58                          (0x40+58) //READ_OCR

//----- PROTOTYPES -----
void                                   xmit_spi(BYTE dat);
BYTE                                   rcvr_spi(void);
void                                   rcvr_spi_m(BYTE *dst);
BYTE                                   wait_ready(void);
void                                   release_spi(void);
void                                   power_on(void);
void                                   power_off(void);
BYTE                                   chk_power(void);
BYTE                                   rcvr_datablock(BYTE *buff, UINT btr);
BYTE                                   xmit_datablock(const BYTE *buff, BYTE token);
BYTE                                   send_cmd(BYTE cmd, DWORD arg);
void                                   disk_timerproc(void);

#endif /* SDCARD_H_ */
