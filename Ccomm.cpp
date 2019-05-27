// Proyecto PVNFIS.EXE
// -------------------
//
// cComm.cpp
// ---------
//
// Implementaci¢n de la clase cComm
//
// Miquel de la Ossa   --- MANGO --- Octubre 2.000
//
//
// UTILIZACION:
//
//    Primero deber n modificarse los tres #defines declarados en
// el archivo "constant.h" para escoger el puerto serie.
//

#include "constant.h"
#include "cReloj.h"
#ifdef DEBUG
   #include "cFile.h"
#endif

#include <ctype.h>  // para toupper
#include <dos.h>    // para REGS
#include <time.h>
#include <stdio.h>
#include <conio.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////

COLA EnCola;

#include "cComm.h"

union REGS r;   

/////////////////////////////////////////////////////////////////////////////////////////////////////
 
//--------------------------------------------------------------------------------------------------- 

#if 0
cComm::cComm()
{
    // Constructor por defecto. No hace nada. No need to have it either
}   
#endif 
//--------------------------------------------------------------------------------------------------- 

// cComm::cComm (constructor)
// ==========================
//    Construcci¢n del objeto cComm.
//    Se inicializa el puerto serie.
//
// Par metros de entrada:
//    iPuerto            de 1 a 2 para indicar el puerto COM
//    iBaudios           de 300 a 9600 para indicar la velocidad
//    iParidad           0=Sin paridad, 1=Impar, 2=Par
//    iPalabra           7 u 8 para longitud de palabra
//    iBitsParada        1 o 2 para bits de parada
//
// Par metros de salida: ninguno.
//
cComm::cComm (const char cTipo,       // Respuesta por 'T'iempo, 'C'adena
         const UCHAR cFin,       // Fin de secuencia de respuesta
         const ULONG iTimeout,   // Timeout
         const int  iBaudios,    // velocidad en bits por segundo
         const int  iParidad,    // c¢digo de paridad
         const int  iPalabra,    // longitud de palabra
         const int  iBitsParada, // bits de parada para fin de caracter
         const int  iCOM,        // numero de puerto COM
         const int  iIRQ)        // numero de IRQ del puerto COM  
         : m_nError(0), m_cParms(0), m_bPICInicializado(FALSE), m_bIntCapturada(FALSE),
           m_iTimeout((iTimeout <= 0 ? 1 : iTimeout)), m_cTipo(toupper(cTipo)), m_cFin(cFin),
           m_iCOM(iCOM), m_iIRQ(iIRQ)
{
   //m_nError = m_cParms = 0;
   //m_bPICInicializado = FALSE;
   //m_bIntCapturada = FALSE;

   if((m_cTipo != 'T') && (m_cTipo != 'C')) {
      m_nError = 550; // Error 550: Parametro cTipo incorrecto
      return;
   }

   //m_iTimeout = (iTimeout <= 0 ? 1 : iTimeout);
   //m_cTipo = toupper(cTipo);
   //m_cFin = cFin;

   switch(m_iCOM) {
      case 0: case 1: case 2: case 3: break;
      default: m_nError = 545; return; // Error 545: COM incorrecto
   }
   
   //m_iCOM = iCOM;

   switch(m_iIRQ)
   {
      case 4: case 5: break;
      default: m_nError = 546; return; // Error 546: IRQ incorrecta
   }
   
   //m_iIRQ = iIRQ;
   
   switch(iBaudios) {
      case  300: m_cParms |= BAUD300; break;
      case 1200: m_cParms |= BAUD1200; break;
      case 2400: m_cParms |= BAUD2400; break;
      case 4800: m_cParms |= BAUD4800; break;
      case 9600: m_cParms |= BAUD9600; break;
      default: m_nError = 541; return; // Error 541: velocidad incorrecta
   }

   switch(iParidad) {
      case SIN_PARIDAD: m_cParms |= NINGUNA; break;
      case PARIDAD_IMPAR: m_cParms |= IMPAR; break;
      case PARIDAD_PAR: m_cParms |= PAR; break;
      default: m_nError = 542; return; // Error 542: paridad incorrecta
   }

   switch(iPalabra) {
      case 7: m_cParms |= LON7; break;
      case 8: m_cParms |= LON8; break;
      default: m_nError = 543; return; // Error 543: longitud palabra incorrecta
   }

   switch(iBitsParada) {
      case 1: m_cParms |= STOP1; break;
      case 2: m_cParms |= STOP2; break;
      default: m_nError = 544; return; // Error 544: bits de para incorrectos
   }
}
//--------------------------------------------------------------------------------------------------- 

// cComm::~cComm (destructor)
// ==========================
//    Destrucci¢n del objeto cComm.
//    Se deshabilita la interrupci¢n hardware y el PIC.
//
cComm::~cComm ()
{
   m_nError = 0;

   if(m_bIntCapturada) { LiberaInterrupcion(); }

   if(m_bPICInicializado) { DeshabilitaPIC(); }
}
//--------------------------------------------------------------------------------------------------- 

// cComm::GetDsr()
// ===============
//    Obtener el estado del puerto serie
//
/*int cComm::GetDsr()
{
    r.h.ah = 0x03;  // Obtener estado del puerto serie
    r.x.dx = COM;
    int86 ( 0x14, &r, &r );
    if ( (r.h.al & 0x20) == 0x20 ) return 1;
    return 0;
}*/
//---------------------------------------------------------------------------------------------


void cComm::BrushCadena(UCHAR arr[MAX_LONG_BUFFER], const int n)
{
	for(int i = 0; i < n; ++i) { if(arr[i] == COMA) { break; } }

	if(i >= n) { return; }
	if(arr[i] != COMA) { return; }

	for(int j = i; j >= 0; --j) { if(!isdigit(arr[j])) { break; } }

	++j;

	for(int k = j; arr[k] == '0' && k < i; ++k) { }
}
//---------------------------------------------------------------------------------------------

void cComm::EnviaCadena(UCHAR arCadena[MAX_LONG_BUFFER], const int nElementos, char cTipo,
                        UINT nNumCar, UCHAR cFin, const UINT bEspera, const UINT nDelay)
{
	BrushCadena(arCadena, nElementos);
	DoEnviaCadena(arCadena, nElementos,cTipo, nNumCar, cFin, bEspera, nDelay);
}
//---------------------------------------------------------------------------------------------

// cComm::EnviaCadena
// ==================
//    Envio de una cadena de caracteres por el puerto serie.
//
// Par metros de entrada:
//    char* szCadena       cadena a enviar
//    char  cTipo          Tipo de respuesta 'T'iempo, 'C'adena
//                         si NULL se respeta inicializaci¢n
//
// Par metros de salida: ninguno
//
void cComm::DoEnviaCadena(const UCHAR arCadena[MAX_LONG_BUFFER], const int nElementos, char cTipo,
                        UINT nNumCar, UCHAR cFin, const UINT bEspera, const UINT nDelay)

{
#ifdef DEBUG
   cFile*  Fichdeb   = new cFile(FICHDEB, "a+b");
#endif   

   if(nNumCar == '\0') { nNumCar = 0; }

   if((cTipo != POR_TIEMPO) && (cTipo != POR_CADENA)) { cTipo = m_cTipo; }
   
   if(!cFin) { cFin = m_cFin; }

   m_nError = EnCola.Indice = m_nNumCarRespuesta = 0; 
   
   cReloj rlj;

   const ULONG ulHoraInicial = rlj.LeerHora();

   const ULONG ulCalculo (m_iTimeout < 1 ? m_iTimeout : m_iTimeout * 1000L);
   
   //if(m_iTimeout < 1) { ulCalculo = m_iTimeout; }
   //else { ulCalculo = m_iTimeout * 1000L; }

#ifdef DEBUG
   Fichdeb->EscribirCaracter('<');
#endif

   for(int i = 0; i < nElementos; ++i) { 
   
      if(nDelay) { rlj.EsperaSegundos(nDelay/1000); } // Posible retardo

      while((inp(LSR) & 0x60) != 0x60 ) { };
      //Espera = time( &Espera );
      //while ( !GetDsr() )
      //{
      //  TiempoEsp = time( &TiempoEsp );
      //  TiempoEsp -= Espera;
      //  if ( TiempoEsp > m_iTimeout )
      //  {
      //      m_nError = 119; // Buffer lleno. temporal
      //      return;
      //  }
      //}
      //r.h.ah = 0x01;
      //r.x.dx = COM;
      //r.h.al = arCadena[i];
      //int86( 0x14, &r, &r );
      outp(THR, arCadena[i]);
#ifdef DEBUG
      Fichdeb->EscribirCaracter(arCadena[i]);
#endif
   }

   while((inp(LSR) & 0x60) != 0x60 ) { };

#ifdef DEBUG
   Fichdeb->EscribirCaracter('>');
   delete Fichdeb;
#endif

   if(!bEspera) { return; }

   while((rlj.LeerHora() - ulHoraInicial) < ulCalculo) {
       if(((cFin) && (EnCola.Cola[EnCola.Indice - 1] == cFin)) ||
            ((nNumCar) && (EnCola.Indice > (nNumCar - 1))) )
       {
          break;
       }
       else
       {
           if(m_iTimeout > 1) { rlj.EsperaSegundos(1); }
       }
   } 
   
   ObtieneRespuesta();
}
//--------------------------------------------------------------------------------------------------- 

// cComm::ObtieneRespuesta
// =======================
//    Forzamos el traslado de los datos depositados en la cola
// al miembro de datos "m_arRespuesta".
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void cComm::ObtieneRespuesta()
{
   m_nError = 0;
#ifdef DEBUG
   cFile*  Fichdeb   = new cFile(FICHDEB, "a+b");
#endif

   for(UINT i = 0; i < EnCola.Indice; ++i) {
       m_arRespuesta[i] = EnCola.Cola[i];
#ifdef DEBUG
       Fichdeb->EscribirCaracter( EnCola.Cola[i] );
#endif
   }      
   
   if(!EnCola.Indice) { m_nError = 551; }    // Error 551: No hay respuesta del dispositivo 
   
   m_nNumCarRespuesta = EnCola.Indice;
#ifdef DEBUG
   delete Fichdeb;
#endif
}
//--------------------------------------------------------------------------------------------------- 

// cComm::SerialHandler
// ====================
//    Manejador de la interrupci¢n de hardware.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void interrupt far cComm::SerialHandler(__CPPARGS)
{
   _disable();

   if(EnCola.Indice < MAX_LONG_BUFFER) {
      EnCola.Cola[EnCola.Indice] = inp(RDR);
      EnCola.Indice++;
   }
   else {
      EnCola.Cola[0] = inp(RDR);
      EnCola.Indice = 1;
   }

   _enable();

   outp(0x20, 0x20);
}
//--------------------------------------------------------------------------------------------------- 

// cComm::CtrlCHandler
// ===================
//    Manejador de la interrupci¢n para Ctrl+C.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void interrupt far cComm::CtrlCHandler(__CPPARGS)
{
   return;
}
//--------------------------------------------------------------------------------------------------- 

// cComm::CapturaInterrupcion
// ==========================
//    Capturamos la interrupci¢n de hardware y Ctrl+C.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void cComm::CapturaInterrupcion()
{
   const int iVectorSerie = m_iIRQ + 8; 
   
   m_nError = 0;

   if(!m_bIntCapturada) {
      /* Utilizamos la interrupci¢n 0x14 para inicializar el puerto serie */
      r.h.ah = 0; // funci¢n 0x00
      r.x.dx = m_iCOM - 1;
      r.h.al = m_cParms;

      m_pOld_Serial_Handler = _dos_getvect(iVectorSerie);   
      
      _dos_setvect(iVectorSerie, SerialHandler);  
      
      m_pOld_CtrlC_Handler = _dos_getvect(CTRL_C); 
      
      _dos_setvect(CTRL_C, CtrlCHandler);

      int86 (0x14, &r, &r); 
      
      m_bIntCapturada = TRUE;
   }
   else { m_nError = 549; } // Error 549: interrupci¢n ya capturada
}
//--------------------------------------------------------------------------------------------------- 

// cComm::LiberaInterrupcion
// =========================
//    Liberamos las interrupciones de hardware y Ctrl+C.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void cComm::LiberaInterrupcion()
{
   const int iVectorSerie = m_iIRQ + 8;
   
   m_nError = 0;

   if(m_bIntCapturada) {
      _dos_setvect(iVectorSerie, m_pOld_Serial_Handler);
      _dos_setvect(CTRL_C, m_pOld_CtrlC_Handler);

      m_bIntCapturada = FALSE;
   }
   else { m_nError = 548; } // Error 8: Interrupci¢n ya liberada
}
//--------------------------------------------------------------------------------------------------- 

// cComm::HabilitaPIC
// ==================
//    Habilitamos el PIC para provocar interrupciones.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void cComm::HabilitaPIC()
{
   int nByte;
   m_nError = 0;

   if(!m_bPICInicializado) { 
   
      outp(0x21, 0x00); // enable all IRQ's
      outp(LCR, inp(IIR) & 0x7f); // set DLAB bit on 0 (normal)
      outp(IER, 0x01); // enable data-avail IRQ at UART
      outp(LCR, 0x13); // set N81
      outp(MCR, 0x0b); // set RTS and DTR
      
      nByte = inp(LSR); // clear pending status
      nByte = inp(RDR); // clear pending status
      nByte = inp(IIR); // clear pending status
      nByte = inp(MSR); // clear pending status
      
      outp(0x20, 0x20);

      m_bPICInicializado = TRUE;
   }
   else { m_nError = 547; } // Error 547: PIC ya inicializado
}
//--------------------------------------------------------------------------------------------------- 

// cComm::DeshabilitaPIC
// =====================
//    Deshabilitamos el PIC para no capturar interrupciones.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void cComm::DeshabilitaPIC()
{
   m_nError = 0;

   if(m_bPICInicializado) {
      outp(0x21, inp(0x21) & 0x10);
      outp(LCR, inp( LCR ) & 0x7f);
      outp(IER, 0x00);
      outp(MCR, 0x00);
      outp(0x20, 0x20);

      m_bPICInicializado = FALSE;
   }
   else { m_nError = 546; } // Error 546 PIC ya deshabilitado
} 
//--------------------------------------------------------------------------------------------------- 

// cComm::GetNumError
// ==================
//    Obtener el £ltimo n£mero de error producido.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
inline int cComm::GetNumError() const
{
   return m_nError;
} 
//--------------------------------------------------------------------------------------------------- 
//--------------------------------------------------------------------------------------------------- 
//--------------------------------------------------------------------------------------------------- 
//--------------------------------------------------------------------------------------------------- 

