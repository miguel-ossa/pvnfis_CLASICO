#include "constant.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

char* abc = "123456789a123456789b123456789c123456789d";
char* def = "123456789e123456789f123456789g123456789h";
char* ghi = "123456789i123456789j123456789k123456789l";

static int GetDsr()
{
    union REGS r;
    r.h.ah = 0x03;
    r.x.dx = 0x0000;
    int86( 0x14, &r, &r);
    if ( (r.h.al & 0x20) == 0x20 ) return 1;
    return 0;
}

void main()
{
    union REGS r;
    int i;

    outp(THR, LF);

    printf("THR %04x, LSR %04x\n", THR, LSR);

    for (i=0; i<41; i++)
    {
      while (!GetDsr()) {}
      if (i == 40)
      {
        r.h.ah = 0x01;
        r.x.dx = 0x0000;
        r.h.al = LF;
        int86( 0x14, &r, &r);
        if ( (r.h.ah & 0x9e) == 0x00 )
        {
           printf("Error fatal %02x\n", r.h.ah);
           exit (-1);
        }
      }
      else
      {
        r.h.ah = 0x01;
        r.x.dx = 0x0000;
        r.h.al = *abc;
        int86( 0x14, &r, &r);
        if ( (r.h.ah & 0x9e) == 0x00 )
        {
           printf("Error fatal %02x\n", r.h.ah);
           exit (-1);
        }
        abc++;
      }
    }
    for (i=0; i<41; i++)
    {
      while (!GetDsr()) {}
      if (i == 40)
      {
        r.h.ah = 0x01;
        r.x.dx = 0x0000;
        r.h.al = LF;
        int86( 0x14, &r, &r);
        if ( (r.h.ah & 0x9e) == 0x00 )
        {
           printf("Error fatal %02x\n", r.h.ah);
           exit (-1);
        }
      }
      else
      {
        r.h.ah = 0x01;
        r.x.dx = 0x0000;
        r.h.al = *def;
        int86( 0x14, &r, &r);
        if ( (r.h.ah & 0x9e) == 0x00 )
        {
           printf("Error fatal %02x\n", r.h.ah);
           exit (-1);
        }
        def++;
      }
    }
    for (i=0; i<41; i++)
    {
      while (!GetDsr()) {}
      if (i == 40)
      {
        r.h.ah = 0x01;
        r.x.dx = 0x0000;
        r.h.al = LF;
        int86( 0x14, &r, &r);
        if ( (r.h.ah & 0x9e) == 0x00 )
        {
           printf("Error fatal %02x\n", r.h.ah);
           exit (-1);
        }
      }
      else
      {
        r.h.ah = 0x01;
        r.x.dx = 0x0000;
        r.h.al = *ghi;
        int86( 0x14, &r, &r);
        if ( (r.h.ah & 0x9e) == 0x00 )
        {
           printf("Error fatal %02x\n", r.h.ah);
           exit (-1);
        }
        ghi++;
      }
    }
}