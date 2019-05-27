// constant.h
// ------------
//
#ifndef _CONSTANT_23112004_H_A_
#define _CONSTANT_23112004_H_A_ 1
// Definicion de etiquetas de uso general.
//
#ifdef __BORLANDC__
   #ifdef __cplusplus
        #define __CPPARGS ...
   #else
      #define __CPPARGS
   #endif
#else
   #define __CPPARGS
#endif

/* Parametrizacion */
//
// NOTA: Modificar los tres #defines siguientes
// para trabajar con un puerto diferente
//#define COM           0            /* Numero de puerto - 1 */
#define Comport       COM1
//#define IRQ           IRQ_COM1

/* Etiqueta para depuracion */
//#define DEBUG         1

/* Generales */
#ifndef BOOL
    #define BOOL      int
#endif
#ifndef TRUE
    #define TRUE      1
#endif
#ifndef FALSE
    #define FALSE     0
#endif
#define POR_TIEMPO    'T'
#define POR_CADENA    'C'
#define POLONIA       "060"
#define POLONIA2      "960"
#define HUNGRIA       "064"
#define TURQUIA       "052"
#define TURQUIA2      "952"
#define PORTUGAL      "010"
#define BRASIL        "508"
#define VACIO         "000"
#define INICIO_MNG    '{'
#define FIN_MNG       '}'
#define SEPARADOR     '|'
#ifdef DEBUG   
   #define FICHDEB    "..\\ficheros\\fichdeb"
#endif
#define FICHFIS       "..\\ficheros\\fichfis"
#define FICHINT       "..\\ficheros\\fichint"
#define FICHFIS_OLD   "..\\ficheros\\fichfis.old"

/* Clases de datos */
#define UCHAR         unsigned char
#define UINT          unsigned int
#define ULONG         unsigned long

/* Clases de errores */
#define ERROR_PVN           '0'
#define ERROR_REPARACION    '1'
#define ERROR_MNG           '2'
#define ERROR_SECUENCIA     '3'
#define ERROR_ESPERA        '4'
#define ERROR_USUARIO       '5'
#define ERROR_ARGUMENTOS    '6'
#define ERROR_IMPRESION     '7'

/* Car cteres */
#define CR            0x0d
#define GSX           0x1d
#define LF            0x0a
#define FF            0x0c
#define ESC           0x1b
#define MFBX          0x80
#define MFB1X         0x81
#define MFB2X         0x82
#define MFEX          0x83
#define DLE           0x10
#define EOT           0x04
#define GS            0x1d
#define FS            0x1c
#define MFB           0xb0
#define SEP           0x0a
#define END           0xb1
#define STX           0x02
#define ACK           0x06
#define SEP_RESPUESTA ';'
#define BLANCO        0x20
#define COMILLA       0x22
#define COMA          0x2c
#define GOMA          COMA
#define PUNTO         0x2e
#define BARRA_DOS     0x5c

/* Comunicaciones */
#define BAUD300       0x40    /* Bits por segundo */
#define BAUD1200      0x80
#define BAUD2400      0xa0
#define BAUD4800      0xc0
#define BAUD9600      0xe0
#define SIN_PARIDAD   0       /* Tipos de paridad */
#define PARIDAD_IMPAR 1
#define PARIDAD_PAR   2
#define NINGUNA       0x00    /* Definicion de paridad */
#define IMPAR         0x18
#define PAR           0x8
#define LON7          0x02    /* Longitud de palabra */
#define LON8          0x03
#define STOP1         0x00    /* Bits de parada */
#define STOP2         0x40
#define THR           (Comport+0)  /* Transmitter Holding Register */
#define RDR           (Comport+0)  /* Receiver Data Register */
#define IER           (Comport+1)  /* Interrupt Enable Register */
#define IIR           (Comport+2)  /* Interrupt Identification Register */
#define LCR           (Comport+3)  /* Line Control Register */
#define MCR           (Comport+4)  /* Modem Control Register */
#define LSR           (Comport+5)  /* Line Status Register */
#define MSR           (Comport+6)  /* Modem Status Register */
#define FCR           (Comport+2)  /* FCR FIFO control register */
#define COM1          0x3f8
//#define COM2          0x2f8
//#define COM3          0x3E8
//#define COM4          0x2E8

//#define IRQ_COM1      4
//#define IRQ_COM2      3
//#define IRQ_COM3      4
//#define IRQ_COM4      3

/* Varios */
#define MAX_LONG_BUFFER 256
#define CTRL_C          0x23    /* Manejador de interrupci¢n para Ctrl+C */

typedef struct _COLA {
    UCHAR Cola[MAX_LONG_BUFFER];
    UINT  Indice;
  } COLA;

//--------------------------------------------------------------------------------------------------
#endif//_CONSTANT_23112004_H_A_