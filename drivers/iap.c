/*
    ALDS (ARM LPC Driver Set)

    iap.c:
          In Application Programming

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -From a LPC2000 datasheet:
             "RAM used by IAP command handler :
              Flash programming commands use the top 32 bytes of on-chip RAM. The
              maximum stack usage in the user allocated stack space is 128 bytes
              and it grows downwards."
             Check the 'STACK_OFFSET' setting in config.h!

*/
/*!
\file
In Application Programming
*/
#include <iap.h>

#ifdef IAP_ENABLED

#include <err.h>

/* IAP commands */
#define IAP_PREPAREWRITE                            50
#define IAP_RAMTOFLASH                              51
#define IAP_ERASESECTOR                             52
#define IAP_BLANKCHECKSECTOR                        53
#define IAP_READPARTID                              54
#define IAP_READBOOTCODEVERSION                     55
#define IAP_COMPARE                                 56
#define IAP_INVOKEISP                               57

/* IAP return codes */
#define IAP_CMD_SUCCESS                             0
#define IAP_INVALID_COMMAND                         1
#define IAP_SRC_ADDR_ERROR                          2
#define IAP_DST_ADDR_ERROR                          3
#define IAP_SRC_ADDR_NOT_MAPPED                     4
#define IAP_DST_ADDR_NOT_MAPPED                     5
#define IAP_COUNT_ERROR                             6
#define IAP_INVALID_SECTOR                          7
#define IAP_SECTOR_NOT_BLANK                        8
#define IAP_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION 9
#define IAP_COMPARE_ERROR                           10
#define IAP_BUSY                                    11

/* Flash sector */
typedef struct sector {
    unsigned int start;
    unsigned short length;
} sector_t;

/* MCU flash layout */
#if __MCU_FLASHTYPE == 1
static const sector_t sectors[32] = {    // Max 256 KB
    {0x00000000, 8192},
    {0x00002000, 8192},
    {0x00004000, 8192},
    {0x00006000, 8192},
    {0x00008000, 8192},
    {0x0000A000, 8192},
    {0x0000C000, 8192},
    {0x0000E000, 8192},
    {0x00010000, 8192},
    {0x00012000, 8192},
    {0x00014000, 8192},
    {0x00016000, 8192},
    {0x00018000, 8192},
    {0x0001A000, 8192},
    {0x0001C000, 8192},
    {0x0001E000, 8192},
    {0x0001F000, 8192},
    {0x00020000, 8192},
    {0x00021000, 8192},
    {0x00022000, 8192},
    {0x00023000, 8192},
    {0x00024000, 8192},
    {0x00025000, 8192},
    {0x00026000, 8192},
    {0x00027000, 8192},
    {0x00028000, 8192},
    {0x00029000, 8192},
    {0x0002A000, 8192},
    {0x0002B000, 8192},
    {0x0002C000, 8192},
    {0x0002D000, 8192},
    {0x0002E000, 8192} };
#elif __MCU_FLASHTYPE == 2
static const sector_t sectors[27] = {    // Max 512 KB
    {0x00000000, 4096},
    {0x00001000, 4096},
    {0x00002000, 4096},
    {0x00003000, 4096},
    {0x00004000, 4096},
    {0x00005000, 4096},
    {0x00006000, 4096},
    {0x00007000, 4096},
    {0x00008000, 32768},
    {0x00010000, 32768},
    {0x00018000, 32768},
    {0x00020000, 32768},
    {0x00028000, 32768},
    {0x00030000, 32768},
    {0x00038000, 32768},
    {0x00040000, 32768},
    {0x00048000, 32768},
    {0x00050000, 32768},
    {0x00058000, 32768},
    {0x00060000, 32768},
    {0x00068000, 32768},
    {0x00070000, 32768},
    {0x00078000, 4096},
    {0x00079000, 4096},
    {0x0007A000, 4096},
    {0x0007B000, 4096},
    {0x0007C000, 4096} };
#endif

/* Part identification */
typedef struct partid {
    unsigned int id;
    unsigned short name;
} partid_t;

/*! List of some partID's i've found. Incomplete and possibly incorrect (so use with caution!);
    the partID organisation seems to be a bit messy. Note that the LPC213x
    series have the same partID's as the LPC214x series (According to the datasheet, that is..).
    The LPC2101/2/3 possibly have the same ID's, too (or the datasheet is incomplete..?) */
static const partid_t iap_partids[] = {
    {0,LPC2101},
    {0,LPC2102},
    {0x0004FF11, LPC2103},
    {0xFFF0FF12, LPC2104},
    {0,LPC2105},
    {0xFFF0FF32, LPC2106},
    {0,LPC2114},
    {0,LPC2119},
    {0,LPC2124},
    {0,LPC2129},
    {0,LPC2194},
    {0x0002FF01, LPC2131},
    {0x0002FF11, LPC2132},
    {0x0002FF12, LPC2134},
    {0x0002FF23, LPC2136},
    {0x0002FF25, LPC2138},
    {0x0002FF01, LPC2141},
    {0x0002FF11, LPC2142},
    {0x0002FF12, LPC2144},
    {0x0002FF23, LPC2146},
    {0x0002FF25, LPC2148}
};

static unsigned int iap_command[5];
static unsigned int iap_result[2];

/* IAP access function, found in iapcall.S */
extern void iap_call(const unsigned int *command, const unsigned int *result);

static char iap_addresstosector(const unsigned int address)
/*!
  Return sector of given address
*/
{
    unsigned char i;

    for(i=0;i<__MCU_FLASHSECTORS;i++) {
        if(address < sectors[i].start) {
            return i-1;
        }
    }
    /* We *might* return an invalid sector number here..
       No worries, though: the IAP function will return IAP_INVALID_SECTOR in that case */
    return i;
}

static error_t iap_prepare(const unsigned char start, const unsigned char end)
/*!
  Prepare sectors from 'start' to 'end' for writing
*/
{
    /* Prepare the command */
    iap_command[0]=IAP_PREPAREWRITE;        // Command
    iap_command[1]=start;                   // Starting sector
    iap_command[2]=end;                     // End sector

    /* Execute.. */
    iap_call(iap_command,iap_result);

    /* .. and evaluate */
    if(iap_result[0]!=IAP_CMD_SUCCESS) {
        return ERROR;
    }
    return GOOD;
}

error_t iap_write(const unsigned int address, const unsigned int *buffer, const unsigned short length)
/*!
  Write a block of data from RAM (located in '*buffer', with length 'length' to
  flash starting at address 'address'
*/
{
    /* Unlock sectors */
    if(iap_prepare(iap_addresstosector(address),iap_addresstosector(address + length)) != GOOD) {
        return ERROR;
    }

    /* Prepare the command */
    iap_command[0]=IAP_RAMTOFLASH;          // Command
    iap_command[1]=address;                 // Starting address of destination
    iap_command[2]=*buffer;                 // Starting address of source
    iap_command[3]=length;                  // Length of source
    iap_command[4]=CCLK/1000;               // Oscillator frequency

    /* Execute.. */
    iap_call(iap_command,iap_result);

    /* .. and evaluate */
    if(iap_result[0]!=IAP_CMD_SUCCESS) {
        return ERROR;
    }
    return GOOD;
}

error_t iap_erase(const unsigned char start, const unsigned char end)
/*!
  Erase flash sectors from 'start' to 'end'
*/
{
    /* Unlock sectors */
    if(iap_prepare(start,end) != GOOD) {
        return ERROR;
    }

    /* Prepare the command */
    iap_command[0]=IAP_ERASESECTOR;         // Command
    iap_command[1]=start;                   // Starting sector
    iap_command[2]=end;                     // End sector
    iap_command[4]=CCLK/1000;               // Oscillator frequency

    /* Execute.. */
    iap_call(iap_command,iap_result);

    /* .. and evaluate */
    if(iap_result[0]!=IAP_CMD_SUCCESS) {
        return ERROR;
    }
    return GOOD;
}

error_t iap_blankcheck(const unsigned char start, const unsigned char end)
/*!
  Check if the given sector(s) are blank. Return TRUE if so, FALSE if not or
  ERROR when the check failed.
*/
{
    /* Prepare the command */
    iap_command[0]=IAP_BLANKCHECKSECTOR;    // Command
    iap_command[1]=start;                   // Starting sector
    iap_command[2]=end;                     // End sector

    /* Execute.. */
    iap_call(iap_command,iap_result);

    /* .. and evaluate */
    if(iap_result[0]==IAP_CMD_SUCCESS) {
        return TRUE;
    }
    else if (iap_result[0]==IAP_SECTOR_NOT_BLANK) {
        return FALSE;
    }
    return ERROR;
}

unsigned int iap_getpartid(void)
/*!
  Return the part ID
*/
{
    /* Prepare the command */
    iap_command[0]=IAP_READPARTID;          // Command

    /* Execute.. */
    iap_call(iap_command,iap_result);

    /* .. and evaluate */
    return iap_result[1];
}

unsigned short iap_getbootloaderversion(void)
/*!
  Return the bootloader version.
  Version is encoded as ((result>>8)&0xFF).(result&0xff) (as in softwareversion major.minor)
*/
{
    /* Prepare the command */
    iap_command[0]=IAP_READBOOTCODEVERSION; // Command

    /* Execute.. */
    iap_call(iap_command,iap_result);

    /* .. and evaluate */
    return (unsigned short)(iap_result[1]&0xFFFF);
}

#else
#warning "Driver disabled"
#endif /*IAP_ENABLED */
