/***************************************************************************
 *    CDROM.C  - CD-ROM interface code (audio routines)                    *
 *                                                                         *
 ***************************************************************************/

#include "build.h"
#include "tekwar.h"

void
rmint86(short intno,struct rmregs *rmptr)
{

}

alcmem(short bytes)
{

}

int
ismscdex(void)
{
	return 0;
}

void
cd_getname(void)
{

}

void
sendIOcmd(short hdl,short bytes,short cmd)
{

}

void
cd_opendoor(short cdhdl)
{

}

void
cd_closedoor(short cdhdl)
{

}

void
cd_reset(short cdhdl)
{

}

void
cd_getaudiodisk(short cdhdl)
{

}

void
cd_getaudiotrack(short cdhdl,char tno)
{

}

int
cdpreinit(void)
{
	return 0;
}

void
cduninit(void)
{

}

void
cd_stop(void)
{

}

int
cd_playtrack(char tno)
{
	return 0;
}

void
cd_resumeplay(void)
{

}

void
cd_play(void)
{

}

void
cd_stopplay(void)
{

}

void 
cd_advancetrack()
{

}

void 
cd_retardtrack()
{

}

void
cd_resetdisc(void)
{

}

