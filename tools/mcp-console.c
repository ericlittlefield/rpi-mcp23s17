#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "mcp23s17.h"


enum RegisterName
{
	IODIR,
	IPOL,
	GPINTEN,
	DEFVAL,
	INTCON,
	IOCONTROL,
	GPPU,
	INTF,
	INTCAP,
	GPIO,
	OLAT,
	NUMOFREGNAMES
};

enum RegisterBank
{
	A,
	B,
	NUMBANKS
};

enum Operation
{
	Write,
	Read,
	NUMOPS
};

struct Register_Value
{
	enum Operation ops;
	enum RegisterName reg;
	enum RegisterBank bank;
	uint8_t value;
};

char reg_table[] = { 0, /*IODIR*/
		               2, /*IPOL*/
					   4, /*GPINTEN*/
		               6, /*DEFVAL*/
		               8, /*INTCON*/
					   10, /*IOCONTROL*/
					   12, /*GPPU*/
					   14, /*INTF*/
					   16, /*INTCAP*/
					   18, /*GPIO*/
					   20  /*OLAT*/ };

char *reg_tokens[] = { "IODIR", "IPOL", "GPINTEN", "DEFVAL", "INTCON", "IOCON", "GPPU", "INTF", "INTCAP", "GPIO", "OLAT" };
char *port_tokens[] = {"A", "B" };

char *strupper( char *src, char *dest, size_t length_dest)
{
	assert(src);
	assert(dest);
	assert(length_dest);

	size_t avail = length_dest;
	memset(dest,length_dest,1);
	char *pSrc = src;
	char *pDest = dest;
	while(*pSrc != 0 && avail > 0)
	{
		*pDest = toupper(*pSrc);
		pSrc++;
		pDest++;
		avail--;
	}

	return dest;
}


int parse_cmd( char *buffer, size_t buffer_len, struct Register_Value *pRegister_Value)
{

  int good = -1;
  char *pInput = strtok(buffer," ");
  enum RegisterName reg = IODIR;
  enum RegisterBank bank = NUMBANKS;
  enum Operation ops = NUMOPS;
  uint8_t value = 0;

  assert(pRegister_Value != NULL);

  if( pInput != NULL && pInput < buffer + buffer_len )
  {
      if( pInput[0] == 'w' || pInput[0] == 'W')
    	ops = Write;
      else if( pInput[0] == 'r' || pInput[0] == 'R')
		ops = Read;


      if( ops != NUMOPS )
    	  pInput = strtok(NULL, " ");

      if( ops != NUMOPS && pInput != NULL && pInput < buffer + buffer_len )
	  {
		 char temp[12] = {0};
		 strupper(pInput, temp,sizeof(temp));
		 for( ; reg != NUMOFREGNAMES; reg++)
		 {
			if( strcmp(temp, reg_tokens[reg]) == 0)
				break;
		 }

		 if( reg < NUMOFREGNAMES )
		 {
			 pInput = strtok(NULL, " ");
			 if( pInput != NULL && (pInput < buffer + buffer_len))
			 {
				 if( (pInput[0] == 'a') || (pInput[0] == 'A'))
					 bank = A;
				 else if( (pInput[0] == 'b') || (pInput[0] == 'B'))
					 bank = B;
				 else
				 {
					 bank = NUMBANKS;
					 printf("Parse Reg bank failed: %s", pInput);
				 }

			 }
		 }
		 else
			 printf("Parse Reg id failed: %s", pInput);

		 if( ops != NUMOPS  && reg !=  NUMOFREGNAMES && bank != NUMBANKS)
		 {
			 if( ops == Write )
			 {
				 pInput = strtok(NULL, " ");

				 if( pInput != NULL && (pInput < buffer + buffer_len))
				 {
					 value = strtol(pInput,NULL, 0);
					 good = 0;
				 }
			 }
			 else
				 value = 0;
			     good = 0;

			 if(good == 0 )
			 {
				 pRegister_Value->ops = ops;
			     pRegister_Value->reg = reg;
			     pRegister_Value->bank = bank;
			     pRegister_Value->value = value;
		     }
		 }
	  }
  }

  return good;
}


int getinput( char *buffer, size_t length, FILE *fp )
{
	int loop = 1;
	char *ptr;
	while(loop)
	{
		ptr = fgets(buffer,length,fp);
		if( ptr != NULL )
		  loop = (buffer[0] == '\n');
		else if( feof(fp))
		{
			loop = 0;
			memset(buffer,0,length);
		}
	}
	return strlen(buffer);
}
int main(int argc, char *argv[])
{
    const int bus = 0;
    const int chip_select = 0;
    const int hw_addr = 0;
    int loop = 1;
    int mcp23s17_fd = mcp23s17_open(bus, chip_select);
    FILE *fp = stdin;

    if( argc > 1)
    {
      fp = fopen(argv[1], "r");
    }

    // config register
    const uint8_t ioconfig = BANK_OFF | \
                             INT_MIRROR_OFF | \
                             SEQOP_OFF | \
                             DISSLW_OFF | \
                             HAEN_ON | \
                             ODR_OFF | \
                             INTPOL_LOW;
    mcp23s17_write_reg(ioconfig, IOCON, hw_addr, mcp23s17_fd);

    char cmd[64];
    size_t len = sizeof(cmd);
    struct Register_Value operation;
    while(loop)
    {   size_t bytes_read = 0;

    	bytes_read = getinput(cmd,len,fp);
    	if( fp != stdin )
    	{
    		if( bytes_read > 0 )
    			printf("Playback: %s", cmd);
    		else
    		{
    			printf("Closed Playback\nContinuing from stdin\n");
    			fclose(fp);
    			fp = stdin;
    			continue;
    		}
    	}

    	if(cmd[0] == 'q' || cmd[0] == 'Q')
    	{
    		loop = 0;
    	    continue;
    	}

    	if( parse_cmd( cmd, len, &operation) == 0)
    	{
    	   uint8_t value = operation.value;
    	   uint8_t reg  = reg_table[operation.reg] + ((operation.bank == A) ? 0 : 0x1);

           if( operation.ops == Write )
           {
        	   mcp23s17_write_reg(value, reg, hw_addr, mcp23s17_fd);
        	   printf("Wrote 0x%x to %s%s (offset 0x%x)\n", value,
        			                                        reg_tokens[operation.reg],
															port_tokens[operation.bank],
															reg);
           }
           else
           {
        	   value = mcp23s17_read_reg(reg, hw_addr, mcp23s17_fd);
               printf("Read 0x%x from %s%s (offset 0x%x)\n", value,
            		                                         reg_tokens[operation.reg],
						                                     port_tokens[operation.bank],
															 reg);
           }
    	}
    	else
    		loop = 0;

    }
    close(mcp23s17_fd);
    return 0;
}


