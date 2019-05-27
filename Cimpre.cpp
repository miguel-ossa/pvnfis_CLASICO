// Proyecto PVNFIS.EXE
// -------------------
//
// cImpre.cpp
// ----------
//
// Implementacion de la clase cImpre
//
// Miquel de la Ossa   --- MANGO --- Octubre 2.000
//

#include "constant.h"
#include "cImpre.h"
#include "cFile.h"
#include "cReloj.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <ctype.h>

#ifdef DEBUG
   #include <conio.h>
#endif

// Tabla de paises
char g_Paises[][5] = { POLONIA, POLONIA2, HUNGRIA, TURQUIA, TURQUIA2, PORTUGAL, BRASIL, VACIO };
char g_Comandos[][7] = { "TESTEO",
                         "NUMCAJ",
                         "NUMTIQ",
                         "FECHAF",
                         "HORAFI",
                         "NUMBOL",
                         "FECHAB",
                         "HORABO",
                         "TOTALB",
                         "CABECB",
                         "LINDEB",
                         "PAGOFI",
                         "CIERRZ",
                         "INFONF",
                         "CANCEL",
                         "PIEFIS",
                         "TESTEA",
                         "TESTHW",
                         "ABRECA",
                         "NUMERO",
                         "DISPLY",
                         "TRASPA",
                         "TEXLIB",
                         "LIBERA",
                         "INICIA",
                         "LECTUX",
                          VACIO };

// cImpre::cImpre
// ==============
//    Constructor por defecto de la clase 'cComm'.
//    Pasa par metros de inicializaci¢n a la clase antecesora.
//    El timeout solamente ser  utilizado si la impresora deja de
// contestar en una operaci¢n en la que se espera respuesta.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
//--------------------------------------------------------------------------------------------------

cImpre::cImpre(int pCom, int pIrq) : cComm(POR_TIEMPO, END, 5, 9600, 0, 8, 1, pCom, pIrq),
  m_nIndPais(-1), m_cClaseError(ERROR_PVN), m_cSigno(NULL)
{
    m_szPais[0] = m_szComando[0] = m_szParametros[0] = NULL;
    m_szRespuesta[0] = m_arCadenaSalida[0] = m_szTotal[0] = NULL;
    m_nNumCarRespuesta = 0;
    m_bBoletaAbierta = m_bAbono = m_bTraspaso = FALSE;
    m_bPrimeraVez = m_bEuros = TRUE;
    m_nError = m_nEstado = m_nLineas = 0;
}
//--------------------------------------------------------------------------------------------------

// cImpre::~cImpre
// ===============
//   Destructor. No hace nada; es el destructor de 'cComm' el que
// se encarga de deshabilitar las interrupciones.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
cImpre::~cImpre()
{
    // No hace nada
}
//--------------------------------------------------------------------------------------------------

void cImpre::HabilitaInterrupciones()
{
    CapturaInterrupcion();
    HabilitaPIC();
}
//--------------------------------------------------------------------------------------------------

// cImpre::GetError
// ================
//    Devuelve el c¢digo de error actual.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida:
//    int m_nError  C¢digo de error
//
//--------------------------------------------------------------------------------------------------

// cImpre::ComponeComando
// ======================
//    Construye el comando de respuesta a COBOL.
//
// Par metros de entrada:
//    char szComando[7]  Literal utilizado para nombre de comando.
//
// Par metros de salida: ninguno.
//
void cImpre::ComponeComando(const char* szComando)
{
    m_szRespuesta[0] = INICIO_MNG;
    m_szRespuesta[1] = SEPARADOR;
    m_szRespuesta[2] = g_Paises[m_nIndPais][0]; // pais
    m_szRespuesta[3] = g_Paises[m_nIndPais][1];
    m_szRespuesta[4] = g_Paises[m_nIndPais][2];
    m_szRespuesta[5] = SEPARADOR;
    m_szRespuesta[6] = NULL;
    strcat(m_szRespuesta, szComando);
    m_szRespuesta[12] = SEPARADOR;
}
//--------------------------------------------------------------------------------------------------

// cImpre::ComponeError
// ====================
//    Utilizado por la funcion 'Testeo' para preparar la respuesta a
// COBOL sobre el £ltimo error hallado.
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void cImpre::ComponeError()
{
    UCHAR arsNumero3[4];

    ComponeComando("TESTEO");
    m_szRespuesta[13] = m_cClaseError;
    m_szRespuesta[14] = SEPARADOR;

    sprintf((char*)arsNumero3, "%03d", m_nError);
    m_szRespuesta[15] = arsNumero3[0];
    m_szRespuesta[16] = arsNumero3[1];
    m_szRespuesta[17] = arsNumero3[2];
    m_szRespuesta[18] = SEPARADOR;

    sprintf((char*)arsNumero3, "%03d", m_nEstado);
    m_szRespuesta[19] = arsNumero3[0];
    m_szRespuesta[20] = arsNumero3[1];
    m_szRespuesta[21] = arsNumero3[2];

    m_szRespuesta[22] = SEPARADOR;
    m_szRespuesta[23] = FIN_MNG;
    m_szRespuesta[24] = NULL;
}
//---------------------------------------------------------------------------------------------------

// cImpre::GetBoletaAbierta  (BOLETA = TICKET)
// ========================
//    Retorna si la boleta estaba abierta o no. (se ha empezado a imprimir el ticket, pero aun no se ha terminado)
//
// Par metros de entrada: ninguno.
//
// Par metros de salida:
//    int  m_bBoletaAbierta  TRUE o FALSE
//
#if 0  // inlined and moved to *.h
int cImpre::GetBoletaAbierta()
{
    return m_bBoletaAbierta;
}
#endif
//---------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------
// Comandos de Impresora
// -----------------------------------------------------------------

//
// TURQUIA
// ==================
//
void cImpre::TURQUIA_Reset()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = '@';
    m_arCadenaSalida[2] = NULL;

    EnviaCadena(m_arCadenaSalida, 2, NULL, NULL, NULL, FALSE, NULL);
} 
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_LetraPeque()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = '!';
    m_arCadenaSalida[2] = 1;
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_AreaTrabajo()
{
    m_arCadenaSalida[0] = GS;
    m_arCadenaSalida[1] = 'W';
    m_arCadenaSalida[2] = 255;
    m_arCadenaSalida[3] = 3;
    m_arCadenaSalida[4] = NULL;

    EnviaCadena(m_arCadenaSalida, 4, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_DeshabilitaBotones()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = 'c';
    m_arCadenaSalida[2] = 5;
    m_arCadenaSalida[3] = 0x01;
    m_arCadenaSalida[4] = NULL;

    EnviaCadena(m_arCadenaSalida, 4, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_PaperStatus()
{
    m_arCadenaSalida[0] = DLE;
    m_arCadenaSalida[1] = EOT;
    m_arCadenaSalida[2] = 3;  // Función 3: transmit error status
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 1, NULL, TRUE, NULL);

    m_nError = m_arRespuesta[0];
    if(cComm::m_nError) {
       m_nError = cComm::m_nError; // Probablemente error de comunicación
       return;
    }
    else if(m_nError) {
        m_nError = ((m_nError & 0x04) == 0x04) ? 99 : 0; // Error 99: error de hardware
        if(m_nError) {
           m_cClaseError = '7';
           return;
        }
    }

    m_arCadenaSalida[0] = DLE;
    m_arCadenaSalida[1] = EOT;
    m_arCadenaSalida[2] = 1; // Función 1: Transmit printer status
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 1, NULL, TRUE, NULL);

    m_nError = m_arRespuesta[0];
    if(cComm::m_nError) {
       m_nError = cComm::m_nError; // Probablemente error de comunicación
       return;
    }
    else if(m_nError) {
        m_nError = ((m_nError & 0x08) == 0x08 ? 96 : 0); // Error 96: tapa abierta
        if(m_nError) {
           m_cClaseError = '7';
           return;
        }
    }

    m_arCadenaSalida[0] = GS;
    m_arCadenaSalida[1] = 'r';
    m_arCadenaSalida[2] = 49; // Transmit paper sensor status
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 1, NULL, TRUE, NULL);

    m_nError = m_arRespuesta[0];
    if(cComm::m_nError) {
       m_nError = cComm::m_nError; // Probablemente error de comunicación
       return;
    }
    else if(m_nError) {
        m_nError << 1; // Bits 4 y 5 indican BOF y TOF
        m_nError = ((m_nError | 0x60) == 0x60 ? 95 : 0); // Error 95: sin papel
        
		if(m_nError) { m_cClaseError = '7'; }
    }
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Testeo()
{
    TURQUIA_Reset();
    TURQUIA_LetraPeque();
    TURQUIA_AreaTrabajo();
    TURQUIA_DeshabilitaBotones();
    // Vaciar el buffer
    m_arCadenaSalida[0] = LF;
    m_arCadenaSalida[1] = NULL;
    EnviaCadena(m_arCadenaSalida, 1, NULL, NULL, NULL, FALSE, NULL);
    TURQUIA_PaperStatus();
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Libera() // Libera el papel
{
    // Liberar el papel
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = 0x71;
    m_arCadenaSalida[2] = NULL;
    EnviaCadena(m_arCadenaSalida, 2, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Cabecb()
{
   UCHAR arNumero2[3];
   char  c;
   UINT  i;

   // Obtener parametro PP
   arNumero2[0] = m_szParametros[0];
   arNumero2[1] = m_szParametros[1];
   arNumero2[2] = NULL;

   const UINT nNumero = atoi((char*)arNumero2);

   if(nNumero != 20 && nNumero != 22 && nNumero != 24 && nNumero != 30 &&
      nNumero != 32 && nNumero != 34 && nNumero != 36
     )
   {
      return;  
   }

   if(m_bAbono && nNumero != 20) { return; }

   if(nNumero >= 32 && nNumero <= 36 && !m_bTraspaso) { return; }

   if(m_bTraspaso && (nNumero < 32 || nNumero > 36)) { return; }

   if(nNumero == 20) {
      m_arCadenaSalida[0] = LF;
      m_arCadenaSalida[1] = NULL;
      EnviaCadena(m_arCadenaSalida, 1, NULL, NULL, NULL, FALSE, 10);

      m_arCadenaSalida[0] = ESC;
      m_arCadenaSalida[1] = 0x64;

      if(m_bAbono) { m_arCadenaSalida[2] = 4; }
      else { m_arCadenaSalida[2] = 3; }

      m_arCadenaSalida[3] = NULL;
      EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, 10);
   }

   if(m_bTraspaso) { i = 4; }
   else { i = 32; }

   for(UINT j = 0; j < i; ++j) { m_arCadenaSalida[j] = BLANCO; }

   if((nNumero == 20) || m_bTraspaso) {
      if(m_bAbono) { i = 37; }
      else {
          if(m_bTraspaso) { i = 0; }
          else { i = 58; }
      }

      for(; j < i; ++j) { m_arCadenaSalida[j] = BLANCO; }

      if(m_bTraspaso && nNumero == 30) { c = BLANCO; }
      else { c = SEPARADOR; }

      for(i = 3; m_szParametros[i] != c; ++i, ++j) { m_arCadenaSalida[j] = m_szParametros[i]; }

      if(!m_bTraspaso) { j -= 10; }
   }
   else {
      for(i = 3; m_szParametros[i] != SEPARADOR; ++i, ++j) { m_arCadenaSalida[j] = m_szParametros[i]; }
   }

   m_arCadenaSalida[j++] = LF;;
   m_arCadenaSalida[j] = NULL;

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);

   if((nNumero == 20 || nNumero == 24) || (m_bTraspaso && nNumero == 36)) {
      m_arCadenaSalida[0] = ESC;
      m_arCadenaSalida[1] = 0x64;
      if(nNumero == 20) {
         if(m_bAbono) { m_arCadenaSalida[2] = 3; }
         else { m_arCadenaSalida[2] = 2; }
      }
      else
      {
          if(m_bTraspaso) { m_arCadenaSalida[2] = 2; }
          else { m_arCadenaSalida[2] = 3; }
      }
      m_arCadenaSalida[3] = NULL;
      EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, 10);
   }
}
//---------------------------------------------------------------------------------------------------
#if 1
void cImpre::TURQUIA_Lindeb() // NEW !!! Linea detalle de la operacion
{
   UINT i, bValor, nPuntos, nCiclos;

   if(m_bAbono) { i = 1; ++m_nLineas; }
   else { i = 30; }

   for(UINT j = 0; j < i; ++j) { m_arCadenaSalida[j] = BLANCO; }

   if(m_bAbono) { nCiclos = 25; }
   else { nCiclos = 17; }

   // Descripcion
   for(i = 17, nPuntos = 0; nPuntos < nCiclos; ++nPuntos, ++j, ++i) {
	   m_arCadenaSalida[j] = m_szParametros[i];
   }

   if(m_bAbono) { nPuntos = 10; }
   else { nPuntos = 2; }

   for(i = 0; i < nPuntos; ++i, ++j) {
       m_arCadenaSalida[j] = BLANCO;
   }

   // Cantidad
   m_arCadenaSalida[j++] = m_szParametros[61];

   // Porcentaje VAT
   if(m_bAbono) { nPuntos = 7; }
   else { nPuntos = 2; }

   for(nCiclos = 0; nCiclos < nPuntos; ++nCiclos, ++j) {
       m_arCadenaSalida[j] = BLANCO;
   }

   if(!m_bAbono) {
		m_arCadenaSalida[j++] = '%';
		m_arCadenaSalida[j++] = m_szParametros[129];
		m_arCadenaSalida[j++] = m_szParametros[130];
		m_arCadenaSalida[j++] = BLANCO;
   }

   // Precio
   bValor = nPuntos = nCiclos = 0;
   for(i = 66; m_szParametros[i] != GOMA; ++i, ++j) {
      if(!bValor) {
         if(m_szParametros[i] != '0') {
            bValor = 1;
            m_arCadenaSalida[j] = m_szParametros[i];
         }
         else { m_arCadenaSalida[j] = BLANCO; }
      }
      else {
         m_arCadenaSalida[j] = m_szParametros[i];
      }

      ++nPuntos;

      if((nPuntos == 3) && (nCiclos < 3)) {

         nPuntos = 0; ++nCiclos;

         if(bValor) { m_arCadenaSalida[++j] = '.'; }
         else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
      }
   }
   if(m_szParametros[i] == GOMA) {
	   m_arCadenaSalida[j++] = m_szParametros[i++];
	   for(int k = 0; k < 2; ++k, ++i, ++j) {
		  if(!bValor) {
			 if(m_szParametros[i] != '0') {
				bValor = 1;
				m_arCadenaSalida[j] = m_szParametros[i];
			 }
			 else { m_arCadenaSalida[j] = BLANCO; }
		  }
		  else {
			 m_arCadenaSalida[j] = m_szParametros[i];
		  }

		  ++nPuntos;

		  if((nPuntos == 3) && (nCiclos < 3)) {

			 nPuntos = 0; ++nCiclos;

			 if(bValor) { m_arCadenaSalida[++j] = '.'; }
			 else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
		  }
	   }
   }

   if(m_bAbono) { nPuntos = 6; }
   else { nPuntos = 2; }

   for(nCiclos = 0; nCiclos < nPuntos; ++nCiclos, ++j) {
      m_arCadenaSalida[j] = BLANCO;
   }

   // Importe linea
   bValor = nPuntos = nCiclos = 0;

   for(i = 140; m_szParametros[i] != COMA; ++i, ++j) {
      if(!bValor) {
         if(m_szParametros[i] != '0') {
            bValor = 1;
            m_arCadenaSalida[j] = m_szParametros[i];
         }
         else { m_arCadenaSalida[j] = BLANCO; }
      }
      else {
         m_arCadenaSalida[j] = m_szParametros[i];
      }

      ++nPuntos;

      if(nPuntos == 3 && nCiclos < 3) {
         nPuntos = 0; ++nCiclos;
         if(bValor) { m_arCadenaSalida[++j] = '.'; }
         else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
      }
   }
	if(m_szParametros[i] == GOMA) {
		m_arCadenaSalida[j++] = m_szParametros[i++];
		for(int k = 0; k < 2; ++k, ++i, ++j) {
			if(!bValor) {
				if(m_szParametros[i] != '0') {
					bValor = 1;
					m_arCadenaSalida[j] = m_szParametros[i];
				}
				else { m_arCadenaSalida[j] = BLANCO; }
			}
			else {
			    m_arCadenaSalida[j] = m_szParametros[i];
			}

			++nPuntos;

			if(nPuntos == 3 && nCiclos < 3) {
				nPuntos = 0; ++nCiclos;
				if(bValor) { m_arCadenaSalida[++j] = '.'; }
				else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
			}
		}
	}

   m_arCadenaSalida[j++] = LF;
   m_arCadenaSalida[j++] = NULL;

   char* q = (char *)m_arCadenaSalida;  
   
   const char* spaces = "   ";
   const int LS = strlen(spaces); 
   
   char* pr = q; 
   while(pr = strstr(pr, " , "), pr != NULL) {
	   *pr++ = '0'; *++pr = '0';
   } 
   pr = q;
   while(pr = strstr(pr, " ,"), pr != NULL) {
	   *pr++ = '0';
   }     
   
   const unsigned int JMAX = 87; 

   for(int ii = 0; ii < JMAX && q[ii] == ' '; ++ii) { }

   if(ii < 10) {//devoluciones
       char* p = strstr(q, spaces);   
       
	   for(unsigned int mm = j - 1; mm >= 0; --mm) { if(isdigit(q[mm]) || q[mm] == ',') { break; } }
	   if(p != NULL) {
		   char* pp = p; 
		   char* ppp = p;
		   do {  
			   ppp = p;
			   p = pp;
			   while(*pp == ' ') { ++pp; }
			   pp = strstr(pp, spaces);
		   } while(pp != NULL); 
		   
		   const unsigned int dj = mm - 80;
		   
		   for(int k = (p - q) + (LS - 1*dj), l = (p - q) + LS; q[l]; ++k, ++l) {
			   q[k] = q[l];
		   }  
		   m_arCadenaSalida[(j -= 1*dj) - 1] = NULL; m_arCadenaSalida[j - 2] = LF;           
	   }
	   p = strstr(q, spaces);
	   if(p != NULL) {
		   char* pp = p; 
		   char* ppp = p;
		   do {
			   ppp = p;
			   p = pp;
			   while(*pp == ' ') { ++pp; }
			   pp = strstr(pp, spaces);
		   } while(pp != NULL);
		   //printf("\n(p - q) = [%ld], *(p - 1) = {%c}, *(ppp - 1) = {%c}", (unsigned long)(p - q), *(p - 1), *(ppp - 1));getchar();
		   if((p - q) <= 64) {
			   for(unsigned int jj = 0; p != ppp; ++jj) { 
				   q[64 - jj] = *--p;
			   }
		   }
	   }


	   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
	   return;
   }

   
   for(unsigned int jj = j; q[jj] == ' ' || q[jj] == '\0' || q[jj] == '\r' || q[jj] == '\n'; --jj) { } 

   if(jj > JMAX) {  
	   const unsigned int dj = ((jj % 2 != 1 ? 1 + jj : jj) - JMAX)/2;         
       char* p = strstr(q, spaces); 
       if(p != NULL) {
		   char* pp = p; 
		   char* ppp = p;
		   do {
			   ppp = p;
			   p = pp;
			   while(*pp == ' ') { ++pp; }
			   pp = strstr(pp, spaces);
		   } while(pp != NULL);
		   for(int k = (p - q) + (LS - dj), l = (p - q) + LS; q[l]; ++k, ++l) {
			   q[k] = q[l];
		   }  
		   m_arCadenaSalida[(j -= dj) - 1] = NULL; m_arCadenaSalida[j - 2] = LF;
		   p = ppp;
		   for(k = (p - q) + (LS - dj), l = (p - q) + LS; q[l]; ++k, ++l) {
			   q[k] = q[l];
		   } 
		   m_arCadenaSalida[(j -= dj) - 1] = NULL; m_arCadenaSalida[j - 2] = LF;

       }
   } 

   for(jj = 0; q[jj] == ' ' || q[jj] == '\0' || q[jj] == '\r' || q[jj] == '\n'; ++jj) { } 

   const unsigned int JMIN = 28; 
   if(jj < JMIN) { 
	   const unsigned int dj = (JMIN - (jj % 2 != 0 ? 1 + jj : jj))/2;  
       char* p = strstr(q, spaces); 
       if(p != NULL) {
		   char* pp = p; 
		   char* ppp = p;
		   do {
			   ppp = p;
			   p = pp;
			   while(*pp == ' ') { ++pp; }
			   pp = strstr(pp, spaces);
		   } while(pp != NULL);
		   for(int k = (p - q) + (LS - dj), l = (p - q) + LS; q[l]; ++k, ++l) {
			   q[k] = q[l];
		   }  
		   m_arCadenaSalida[(j -= dj) - 1] = NULL; m_arCadenaSalida[j - 2] = LF;
		   p = ppp;
		   for(k = (p - q) + (LS - dj), l = (p - q) + LS; q[l]; ++k, ++l) {
			   q[k] = q[l];
		   } 
		   m_arCadenaSalida[(j -= dj) - 1] = NULL; m_arCadenaSalida[j - 2] = LF;
  		   
		   char* qq = new char(j - jj + 1);
		   if(qq != NULL) {
			   for(unsigned int kk = jj, ll = 0; kk < j; ++kk, ++ll) { qq[ll] = q[kk]; }
			   for(kk = jj; kk < JMIN; ++kk) { q[kk] = ' '; }
			   for(kk = JMIN, ll = 0; qq[ll]; ++kk, ++ll) { q[kk] = qq[ll]; }
			   delete [] qq; qq = NULL;
		   }


       }
   }
   else if(jj > JMIN) { 
	   const unsigned int dj = jj - JMIN;  	   
       char* p = strstr(q, spaces); 
       if(p != NULL) {
		   char* pp = p; 
		   char* ppp = p;
		   do {
			   ppp = p;
			   p = pp;
			   while(*pp == ' ') { ++pp; }
			   pp = strstr(pp, spaces);
		   } while(pp != NULL);
		   p = ppp;
		   for(char *qc = q + JMIN, *pc = q + jj; (p - pc) > LS; ++qc, ++pc) {
			   *qc = *pc;
		   }
       }
   } 
   
   char* p = strstr(q, spaces); 
   const int dj = 1;
   if(p != NULL) {
	   char* pp = p; 
	   char* ppp = p;
	   do {
		   ppp = p;
		   p = pp;
		   while(*pp == ' ') { ++pp; }
		   pp = strstr(pp, spaces);
	   } while(pp != NULL);
	   for(int k = (p - q) + (LS - dj), l = (p - q) + LS; q[l]; ++k, ++l) {
		   q[k] = q[l];
	   }  
	   m_arCadenaSalida[(j -= dj) - 1] = NULL; m_arCadenaSalida[j - 2] = LF;
   }
   
   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);

}//new
//---------------------------------------------------------------------------------------------------
#else
void cImpre::TURQUIA_Lindeb() // OLD !!! Linea detalle de la operacion
{
   UINT i, bValor, nPuntos, nCiclos;

   if(m_bAbono) { i = 1; ++m_nLineas; }
   else { i = 30; }

   for(UINT j = 0; j < i; ++j) { m_arCadenaSalida[j] = BLANCO; }

   if(m_bAbono) { nCiclos = 25; }
   else { nCiclos = 17; }

   // Descripcion
   for(i = 17, nPuntos = 0; nPuntos < nCiclos; ++nPuntos, ++j, ++i) {
	   m_arCadenaSalida[j] = m_szParametros[i];
   }

   if(m_bAbono) { nPuntos = 10; }
   else { nPuntos = 2; }

   for(i = 0; i < nPuntos; ++i, ++j) {
       m_arCadenaSalida[j] = BLANCO;
   }

   // Cantidad
   m_arCadenaSalida[j++] = m_szParametros[61];

   // Porcentaje VAT
   if(m_bAbono) { nPuntos = 7; }
   else { nPuntos = 2; }

   for(nCiclos = 0; nCiclos < nPuntos; ++nCiclos, ++j) {
       m_arCadenaSalida[j] = BLANCO;
   }

   if(!m_bAbono) {
		m_arCadenaSalida[j++] = '%';
		m_arCadenaSalida[j++] = m_szParametros[129];
		m_arCadenaSalida[j++] = m_szParametros[130];
		m_arCadenaSalida[j++] = BLANCO;
   }

   // Precio
   bValor = nPuntos = nCiclos = 0;
   for(i = 66; m_szParametros[i] != GOMA; ++i, ++j) {
      if(!bValor) {
         if(m_szParametros[i] != '0') {
            bValor = 1;
            m_arCadenaSalida[j] = m_szParametros[i];
         }
         else { m_arCadenaSalida[j] = BLANCO; }
      }
      else {
         m_arCadenaSalida[j] = m_szParametros[i];
      }

      ++nPuntos;

      if((nPuntos == 3) && (nCiclos < 3)) {

         nPuntos = 0; ++nCiclos;

         if(bValor) { m_arCadenaSalida[++j] = '.'; }
         else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
      }
   }
   if(m_szParametros[i] == GOMA) {
	   m_arCadenaSalida[j++] = m_szParametros[i++];
	   for(int k = 0; k < 2; ++k, ++i, ++j) {
		  if(!bValor) {
			 if(m_szParametros[i] != '0') {
				bValor = 1;
				m_arCadenaSalida[j] = m_szParametros[i];
			 }
			 else { m_arCadenaSalida[j] = BLANCO; }
		  }
		  else {
			 m_arCadenaSalida[j] = m_szParametros[i];
		  }

		  ++nPuntos;

		  if((nPuntos == 3) && (nCiclos < 3)) {

			 nPuntos = 0; ++nCiclos;

			 if(bValor) { m_arCadenaSalida[++j] = '.'; }
			 else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
		  }
	   }
   }

   if(m_bAbono) { nPuntos = 6; }
   else { nPuntos = 2; }

   for(nCiclos = 0; nCiclos < nPuntos; ++nCiclos, ++j) {
      m_arCadenaSalida[j] = BLANCO;
   }

   // Importe linea
   bValor = nPuntos = nCiclos = 0;

   for(i = 140; m_szParametros[i] != COMA; ++i, ++j) {
      if(!bValor) {
         if(m_szParametros[i] != '0') {
            bValor = 1;
            m_arCadenaSalida[j] = m_szParametros[i];
         }
         else { m_arCadenaSalida[j] = BLANCO; }
      }
      else {
         m_arCadenaSalida[j] = m_szParametros[i];
      }

      ++nPuntos;

      if(nPuntos == 3 && nCiclos < 3) {
         nPuntos = 0; ++nCiclos;
         if(bValor) { m_arCadenaSalida[++j] = '.'; }
         else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
      }
   }
	if(m_szParametros[i] == GOMA) {
		m_arCadenaSalida[j++] = m_szParametros[i++];
		for(int k = 0; k < 2; ++k, ++i, ++j) {
			if(!bValor) {
				if(m_szParametros[i] != '0') {
					bValor = 1;
					m_arCadenaSalida[j] = m_szParametros[i];
				}
				else { m_arCadenaSalida[j] = BLANCO; }
			}
			else {
			    m_arCadenaSalida[j] = m_szParametros[i];
			}

			++nPuntos;

			if(nPuntos == 3 && nCiclos < 3) {
				nPuntos = 0; ++nCiclos;
				if(bValor) { m_arCadenaSalida[++j] = '.'; }
				else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
			}
		}
	}

   m_arCadenaSalida[j++] = LF;
   m_arCadenaSalida[j++] = NULL;

   char* q = (char *)m_arCadenaSalida;
   char* p = strstr(q, "        ");
   if(p != NULL) {
	   for(int k = (p - q) + 3, l = (p - q) + 8; q[l]; ++k, ++l) {
		   q[k] = q[l];
	   }
	   m_arCadenaSalida[k] = m_arCadenaSalida[l];
	   j = k + 1;
	   //for(; k < j; ++k) { m_arCadenaSalida[k] = '\0'; }
   }

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);

} //OLD !!!
#endif//#if 0
//---------------------------------------------------------------------------------------------------

//
// Imprimir el total en letras (para PIEFIS)
//
void cImpre::TURQUIA_Implet()
{
   BOOL bPrimeraVez = TRUE;
   int  nTope, j;
   char* szImporte = NULL;

   for(int i = 0; m_szTotal[i] != NULL; i++) {
      if(bPrimeraVez) {
         if(m_szTotal[i] == '0') { m_szTotal[i] = PUNTO; }
         else { bPrimeraVez = FALSE; }
      }
   }

   //printf("m_szTotal = {%s}", m_szTotal);getchar();

   spawnl(P_WAIT, "PVNLET.exe", "PVNLET.exe", g_Paises[m_nIndPais], m_szTotal, NULL); // Ejecutamos PVNLET.EXE

   cFile* pFichlet = new cFile("PVNLET.TXT", "rb"); // Obtenemos el importe en letras
   if(pFichlet != NULL && pFichlet->LeerSigLinea() != EOF) { szImporte = pFichlet->GetLinea(); }
   delete pFichlet; pFichlet = NULL;

   if(m_bAbono) {
      m_arCadenaSalida[0] = ESC;
      m_arCadenaSalida[1] = 0x64;
      m_arCadenaSalida[2] = 3;
      m_arCadenaSalida[3] = NULL;
      EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, 10);

      for(j = 0; j < 12; ++j) { m_arCadenaSalida[j] = BLANCO; }
         nTope = 50;
   }
   else
   {
      for(j = 0, i = 0; i < 28; ++i, ++j) { m_arCadenaSalida[j] = BLANCO; }
      nTope = 60;
   }

   for(i = 0; *szImporte != NULL && i < nTope; ++j, ++i) {
      m_arCadenaSalida[j] = *szImporte++;
   }

   m_arCadenaSalida[j++] = LF;
   m_arCadenaSalida[j] = NULL;

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Piefis() // El pie de la operacion
{
   UCHAR arNumero2[3];
   UINT  i, j = 0;

   // Obtener parametro PP
   arNumero2[0] = m_szParametros[0];
   arNumero2[1] = m_szParametros[1];
   arNumero2[2] = NULL;

   const UINT nNumero = atoi((char*)arNumero2);

   if(nNumero == 99) {
      TURQUIA_Expulsar();
      return;
   }

   if(m_bTraspaso) { return; }

   if(m_bAbono) {

      if((nNumero < 30 || nNumero > 42 || nNumero == 36) && nNumero != 14) { return; }

      if(nNumero == 14 || nNumero == 34 || nNumero == 38) {
         m_arCadenaSalida[0] = ESC; // Saltar n lineas
         m_arCadenaSalida[1] = 0x64;
         switch(nNumero) {
            case 14: m_arCadenaSalida[2] = 6 - m_nLineas; break;
            case 34: m_arCadenaSalida[2] = 4; break;
            case 38: m_arCadenaSalida[2] = 3; break;
         }
         m_arCadenaSalida[3] = NULL;
         EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, 10);
      }
   }
   else if(nNumero < 10 || nNumero > 32 || nNumero == 18) { return; } 

   if(nNumero == 10 || nNumero == 16) {

      for(j = 0; j < 65; ++j) { m_arCadenaSalida[j] = BLANCO; }

      for(i = 3; m_szParametros[i] != BLANCO; ++i, ++j) {
          m_arCadenaSalida[j] = m_szParametros[i];
	  }

      m_arCadenaSalida[j++] = LF;
      m_arCadenaSalida[j] = NULL;

      EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);

      if(nNumero == 16) { TURQUIA_Implet(); }

      return;
   }

   if(nNumero == 12 || nNumero == 14) {
      for(; j < 58; ++j) { m_arCadenaSalida[j] = BLANCO; }
   }

   if(m_bAbono) {
      switch(nNumero) {
         case 14: case 30:
              for(j = 0; j < 63; ++j) { m_arCadenaSalida[j] = BLANCO; }
              for(i = 14; m_szParametros[i] != GOMA; ++j, ++i) {
                  m_arCadenaSalida[j] = m_szParametros[i];
			  }
			  if(m_szParametros[i] == GOMA) {
				  m_arCadenaSalida[j++] = m_szParametros[i++];
				  for(int k = 0; k < 2; ++k, ++i, ++j) { 
					  m_arCadenaSalida[j] = m_szParametros[i];
				  }
			  }
              break;
         case 34:
              for(j = 0; j < 63; ++j) { m_arCadenaSalida[j] = BLANCO; }
              for(i = 3; m_szParametros[i] != GOMA; ++j, ++i) {
                  m_arCadenaSalida[j] = m_szParametros[i];
			  }
			  if(m_szParametros[i] == GOMA) {
				  m_arCadenaSalida[j++] = m_szParametros[i++];
				  for(int k = 0; k < 2; ++k, ++i, ++j) { 
					  m_arCadenaSalida[j] = m_szParametros[i];
				  }
			  }
              break;
         case 38: case 40: case 42:
              for(j = 0; j < 22; ++j) { m_arCadenaSalida[j] = BLANCO; }
              for(i = 3; m_szParametros[i] != SEPARADOR; ++j, ++i) {
                  m_arCadenaSalida[j] = m_szParametros[i];
			  }
              break;
         default: break;
      }
   }
   else {
      if(nNumero != 12 && nNumero != 14) {
         for(i = 0; i < 28; ++i, ++j) { m_arCadenaSalida[j] = BLANCO; }
      }

      if(nNumero == 12 || nNumero == 14) { // Totales
         for(i = 3; m_szParametros[i] != GOMA; ++i, ++j) {
             m_arCadenaSalida[j] = m_szParametros[i];
		 }
		 if(m_szParametros[i] == GOMA) {
			  m_arCadenaSalida[j++] = m_szParametros[i++];
			  for(int k = 0; k < 2; ++k, ++i, ++j) { 
				  m_arCadenaSalida[j] = m_szParametros[i];
			  }
		  }
      }
      else {
         if(nNumero > 18 && nNumero < 30) { // Importes
            for(i = 3; m_szParametros[i] != GOMA; ++i, ++j) { // Importe sin coma
                m_arCadenaSalida[j] = m_szParametros[i];
            }
			if(m_szParametros[i] == GOMA) {
				  m_arCadenaSalida[j++] = m_szParametros[i++];
				  for(int k = 0; k < 2; ++k, ++i, ++j) { 
					  m_arCadenaSalida[j] = m_szParametros[i];
				  }
			    i += 1;
			}else { i += 4; }
            //i += 4;
			
            for(; m_szParametros[i] != SEPARADOR; ++i, ++j) { // TL
                m_arCadenaSalida[j] = m_szParametros[i];
			}
         }
         else {
            if(nNumero == 30) {
               for(i = 3; m_szParametros[i] != GOMA; ++i, ++j) {
                   m_arCadenaSalida[j] = m_szParametros[i];
			   }
			   if(m_szParametros[i] == GOMA) {
				   m_arCadenaSalida[j++] = m_szParametros[i++];
				   for(int k = 0; k < 2; ++k, ++i, ++j) {
					   m_arCadenaSalida[j] = m_szParametros[i];
				   }
			   }
            }
            else {
               for(i = 3; m_szParametros[i] != SEPARADOR; ++i, ++j) {
                   m_arCadenaSalida[j] = m_szParametros[i];
			   }
            }
         }
      }
   }

   m_arCadenaSalida[j++] = LF;
   m_arCadenaSalida[j] = '\0';

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);

   if(m_bAbono && (nNumero == 34)) { TURQUIA_Implet(); }
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Totalb()
{
   if(m_szParametros[0] == '-') { m_bAbono = TRUE; }

   // Nos guardamos el total del documento
   for(int j = 0, i = 5; m_szParametros[i] != SEPARADOR; ++i, ++j) {
       m_szTotal[j] = m_szParametros[i];
   }
   m_szTotal[j] = '\0';
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Fechab() // La fecha del registro de la operacion
{
	if(!m_bTraspaso) { return; }

    UCHAR arNumero2[3];   

   m_arCadenaSalida[0] = ESC; // Saltar n lineas
   m_arCadenaSalida[1] = 0x64;
   m_arCadenaSalida[2] = 5;// == n
   m_arCadenaSalida[3] = NULL;
   EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, 10);

   // Obtener año
   arNumero2[0] = m_szParametros[0];
   arNumero2[1] = m_szParametros[1];
   arNumero2[2] = NULL;

   const UINT nNumero = atoi((char*)arNumero2);

   for(UINT j = 0; j < 57; ++j) { m_arCadenaSalida[j] = BLANCO; }

   m_arCadenaSalida[j++] = m_szParametros[4];
   m_arCadenaSalida[j++] = m_szParametros[5];
   m_arCadenaSalida[j++] = '/';
   m_arCadenaSalida[j++] = m_szParametros[2];
   m_arCadenaSalida[j++] = m_szParametros[3];
   m_arCadenaSalida[j++] = '/';

   if(nNumero < 50) {
      m_arCadenaSalida[j++] = '2';
      m_arCadenaSalida[j++] = '0';
   }
   else {
      m_arCadenaSalida[j++] = '1';
      m_arCadenaSalida[j++] = '9';
   }

   m_arCadenaSalida[j++] = m_szParametros[0];
   m_arCadenaSalida[j++] = m_szParametros[1];
   m_arCadenaSalida[j++] = LF;
   m_arCadenaSalida[j] = NULL;

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Texlib() // El texto libre
{	
   ++m_nLineas;

   for(UINT j = 0; j < 3; ++j) { m_arCadenaSalida[j] = BLANCO; }

   for(UINT i = 0; m_szParametros[i] != SEPARADOR; ++i, ++j) {
       m_arCadenaSalida[j] = m_szParametros[i];
   }

   m_arCadenaSalida[j++] = LF;
   m_arCadenaSalida[j] = NULL;

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Lintrasp()
{
   BOOL bValor;
   UINT nPuntos, nCiclos;

   // codigo de articulo
   for(UINT j = 0, i = 2; m_szParametros[i] != SEPARADOR; ++i, ++j) {
       m_arCadenaSalida[j] = m_szParametros[i];
   }
   for(i = 11; m_szParametros[i] != SEPARADOR; ++i, ++j) {
       m_arCadenaSalida[j] = m_szParametros[i];
   }
   for(i = 14; m_szParametros[i] != SEPARADOR; ++i, ++j) {
       m_arCadenaSalida[j] = m_szParametros[i];
   }

   m_arCadenaSalida[j++] = BLANCO;

   // descripcion
   for(nPuntos = 0, i = 17; nPuntos < 20; ++i, ++j, ++nPuntos) {
       m_arCadenaSalida[j] = m_szParametros[i];
   }

   // tipo de unidades
   for(char* szTipo = " ADET        "; *szTipo != NULL; ++j) {
       m_arCadenaSalida[j] = *szTipo++;
   }

   // cantidad temporal
   m_arCadenaSalida[j++] = (m_szParametros[60] == '0' ? BLANCO : m_szParametros[60]);
   m_arCadenaSalida[j++] = m_szParametros[61];

   // importe neto
   bValor = nPuntos = nCiclos = 0;
   for(i = 66; m_szParametros[i] != GOMA; ++i, ++j) {
      if(!bValor) {
         if(m_szParametros[i] != '0') {
            bValor = 1;
            m_arCadenaSalida[j] = m_szParametros[i];
         }
         else { m_arCadenaSalida[j] = BLANCO; }
      }
      else { m_arCadenaSalida[j] = m_szParametros[i]; }

      ++nPuntos;

      if(nPuntos == 3 && nCiclos < 3) {
         nPuntos = 0; ++nCiclos;

         if(bValor) { m_arCadenaSalida[++j] = '.'; }
         else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
      }
   }
   if(m_szParametros[i] == GOMA) {
		m_arCadenaSalida[j++] = m_szParametros[i++];
		for(int k = 0; k < 2; ++k, ++i, ++j) {
				if(!bValor) {
				if(m_szParametros[i] != '0') {
				bValor = 1;
				m_arCadenaSalida[j] = m_szParametros[i];
				}
				else { m_arCadenaSalida[j] = BLANCO; }
			}
			else { m_arCadenaSalida[j] = m_szParametros[i]; }

			++nPuntos;

			if(nPuntos == 3 && nCiclos < 3) {
				nPuntos = 0; ++nCiclos;

				if(bValor) { m_arCadenaSalida[++j] = '.'; }
				else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
			}
		}
   }

   if(!bValor) { m_arCadenaSalida[j++] = '0'; }

   for(nCiclos = 0; nCiclos < 5; ++nCiclos, ++j) { m_arCadenaSalida[j] = BLANCO; }

   // Importe linea
   bValor = nPuntos = nCiclos = 0;

   for(i = 140; m_szParametros[i] != GOMA; ++i, ++j) {
      if(!bValor) {
         if(m_szParametros[i] != '0') {
            bValor = 1;
            m_arCadenaSalida[j] = m_szParametros[i];
         }
         else { m_arCadenaSalida[j] = BLANCO; }
      }
      else { m_arCadenaSalida[j] = m_szParametros[i]; }

      ++nPuntos;

      if((nPuntos == 3) && (nCiclos < 3)) {

         nPuntos = 0; ++nCiclos;

         if(bValor) { m_arCadenaSalida[++j] = '.'; }
         else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
      }
   }
	if(m_szParametros[i] == GOMA) {
		m_arCadenaSalida[j++] = m_szParametros[i++];
		for(int k = 0; k < 2; ++k, ++i, ++j) {
			if(!bValor) {
				if(m_szParametros[i] != '0') {
					bValor = 1;
					m_arCadenaSalida[j] = m_szParametros[i];
				}
				else { m_arCadenaSalida[j] = BLANCO; }
			}
			else { m_arCadenaSalida[j] = m_szParametros[i]; }

			++nPuntos;

			if((nPuntos == 3) && (nCiclos < 3)) {

				nPuntos = 0; ++nCiclos;

				if(bValor) { m_arCadenaSalida[++j] = '.'; }
				else if(nCiclos == 2) { m_arCadenaSalida[++j] = BLANCO; }
			}
		}
	}

   m_arCadenaSalida[j++] = LF;
   m_arCadenaSalida[j] = NULL;

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Infonf()
{
   char sLinea[3];
   UINT i, j;

   ++m_nLineas;

   sLinea[0] = m_szParametros[3];
   sLinea[1] = m_szParametros[4];
   sLinea[2] = NULL;

   if(strcmp(sLinea, "11") && strcmp(sLinea, "12") && strcmp(sLinea, "13") && 
	  strcmp(sLinea, "15") && strcmp(sLinea, "FF")
	 )
   {
        return;
   }

   if(!strcmp(sLinea, "FF")) { TURQUIA_Expulsar(); return; }
   if(!strcmp(sLinea, "11")) { i = 66; }
   else if(!strcmp(sLinea, "12")) { i = 73; }
   else {
      if(!strcmp(sLinea, "13")) {
         m_arCadenaSalida[0] = LF;
         m_arCadenaSalida[1] = NULL;
         EnviaCadena(m_arCadenaSalida, 1, NULL, NULL, NULL, FALSE, 10);
      }
      i = 48;
   }

   for(j = 0; j < i; j++) { m_arCadenaSalida[j] = BLANCO; }

   for(i = 6; m_szParametros[i] != SEPARADOR && m_szParametros[i] != GOMA; ++i, ++j) {
       m_arCadenaSalida[j] = m_szParametros[i];
   }
   if(m_szParametros[i] == GOMA) {
	   m_arCadenaSalida[i++, j++] = GOMA;
	   for(int k = 0; k < 2; ++k, ++i, ++j) {
		   m_arCadenaSalida[j] = m_szParametros[i];
	   }
   }

   m_arCadenaSalida[j++] = LF;;
   m_arCadenaSalida[j] = NULL;

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
}
//---------------------------------------------------------------------------------------------------

// Expulsar el papel en Turquia
//
void cImpre::TURQUIA_Expulsar()
{
   TURQUIA_PaperStatus();

   m_arCadenaSalida[0] = FF;
   m_arCadenaSalida[1] = NULL;

   EnviaCadena(m_arCadenaSalida, 1, NULL, NULL, NULL, FALSE, 10);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA_Procesa()
{
   cReloj Reloj;

    switch(m_nIndComando) {
        case 0: // TESTEO
             TURQUIA_Testeo();
             break;
        case 1: // NUMCAJ
             break;
        case 2: // NUMTIQ
             break;
        case 3: // FECHAF
             break;
        case 4: // HORAFI
             break;
        case 5: // NUMBOL
             break;
        case 6: // FECHAB
             TURQUIA_Fechab();
             break;
        case 7: // HORABO
             break;
        case 8: // TOTALB
             TURQUIA_Totalb();
             Reloj.EsperaSegundos(0.1);
             break;
        case 9: // CABECB
             TURQUIA_Cabecb();
             break;
        case 10:// LINDEB
             if(m_bTraspaso) {
                TURQUIA_Lintrasp();
                Reloj.EsperaSegundos(0.1);
             }
             else {
                TURQUIA_Lindeb();
                Reloj.EsperaSegundos(0.1);
             }
             break;
        case 11:// PAGOFI
             break;
        case 12:// CIERRZ
             break;
        case 13:// INFONF
             TURQUIA_Infonf(); // para vales
             break;
        case 14:// CANCEL
             break;
        case 15:// PIEFIS
             TURQUIA_Piefis();
             if(!m_bTraspaso) { Reloj.EsperaSegundos(0.1); }
             break;
        case 16:// TESTEA
             break;
        case 17:// TESTHW
             break;
        case 18:// ABRECA
             break;
        case 19:// NUMERO
             break;
        case 20:// DISPLY
             break;
        case 21:// TRASPA
             m_bTraspaso = TRUE;
             break;
        case 22:// TEXLIB
             TURQUIA_Texlib();
             Reloj.EsperaSegundos(0.1);
             break;
        case 23:// LIBERA
             TURQUIA_Libera();
             break;
        case 24:// INICIA
             break;
        case 25:// LECTUX
             break;
        default:// ­­FUERA DE MADRE!!
             m_nError = 599;
             break;
    }
    ComponeError();
}
//---------------------------------------------------------------------------------------------------

//
// TURQUIA2
// ========
//
// NOTA: Impresora TM-U950 de Turquia. Parecida a la de Chile.
//

void cImpre::TURQUIA2_Testeo()
{
    /*
	TURQUIA_Reset();
    TURQUIA_LetraPeque();
    TURQUIA_AreaTrabajo();
    TURQUIA_DeshabilitaBotones();	
	m_arCadenaSalida[0] = DLE;
    m_arCadenaSalida[1] = EOT;
    m_arCadenaSalida[2] = 1;
    m_arCadenaSalida[3] = NULL;
    EnviaCadena(m_arCadenaSalida, 3, NULL, 5, NULL, FALSE, NULL);
	*/
    
    char* sz_texto = "123456789a123456789b123456789c123456789d123456789e123456789f123456789g123456789h123456789i";

    m_arCadenaSalida[0] = ESC; // Inicializar impresora
    m_arCadenaSalida[1] = '@';
    m_arCadenaSalida[2] = NULL;
    EnviaCadena(m_arCadenaSalida, 2, NULL, NULL, NULL, FALSE, NULL);
    m_arCadenaSalida[0] = ESC; // Seleccionar SLIP
    m_arCadenaSalida[1] = 'c';
    m_arCadenaSalida[2] = '0';
    m_arCadenaSalida[3] = 0x04;
    m_arCadenaSalida[4] = NULL;
    EnviaCadena(m_arCadenaSalida, 4, NULL, NULL, NULL, FALSE, NULL);
    
    for(int i = 0; *sz_texto != NULL; i++) { m_arCadenaSalida[i] = *sz_texto++; }

    m_arCadenaSalida[++i] = 0x0c;
    m_arCadenaSalida[++i] = NULL;
    EnviaCadena(m_arCadenaSalida, i, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::TURQUIA2_Procesa()
{
   cReloj Reloj;

    switch(m_nIndComando) {
        case 0: // TESTEO
             TURQUIA2_Testeo();
             break;
        case 1: // NUMCAJ
             break;
        case 2: // NUMTIQ
             break;
        case 3: // FECHAF
             break;
        case 4: // HORAFI
             break;
        case 5: // NUMBOL
             break;
        case 6: // FECHAB
             break;
        case 7: // HORABO
             break;
        case 8: // TOTALB
             break;
        case 9: // CABECB
             break;
        case 10:// LINDEB
             break;
        case 11:// PAGOFI
             break;
        case 12:// CIERRZ
             break;
        case 13:// INFONF
             break;
        case 14:// CANCEL
             break;
        case 15:// PIEFIS
             break;
        case 16:// TESTEA
             break;
        case 17:// TESTHW
             break;
        case 18:// ABRECA
             break;
        case 19:// NUMERO
             break;
        case 20:// DISPLY
             break;
        case 21:// TRASPA
             break;
        case 22:// TEXLIB
             break;
        case 23:// LIBERA
             break;
        case 24:// INICIA
             break;
        case 25:// LECTUX
             break;
        default:// ­­FUERA DE MADRE!!
             m_nError = 599;
             break;
    }
    ComponeError();
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

//
// PORTUGAL
// ========
//
// NOTA: Utiliza la misma impresora que Turquía, pero solamente para imprimir talones.
//

void cImpre::PORTUGAL_Infonf()
{
   BOOL bPrimeraVez = TRUE;
   char sLinea[3];
   UINT i, j, nContador;
   char* szImporte;

   ++m_nLineas;

   sLinea[0] = m_szParametros[3];
   sLinea[1] = m_szParametros[4];
   sLinea[2] = NULL;

   if(strcmp(sLinea, "05") && strcmp(sLinea, "10") && strcmp(sLinea, "20") &&
      strcmp(sLinea, "30") && strcmp(sLinea, "FF")
     )  
   {
        return; 
   }

   if(!strcmp(sLinea, "05")) { // Euros o escudos
      if(m_szParametros[6] != 'E') { m_bEuros = FALSE; } // Por defecto, se trabaja en euros         
   }

   if(!strcmp(sLinea, "10")) { // Importe en cifras

      // Nos guardamos el total del documento
      for(j = 0, i = 9; m_szParametros[i] != SEPARADOR && m_szParametros[i] != BLANCO; ++i, ++j) {
         m_szTotal[j] = m_szParametros[i]; 
      }
      m_szTotal[j] = NULL;

      // Los primeros ceros seran puntos
      for(i = 0; m_szTotal[i] != NULL; ++i) {
         if(bPrimeraVez) {
            if(m_szTotal[i] == '0') { m_szTotal[i] = PUNTO; }
            else { bPrimeraVez = FALSE; }
         }
      }

      /*m_arCadenaSalida[0] = ESC; // Saltar n lineas
      m_arCadenaSalida[1] = 0x64;
      m_arCadenaSalida[2] = 1;
      m_arCadenaSalida[3] = NULL;
      EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, 10);*/

      if(!m_bEuros) {
        for(j = 0; j < 48; ++j) { m_arCadenaSalida[j] = BLANCO; }

        for(i = 0, nContador = 0; m_szTotal[i] != COMA; i++, nContador++) {
            if(m_szTotal[i] != PUNTO) {
                m_arCadenaSalida[j++] = m_szTotal[i];
            }
            else { m_arCadenaSalida[j++] = BLANCO; }
            m_arCadenaSalida[j++] = BLANCO;
            m_arCadenaSalida[j++] = BLANCO;

            if(nContador % 2) { m_arCadenaSalida[j++] = BLANCO; }
         }
      }
      else { // Talon en euros
        for(j = 0; j < 47; ++j) { m_arCadenaSalida[j] = BLANCO; }

        for(i = 0, nContador = 0; nContador < 15; ++i, ++nContador) {
            if(m_szTotal[i] != PUNTO && m_szTotal[i] != COMA) {
               m_arCadenaSalida[j++] = m_szTotal[i];
            }
            else { m_arCadenaSalida[j++] = BLANCO; }
            if(m_szTotal[i] != COMA) {
                m_arCadenaSalida[j++] = BLANCO;
                m_arCadenaSalida[j++] = BLANCO;
            }
        }
      }

      m_arCadenaSalida[j++] = LF;
      m_arCadenaSalida[j] = NULL;

      EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
   }

   if(!strcmp(sLinea, "20")) { // Fecha talon
      m_arCadenaSalida[0] = ESC; // Saltar n lineas
      m_arCadenaSalida[1] = 0x64;
      m_arCadenaSalida[2] = 2;
      m_arCadenaSalida[3] = NULL;
      EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, 10);

      for(j = 0; j < 62; ++j) { m_arCadenaSalida[j] = BLANCO; }

      for(i = 6, nContador = 0; m_szParametros[i] != BLANCO; ++i, ++nContador) {
         m_arCadenaSalida[j++] = m_szParametros[i];

         m_arCadenaSalida[j++] = BLANCO;
         m_arCadenaSalida[j++] = BLANCO;

         if(nContador % 2) {
            m_arCadenaSalida[j++] = BLANCO;

            if(nContador == 5 || nContador == 7) {
               m_arCadenaSalida[j++] = BLANCO;
            }
         }
      }    

      m_arCadenaSalida[j++] = LF;
      m_arCadenaSalida[j] = NULL;

      EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
   }

   if(!strcmp(sLinea, "30")) { // Empresa (beneficiario)
   
      for(j = 0; j < 6; ++j) { m_arCadenaSalida[j] = BLANCO; }

      for(i = 6; m_szParametros[i] != SEPARADOR; ++j, ++i) {
         m_arCadenaSalida[j] = m_szParametros[i];  
      }

      m_arCadenaSalida[j++] = LF;
      m_arCadenaSalida[j] = NULL;

      EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);
   }

   if(!strcmp(sLinea, "FF")) { // The end-->Importe en letras
      m_arCadenaSalida[0] = ESC; // Saltar n lineas
      m_arCadenaSalida[1] = 0x64;
      m_arCadenaSalida[2] = 1;
      m_arCadenaSalida[3] = NULL;
      EnviaCadena(m_arCadenaSalida, 3, NULL, NULL, NULL, FALSE, 10);

      if(m_bEuros) {
        spawnl(P_WAIT, "PVNLET.exe", "PVNLET.exe", g_Paises[m_nIndPais], m_szTotal, "E", NULL); // Ejecutamos PVNLET.EXE
      }
      else {
        spawnl(P_WAIT, "PVNLET.exe", "PVNLET.exe", g_Paises[m_nIndPais], m_szTotal, NULL); // Ejecutamos PVNLET.EXE
      }

      cFile* pFichlet = new cFile("PVNLET.TXT", "rb"); // Obtenemos el importe en letras 
      
      if(pFichlet != NULL && pFichlet->LeerSigLinea() != EOF) {
         szImporte = pFichlet->GetLinea(); 
      }
      delete pFichlet; pFichlet = NULL; // We still MAY apply delete to a NULL-pointer

      for(j = 0; j < 6; ++j) { m_arCadenaSalida[j] = BLANCO; }

      for(; *szImporte != NULL; ++j) { m_arCadenaSalida[j] = *szImporte++; }

      for(; j < 88; ++j) { m_arCadenaSalida[j] = '-'; }

      m_arCadenaSalida[j++] = LF;
      m_arCadenaSalida[j] = NULL;

      EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, 10);

      TURQUIA_Expulsar();  
      
      return;
   }
}
//---------------------------------------------------------------------------------------------------

void cImpre::PORTUGAL_Procesa()
{
   cReloj Reloj;

    switch(m_nIndComando) {
        case 0: TURQUIA_Testeo(); break; // TESTEO
        case 1: break; // NUMCAJ
        case 2: break; // NUMTIQ
        case 3: break; // FECHAF
        case 4: break; // HORAFI
        case 5: break; // NUMBOL
        case 6: break; // FECHAB
        case 7: break; // HORABO
        case 8: break; // TOTALB
        case 9: break; // CABECB
        case 10: break; // LINDEB
        case 11: break; // PAGOFI
        case 12: break; // CIERRZ
        case 13: PORTUGAL_Infonf(); break; // INFONF: Impresión del talón
        case 14: break; // CANCEL
        case 15: break; // PIEFIS
        case 16: break; // TESTEA
        case 17: break; // TESTHW
        case 18: break; // ABRECA
        case 19: break; // NUMERO
        case 20: break; // DISPLY
        case 21: break; // TRASPA
        case 22: break; // TEXLIB
        case 23: TURQUIA_Libera(); break; // LIBERA
        case 24: break; // INICIA
        case 25: break; // LECTUX
        default: m_nError = 599; break; // ­­FUERA DE MADRE!!
    }
    ComponeError();
}
//---------------------------------------------------------------------------------------------------

#include <dos.h> 
//-------------------------------------------------------------------------------------------------------
#if 0
BOOL FileExists(const char* strFullFileName)
{   
    const unsigned int nAttrib = _A_NORMAL | _A_RDONLY;
    
    _find_t ft;
    
    unsigned int nRes = _dos_findfirst(strFullFileName, nAttrib, &ft);
    
    return (nRes == 0);
}
#endif
//---------------------------------------------------------------------------------------------------

//
// HUNGRIA
// =======
//
void cImpre::HUNGRIA_Ejecuta()
{
#define FICHBAT ".\\hgrbat.bat"
#define FICHERR ".\\hungria.err"

    cFile* Bat_Hungria = new cFile(FICHBAT, "wb");
    Bat_Hungria->EscribirLinea( "@ECHO OFF" );
    Bat_Hungria->EscribirCaracter( CR );
    Bat_Hungria->EscribirCaracter( LF );
    Bat_Hungria->EscribirLinea( "IF EXIST .\\HUNGRIA.ERR DEL .\\HUNGRIA.ERR" );
    Bat_Hungria->EscribirCaracter( CR );
    Bat_Hungria->EscribirCaracter( LF );
    Bat_Hungria->EscribirLinea( (char*)m_arCadenaSalida );
    Bat_Hungria->EscribirCaracter( CR );
    Bat_Hungria->EscribirCaracter( LF );
    Bat_Hungria->EscribirLinea( "IF ERRORLEVEL 1 GOTO ERROR" );
    Bat_Hungria->EscribirCaracter( CR );
    Bat_Hungria->EscribirCaracter( LF );
    Bat_Hungria->EscribirLinea( "GOTO FIN" );
    Bat_Hungria->EscribirCaracter( CR );
    Bat_Hungria->EscribirCaracter( LF );
    Bat_Hungria->EscribirLinea( ":ERROR" );
    Bat_Hungria->EscribirCaracter( CR );
    Bat_Hungria->EscribirCaracter( LF );
    Bat_Hungria->EscribirLinea( "ECHO. > .\\HUNGRIA.ERR" );
    Bat_Hungria->EscribirCaracter( CR );
    Bat_Hungria->EscribirCaracter( LF );
    Bat_Hungria->EscribirLinea( ":FIN" );
    Bat_Hungria->EscribirCaracter( CR );
    Bat_Hungria->EscribirCaracter( LF );
    delete Bat_Hungria;

    strcpy( (char*)m_arCadenaSalida, FICHBAT );
    system( (char*)m_arCadenaSalida );
#if 0
    if(!FileExists(FICHERR)) { 
        m_nError = 701;
    }
#else
    // Comprueba si hay error
    cFile*  FichErr   = new cFile(FICHERR, "rb");
    if (!FichErr->GetError())
       m_nError = 701;
    delete FichErr;
#endif
}
void cImpre::HUNGRIA_Numbol()
{
    UINT i, j;

    strcpy( (char*)m_arCadenaSalida, "PRSAMU.EXE ECR_0001 /PR " );
    j = strlen( (char*)m_arCadenaSalida );
    m_arCadenaSalida[j] = COMILLA; j++;

    for ( char* szSzam = "Szam <"; *szSzam != NULL; j++ ) // "Numero" en hungaro
        m_arCadenaSalida[j] = *szSzam++;

    for ( i=3; (m_szParametros[i] != SEPARADOR); i++, j++ )
        m_arCadenaSalida[j] = m_szParametros[i];

    m_arCadenaSalida[j] = '>'; j++;
    m_arCadenaSalida[j] = COMILLA; j++;
    m_arCadenaSalida[j] = NULL;

    m_bBoletaAbierta = TRUE;

    HUNGRIA_Ejecuta();
}
void cImpre::HUNGRIA_Lindeb()
{
    UINT i, j, bPrimeraVez;

    m_bBoletaAbierta = TRUE;

    if ( m_bBoletaAbierta )
    {
        strcpy( (char*)m_arCadenaSalida, "PRSAMU.EXE ECR_0001 /T " );
        j = strlen( (char*)m_arCadenaSalida );
        m_arCadenaSalida[j] = COMILLA; j++;
        m_arCadenaSalida[j] = '0'; j++;
        m_arCadenaSalida[j] = '1'; j++;
        m_arCadenaSalida[j] = ','; j++;

        // Descripcion
        for ( i=17; (m_szParametros[i] != SEPARADOR); i++, j++ )
            if ( m_szParametros[i] == COMA )
               m_arCadenaSalida[i] = BLANCO;
            else
               m_arCadenaSalida[j] = m_szParametros[i];

        m_arCadenaSalida[j] = COMA; j++;

        // Total
        bPrimeraVez = TRUE;
        for ( i=146; (m_szParametros[i] != SEPARADOR); i++ )
        {
            if ( m_szParametros[i] == COMA )
            {
               if (bPrimeraVez)
               {
                  m_arCadenaSalida[j] = '0'; j++;
                  bPrimeraVez = FALSE;
               }
               m_arCadenaSalida[j] = '.'; j++;
            }
            else
            {
               if ( m_szParametros[i] != '0' )
               {
                  m_arCadenaSalida[j] = m_szParametros[i]; j++;
                  bPrimeraVez = FALSE;
               }
               else
               {
                  if ( !bPrimeraVez )
                  {
                     m_arCadenaSalida[j] = m_szParametros[i];
                     j++;
                  }
               }
            }
        }
        j--; // Eliminamos el ultimo cero
        m_arCadenaSalida[j] = COMILLA; j++;
        m_arCadenaSalida[j] = NULL;

        HUNGRIA_Ejecuta();
        if ( m_nError )
           return;

        strcpy( (char*)m_arCadenaSalida, "PRSAMU.EXE ECR_0001 /PR " );
        j = strlen( (char*)m_arCadenaSalida );
        m_arCadenaSalida[j] = COMILLA; j++;
        m_arCadenaSalida[j] = BLANCO; j++;

        // Modelo
        for ( i=2; (m_szParametros[i] != SEPARADOR); i++, j++ )
            m_arCadenaSalida[j] = m_szParametros[i];

        m_arCadenaSalida[j] = BLANCO; j++;

        // Talla
        for ( i=11; (m_szParametros[i] != SEPARADOR); i++, j++ )
            m_arCadenaSalida[j] = m_szParametros[i];

        m_arCadenaSalida[j] = BLANCO; j++;

        // Color
        for ( i=14; (m_szParametros[i] != SEPARADOR); i++, j++ )
            m_arCadenaSalida[j] = m_szParametros[i];

        m_arCadenaSalida[j] = BLANCO; j++;
        m_arCadenaSalida[j] = BLANCO; j++;

        // Cantidad
        m_arCadenaSalida[j] = m_szParametros[61]; j++;

        m_arCadenaSalida[j] = BLANCO; j++;
        m_arCadenaSalida[j] = 'x'; j++;
        m_arCadenaSalida[j] = BLANCO; j++;

        // Total
        bPrimeraVez = TRUE;
        for ( i=72; (m_szParametros[i] != SEPARADOR); i++ )
        {
            if ( m_szParametros[i] == COMA )
            {
                if ( bPrimeraVez )
                {
                    m_arCadenaSalida[j] = '0'; j++;
                    bPrimeraVez = FALSE;
                }
                m_arCadenaSalida[j] = '.'; j++;
            }
            else
            {
                if ( m_szParametros[i] != '0' )
                {
                    m_arCadenaSalida[j] = m_szParametros[i]; j++;
                    bPrimeraVez = FALSE;
                }
                else
                {
                    if ( !bPrimeraVez )
                    {
                        m_arCadenaSalida[j] = m_szParametros[i];
                        j++;
                    }
                }
            }
        }
        j--; // Eliminamos el ultimo cero
        m_arCadenaSalida[j] = COMILLA; j++;
        m_arCadenaSalida[j] = NULL;

       HUNGRIA_Ejecuta();
    }
    else
    {
        m_nError = 535;
    }
}

void cImpre::HUNGRIA_Piefis()
{
    UINT i, j;

    if ( !m_bBoletaAbierta )
    {
       m_nError = 537;
       return;
    }
    strcpy( (char*)m_arCadenaSalida, "PRSAMU.EXE ECR_0001 /PR " );
    j = strlen( (char*)m_arCadenaSalida );
    m_arCadenaSalida[j] = COMILLA; j++;

    for ( i=3; (m_szParametros[i] != SEPARADOR); i++, j++ )
        m_arCadenaSalida[j] = m_szParametros[i];

    m_arCadenaSalida[j] = COMILLA; j++;
    m_arCadenaSalida[j] = NULL;

    HUNGRIA_Ejecuta();
}
void cImpre::HUNGRIA_Pagofi()
{
    UINT i, j, bPrimeraVez;

    if ( !m_bBoletaAbierta )
    {
       m_nError = 536;
       return;
    }
    strcpy( (char*)m_arCadenaSalida, "PRSAMU.EXE ECR_0001 /F" );
    j = strlen( (char*)m_arCadenaSalida );

    // Forma de pago
    if ( m_szParametros[1] == '1' )
    {
       m_arCadenaSalida[j] = 'K';
    }
    else
    {
       if ( m_szParametros[1] == '2' )
       {
           m_arCadenaSalida[j] = 'C';
       }
       else
       {
           if ( m_szParametros[1] == '3' )
           {
              m_arCadenaSalida[j] = 'H';
           }
           else
           {
              m_nError = 530; // Formato incorrecto
              return;
           }
       }
    }
    j++;

    m_arCadenaSalida[j] = BLANCO; j++;

    // Importe
    bPrimeraVez = TRUE;
    for ( i=44; (m_szParametros[i] != SEPARADOR); i++ )
    {
       if ( m_szParametros[i] == COMA )
       {
           if (bPrimeraVez)
           {
               m_arCadenaSalida[j] = '0'; j++;
               bPrimeraVez = FALSE;
           }
           m_arCadenaSalida[j] = '.'; j++;
       }
       else
       {
           if ( m_szParametros[i] != '0' )
           {
              m_arCadenaSalida[j] = m_szParametros[i]; j++;
              bPrimeraVez = FALSE;
           }
           else
           {
              if ( !bPrimeraVez )
              {
                  m_arCadenaSalida[j] = m_szParametros[i];
                  j++;
              }
           }
       }
    }
    j--; // Eliminamos el ultimo cero

    m_arCadenaSalida[j] = NULL;

    HUNGRIA_Ejecuta();
}

void cImpre::HUNGRIA_Cierrz()
{
    if ( !m_bBoletaAbierta )
    {
       strcpy( (char*)m_arCadenaSalida, "PRSAMU.EXE ECR_0001 /Z" );

       HUNGRIA_Ejecuta();
    }
    else m_nError = 539;
}

void cImpre::HUNGRIA_Procesa()
{
    switch ( m_nIndComando )
    {
        case 0: // TESTEO
             break;
        case 1: // NUMCAJ
             break;
        case 2: // NUMTIQ
             break;
        case 3: // FECHAF
             break;
        case 4: // HORAFI
             break;
        case 5: // NUMBOL
             HUNGRIA_Numbol();
             break;
        case 6: // FECHAB
             break;
        case 7: // HORABO
             break;
        case 8: // TOTALB
             break;
        case 9: // CABECB
             break;
        case 10:// LINDEB
             HUNGRIA_Lindeb();
             break;
        case 11:// PAGOFI
             HUNGRIA_Pagofi();
             break;
        case 12:// CIERRZ
             HUNGRIA_Cierrz();
             break;
        case 13:// INFONF
             break;
        case 14:// CANCEL
             break;
        case 15:// PIEFIS
             break;
        case 16:// TESTEA
             break;
        case 17:// TESTHW
             break;
        case 18:// ABRECA
             break;
        case 19:// NUMERO
             break;
        case 20:// DISPLY
             break;
        case 21:// TRASPA
             break;
        case 22:// TEXLIB
             break;
        case 23:// LIBERA
             break;
        case 24:// INICIA
             break;
        case 25:// LECTUX
             break;
        default:// ­­FUERA DE MADRE!!
             m_nError = 599;
             break;
    }
    ComponeError();
}
//---------------------------------------------------------------------------------------------------

//
// POLONIA
// ==================
//
// cImpre::MFCaError
// =================
//     Pasar error del MF al objeto cImpre
//
// Parametros de entrada: ninguno.
//
// Parametros de salida: ninguno.
//
void cImpre::POLONIA_MFCaError()
{
    UCHAR cNumero[1];

    itoa((int)m_arRespuesta[5], (char*)cNumero, 10); // clase error
    m_cClaseError = cNumero[0];

    m_nError  = m_arRespuesta[6];
    m_nEstado = m_arRespuesta[7];
}
//---------------------------------------------------------------------------------------------------

// cImpre::QueryE
// ==============
//     Interroga a la impresora por el ultimo error y pasa el error
// al objeto 'cImpre'.
//
// Parametros de entrada: ninguno.
//
// Parametros de salida: ninguno.
//
void cImpre::POLONIA_QueryE()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = '+';
    m_arCadenaSalida[2] = 'E';
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 9, NULL, TRUE, NULL);

    POLONIA_MFCaError();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Cierra_report()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFB;
    m_arCadenaSalida[2] = 'O';
    m_arCadenaSalida[3] = SEP;
    m_arCadenaSalida[4] = END;
    m_arCadenaSalida[5] = NULL;

    EnviaCadena(m_arCadenaSalida, 5, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Testeo()
{
    POLONIA_QueryE();
    ComponeError();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Numcaj()
{
    UCHAR arsNumero3[4];

    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFB;
    m_arCadenaSalida[2] = 'U';
    m_arCadenaSalida[3] = SEP;
    m_arCadenaSalida[4] = 'I';
    m_arCadenaSalida[5] = SEP;
    m_arCadenaSalida[6] = END;
    m_arCadenaSalida[7] = NULL;

    EnviaCadena(m_arCadenaSalida, 7, NULL, NULL, SEP, TRUE, NULL);

    ComponeComando(g_Comandos[m_nIndComando]);

    itoa((int)m_arRespuesta[5], (char*)arsNumero3, 10); // clase error
    m_szRespuesta[13] = m_arRespuesta[8];
    m_szRespuesta[14] = m_arRespuesta[9];

    m_szRespuesta[15] = SEPARADOR;
    m_szRespuesta[16] = FIN_MNG;
    m_szRespuesta[17] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Numtiq()
{
    UCHAR arNumero20[20];
    UINT  i, j;

    m_arCadenaSalida[0]  = ESC;
    m_arCadenaSalida[1]  = MFB;
    m_arCadenaSalida[2]  = 'U';
    m_arCadenaSalida[3]  = SEP;
    m_arCadenaSalida[4]  = 'L';
    m_arCadenaSalida[5]  = SEP;

    sprintf((char*)arNumero20, "%u", 0x9001);
    for(i=0, j=6; (arNumero20[i] != NULL); i++,j++)
        m_arCadenaSalida[j] = arNumero20[i];

    m_arCadenaSalida[j] = SEP; j++;
    m_arCadenaSalida[j] = END; j++;
    m_arCadenaSalida[j] = NULL;

    EnviaCadena(m_arCadenaSalida, j, NULL, NULL, SEP, TRUE, NULL);

    ComponeComando(g_Comandos[m_nIndComando]);

    // Avanzamos hasta el primer delimitador
    for(i=0; (m_arRespuesta[i] != SEP_RESPUESTA); i++);
    i++;

    // Cargamos el valor devuelto hasta el siguiente delimitador
    for(j=0; (m_arRespuesta[i] != SEP_RESPUESTA); i++, j++)
       arNumero20[j] = m_arRespuesta[i];
    arNumero20[j] = NULL;

    // Rellenamos el destino con ceros por la dif. de longitud
    for(i=13, j=15; (j>strlen((char*)arNumero20)); j--, i++)
       m_szRespuesta[i] = '0';

    // Cargamos el valor al string destino
    for(j=0; (arNumero20[j] != NULL); i++, j++)
       m_szRespuesta[i] = arNumero20[j];

    m_szRespuesta[i] = SEPARADOR; i++;
    m_szRespuesta[i] = FIN_MNG; i++;
    m_szRespuesta[i] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Fechaf()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFB;
    m_arCadenaSalida[2] = 'U';
    m_arCadenaSalida[3] = SEP;
    m_arCadenaSalida[4] = 'T';
    m_arCadenaSalida[5] = SEP;
    m_arCadenaSalida[6] = END;
    m_arCadenaSalida[7] = NULL;

    EnviaCadena(m_arCadenaSalida, 7, NULL, NULL, SEP, TRUE, NULL);

    ComponeComando(g_Comandos[m_nIndComando]);

    m_szRespuesta[13] = m_arRespuesta[6];
    m_szRespuesta[14] = m_arRespuesta[7];
    m_szRespuesta[15] = m_arRespuesta[9];
    m_szRespuesta[16] = m_arRespuesta[10];
    m_szRespuesta[17] = m_arRespuesta[12];
    m_szRespuesta[18] = m_arRespuesta[13];
    m_szRespuesta[19] = SEPARADOR;
    m_szRespuesta[20] = FIN_MNG;
    m_szRespuesta[21] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Horafi()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFB;
    m_arCadenaSalida[2] = 'U';
    m_arCadenaSalida[3] = SEP;
    m_arCadenaSalida[4] = 'T';
    m_arCadenaSalida[5] = SEP;
    m_arCadenaSalida[6] = END;
    m_arCadenaSalida[7] = NULL;

    EnviaCadena(m_arCadenaSalida, 7, NULL, NULL, SEP, TRUE, NULL);

    ComponeComando(g_Comandos[m_nIndComando]);

    m_szRespuesta[13] = m_arRespuesta[15];
    m_szRespuesta[14] = m_arRespuesta[16];
    m_szRespuesta[15] = m_arRespuesta[18];
    m_szRespuesta[16] = m_arRespuesta[19];
    m_szRespuesta[17] = m_arRespuesta[21];
    m_szRespuesta[18] = m_arRespuesta[22];
    m_szRespuesta[19] = '0';
    m_szRespuesta[20] = '0';
    m_szRespuesta[21] = SEPARADOR;
    m_szRespuesta[22] = FIN_MNG;
    m_szRespuesta[23] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Totalb()
{
    if(m_bBoletaAbierta)
    {
        m_arCadenaSalida[0] = ESC;
        m_arCadenaSalida[1] = MFB;
        m_arCadenaSalida[2] = '9';
        m_arCadenaSalida[3] = SEP;
        m_arCadenaSalida[4] = SEP;
        m_arCadenaSalida[5] = END;
        m_arCadenaSalida[6] = NULL;

        EnviaCadena(m_arCadenaSalida, 6, NULL, NULL, NULL, FALSE, NULL);

        POLONIA_Testeo();
    }
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Cabecb()
{
    UCHAR arNumero2[3];
    UINT  nNumero;

    // Obtener parametro PP
    arNumero2[0] = m_szParametros[0];
    arNumero2[1] = m_szParametros[1];
    arNumero2[2] = NULL;

    nNumero = atoi((char*)arNumero2);

    if(nNumero == 1) {
        m_arCadenaSalida[0] = ESC;
        m_arCadenaSalida[1] = MFB;
        m_arCadenaSalida[2] = '0';
        m_arCadenaSalida[3] = SEP;
        m_arCadenaSalida[4] = SEP;
        m_arCadenaSalida[5] = END;

        EnviaCadena(m_arCadenaSalida, 6, NULL, NULL, NULL, FALSE, NULL);

        if(POLONIA_QueryE(), m_nError) {
            POLONIA_Testeo();
            return;
        }

        m_bBoletaAbierta = TRUE;
    }
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Lindeb()
{
    UCHAR arNumero20[21];
    UINT  nNumero, i, j;

    if(m_bBoletaAbierta) {
        if(m_szParametros[0] == '+') {
            m_arCadenaSalida[0] = ESC;
            m_arCadenaSalida[1] = MFB;
            m_arCadenaSalida[2] = '1';
            m_arCadenaSalida[3] = SEP;
            m_arCadenaSalida[4] = '1';
            m_arCadenaSalida[5] = SEP;

            m_arCadenaSalida[6]  = m_szParametros[2]; // Modelo
            m_arCadenaSalida[7]  = m_szParametros[3];
            m_arCadenaSalida[8]  = m_szParametros[4];
            m_arCadenaSalida[9]  = m_szParametros[5];
            m_arCadenaSalida[10] = m_szParametros[6];
            m_arCadenaSalida[11] = m_szParametros[7];
            m_arCadenaSalida[12] = m_szParametros[8];
            m_arCadenaSalida[13] = m_szParametros[9];
            m_arCadenaSalida[14] = BLANCO;
            m_arCadenaSalida[15] = m_szParametros[11]; // Talla
            m_arCadenaSalida[16] = m_szParametros[12];
            m_arCadenaSalida[17] = BLANCO;
            m_arCadenaSalida[18] = m_szParametros[14]; // Color
            m_arCadenaSalida[19] = m_szParametros[15];
            m_arCadenaSalida[20] = BARRA_DOS;

            arNumero20[0] = m_szParametros[59]; // Cantidad
            arNumero20[1] = m_szParametros[60];
            arNumero20[2] = m_szParametros[61];
            arNumero20[3] = NULL;

            nNumero = atoi((char*)arNumero20);

            sprintf((char*)arNumero20, "%d", nNumero);

            for(i=0,j=21; (arNumero20[i] != NULL); i++,j++)
               m_arCadenaSalida[j] = arNumero20[i];

            m_arCadenaSalida[j] = BLANCO; j++;
            m_arCadenaSalida[j] = '*'; j++;
            m_arCadenaSalida[j] = BLANCO; j++;

            for(i=63; (m_szParametros[i] == '0'); i++);

            if(m_szParametros[i] == COMA)
            {
               m_arCadenaSalida[j] = '0'; j++;
            }
            // Precio
            for(; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];
            j--; // Eliminamos el tercer decimal

            // S¡mbolo moneda
            for(char* szMoneda = " zl\\"; (*szMoneda != NULL); j++)
               m_arCadenaSalida[j] = *szMoneda++;

            // Descripci¢n
            for(i=17;
               ((m_szParametros[i] != SEPARADOR) &&
               (i < 57)) ; i++, j++)
            {
                m_arCadenaSalida[j] = m_szParametros[i];
            }

            m_arCadenaSalida[j] = SEP; j++;

            // Importe de la linea
            for(i=137; (m_szParametros[i] == '0'); i++);

            if(m_szParametros[i] == COMA)
            {
               m_arCadenaSalida[j] = '0'; j++;
            }

            for(; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];
            j--; // Eliminamos el tercer decimal

            m_arCadenaSalida[j] = SEP; j++;
            m_arCadenaSalida[j] = m_szParametros[135]; j++; // Cat. VAT

            m_arCadenaSalida[j] = SEP; j++;
            m_arCadenaSalida[j] = END; j++;
            m_arCadenaSalida[j] = NULL;
        }
        else // item negativo
        {
            m_arCadenaSalida[0] = ESC;
            m_arCadenaSalida[1] = MFB;
            m_arCadenaSalida[2] = '2';
            m_arCadenaSalida[3] = SEP;
            m_arCadenaSalida[4] = '1';
            m_arCadenaSalida[5] = SEP;

            m_arCadenaSalida[6] = 'S';
            m_arCadenaSalida[7] = SEP;
            m_arCadenaSalida[8] = 'N';
            m_arCadenaSalida[9] = SEP;

            m_arCadenaSalida[10] = m_szParametros[2]; // Modelo
            m_arCadenaSalida[11] = m_szParametros[3];
            m_arCadenaSalida[12] = m_szParametros[4];
            m_arCadenaSalida[13] = m_szParametros[5];
            m_arCadenaSalida[14] = m_szParametros[6];
            m_arCadenaSalida[15] = m_szParametros[7];
            m_arCadenaSalida[16] = m_szParametros[8];
            m_arCadenaSalida[17] = m_szParametros[9];
            m_arCadenaSalida[18] = BLANCO;
            m_arCadenaSalida[19] = m_szParametros[11]; // Talla
            m_arCadenaSalida[20] = m_szParametros[12];
            m_arCadenaSalida[21] = BLANCO;
            m_arCadenaSalida[22] = m_szParametros[14]; // Color
            m_arCadenaSalida[23] = m_szParametros[15];
            m_arCadenaSalida[24] = BARRA_DOS;

            // Descripci¢n
            for(i=17, j=25;
               ((m_szParametros[i] != SEPARADOR) &&
               (i < 57)) ; i++, j++)
            {
                m_arCadenaSalida[j] = m_szParametros[i];
            }

            m_arCadenaSalida[j] = SEP; j++;

            // Importe de la l¡nea
            for(i=137; (m_szParametros[i] == '0'); i++);

            if(m_szParametros[i] == COMA)
            {
               m_arCadenaSalida[j] = '0'; j++;
            }

            for(; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];
            j--; // Eliminamos el tercer decimal

            m_arCadenaSalida[j] = SEP; j++;
            m_arCadenaSalida[j] = m_szParametros[135]; j++; // Cat. VAT

            m_arCadenaSalida[j] = SEP; j++;
            m_arCadenaSalida[j] = END; j++;
            m_arCadenaSalida[j] = NULL;
        }
    }
    else
    {
        m_nError = 535;
        return;
    }

    EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);

    POLONIA_QueryE();
    if(m_nError)
       POLONIA_Testeo();

}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Pagofi()
{
    UINT i, j;

    if(m_bBoletaAbierta)
    {
        m_arCadenaSalida[0] = ESC;
        m_arCadenaSalida[1] = MFB;
        m_arCadenaSalida[2] = '8';
        m_arCadenaSalida[3] = SEP;

        // Descripcion del pago
        for(i=5, j=4;
           ((m_szParametros[i] != SEPARADOR) &&
           (i < 45)); j++, i++)
        {
            m_arCadenaSalida[j] = m_szParametros[i];
        }

        m_arCadenaSalida[j] = SEP; j++;

        // Importe del pago
        for(i=45; (m_szParametros[i] == '0'); i++);

        for(; (m_szParametros[i] != SEPARADOR); i++, j++)
           m_arCadenaSalida[j] = m_szParametros[i];
        j--; // Eliminamos el tercer decimal

        m_arCadenaSalida[j] = SEP; j++;

        m_arCadenaSalida[j] = m_szParametros[1]; // Tipo de pago
        j++;

        m_arCadenaSalida[j] = SEP; j++;
        m_arCadenaSalida[j] = 'N'; j++;
        m_arCadenaSalida[j] = SEP; j++;
        m_arCadenaSalida[j] = END; j++;

        m_arCadenaSalida[j] = NULL;

        EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);

        POLONIA_QueryE();
        if(m_nError)
           POLONIA_Testeo();
    }
    else
    {
        m_nError = 536;
    }
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Cierrz()
{
    if(!m_bBoletaAbierta)
    {
        m_arCadenaSalida[0] = ESC;
        m_arCadenaSalida[1] = MFB;
        m_arCadenaSalida[2] = 'B';
        m_arCadenaSalida[3] = SEP;
        m_arCadenaSalida[4] = END;
        m_arCadenaSalida[5] = NULL;

        EnviaCadena(m_arCadenaSalida, 5, NULL, NULL, NULL, FALSE, NULL);

        //POLONIA_Testeo();
    }
    else m_nError = 539;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Infonf() // Reports
{
    UCHAR arNumeroInforme[3], arNumeroLinea[3];
    UINT  i, j;

    arNumeroInforme[0] = NULL;
    arNumeroLinea[0]   = m_szParametros[3];
    arNumeroLinea[1]   = m_szParametros[4];
    arNumeroLinea[2]   = NULL;

    if(!m_bBoletaAbierta)
    {
        if(!strcmp((char*)arNumeroLinea, "FF"))
        {
            POLONIA_Cierra_report();
            POLONIA_Testeo();
        }
        else
        {
            arNumeroInforme[0] = m_szParametros[0];
            arNumeroInforme[1] = m_szParametros[1];
            arNumeroInforme[2] = NULL;

            m_arCadenaSalida[0] = ESC;
            m_arCadenaSalida[1] = MFB;
            m_arCadenaSalida[2] = 'N';
            m_arCadenaSalida[3] = SEP;
            m_arCadenaSalida[4] = arNumeroInforme[0];
            m_arCadenaSalida[5] = arNumeroInforme[1];
            m_arCadenaSalida[6] = SEP;
            m_arCadenaSalida[7] = arNumeroLinea[0];
            m_arCadenaSalida[8] = arNumeroLinea[1];
            m_arCadenaSalida[9] = SEP;

            // Parametro texto
            for(i=6,j=10; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];

            if(j != 10) // Eliminar espacios a la derecha
            {
                for(j--; (m_arCadenaSalida[j] == BLANCO); j--) ;
                j++;
            }

            m_arCadenaSalida[j] = SEP; j++;
            m_arCadenaSalida[j] = END; j++;
            m_arCadenaSalida[j] = NULL;

            EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);
            POLONIA_QueryE();
            if(m_nError)
               POLONIA_Testeo();
        }
    }
    else m_nError = 539;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Cancel()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFB;
    m_arCadenaSalida[2] = 'A';
    m_arCadenaSalida[3] = SEP;
    m_arCadenaSalida[4] = END;
    m_arCadenaSalida[5] = NULL;

    EnviaCadena(m_arCadenaSalida, 5, NULL, NULL, NULL, FALSE, NULL);

    POLONIA_QueryE();
    if(m_nError)       // Si no conseguimos cancelar,
       POLONIA_Cierra_report(); // intentamos cerrar un listado predefinido
                        /* NOTA: Esto puede finalmente derivar en error 86,
                           obligando a apagar el equipo. */
    POLONIA_Testeo();
}
//---------------------------------------------------------------------------------------------------

/*void cImpre::POLONIA_Testea()
{
    UCHAR arsNumero3[4];
    UINT  i, j;

    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = '+';
    m_arCadenaSalida[2] = 'A';
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 14, NULL, TRUE, NULL);

    ComponeComando(g_Comandos[m_nIndComando]);

    for(i=5, j=13; i<14; i++)
    {
        sprintf((char*)arsNumero3, "%03d", (int)m_arRespuesta[i]);
        m_szRespuesta[j] = arsNumero3[0]; j++;
        m_szRespuesta[j] = arsNumero3[1]; j++;
        m_szRespuesta[j] = arsNumero3[2]; j++;
        m_szRespuesta[j] = SEPARADOR; j++;
    }
    m_szRespuesta[j] = FIN_MNG; j++;
    m_szRespuesta[j] = NULL;
}*/
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Testhw()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFB;
    m_arCadenaSalida[2] = 'X';
    m_arCadenaSalida[3] = SEP;
    m_arCadenaSalida[4] = m_szParametros[0];
    m_arCadenaSalida[5] = SEP;
    m_arCadenaSalida[6] = END;
    m_arCadenaSalida[7] = NULL;

    EnviaCadena(m_arCadenaSalida, 7, NULL, NULL, NULL, FALSE, NULL);

    POLONIA_Testeo();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Abreca()
{
    m_arCadenaSalida[0] = FS;
    m_arCadenaSalida[1] = NULL;

    EnviaCadena(m_arCadenaSalida, 1, NULL, NULL, NULL, FALSE, NULL);

    POLONIA_Testeo();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Numero()
{
	  enum { N = 20 };
    UCHAR arNumero20[N];
    UINT  i, j;

    m_arCadenaSalida[0]  = ESC;
    m_arCadenaSalida[1]  = MFB;
    m_arCadenaSalida[2]  = 'U';
    m_arCadenaSalida[3]  = SEP;
    m_arCadenaSalida[4]  = 'L';
    m_arCadenaSalida[5]  = SEP;//page 252

    sprintf((char*)arNumero20, "%u", 0x9002);
    for(i=0, j=6; (arNumero20[i] != '\0'); i++,j++)
       m_arCadenaSalida[j] = arNumero20[i];

    m_arCadenaSalida[j] = SEP; j++;
    m_arCadenaSalida[j] = END; j++;
    m_arCadenaSalida[j] = NULL;

    EnviaCadena(m_arCadenaSalida, j, NULL, NULL, SEP, TRUE, NULL);

    ComponeComando(g_Comandos[m_nIndComando]);

    // Avanzamos hasta el primer delimitador
    for(i=0; (m_arRespuesta[i] != SEP_RESPUESTA); i++);
    i++;

    // Cargamos el valor devuelto hasta el siguiente delimitador
    for(j=0; (m_arRespuesta[i] != SEP_RESPUESTA); i++, j++)
       arNumero20[j] = m_arRespuesta[i];
    arNumero20[j] = NULL;
#if 0    
    printf("\nHello from POLONIA_Numero !!!\narNumero20 = {%s}\n, j = [%d]\n", (char *)arNumero20, j);getchar();

	if(strchr((char *)arNumero20, '-') != NULL) {//Parche de signed short -> unsigned short
	    printf("BEFORE the parch: arNumero20 = {%s}\n, j = [%d]\n", (char *)arNumero20, j);getchar();
        unsigned char arr1[N]; 
		unsigned char* p = (unsigned char *)arNumero20; unsigned char* q = (unsigned char *)arr1;
		while(*p == '0') { ++p; }
		while((*q++ = *p++) != 0x00) { }

		short l = atoi((char *)arr1);
		unsigned short ul = (unsigned short)l;
		memset((void *)arNumero20, 0x00, N);
		sprintf((char *)arNumero20, "%d", ul);
		j = strlen((char *)arNumero20);  
		//printf("AFTER the parch: arNumero20 = {%s}\n, j = [%d]\n", (char *)arNumero20, j);getchar();
	}
#endif
    // Rellenamos el destino con ceros por la dif. de longitud
    for(i=13, j=15; (j>strlen((char*)arNumero20)); j--, i++)
       m_szRespuesta[i] = '0';

    // Cargamos el valor al string destino
    for(j=0; (arNumero20[j] != NULL); i++, j++)
        m_szRespuesta[i] = arNumero20[j];

    m_szRespuesta[i] = SEPARADOR; i++;
    m_szRespuesta[i] = FIN_MNG; i++;
    m_szRespuesta[i] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Disply()
{
    UINT i, j;

    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFB;
    m_arCadenaSalida[2] = 'W';
    m_arCadenaSalida[3] = SEP;

    for(j=4, i=0; (m_szParametros[i] != SEPARADOR); i++, j++)
       m_arCadenaSalida[j] = m_szParametros[i];


    m_arCadenaSalida[j] = SEP; j++;
    m_arCadenaSalida[j] = SEP; j++;
    m_arCadenaSalida[j] = END; j++;
    m_arCadenaSalida[j] = NULL;

    for(i=0; (m_szParametros[i] != SEPARADOR); i++);
    i++;

    j -= 2;

    for(; (m_szParametros[i] != SEPARADOR); i++, j++)
        m_arCadenaSalida[j] = m_szParametros[i];

    m_arCadenaSalida[j] = SEP; j++;
    m_arCadenaSalida[j] = END; j++;
    m_arCadenaSalida[j] = NULL;

    EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Inicia()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFB;
    m_arCadenaSalida[2] = 'H';
    m_arCadenaSalida[3] = SEP;
    m_arCadenaSalida[4] = 'P';  // Set terminal number (hasta 5 caracteres)
    m_arCadenaSalida[5] = SEP;
    m_arCadenaSalida[6] = 'A';
    m_arCadenaSalida[7] = 'B';
    m_arCadenaSalida[8] = ' ';
    m_arCadenaSalida[9] = '0';
    m_arCadenaSalida[10] = '1';
    m_arCadenaSalida[11] = SEP;
    m_arCadenaSalida[12] = END;
    m_arCadenaSalida[13] = NULL;

    EnviaCadena(m_arCadenaSalida, 13, NULL, NULL, NULL, FALSE, NULL);

    POLONIA_Testeo();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA_Procesa()
{
    switch (m_nIndComando)
    {
        case 0: // TESTEO
             POLONIA_Testeo();
             break;
        case 1: // NUMCAJ
             POLONIA_Numcaj();
             break;
        case 2: // NUMTIQ
             POLONIA_Numtiq();
             break;
        case 3: // FECHAF
             POLONIA_Fechaf();
             break;
        case 4: // HORAFI
             POLONIA_Horafi();
             break;
        case 5: // NUMBOL
             break;
        case 6: // FECHAB
             break;
        case 7: // HORABO
             break;
        case 8: // TOTALB
             POLONIA_Totalb();
             break;
        case 9: // CABECB
             POLONIA_Cabecb();
             break;
        case 10:// LINDEB
             POLONIA_Lindeb();
             break;
        case 11:// PAGOFI
             POLONIA_Pagofi();
             break;
        case 12:// CIERRZ
             POLONIA_Cierrz();
             break;
        case 13:// INFONF
             POLONIA_Infonf();
             break;
        case 14:// CANCEL
             POLONIA_Cancel();
             break;
        case 15:// PIEFIS
             break;
        case 16:// TESTEA
             //POLONIA_Testea();
             break;
        case 17:// TESTHW
             POLONIA_Testhw();
             break;
        case 18:// ABRECA
             POLONIA_Abreca();
             break;
        case 19:// NUMERO
             POLONIA_Numero();
             break;
        case 20:// DISPLY
             POLONIA_Disply();
             break;
        case 21:// TRASPA
             break;
        case 22:// TEXLIB
             break;
        case 23:// LIBERA
             break;
        case 24:// INICIA
             //POLONIA_Inicia();
             break;
        case 25:// LECTUX
             break;
        default:// ­­FUERA DE MADRE!!
             m_nError = 599;
             break;
    }
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

//
// POLONIA2
// ==================
//
// cImpre::MFCaError
// =================
//     Pasar error del MF al objeto cImpre
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void cImpre::POLONIA2_MFCaError()
{
    UCHAR arsNumero2[3];

    m_cClaseError = 0x30;

    if(m_nNumCarRespuesta)
    {
        sprintf((char*)arsNumero2, "%02X", m_arRespuesta[5]);
        m_nError = atoi((char*)arsNumero2);
        sprintf((char*)arsNumero2, "%02X", m_arRespuesta[4]);
        m_nEstado = atoi((char*)arsNumero2);
    }
    else m_nError = 551;    // Error 551: No hay respuesta del dispositivo

}
//---------------------------------------------------------------------------------------------------

// cImpre::QueryE
// ==============
//     Interroga a la impresora por el ultimo error y pasa el error
// al objeto 'cImpre'.
//
// Parametros de entrada: ninguno.
//
// Parametros de salida: ninguno.
//
void cImpre::POLONIA2_QueryE()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = '+';
    m_arCadenaSalida[2] = 0x09;
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 6, NULL, TRUE, NULL);

    POLONIA2_MFCaError();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Testeo()
{
    POLONIA2_QueryE();
    ComponeError();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Numero()
{
    char* sz_grn_printouts       = "grn_n.printouts";
    char* sz_day_printouts       = "day_n.printouts";
    char* sz_day_printouts_async = "day_n.printouts_async";
    char arNumero[11];
    UINT i,j,k,bValor,iVeces;
    //long lNumero1, lNumero2;
    unsigned long lNumero1, lNumero2;  
    
    //printf("\nHello from POLONIA2_Numero !!!\n");getchar();
    
    iVeces = 0;
 //   
 grn_printouts:
 //
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'L';
    m_arCadenaSalida[3] = 'T';
    for(i=4; *sz_grn_printouts !=NULL; i++)
        m_arCadenaSalida[i] = *sz_grn_printouts++;
    m_arCadenaSalida[i] = ESC; i++;
    m_arCadenaSalida[i] = MFEX; i++;
    m_arCadenaSalida[i] = NULL;
    
    EnviaCadena(m_arCadenaSalida, i, NULL, 10, NULL, TRUE, NULL);

    j = 6; k = 10;
    if(m_arRespuesta[0] == 0x06)
    {
       j = 7;
       k = 11;
    }
    m_nError = 0;
    for(i=0; i<k; i++, j++)
    {
        if(!isdigit(m_arRespuesta[j]))
        {
           m_cClaseError = '1';
           m_nError = 534; // Error de formato en la respuesta
        }
        else arNumero[i] = m_arRespuesta[j];
    }
    arNumero[i] = NULL;
    if(m_nError)
    {
       if(iVeces)
       {
          ComponeError();
          return;
       }
       else
       {
          iVeces++;
          goto grn_printouts;
       }
    }
    lNumero1 = atol(arNumero);
    
    iVeces = 0;
//
day_printouts:
//                         
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'L';
    m_arCadenaSalida[3] = 'T';
    for(i=4; *sz_day_printouts !=NULL; i++)
        m_arCadenaSalida[i] = *sz_day_printouts++;
    m_arCadenaSalida[i] = ESC; i++;
    m_arCadenaSalida[i] = MFEX; i++;
    m_arCadenaSalida[i] = NULL;
    
    EnviaCadena(m_arCadenaSalida, i, NULL, 10, NULL, TRUE, NULL);

    j = 6; k = 10;
    if(m_arRespuesta[0] == 0x06)
    {
       j = 7;
       k = 11;
    }
    for(i=0; i<k; i++, j++)
    {
        if(!isdigit(m_arRespuesta[j]))
        {
           m_cClaseError = '1';
           m_nError = 534; // Error de formato en la respuesta
        }
        else arNumero[i] = m_arRespuesta[j];
    }
    arNumero[i] = NULL;
    if(m_nError)
    {
        if(iVeces)
        {
            ComponeError();
            return;
        }
        else
        {
            iVeces++;
            goto day_printouts;
        }
    }
    lNumero2 = atol(arNumero);
    lNumero1 += lNumero2;
                     
    iVeces = 0;
//
day_async:
//    
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'L';
    m_arCadenaSalida[3] = 'T';
    for(i=4; *sz_day_printouts_async !=NULL; i++)
        m_arCadenaSalida[i] = *sz_day_printouts_async++;
    m_arCadenaSalida[i] = ESC; i++;
    m_arCadenaSalida[i] = MFEX; i++;
    m_arCadenaSalida[i] = NULL;
    
    EnviaCadena(m_arCadenaSalida, i, NULL, 10, NULL, TRUE, NULL);

    j = 6; k = 10;
    if(m_arRespuesta[0] == 0x06)
    {
       j = 7;
       k = 11;
    }
    for(i=0; i<k; i++, j++)
    {
        if(!isdigit(m_arRespuesta[j]))
        {
           m_cClaseError = '1';
           m_nError = 534; // Error de formato en la respuesta
        }
        else arNumero[i] = m_arRespuesta[j];
    }
    arNumero[i] = NULL;
    if(m_nError)
    {
        if(iVeces)
        {
            ComponeError();
            return;
        }
        else
        {
            iVeces++;
            goto day_async;
        }   
    }
    lNumero2 = atol(arNumero);
    lNumero1 += lNumero2;
                     
    ComponeComando(g_Comandos[m_nIndComando]);

    for(i=0,j=13; i<5; i++,j++)
        m_szRespuesta[j] = '0';   
         
 #if 0       
     printf("BEFORE: lNumero1 = [%ld]\n", lNumero1); getchar();
     if(lNumero1 < 0) { 
         short sh = (short)lNumero1; 
         unsigned short ush = (unsigned short)sh;  
         lNumero1 = ush;
     } 
     printf("AFTER: lNumero1 = [%ld]\n", lNumero1); getchar();
#endif
        
    //sprintf((char*)arNumero, "%10d", lNumero1);//bugfixed311007 
    sprintf((char*)arNumero, "%10ld", lNumero1);

    bValor = 0;
    for(i=0; i<10; i++, j++)
    {
        if((arNumero[i] == ' ') && (!bValor))
            m_szRespuesta[j] = '0';
        else {
            m_szRespuesta[j] = arNumero[i];
            bValor = 1;
        }
    }

    m_szRespuesta[j] = SEPARADOR; j++;
    m_szRespuesta[j] = FIN_MNG; j++;
    m_szRespuesta[j] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Numcaj()
{
    ComponeComando(g_Comandos[m_nIndComando]);

    m_szRespuesta[13] = '0';
    m_szRespuesta[14] = '0';
    m_szRespuesta[15] = SEPARADOR;
    m_szRespuesta[16] = FIN_MNG;
    m_szRespuesta[17] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Fechaf()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = '+';
    m_arCadenaSalida[2] = 0x0A;
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 34, NULL, TRUE, NULL);

    ComponeComando(g_Comandos[m_nIndComando]);

    m_szRespuesta[13] = m_arRespuesta[16];
    m_szRespuesta[14] = m_arRespuesta[17];
    m_szRespuesta[15] = m_arRespuesta[14];
    m_szRespuesta[16] = m_arRespuesta[15];
    m_szRespuesta[17] = m_arRespuesta[12];
    m_szRespuesta[18] = m_arRespuesta[13];

    m_szRespuesta[19] = SEPARADOR;
    m_szRespuesta[20] = FIN_MNG;
    m_szRespuesta[21] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Horafi()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = '+';
    m_arCadenaSalida[2] = 0x0A;
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 34, NULL, TRUE, NULL);

    ComponeComando(g_Comandos[m_nIndComando]);

    m_szRespuesta[13] = m_arRespuesta[10];
    m_szRespuesta[14] = m_arRespuesta[11];
    m_szRespuesta[15] = m_arRespuesta[8];
    m_szRespuesta[16] = m_arRespuesta[9];
    m_szRespuesta[17] = m_arRespuesta[6];
    m_szRespuesta[18] = m_arRespuesta[7];
    m_szRespuesta[19] = '0';
    m_szRespuesta[20] = '0';

    m_szRespuesta[21] = SEPARADOR;
    m_szRespuesta[22] = FIN_MNG;
    m_szRespuesta[23] = NULL;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Cabecb()
{
    UCHAR arNumero2[3];
    UINT  nNumero;

    // Obtener parametro PP
    arNumero2[0] = m_szParametros[0];
    arNumero2[1] = m_szParametros[1];
    arNumero2[2] = NULL;

    nNumero = atoi((char*)arNumero2);

    if(nNumero == 1)
    {
        m_arCadenaSalida[0] = ESC;
        m_arCadenaSalida[1] = MFBX;
        m_arCadenaSalida[2] = 'C';
        m_arCadenaSalida[3] = ESC;
        m_arCadenaSalida[4] = MFEX;
        m_arCadenaSalida[5] = NULL;

        EnviaCadena(m_arCadenaSalida, 5, NULL, 1, NULL, TRUE, NULL);

        POLONIA2_QueryE();
        if(m_nError)
        {
            POLONIA2_Testeo();
            return;
        }

        m_bBoletaAbierta = TRUE;
    }
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Lindeb()
{
    UCHAR arNumero20[21];
    UINT  nNumero, i, j;

    if(m_bBoletaAbierta)
    {
        if(m_szParametros[0] == '+')
        {
            m_arCadenaSalida[0] = ESC;
            m_arCadenaSalida[1] = MFBX;
            m_arCadenaSalida[2] = 'D';

            m_arCadenaSalida[3]  = m_szParametros[2]; // Modelo
            m_arCadenaSalida[4]  = m_szParametros[3];
            m_arCadenaSalida[5]  = m_szParametros[4];
            m_arCadenaSalida[6]  = m_szParametros[5];
            m_arCadenaSalida[7]  = m_szParametros[6];
            m_arCadenaSalida[8]  = m_szParametros[7];
            m_arCadenaSalida[9]  = m_szParametros[8];
            m_arCadenaSalida[10] = m_szParametros[9];
            m_arCadenaSalida[11] = BLANCO;
            m_arCadenaSalida[12] = m_szParametros[11]; // Talla
            m_arCadenaSalida[13] = m_szParametros[12];
            m_arCadenaSalida[14] = BLANCO;
            m_arCadenaSalida[15] = m_szParametros[14]; // Color
            m_arCadenaSalida[16] = m_szParametros[15];

            m_arCadenaSalida[17] = NULL;

            arNumero20[0] = m_szParametros[59]; // Cantidad
            arNumero20[1] = m_szParametros[60];
            arNumero20[2] = m_szParametros[61];
            arNumero20[3] = NULL;

            nNumero = atoi((char*)arNumero20);

            sprintf((char*)arNumero20, "%d", nNumero);

            for(i=0,j=18; (arNumero20[i] != NULL); i++,j++)
               m_arCadenaSalida[j] = arNumero20[i];

            m_arCadenaSalida[j] = BLANCO; j++;
            m_arCadenaSalida[j] = '*'; j++;
            m_arCadenaSalida[j] = BLANCO; j++;

            for(i=63; (m_szParametros[i] == '0'); i++);

            if(m_szParametros[i] == COMA)
            {
               m_arCadenaSalida[j] = '0'; j++;
            }
            // Precio
            for(; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];
            j--; // Eliminamos el tercer decimal

            // S¡mbolo moneda
            for(char* szMoneda = " zl "; (*szMoneda != NULL); j++)
               m_arCadenaSalida[j] = *szMoneda++;

            // Descripcion
            for(i=17; i<37; i++, j++)
            {
                m_arCadenaSalida[j] = m_szParametros[i];
            }


            m_arCadenaSalida[j] = ESC; j++;
            m_arCadenaSalida[j] = MFB1X; j++;
            m_arCadenaSalida[j] = 'a'; j++;

            // Importe de la linea
            for(i=137; (m_szParametros[i] == '0'); i++);

            if(m_szParametros[i] == COMA)
            {
               m_arCadenaSalida[j] = '0'; j++;
            }

            for(; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];
            j--; // Eliminamos el tercer decimal

            m_arCadenaSalida[j] = ESC; j++;
            m_arCadenaSalida[j] = MFB2X; j++;
            m_arCadenaSalida[j] = m_szParametros[135]; j++; // Cat. VAT

            m_arCadenaSalida[j] = ESC; j++;
            m_arCadenaSalida[j] = MFEX; j++;
            m_arCadenaSalida[j] = NULL;
        }
        else // item negativo
        {
            m_arCadenaSalida[0] = ESC;
            m_arCadenaSalida[1] = MFBX;
            m_arCadenaSalida[2] = 'D';

            m_arCadenaSalida[3]  = m_szParametros[2]; // Modelo
            m_arCadenaSalida[4]  = m_szParametros[3];
            m_arCadenaSalida[5]  = m_szParametros[4];
            m_arCadenaSalida[6]  = m_szParametros[5];
            m_arCadenaSalida[7]  = m_szParametros[6];
            m_arCadenaSalida[8]  = m_szParametros[7];
            m_arCadenaSalida[9]  = m_szParametros[8];
            m_arCadenaSalida[10] = m_szParametros[9];
            m_arCadenaSalida[11] = BLANCO;
            m_arCadenaSalida[12] = m_szParametros[11]; // Talla
            m_arCadenaSalida[13] = m_szParametros[12];
            m_arCadenaSalida[14] = BLANCO;
            m_arCadenaSalida[15] = m_szParametros[14]; // Color
            m_arCadenaSalida[16] = m_szParametros[15];

            m_arCadenaSalida[17] = NULL;

            arNumero20[0] = m_szParametros[58]; // Cantidad
            arNumero20[0] = m_szParametros[59];
            arNumero20[1] = m_szParametros[60];
            arNumero20[2] = m_szParametros[61];
            arNumero20[3] = NULL;

            nNumero = atoi((char*)arNumero20);

            sprintf((char*)arNumero20, "%d", nNumero);

            for(i=0,j=18; (arNumero20[i] != NULL); i++,j++)
               m_arCadenaSalida[j] = arNumero20[i];

            m_arCadenaSalida[j] = BLANCO; j++;
            m_arCadenaSalida[j] = '*'; j++;
            m_arCadenaSalida[j] = BLANCO; j++;

            for(i=63; (m_szParametros[i] == '0'); i++);

            if(m_szParametros[i] == COMA)
            {
               m_arCadenaSalida[j] = '0'; j++;
            }
            // Precio
            for(; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];
            j--; // Eliminamos el tercer decimal

            // S¡mbolo moneda
            for(char* szMoneda = " zl "; (*szMoneda != NULL); j++)
               m_arCadenaSalida[j] = *szMoneda++;

            // Descripcion
            for(i=17; i<37; i++, j++)
            {
                m_arCadenaSalida[j] = m_szParametros[i];
            }


            m_arCadenaSalida[j] = ESC; j++;
            m_arCadenaSalida[j] = MFB1X; j++;
            m_arCadenaSalida[j] = 'c'; j++;
            m_arCadenaSalida[j] = '-'; j++;

            // Importe de la linea
            for(i=137; (m_szParametros[i] == '0'); i++);

            if(m_szParametros[i] == COMA)
            {
               m_arCadenaSalida[j] = '0'; j++;
            }

            for(; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];
            j--; // Eliminamos el tercer decimal

            m_arCadenaSalida[j] = ESC; j++;
            m_arCadenaSalida[j] = MFB2X; j++;
            m_arCadenaSalida[j] = m_szParametros[135]; j++; // Cat. VAT

            m_arCadenaSalida[j] = ESC; j++;
            m_arCadenaSalida[j] = MFEX; j++;
            m_arCadenaSalida[j] = NULL;
        }
    }
    else
    {
        m_nError = 535;
        return;
    }

   EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);

    POLONIA2_QueryE();
    if(m_nError)
       POLONIA2_Testeo();

}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Cierra_report()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'N';
    m_arCadenaSalida[3] = ESC;
    m_arCadenaSalida[4] = MFEX;
    m_arCadenaSalida[5] = NULL;

    EnviaCadena(m_arCadenaSalida, 5, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Cancel()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'T';
    m_arCadenaSalida[3] = ESC;
    m_arCadenaSalida[4] = MFB1X;
    m_arCadenaSalida[5] = 'c';
    m_arCadenaSalida[6] = ESC;
    m_arCadenaSalida[7] = MFEX;
    m_arCadenaSalida[8] = NULL;

    EnviaCadena(m_arCadenaSalida, 8, NULL, NULL, NULL, FALSE, NULL);

    POLONIA2_QueryE();
    if(m_nError)       // Si no conseguimos cancelar,
       POLONIA2_Cierra_report(); // intentamos cerrar un listado predefinido
    POLONIA2_Testeo();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Pagofi()
{
    UINT i, j;

    if(m_bBoletaAbierta)
    {
        m_arCadenaSalida[0] = ESC; // Transaction total
        m_arCadenaSalida[1] = MFBX;
        m_arCadenaSalida[2] = 'T';
        m_arCadenaSalida[3] = ESC;
        m_arCadenaSalida[4] = MFB1X;
        m_arCadenaSalida[5] = 'a';

        for(j=6, i=0; (m_szTotal[i] != NULL); i++,j++)
            m_arCadenaSalida[j] = m_szTotal[i];

        m_arCadenaSalida[j] = ESC; j++;
        m_arCadenaSalida[j] = MFEX; j++;
        m_arCadenaSalida[j] = NULL;

        EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);

        POLONIA2_QueryE();
        if(m_nError)
           POLONIA2_Testeo();

        m_arCadenaSalida[0] = ESC; // Trailer
        m_arCadenaSalida[1] = MFBX;
        m_arCadenaSalida[2] = 'R';

        switch(m_szParametros[1]) // Tipo de pago
        {
            case '1': m_arCadenaSalida[3] = 'E'; break; // Efectivo
            case '2': m_arCadenaSalida[3] = 'F'; break; // Cheque
            case '3': m_arCadenaSalida[3] = 'G'; break; // Tarjeta
            case '4': m_arCadenaSalida[3] = 'H'; break; // Vale
            case '5': m_arCadenaSalida[3] = 'K'; break; // Credito
            case '6': m_arCadenaSalida[3] = 'O'; break; // Divisa
            default: m_arCadenaSalida[3] = NULL; break;
        }


        // Descripcion del pago
        for(i=5, j=4;
             (m_szParametros[i] != SEPARADOR) &&
             (i < 15); j++, i++)
        {
           m_arCadenaSalida[j] = m_szParametros[i];
        }

        m_arCadenaSalida[j] = BLANCO; j++;

        // Importe del pago
        for(i=45; (m_szParametros[i] == '0'); i++);

        for(; (m_szParametros[i] != SEPARADOR); i++, j++)
           m_arCadenaSalida[j] = m_szParametros[i];
        j--; // Eliminamos el tercer decimal

        m_arCadenaSalida[j] = ESC; j++;
        m_arCadenaSalida[j] = MFEX; j++;
        m_arCadenaSalida[j] = NULL;

        EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);
        POLONIA2_Testeo();
    }
    else
    {
        m_nError = 536;
    }
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Totalb()
{
    UINT i, j;

    if(m_bBoletaAbierta)
    {
        m_arCadenaSalida[0] = ESC; // End of sale
        m_arCadenaSalida[1] = MFBX;
        m_arCadenaSalida[2] = 'E';
        m_arCadenaSalida[3] = ESC;
        m_arCadenaSalida[4] = MFEX;
        m_arCadenaSalida[5] = NULL;

        EnviaCadena(m_arCadenaSalida, 5, NULL, NULL, NULL, FALSE, NULL);

        POLONIA2_QueryE();
        if(m_nError)
           POLONIA2_Testeo();
    }
    else {
        for(i=2; (m_szParametros[i] == '0'); i++);

        j = 0;

        if(m_szParametros[i] == COMA)
        {
           m_szTotal[j] = '0'; j++;
        }

        for(; m_szParametros[i] != SEPARADOR; j++,i++)
            m_szTotal[j] = m_szParametros[i];
        j--; // Eliminamos el tercer decimal
        m_szTotal[j] = NULL;
    }
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Cierrz()
{
    if(!m_bBoletaAbierta)
    {
        m_arCadenaSalida[0] = ESC;
        m_arCadenaSalida[1] = MFBX;
        m_arCadenaSalida[2] = 'B';
        m_arCadenaSalida[3] = ESC;
        m_arCadenaSalida[4] = MFEX;
        m_arCadenaSalida[5] = NULL;

        EnviaCadena(m_arCadenaSalida, 5, NULL, NULL, NULL, FALSE, NULL);
    }
    else m_nError = 539;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Abreca()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'X';
    m_arCadenaSalida[3] = '1';
    m_arCadenaSalida[4] = '9';
    m_arCadenaSalida[5] = ESC;
    m_arCadenaSalida[6] = MFEX;
    m_arCadenaSalida[7] = NULL;

    EnviaCadena(m_arCadenaSalida, 7, NULL, NULL, NULL, FALSE, NULL);

    POLONIA2_QueryE();
    if(m_nError)
       POLONIA2_Testeo();
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Infonf() // Reports
{
    UCHAR arNumeroInforme[3], arNumeroLinea[3];
    UINT  i, j, k;

    arNumeroInforme[0] = NULL;
    arNumeroLinea[0]   = m_szParametros[3];
    arNumeroLinea[1]   = m_szParametros[4];
    arNumeroLinea[2]   = NULL;

    if(!m_bBoletaAbierta)
    {
        if(!strcmp((char*)arNumeroLinea, "FF"))
        {
            POLONIA2_Cierra_report();
            POLONIA2_Testeo();
        }
        else
        {
            arNumeroInforme[0] = m_szParametros[0];
            arNumeroInforme[1] = m_szParametros[1];
            arNumeroInforme[2] = NULL;

            m_arCadenaSalida[0] = ESC;
            m_arCadenaSalida[1] = MFBX;
            m_arCadenaSalida[2] = 'Z';
            m_arCadenaSalida[3] = arNumeroInforme[0];
            m_arCadenaSalida[4] = arNumeroInforme[1];
            m_arCadenaSalida[5] = arNumeroLinea[0];
            m_arCadenaSalida[6] = arNumeroLinea[1];

            // Parametro texto
            for(i=6,j=7; (m_szParametros[i] != SEPARADOR); i++, j++)
               m_arCadenaSalida[j] = m_szParametros[i];

            if(j != 7) // Eliminar espacios a la derecha
            {
               for(j--; (m_arCadenaSalida[j] == BLANCO); j--) ;
               j++;
            }
            m_arCadenaSalida[j] = ESC; j++;
            m_arCadenaSalida[j] = MFEX; j++;
            m_arCadenaSalida[j] = NULL;

            EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);

            if((!strcmp((char*)arNumeroLinea, "08")) &&
                 (!strcmp((char*)arNumeroInforme, "0A")))
            {
                for(k=1; k<3; k++)
                {
                    m_arCadenaSalida[0] = ESC;
                    m_arCadenaSalida[1] = MFBX;
                    m_arCadenaSalida[2] = 'Z';
                    m_arCadenaSalida[3] = arNumeroInforme[0];
                    m_arCadenaSalida[4] = arNumeroInforme[1];
                    m_arCadenaSalida[5] = '0';
                    m_arCadenaSalida[6] = '8';
                    m_arCadenaSalida[7] = ESC;
                    m_arCadenaSalida[8] = MFEX;
                    m_arCadenaSalida[9] = NULL;

                    EnviaCadena(m_arCadenaSalida, 9, NULL, NULL, NULL, FALSE, NULL);
                }
            }
            POLONIA2_QueryE();
            if(m_nError)
               POLONIA2_Testeo();
        }
    }
    else m_nError = 539;
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Testhw()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'L';
    m_arCadenaSalida[3] = m_szParametros[0];
    m_arCadenaSalida[4] = ESC;
    m_arCadenaSalida[5] = MFEX;
    m_arCadenaSalida[6] = NULL;

    EnviaCadena(m_arCadenaSalida, 6, NULL, 30, NULL, TRUE, NULL);
}
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Disply()
{
    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'G';
    m_arCadenaSalida[3] = ESC;
    m_arCadenaSalida[4] = MFB1X;
    m_arCadenaSalida[5] = '0';

    for(UINT j = 6, i = 0; m_szParametros[i] != SEPARADOR; ++i, ++j) {
       m_arCadenaSalida[j] = m_szParametros[i];
	}


    m_arCadenaSalida[j++] = ESC;
    m_arCadenaSalida[j++] = MFEX;
    m_arCadenaSalida[j] = NULL;

    EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);

    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'G';
    m_arCadenaSalida[3] = ESC;
    m_arCadenaSalida[4] = MFB1X;
    m_arCadenaSalida[5] = '1';

    for(i = 0; (m_szParametros[i] != SEPARADOR); i++) { }
    
	++i;

    for(j = 6; m_szParametros[i] != SEPARADOR; ++i, ++j) { 
		m_arCadenaSalida[j] = m_szParametros[i]; 
	}

    m_arCadenaSalida[j++] = ESC;
    m_arCadenaSalida[j++] = MFEX;
    m_arCadenaSalida[j] = NULL;

    EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);
}
//---------------------------------------------------------------------------------------------------

/*void cImpre::POLONIA2_Inicia()
{
    UINT i=0, j=3, k;

    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = MFBX;
    m_arCadenaSalida[2] = 'h';


    for(k=1; k<7; k++)
    {
        for(; (m_szParametros[i] != SEPARADOR); i++, j++)
            m_arCadenaSalida[j] = m_szParametros[i];

        for(j--; (m_arCadenaSalida[j] == BLANCO); j--) ;
        j++;

        if(k != 6)
            m_arCadenaSalida[j] = LF; j++; i++;
    }


    m_arCadenaSalida[j] = ESC; j++;
    m_arCadenaSalida[j] = MFEX; j++;
    m_arCadenaSalida[j] = NULL;

    EnviaCadena(m_arCadenaSalida, j, NULL, NULL, NULL, FALSE, NULL);

    POLONIA2_Testeo();
}*/
//---------------------------------------------------------------------------------------------------

void cImpre::POLONIA2_Procesa()
{
    switch(m_nIndComando) {
        case 0: // TESTEO
             POLONIA2_Testeo();
             break;
        case 1: // NUMCAJ Comando falso; siempre devuelve 01
             POLONIA2_Numcaj();
             break;
        case 2: // NUMTIQ
             break;
        case 3: // FECHAF
             POLONIA2_Fechaf();
             break;
        case 4: // HORAFI
             POLONIA2_Horafi();
             break;
        case 5: // NUMBOL
             break;
        case 6: // FECHAB
             break;
        case 7: // HORABO
             break;
        case 8: // TOTALB
             POLONIA2_Totalb();
             break;
        case 9: // CABECB
             POLONIA2_Cabecb();
             break;
        case 10:// LINDEB
             POLONIA2_Lindeb();
             break;
        case 11:// PAGOFI
             POLONIA2_Pagofi();
             break;
        case 12:// CIERRZ
             POLONIA2_Cierrz();
             break;
        case 13:// INFONF
             POLONIA2_Infonf();
             break;
        case 14:// CANCEL
             POLONIA2_Cancel();
             break;
        case 15:// PIEFIS
             break;
        case 16:// TESTEA
             break;
        case 17:// TESTHW
             POLONIA2_Testhw();
             break;
        case 18:// ABRECA
             POLONIA2_Abreca();
             break;
        case 19:// NUMERO
             POLONIA2_Numero();
             break;
        case 20:// DISPLY
             POLONIA2_Disply();
             break;
        case 21:// TRASPA
             break;
        case 22:// TEXLIB
             break;
        case 23:// LIBERA
             break;
        case 24:// INICIA
             break;
        //case 24:// INICIA
        // Teoricamente, en la fiscalizacion se inicializaran las cabeceras
             //POLONIA2_Inicia();
             //break;
        case 25:// LECTUX
             break;
        default:// ­­FUERA DE MADRE!!
             m_nError = 599;
             break;
    }
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

//
// BRASIL
// ======
//
// cImpre::MFCaError
// =================
//     Pasar error del MF al objeto cImpre
//
// Par metros de entrada: ninguno.
//
// Par metros de salida: ninguno.
//
void cImpre::BRASIL_MFCaError()
{
    UCHAR arsNumero2[3];

    m_cClaseError = 0x30;

    if(m_nNumCarRespuesta) {
        sprintf((char*)arsNumero2, "%02X", m_arRespuesta[1]);
        m_nError = atoi((char*)arsNumero2);
        sprintf((char*)arsNumero2, "%02X", m_arRespuesta[2]);
        m_nEstado = atoi((char*)arsNumero2);
    }
    else { m_nError = 551; }    // Error 551: No hay respuesta del dispositivo

}
//---------------------------------------------------------------------------------------------------

// cImpre::QueryE
// ==============
//     Interroga a la impresora por el ultimo error y pasa el error
// al objeto 'cImpre'.
//
// Parametros de entrada: ninguno.
//
// Parametros de salida: ninguno.
//
void cImpre::BRASIL_QueryE()
{
/*    m_arCadenaSalida[0] = ESC;
    m_arCadenaSalida[1] = '+';
    m_arCadenaSalida[2] = 0x09;
    m_arCadenaSalida[3] = NULL;

    EnviaCadena(m_arCadenaSalida, 3, NULL, 6, NULL, TRUE, NULL);

    POLONIA2_MFCaError();*/
}
//---------------------------------------------------------------------------------------------------

void cImpre::BRASIL_LectuX()
{
    m_arCadenaSalida[0] = STX;
    m_arCadenaSalida[1] = 0x04;
    m_arCadenaSalida[2] = NULL;
    m_arCadenaSalida[3] = 0x1b;
    m_arCadenaSalida[4] = 0x06;
    m_arCadenaSalida[5] = 0x21;
    m_arCadenaSalida[6] = NULL;
    m_arCadenaSalida[7] = NULL;

    EnviaCadena(m_arCadenaSalida, 7, NULL, 3, NULL, TRUE, NULL);
    
    BRASIL_MFCaError();
    ComponeError();
}
//---------------------------------------------------------------------------------------------------

void cImpre::BRASIL_Cabecb()
{

    UCHAR arNumero2[3];
    UINT  nNumero;

    // Obtener parametro PP
    arNumero2[0] = m_szParametros[0];
    arNumero2[1] = m_szParametros[1];
    arNumero2[2] = NULL;

    nNumero = atoi((char*)arNumero2);

    if(nNumero == 1)
    {
        m_arCadenaSalida[0] = STX;
        m_arCadenaSalida[1] = 0x04;
        m_arCadenaSalida[2] = NULL;
        m_arCadenaSalida[3] = 0x1b;
        m_arCadenaSalida[4] = NULL;
        m_arCadenaSalida[5] = 0x1b;
        m_arCadenaSalida[6] = NULL;
        m_arCadenaSalida[7] = NULL;

        EnviaCadena(m_arCadenaSalida, 7, NULL, 3, NULL, TRUE, NULL);
    }
}
//---------------------------------------------------------------------------------------------------

void cImpre::BRASIL_Lindeb()
{
    UCHAR arNumero20[21];
    UINT  nNumero, i, j;
    UINT  nSuma, nCont = 0;
    div_t div_result;
    
    if(m_szParametros[0] == '+') {
        m_arCadenaSalida[0] = STX;
                                             
        m_arCadenaSalida[3]  = ESC; ++nCont;
        m_arCadenaSalida[4]  = 0x09; ++nCont;
        m_arCadenaSalida[5]  = m_szParametros[2]; ++nCont; // Modelo
        m_arCadenaSalida[6]  = m_szParametros[3]; ++nCont;
        m_arCadenaSalida[7]  = m_szParametros[4]; ++nCont;
        m_arCadenaSalida[8]  = m_szParametros[5]; ++nCont;
        m_arCadenaSalida[9]  = m_szParametros[6]; ++nCont;
        m_arCadenaSalida[10] = m_szParametros[7]; ++nCont;
        m_arCadenaSalida[11] = m_szParametros[8]; ++nCont;
        m_arCadenaSalida[12] = m_szParametros[9]; ++nCont;
        m_arCadenaSalida[13] = BLANCO; ++nCont;
        m_arCadenaSalida[14] = m_szParametros[11]; ++nCont; // Talla
        m_arCadenaSalida[15] = m_szParametros[12]; ++nCont;
        m_arCadenaSalida[16] = m_szParametros[14]; ++nCont; // Color
        m_arCadenaSalida[17] = m_szParametros[15]; ++nCont;

        // Descripcion
        for(j = 18, i = 17; j < 47; i++, j++) {
            m_arCadenaSalida[j] = m_szParametros[i]; ++nCont;
        }

        m_arCadenaSalida[47] = m_szParametros[129]; ++nCont; //%vat (num. aliquota)
        m_arCadenaSalida[48] = m_szParametros[130]; ++nCont;
                                  
        arNumero20[0] = m_szParametros[59]; // Cantidad
        arNumero20[1] = m_szParametros[60];
        arNumero20[2] = m_szParametros[61];
        arNumero20[3] = NULL;
            
        nNumero = atoi((char*)arNumero20);

        sprintf((char*)arNumero20, "%04d", nNumero);

        for(i = 0,j = 49; arNumero20[i] != NULL; ++i, ++j) {
           m_arCadenaSalida[j] = arNumero20[i]; ++nCont;
        }

        // Precio
        for(i = 72; m_szParametros[i] != COMA; ++i, ++j) {
           m_arCadenaSalida[j] = m_szParametros[i]; ++nCont;
        }
        ++i; // Eliminamos la coma
        m_arCadenaSalida[j++] = m_szParametros[i++]; ++nCont;
        m_arCadenaSalida[j++] = m_szParametros[i++]; ++nCont;
            
        m_arCadenaSalida[j++] = m_szParametros[103]; ++nCont; // % dto
        m_arCadenaSalida[j++] = m_szParametros[104]; ++nCont;
        m_arCadenaSalida[j++] = m_szParametros[106]; ++nCont;
        m_arCadenaSalida[j++] = m_szParametros[107]; ++nCont;
            
        for(nSuma = 0, i = 3; i < j; ++i) {   
//            c = m_arCadenaSalida[i];
            nSuma += (int)m_arCadenaSalida[i];
        }
            
        // Suma de los valores del comando CSL y CSH
        if(div_result.quot = 0, nSuma > 256) { div_result = div(nSuma, 256); }
        else { div_result.rem = nSuma; }
                
        m_arCadenaSalida[j++] = div_result.rem;
        m_arCadenaSalida[j++] = div_result.quot;
        nCont += 2;
            
        m_arCadenaSalida[j] = NULL;
            
        // Numero de bytes enviados NBL y NBH
        if(div_result.quot = 0, nCont > 256) { div_result = div(nCont, 256); }
        else { div_result.rem = nCont; }
                
        m_arCadenaSalida[1] = div_result.rem; 
        m_arCadenaSalida[2] = div_result.quot;
    }

    EnviaCadena(m_arCadenaSalida, j, NULL, 3, NULL, TRUE, NULL);

    BRASIL_MFCaError();
    ComponeError();
}
//---------------------------------------------------------------------------------------------------

void cImpre::BRASIL_Cancel()
{
    m_arCadenaSalida[0] = STX;
    m_arCadenaSalida[1] = 0x04;
    m_arCadenaSalida[2] = NULL;
    m_arCadenaSalida[3] = 0x1b;
    m_arCadenaSalida[4] = 0x0e;
    m_arCadenaSalida[5] = 0x29;
    m_arCadenaSalida[6] = NULL;
    m_arCadenaSalida[7] = NULL;

    EnviaCadena(m_arCadenaSalida, 7, NULL, 3, NULL, TRUE, NULL);
    
    BRASIL_MFCaError();
    ComponeError();
} 
//---------------------------------------------------------------------------------------------------

void cImpre::BRASIL_Procesa()
{
    switch(m_nIndComando) {
        case 0: // TESTEO
             break;
        case 1: // NUMCAJ
             break;
        case 2: // NUMTIQ
             break;
        case 3: // FECHAF
             break;
        case 4: // HORAFI
             break;
        case 5: // NUMBOL
             break;
        case 6: // FECHAB
             break;
        case 7: // HORABO
             break;
        case 8: // TOTALB
             break;
        case 9: // CABECB
             BRASIL_Cabecb();
             break;
        case 10:// LINDEB
             BRASIL_Lindeb();
             break;
        case 11:// PAGOFI
             break;
        case 12:// CIERRZ
             break;
        case 13:// INFONF
             break;
        case 14:// CANCEL
             BRASIL_Cancel();
             break;
        case 15:// PIEFIS
             break;
        case 16:// TESTEA
             break;
        case 17:// TESTHW
             break;
        case 18:// ABRECA
             break;
        case 19:// NUMERO
             break;
        case 20:// DISPLY
             break;
        case 21:// TRASPA
             break;
        case 22:// TEXLIB
             break;
        case 23:// LIBERA
             break;
        case 24:// INICIA
             break;
        case 25:// LECTUX
             BRASIL_LectuX();
             break;
        default:// ­­FUERA DE MADRE!!
             m_nError = 599;
             break;
    }
} 
//---------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------
// Fin de los Comandos de Impresora
// -----------------------------------------------------------------
//
// cImpre::DesglosaComando
// -----------------------
//
//    Extraer de la linea de argumentos del fichero los valores adecuados.
//
// Parametros de entrada:
//    char* szComando    Comando fiscal
//
// Parametros de salida:
//    int   m_nError
//
// Obtenci¢n de 'm_szPais', 'm_szComando' y 'm_szParametros'.
//
int cImpre::DesglosaComando(const char* szComando)
{
    if(szComando[0] != '{') {
        m_nError = 545; // Formato fichfis incorrecto
        return TRUE;
    }

    /* DESGLOSAR CODIGO DE PAIS */
    for(UINT i = 2, j = 0; i < 5; ++i, ++j) { m_szPais[j] = szComando[i]; }
    
    m_szPais[j] = NULL;

    m_nError = 505; // Pais no implementado
    for(m_nIndPais = 0; strcmp(g_Paises[m_nIndPais], VACIO); ++m_nIndPais) {
        if(!strcmp(g_Paises[m_nIndPais], m_szPais)) {
           m_nError = 0;
           break;
        }
    }
    if(m_nError) { return TRUE; }

    /* DESGLOSAR OPERACION */
    for(i = 6, j = 0; i < 12; ++i, ++j) { m_szComando[j] = szComando[i]; }
    
    m_szComando[j] = NULL;

    m_nError = 530; // Comando erroneo en Fichfis
    for(m_nIndComando = 0; strcmp(g_Comandos[m_nIndComando], VACIO); ++m_nIndComando) {
        if(!strcmp(g_Comandos[m_nIndComando], m_szComando)) {
           m_nError = 0;
           break;
        }
    }
    if(m_nError) { return TRUE; }

    /* DESGLOSAR PARAMETROS OPERACION */
    for(i = 13, j = 0; (szComando[i] != '}') && (szComando[i] != '\0'); ++i, ++j) {
        m_szParametros[j] = szComando[i];
    }

    m_szParametros[j] = NULL;

    if(szComando[i] != '}') {
        m_nError = 545; // Formato fichfis incorrecto
        return TRUE;
    }

    return FALSE;
}
//---------------------------------------------------------------------------------------------------

//
// cImpre::InterpretaComando
// -------------------------
//
//    Ejecutar las funciones pertinentes al comando solicitado.
//
// Par metros de entrada:
//    char* szComando    Comando fiscal
//
// Par metros de salida:
//    Ninguno, pero actualiza 'm_nError'
//
void cImpre::InterpretaComando(const char szComando[MAX_LONG_BUFFER])
{
    m_nError = 0;

	if(!DesglosaComando(szComando)) {

		// Hungria no funciona mediante interrupciones
		if(!strcmp(g_Paises[m_nIndPais], HUNGRIA)) { HUNGRIA_Procesa(); }

		else {

			if(m_bPrimeraVez) {
				HabilitaInterrupciones();
				m_bPrimeraVez = FALSE;
			}

			if(!strcmp(g_Paises[m_nIndPais], POLONIA2)) { POLONIA2_Procesa(); }
			else if(!strcmp(g_Paises[m_nIndPais], POLONIA)) { POLONIA_Procesa(); }
			else if(!strcmp(g_Paises[m_nIndPais], TURQUIA)) { TURQUIA_Procesa(); }
			else if(!strcmp(g_Paises[m_nIndPais], TURQUIA2)) { TURQUIA2_Procesa(); }
			else if(!strcmp(g_Paises[m_nIndPais], PORTUGAL)) { PORTUGAL_Procesa(); }
			else if(!strcmp(g_Paises[m_nIndPais], BRASIL)) { BRASIL_Procesa(); }
		}
	}
	else {
		m_nError = 505;
		ComponeError();
	}
}
//---------------------------------------------------------------------------------------------------
#if 0
const int cImpre::GetNthChunk(const char* q, const int L)
{
}
#endif
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
