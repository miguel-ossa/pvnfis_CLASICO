// pvnfis.cpp (m¢dulo principal)
// ----------
//
//    Programa interface universal impresoras fiscales.
//
// Miquel de la Ossa   --- MANGO --- Octubre 2.000
//
// Compilable con Microsoft Visual C++ 1.52 y Borland C++ 3.0

#include "Constant.h"
#include "cImpre.h"
#include "cFile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef DEBUG
   #include <conio.h>
#endif

void main()
{
    int bPrimeraVez = TRUE;
    int iCOM, iIRQ;
    char  szParametros[MAX_LONG_BUFFER];
    UCHAR arNumero[2];

    cImpre* Impresora;
    
    cFile*  Fichfis   = new cFile(FICHFIS, "rb");
                                     
    // N£cleo principal.
    // Mientras no exista error en la lectura y los comandos
    // se ejecuten correctamente, continuar...
    while ((!Fichfis->GetError()) && (Fichfis->LeerSigLinea() != EOF))
    {
        if(bPrimeraVez)
        {
            // Si es la primera vez, obtenemos los valores
            // del comando COMIRQ para inicializar el objeto
            // "Impresora", que a su vez deriva de "cComm".
            strcpy(szParametros, Fichfis->GetLinea());
            // Obtener parametro P
            arNumero[0] = szParametros[13];
            arNumero[1] = NULL;
            iCOM = atoi((char*)arNumero);
            // Obtener parametro I
            arNumero[0] = szParametros[15];
            arNumero[1] = NULL;
            iIRQ = atoi((char*)arNumero);    
                           
            Impresora = new cImpre(iCOM, iIRQ);
            bPrimeraVez = FALSE;
        }
        else
        {
            Impresora->InterpretaComando(Fichfis->GetLinea());
            if(Impresora->GetError())
                break;
        }
    }
    delete Fichfis;

    // Grabaci¢n de Fichint ---> Respuesta
    cFile* Fichint = new cFile(FICHINT, "wb");
    Fichint->EscribirLinea(Impresora->m_szRespuesta);
    delete Fichint;

    remove(FICHFIS_OLD);
    rename(FICHFIS, FICHFIS_OLD); // Comandos de entrada ---> FICHFIS.OLD
    rename(FICHINT, FICHFIS);  // Respuesta = FICHFIS

    delete Impresora; // Siempre debe cerrarse, para restaurar interrupciones
                              
}