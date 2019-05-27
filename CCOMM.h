// cComm.h
// --------- 
#ifndef CCOMM22112004_H_A_
#define CCOMM22112004_H_A_ 1
//
// Definici¢n de la clase COMM para las comunicaciones en serie
// 
 
//--------------------------------------------------------------------------------------------------

class cComm
{
public:
	// Constructor/destructor
	//cComm ();//make it private
	cComm (const char cTipo,       // 'T'iempo, 'C'adena
		const UCHAR cFin,        // Car cter fin de secuencia
		const ULONG iTimeout,   // Timeout
		const int iBaudios,     // velocidad en bits por segundo
		const int iParidad,     // c¢digo de paridad
		const int iPalabra,     // longitud de palabra
		const int iBitsParada,  // bits de parada para fin de car cter
		const int iCOM,         // numero de puerto COM
		const int iIRQ          // numero de IRQ del puerto COM
	);        
	virtual ~cComm();

protected:
	void CapturaInterrupcion();
	void LiberaInterrupcion();
	void HabilitaPIC();
	void DeshabilitaPIC();
	void EnviaCadena(UCHAR arCadena[MAX_LONG_BUFFER], const int nElementos, char cTipo,
	                 UINT nNumCar, UCHAR cFin, const UINT bEspera, const UINT nDelay);
private:
	void BrushCadena(UCHAR arCadena[MAX_LONG_BUFFER], const int nElementos);
	void DoEnviaCadena(const UCHAR arCadena[MAX_LONG_BUFFER], const int nElementos, char cTipo,
	                 UINT nNumCar, UCHAR cFin, const UINT bEspera, const UINT nDelay);
	int GetNumError() const;   // Devuelve el n£m. del £ltimo error 

private:
       // Implementaci¢n       
#ifdef __TURBOC__       
	void interrupt (far * m_pOld_Serial_Handler)(__CPPARGS);
	void interrupt (far * m_pOld_CtrlC_Handler)(__CPPARGS);
#else
	typedef void (interrupt far * ptrINTERRUPT)(__CPPARGS);
	ptrINTERRUPT m_pOld_Serial_Handler;
	ptrINTERRUPT m_pOld_CtrlC_Handler;
#endif       
	// Operaciones
	static void interrupt far cComm::SerialHandler(__CPPARGS);
	static void interrupt far cComm::CtrlCHandler(__CPPARGS);
	void ObtieneRespuesta();
	//int   GetDsr( void );

protected:
	// Implementacion esta y descendientes
	UCHAR m_arRespuesta[MAX_LONG_BUFFER];
	int   m_nNumCarRespuesta;
	int   m_nError;

private:
	int   m_bPICInicializado;
	int   m_bIntCapturada;
	const char  m_cTipo;
	const UCHAR m_cFin;
	UCHAR m_cParms;
	const int   m_iCOM;
	const int   m_iIRQ;
	const unsigned long m_iTimeout;

private:
    cComm();    
    cComm(const cComm& r);
    cComm& operator=(const cComm& r);   
};           
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
#endif//CCOMM22112004_H_A_
