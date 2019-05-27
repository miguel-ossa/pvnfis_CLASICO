// Proyecto PVNFIS.EXE
// -------------------
//
// cReloj.cpp
// ----------
//
// Implementaci¢n de la clase cReloj para funciones horarias.
//
// Miquel de la Ossa   --- MANGO --- Octubre 2.000
//

#include <dos.h>

#include "cReloj.h"

cReloj::cReloj()
{
   // Parece que no hay nada que hacer
}

unsigned long cReloj::LeerHora()
{
   unsigned long ulMilisegundos;
   union REGS r;

   r.h.ah = 0x2c;
   int86( 0x21, &r, &r );

   ulMilisegundos = (long)r.h.dl * 10;       // dl: centesimas
   ulMilisegundos += (long)r.h.dh * 1000;    // dh:segundos
   ulMilisegundos += (long)r.h.cl * 60000L;  // cl:minutos
   ulMilisegundos += (long)r.h.ch * 3600000L;// ch:horas
   return (ulMilisegundos);
}

void cReloj::EsperaSegundos( float fSegundos )
{
   unsigned long ulLapso;

   ulLapso = LeerHora() + (unsigned long)(fSegundos * 1000);
   while (LeerHora() < ulLapso);
}