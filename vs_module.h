/*
 * vs_module.h
 *
 *  Created on: Jun 18, 2013
 *      Author: niroren
 */

#ifndef VS_MODULE_H_
#define VS_MODULE_H_

//----- DEFINES -----
#define DEFAULT_VOLUME                 (40)     //   0 -   100 %
#define DEFAULT_BASSAMP                (5)      //   0 -    15 dB
#define DEFAULT_BASSFREQ               (100)    //  20 -   150 Hz
#define DEFAULT_TREBLEAMP              (0)      //  -8 -     7 dB
#define DEFAULT_TREBLEFREQ             (15000)  //1000 - 15000 Hz
#define DEFAULT_NUM_UNITS              (2)      // Two decoder units

//VS FiFo
#define VS_BUFSIZE                     (42*1024) //42 kBytes

//Audio Format
#define FORMAT_UNKNOWN                 (0)
#define FORMAT_WAV                     (1)
#define FORMAT_MP3                     (2)
#define FORMAT_AAC                     (3)
#define FORMAT_OGG                     (4)
#define FORMAT_WMA                     (5)
#define FORMAT_FLAC                    (6)

//Clock
#define VS_XTAL                        (12288000UL)

//Opcode
#define VS_READ                        (0x03)
#define VS_WRITE                       (0x02)

//Register
#define VS_MODE                        (0x00)   //Mode control
#define SM_RESET                       (1<< 2)  //Soft Reset
#define SM_CANCEL                      (1<< 3)  //Cancel Decoding
#define SM_STREAM                      (1<< 6)  //Stream Mode
#define SM_SDINEW                      (1<<11)  //VS1002 native SPI modes
#define VS_STATUS                      (0x01)   //Status
#define VS_BASS                        (0x02)   //Built-in bass/treble enhancer
#define VS_CLOCKF                      (0x03)   //Clock freq + multiplier
#define VS1033_SC_MUL_2X               (0x4000)
#define VS1033_SC_MUL_3X               (0x8000)
#define VS1033_SC_MUL_4X               (0xC000)
#define VS1053_SC_MUL_2X               (0x2000)
#define VS1053_SC_MUL_3X               (0x6000)
#define VS1053_SC_MUL_4X               (0xA000)
#define VS_DECODETIME                  (0x04)   //Decode time in seconds
#define VS_AUDATA                      (0x05)   //Misc. audio data
#define VS_WRAM                        (0x06)   //RAM write/read
#define VS_WRAMADDR                    (0x07)   //Base address for RAM write/read
#define VS_HDAT0                       (0x08)   //Stream header data 0
#define VS_HDAT1                       (0x09)   //Stream header data 1
#define VS_AIADDR                      (0x0A)   //Start address of application
#define VS_VOL                         (0x0B)   //Volume control
//RAM Data
#define VS_RAM_ENDFILLBYTE             (0x1E06)  //End fill byte


typedef union
{
  uint8_t  b8[VS_BUFSIZE];
  uint16_t b16[VS_BUFSIZE/2];
  uint32_t b32[VS_BUFSIZE/4];
} VSBUFFER;

typedef union
{
	unsigned int vs_playing = 0;
	int vs_vol = 0,
	    vs_bassAmp = 0,
	    vs_bassFreq = 0,
	    vs_trebAmp = 0,
	    vs_trebFreq = 0;
} VSSTATUS;

//----- GLOBALS -----
extern VSBUFFER vs_buffers[DEFAULT_NUM_UNITS];
extern VSSTATUS vs_status[DEFAULT_NUM_UNITS];

//----- PROTOTYPES -----

//Transformed
void                vs_init(int unit);
void                vs_bufreset(int unit);
void                vs_bufsethead(int unit, unsigned int head);
void                vs_reset(int unit);
void                vs_write_reg(int unit, unsigned int reg, unsigned int data);
unsigned int        vs_read_reg(int unit, unsigned int reg);
unsigned int        vs_request(int unit);
void                vs_setvolume(int unit, int vol);      //   0 -   100 %, 0=off
void                vs_write_volume(int unit);
void                vs_setbassfreq(int unit, int freq);   //  20 -   150 Hz
void                vs_write_bass(int unit);
void                vs_setbassamp(int unit, int amp);    //   0 -    15 dB
void                vs_settreblefreq(int unit, int freq); //1000 - 15000 Hz
void                vs_settrebleamp(int unit, int amp);   //  -8 -     7 dB


//Not Transformed

//TODO: Transform functions for 2 units mode
void                                   vs_requesthandler(void);
unsigned char                          vs_bufgetc(void);
void                                   vs_bufputs(const unsigned char *s, unsigned int len);
unsigned int                           vs_buffree(void);
unsigned int                           vs_buflen(void);



int                                    vs_gettreblefreq(void);
int                                    vs_gettrebleamp(void);
int                                    vs_getbassfreq(void);
int                                    vs_getbassamp(void);
int                                    vs_getvolume(void);

void                                   vs_data(unsigned int c);
void                                   vs_write_plugin(const unsigned short *plugin, unsigned int len);
unsigned int                           vs_read_ram(unsigned int addr);
void                                   vs_pause(void);
void                                   vs_play(void);
void                                   vs_stopstream(void);
void                                   vs_stop(void);
void                                   vs_start(void);

#endif /* VS_MODULE_H_ */
