// cFile.h
// --------- 
#ifndef CFILE_22112004_H_A_
#define CFILE_22112004_H_A_ 1  

#include <stdio.h>
//
// Declaraci¢n de la clase cFile para el intercambio de informaci¢n
// en secuenciales E/S.
// 
#if 0
typedef struct {
   short            level;
   unsigned         flags;
   char             fd;
   unsigned char    hold;
   short            bsize;
   unsigned char   *buffer, *curp;
   unsigned         istemp;
   short            token;
} FICHERO; 
#endif

//--------------------------------------------------------------------------------------------------

class cFile
{
public:
       // Constructor
       cFile(const char* szNombre, const char* szModo);
       ~cFile();
// Comportamiento
public:       
       int Abrir(const char* szNombre, const char* szModo);
       int Cerrar();
public:
       // Implementaci¢n
       int GetError();
       char* GetLinea();
       int EscribirLinea(const char* szLinea);
       int EscribirCaracter(const char ch);
       char LeerSigCaracter();
       int  LeerSigLinea(); 
       
private:
       int m_nError;
       //FICHERO* lpszArchivo;
	   FILE* m_pFile;
       char m_szLinea[MAX_LONG_BUFFER]; 
       
private:
    cFile();
    cFile(const cFile& r);
    cFile& operator=(const cFile& r);             
};
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
#endif//CFILE_22112004_H_A_