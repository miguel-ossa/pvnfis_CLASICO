// cImpre.h
// ----------  
#ifndef _CIMPRE23112004_H_A_
#define _CIMPRE23112004_H_A_ 1
//
// Definici¢n de la clase cImpre para manejo de impresoras serie.
//  

//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "cComm.h"

//----------------------------------------------------------------------------------------------------

class cImpre : public cComm
{
public:
// Constructor/destructor
	cImpre(int pCom, int iIrq);
	~cImpre();

public:
	// Implementaci¢n
	char m_szRespuesta[MAX_LONG_BUFFER];
	// Comportamiento
	void HabilitaInterrupciones();
	int GetBoletaAbierta() const { return m_bBoletaAbierta; }
	void ComponeError();
	void InterpretaComando(const char szComando[MAX_LONG_BUFFER]);
	int GetError() const { return m_nError; }

private:  
	const int GetNthChunk(const char* q, const int L);
	// ------------->Comportamiento
	int DesglosaComando(const char* szComando);
	void ComponeComando(const char* szComando);
	// Comandos de la impresora
	// TURQUIA
	void TURQUIA_Procesa();
	void TURQUIA_Testeo();
	void TURQUIA_Reset();
	void TURQUIA_PaperStatus();
	void TURQUIA_LetraPeque();
	void TURQUIA_AreaTrabajo();
	void TURQUIA_DeshabilitaBotones();
	void TURQUIA_Cabecb();
	void TURQUIA_Lindeb();
	void TURQUIA_Piefis();
	void TURQUIA_Totalb();
	void TURQUIA_Fechab();
	void TURQUIA_Texlib();
	void TURQUIA_Lintrasp();
	void TURQUIA_Expulsar();
	void TURQUIA_Implet();
	void TURQUIA_Infonf();
	void TURQUIA_Libera();
	// TURQUIA2
	void TURQUIA2_Procesa();
	void TURQUIA2_Testeo();
	// HUNGRIA
	void HUNGRIA_Ejecuta();
	void HUNGRIA_Procesa();
	void HUNGRIA_Numbol();
	void HUNGRIA_Lindeb();
	void HUNGRIA_Piefis();
	void HUNGRIA_Pagofi();
	void HUNGRIA_Cierrz();
	// POLONIA
	void POLONIA_Procesa();
	void POLONIA_MFCaError();
	void POLONIA_QueryE();
	void POLONIA_Cierra_report();
	void POLONIA_Testeo();
	void POLONIA_Numcaj();
	void POLONIA_Numtiq();
	void POLONIA_Fechaf();
	void POLONIA_Horafi();
	void POLONIA_Totalb();
	void POLONIA_Cabecb();
	void POLONIA_Lindeb();
	void POLONIA_Pagofi();
	void POLONIA_Cierrz();
	void POLONIA_Infonf();
	//void POLONIA_Testea();
	void POLONIA_Testhw();
	void POLONIA_Abreca();
	void POLONIA_Numero();
	void POLONIA_Disply();
	void POLONIA_Cancel();
	void POLONIA_Inicia();
	// POLONIA2
	void POLONIA2_Procesa();
	void POLONIA2_QueryE();
	void POLONIA2_Testeo();
	void POLONIA2_MFCaError();
	void POLONIA2_Numero();
	void POLONIA2_Numcaj();
	void POLONIA2_Fechaf();
	void POLONIA2_Horafi();
	void POLONIA2_Cabecb();
	void POLONIA2_Lindeb();
	void POLONIA2_Cancel();
	void POLONIA2_Cierra_report();
	void POLONIA2_Pagofi();
	void POLONIA2_Totalb();
	void POLONIA2_Cierrz();
	void POLONIA2_Abreca();
	void POLONIA2_Infonf();
	void POLONIA2_Testhw();
	void POLONIA2_Disply();
	void POLONIA2_Inicia();
	// PORTUGAL
	void PORTUGAL_Procesa();
	void PORTUGAL_Infonf();
	// BRASIL
	void BRASIL_Procesa();
	void BRASIL_MFCaError();
	void BRASIL_QueryE();
	void BRASIL_LectuX();
	void BRASIL_Cabecb();
	void BRASIL_Lindeb();
	void BRASIL_Cancel();

private:
	BOOL m_bEuros;
	BOOL m_bBoletaAbierta;
	BOOL m_bPrimeraVez;
	BOOL m_bAbono; // Indica si la factura es negativa
	BOOL m_bTraspaso; // Indica que se trata de un traspaso
	UINT m_nLineas;
	UINT m_nIndComando;
	UINT m_nIndPais;
	int m_nError;
	int m_nEstado;
	char m_cClaseError;
	char m_cSigno;
	char m_szPais[4];
	char m_szParametros[MAX_LONG_BUFFER];
	char m_szComando[MAX_LONG_BUFFER];
	char m_szTotal[17];
	UCHAR m_arCadenaSalida[MAX_LONG_BUFFER];   
	
private:
    cImpre(const cImpre& r);
    cImpre& operator=(const cImpre& r);
};                          
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#endif//_CIMPRE23112004_H_A_