// Proyecto PVNFIS.EXE
// -------------------
//
// cFile.cpp
// ---------
//
// Implementacion de la clase cFile para el intercambio de informaci¢n
// en secuenciales E/S.
//
// Miquel de la Ossa   --- MANGO --- Octubre 2.000
//
// cFile.cpp
// ---------
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#include "constant.h"
#include "cFile.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------

cFile::cFile(const char* szNombre, const char* szModo) : m_pFile(NULL)
{
    m_nError = Abrir(szNombre, szModo);
}
//--------------------------------------------------------------------------------------------------

cFile::~cFile()
{
    m_nError = Cerrar();
}
//--------------------------------------------------------------------------------------------------

int cFile::Abrir(const char* szNombre, const char* szModo )
{
    //return ( (lpszArchivo = (FICHERO*)fopen (szNombre, szModo)) == NULL ); 
    if(m_pFile != NULL) { Cerrar(); }  
    
	return ((m_pFile = fopen (szNombre, szModo)) == NULL);
}
//--------------------------------------------------------------------------------------------------

int cFile::Cerrar()
{
    //return fclose( (FILE*)lpszArchivo );
	const int nRet = fclose(m_pFile); m_pFile = NULL; return nRet;
}
//--------------------------------------------------------------------------------------------------

int cFile::GetError()
{
    return m_nError;
}
//--------------------------------------------------------------------------------------------------

char cFile::LeerSigCaracter()
{
    //char ch = fgetc( (FILE*)lpszArchivo );
	const char ch = fgetc(m_pFile);

    if(ch == EOF) { m_nError = TRUE; }

    return ch;
}
//--------------------------------------------------------------------------------------------------

int cFile::EscribirCaracter(const char ch)
{
    char c;

    //if ( (c = fputc (ch, (FILE*)lpszArchivo)) == EOF ) { m_nError = TRUE; }
	if((c = fputc(ch, m_pFile)) == EOF) { m_nError = TRUE; }

    return c;
}
//--------------------------------------------------------------------------------------------------

int cFile::EscribirLinea(const char* szLinea)
{
    for(int i = 0; (szLinea[i] != NULL); ++i) {
        if(EscribirCaracter(szLinea[i]) == EOF) {
            m_nError = TRUE;
            return EOF;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------------------

int cFile::LeerSigLinea()
{
	//for(int k = 0; k < MAX_LONG_BUFFER; ++k) { m_szLinea[k] = '\0'; }//no effect

    char  ch;
    int   i = 0;

    m_szLinea[0] = NULL;

    do {
        //ch = fgetc ((FILE*)lpszArchivo);
		ch = fgetc(m_pFile);
        if((ch != EOF) && (ch != LF) && (ch != CR)) {
           m_szLinea[i] = ch;
           i++;
        }
    } while((ch != EOF) && (ch != CR));

    if(i) {
       m_szLinea[i] = NULL;
       return 0;
    }
    else {
        m_nError = TRUE;
        return EOF;
    }
}
//--------------------------------------------------------------------------------------------------

char* cFile::GetLinea()
{
    return m_szLinea;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
