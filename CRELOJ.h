// cReloj.h
// ----------
#ifndef _CRELOJ_22112004_H_A_
#define _CRELOJ_22112004_H_A_ 1
//
// Declaraci¢n de la clase cReloj para funciones horarias.
//
class cReloj
{
public:
	// Constructor (requerido)
	cReloj();
	// Comportamiento
	unsigned long LeerHora();
	void  EsperaSegundos(float fSegundos);

private:
	cReloj(const cReloj& r);
	cReloj& operator=(const cReloj& r);
};
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
#endif//_CRELOJ_22112004_H_A_
