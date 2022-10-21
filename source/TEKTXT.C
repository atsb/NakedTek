/***************************************************************************
 *   TEKTXT.C  - text based & MiSCcellaneous routines for Tekwar game      *
 *   Uses BIOS modes for compatibility and scrolling.                      *
 *   Includes CD code.                                                     *
 *                                                     12/14/94 Les Bird   *
 ***************************************************************************/

#include <fcntl.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "time.h"
#include "build.h"
#include "tekwar.h"        
#include "tekver.c"

#define   CDRELEASE
#define   COPYRIGHTSTRING     "INTRACORP1995TW"
#define   GMIN           60
#define   GHRS           3600

char      cddriveletter='D';
unsigned  criticalerror=0L;

struct    devhdrtype {
     long      nextdrvr;
     short     attrib;
     short     str_routine;
     short     int_routine;
     char      name[8];
     char      idunno[2];
     char      drive;
     char      numunits;
};

struct    devlsttype {
     char      subunit;
     struct    devhdrtype    *devhdr;
};

struct    dosreqhdrtype {
	char      length;  
	char      subunit;
	char		commandcode;
	short	status; 
	char		rsrvd[8];
};

struct    ioctlhdrtype {
     struct    dosreqhdrtype  doshdr;
     char      mediadescriptor;     // always 0
     char      *ctrlblkptr;
     short     blklength;
     short     startsector;         // always 0
     char      *volidptr;           // always 0
};

struct    rminfo    {
     long      edi,
               esi,
               ebp;
     long      reserved_by_system;
     long      ebx,
               edx,
               ecx,
               eax;
     short     flags;
     short     es,ds,fs,gs,ip,cs,sp,ss;
};
struct    rminfo    rmi;

extern    void cduninit(void);

void      *reqptr;
char      gcdsubunit;
short     gcdhdraddr;
short     gcdstr;
short     gcdint;
short     gcderror;
short     gcdbusy;
short     gcddone;
short     gcderrcode;

enum {
     BLACK=0,
     BLUE,
     GREEN,
     CYAN,
     RED,
     MAGENTA,
     BROWN,
     LIGHTGRAY,
     DARKGRAY,
     LIGHTBLUE,
     LIGHTGREEN,
     LIGHTCYAN,
     LIGHTRED,
     LIGHTMAGENTA,
     YELLOW,
     WHITE
};

int  ovmode,
     vmodeflag=0;

static
int  cursbot,curstop,
     cursx,cursy,
     winx1,winy1,
     winx2,winy2,
     winatr;

#pragma aux setvmode =\
 	"int 0x10",\
	parm [eax]\


clock_t   crstart,crend;     
clock_t   gameseconds;

void
startgametime()
{
#ifdef TIMERESTORE
     _dos_gettime(&dtime);
     crstart=clock();
#endif
}

void
endgametime()
{
#ifdef TIMERESTORE     
     crend=clock();
     gameseconds=crend/CLOCKS_PER_SEC;

     printf("%lu seconds expired.\n", gameseconds);

     dtime.second=( unsigned char)(gameseconds%60L);
     dtime.minute=( unsigned char)(gameseconds/60L);
     dtime.hour  =( unsigned char)(gameseconds/3600L);;
     _dos_settime(&dtime);
#endif
}


int
getvmode(void)
{

}

void
crash(char *s,...)
{
     va_list argptr;

     musicoff();
	sendlogoff();        
	uninitmultiplayers();
	uninitsb();
     cduninit();
	uninittimer();
	uninitkeys();
	uninitengine();
	uninitgroupfile();
     teksavesetup();
	setvmode(ovmode); 
     endgametime();

     va_start(argptr,s);
     vprintf(s,argptr);
     va_end(argptr);
     printf("\n");
     exit(0);
}

void
setatr(int fore,int back)
{
     winatr=(((back&0x0F)<<4)|(fore&0x0F));
}

void
setcsize(int t,int b)
{

}

void
gotoxy(int x,int y)
{

}

void
getcurs(void)
{

}

void
setregion(int x1,int y1,int x2,int y2)
{
     winx1=x1;
     winy1=y1;
     winx2=x2;
     winy2=y2;
}

void
scrollup(void)
{

}

void
scrolldn(void)
{

}

void
clrregion(int fore,int back)
{

}

void
tprintf(char *fmt,...)
{

}

void
printfat(int x,int y,char *fmt,...)
{
     char buf[80];
     va_list arglist;

     if (vmodeflag) {
          return;
     }
     va_start(arglist,fmt);
     vsprintf(buf,fmt,arglist);
     va_end(arglist);
     if (x == -1) {
          x=40-strlen(buf)/2;
     }
     else if (x == 0) {
          x=cursx;
     }
     if (y == 0) {
          y=cursy;
     }
     gotoxy(x,y);
     tprintf(buf);
}

int
criterrhndlr(unsigned deverr, unsigned errcode, unsigned *devhdr)
{

}

int
getDOSmem(int paragraphs, int *selector )
{

}

int
freeDOSmem(int selector  )
{

}

int
cdinfo(void)
{

}

int
cdioctl(int action)
{

}

struct    meminfotype {
     long      lab;
     long      mupa;
     long      mlpa;
     long      tlp;
     long      tup;
     long      fp;
     long      tpp;
     long      flp;
     long      spf;
     char      rsrvd[12];
};

void
meminfo(void)
{

}

void
dpmiinfo(void)
{

}

int
verifycd()
{

}

extern    int asmcpuid(void);    

extern    void videocheck(void);

void
tektextmode(void)
{
     int       i,dl;

     // check for game won file and delete it
     if( access("tekv.dat", 0) == 0 ) {
          remove("tekv.dat");
     }

     ovmode=getvmode();
     setvmode(0x03);
     setregion(0,0,79,24);
     clrregion(WHITE,BLACK);
     gotoxy(0,0);
     setatr(WHITE,MAGENTA);
     for (i=0 ; i < 80 ; i++) {
          tprintf(" ");
     }
     gotoxy(0,0);
     printfat(-1,0,TITLE,VERS);
     setregion(0,1,79,24);
     gotoxy(0,2);
     setatr(MAGENTA,WHITE);

     _harderr(criterrhndlr);          

    #ifdef VERIFYCPU
     switch( asmcpuid() ) {
     case 1:
          printf("TEKWAR requires 486 or better.\n");
          exit(-1);
     case 2:
          // is a 486
          break;
     default:
          break;
     }
    #endif

     installgphandler();
     videocheck();
}
