// PVNLET.CPP
// ----------
//
// Conversion de numeros a letras en varios idiomas.
//    Implementada Turquía y Portugal.
//
// Parametros de entrada:
//       País en formato "nnn" e importe en formato "nnnnnnnnnnnn,nnn"
//       (Los ceros iniciales del importe se sustituirán por puntos, ej: "..4345233100,000")
//
// Parametros de salida:
//       Errorlevel 0 si conversion OK, PVNLET.TXT con el texto
//       Errorlevel 1 si error en numero de argumentos
//       Errorlevel 2 si error en formato de argumento
//
// Miquel de la Ossa    Enero 2.001    Mango
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "constant.h"
#include "cFile.h"

// Tabla de paises
char g_Paises[][4] = { TURQUIA, PORTUGAL, VACIO };
UINT g_nIndPais;

int main(int argc, char *argv[])
{
   int   i, swCentavos = FALSE;
   enum { N = 1024 };
   //char* szImporte = "";
   char szImporte[N]; szImporte[0] = '\0';
   char  sNumero3[4];
   //
   // Matrices que contienen los literales a ser posteriormente impresos
   //
   char  arPortugal[7][10][19] = { // matriz 6x10 de char[19]
      // ,uno,dos,etc.
      { "", "UM", "DOIS", "TRÒS", "QUATRO", "CINCO", "SEIS", "SETE", "OITO", "NOVE" },
      // ,diez,veinte,etc.
      { "", "DEZ", "VINTE", "TRINTA", "QUARENTA", "CINQUENTA", "SECENTA", "SETENTA", "OITENTA", "NOVENTA" },
      // cien
      { "", "CEM", "DUZENTOS", "TREZENTOS", "QUATROCENTOS", "QUINIENTOS", "SEISCENTOS", "SETECENTOS", 
         "OITOCENTOS", "NOVECENTOS" },
      // mil
      { "MIL", "MIL", "MIL", "MIL", "MIL", "MIL", "MIL", "MIL", 
        "OITO MIL", "NOVE MIL" },
      // millon
      { "MILHÇO", "MILHÇO", "MILHåES", "MILHåES", "MILHåES", "MILHåES", "MILHåES", "MILHåES", 
        "MILHÕES", "MILHÕES" },
      // mil millones
      { "MIL MILHåES", "MIL MILHåES", "MIL MILHåES", "MIL MILHåES", "MIL MILHåES", "MIL MILHåES", 
         "MIL MILHÕES", "MIL MILHÕES", "MIL MILHÕES", "MIL MILHÕES" },
      // ,dos,tres,etc.
      { "", "ONZE", "DOZE", "TREZE", "QUATORZE", "QUINZE", "DEZASSEIS", "DEZASETE", "DEZOITO",
        "DEZANOVE" },
       
   };
   //**************************************************************************************
   char  arTurquia[7][10][7] = {  // matriz 6x10 de char[7]
      // ,uno,dos,etc.
      { "", "BIR", "IKI", "UC", "DORT", "BES", "ALTI", "YEDI", "SEKIZ", "DOKUZ" },
      // ,diez,veinte,etc.
      { "", "ON", "YIRMI", "OTUZ", "KIRK", "ELLI", "ATMIS", "YETMIS", "SEKSEN", "DOKSAN" },
      // cien
      { "YUZ", "YUZ", "YUZ", "YUZ", "YUZ", "YUZ", "YUZ", "YUZ", "YUZ", "YUZ" },
      // mil
      { "BIN", "BIN", "BIN", "BIN", "BIN", "BIN", "BIN", "BIN", "BIN", "BIN" },
      // millon
      { "MILYON", "MILYON", "MILYON", "MILYON", "MILYON", "MILYON", "MILYON", "MILYON", "MILYON", "MILYON" },
      // mil millones
      { "MILYAR", "MILYAR", "MILYAR", "MILYAR", "MILYAR", "MILYAR", "MILYAR", "MILYAR", "MILYAR", "MILYAR" },
      // ,dos,tres,etc.
      { "", "", "IKI", "UC", "DORT", "BES", "ALTI", "YEDI", "SEKIZ", "DOKUZ" },
       
   };
   
   //
   // Reglas de comportamiento
   // ========================
   //
   //    Cada FILA representa un dígito de la cantidad a convertir, de arriba a abajo
   // y de izquierda a derecha, desde el dígito más significativo hasta las unidades.
   //    Cada COLUMNA representa un índice de la tabla de literales. El hecho de que haya dos
   // columnas es para solucionar el caso de que tengamos que aplicar más de un literal en
   // determinada posición del número, ej: [DOS] [MILLONES] --> valor 0 y valor 4.
   //
   int  nReglaTur[13][2] = {     // Regla para TURQUIA
   {  6,  2 }, // 6, 2 quiere decir que accederemos a la columna 6, posición n de la tabla de arriba,
   {  1, -1 }, //      donde n es el numero a convertir a literal
   {  0,  5 }, // 5 = MIL MILLONES
   {  6,  2 }, // 2 = CIEN, DOSCIENTOS
   {  1, -1 }, // un -1 significa no hacer nada
   {  0,  4 }, // 4 = MILLON, MILLONES
   {  6,  2 }, // 2 = CIEN, DOSCIENTOS
   {  1, -1 },
   {  0,  3 }, // 3 = MIL, DOS MIL
   {  6,  2 }, // 2 = CIEN, DOSCIENTOS
   {  1, -1 },
   {  0, -1 }
   };
   
   int  nReglaPor[15][2] = {     // Regla para PORTUGAL
   {  6,  2 }, // 6 = ONCE, DOCE
   {  1, -1 }, // 1= DIEZ, VEINTE
   {  0,  5 }, // 5 = MIL MILLONES
   {  2, -1 }, // 2 = CIEN, DOSCIENTOS
   {  1, -1 }, // un -1 significa no hacer nada
   {  0,  4 }, // 4 = MILLON, MILLONES
   {  2, -1 }, // 2 = CIEN, DOSCIENTOS
   {  1, -1 }, // -1 = NO HACER NADA
   {  0,  3 }, // 3 = MIL, DOS MIL
   {  2, -1 }, // 2 = CIEN, DOSCIENTOS
   {  1, -1 },
   {  0, -1 },
   { -1, -1 },
   {  1, -1 }, // primera pos. decimal
   {  0, -1 }
   };
   
   remove("PVNLET.TXT");
   
   // ------------------------>Filtros
   if(argc < 3)
   {
      return 1; // Error en numero de argumentos
   }

   for(i = 0; i < 3; ++i) { // País
      if(!isdigit(argv[1][i])) { return 2; } // Error en formato de argumentos
   }

   //printf("argv[2] = {%s}", argv[2]);getchar();
   
         
   for(i = 0; i < 15; ++i) // en la posicion 15 tenemos la coma
      if(!isdigit(argv[2][i]) && !ispunct(argv[2][i])) {
         return 2; // Error en formato de argumentos
      }
      
   for(g_nIndPais = 0, i = 99; strcmp(g_Paises[g_nIndPais], VACIO); ++g_nIndPais) {// ¿País implementado?
        if(!strcmp(g_Paises[g_nIndPais], argv[1])) { i = 0; break; }
    }
    
    if(i) { return 2; } // Error. País no implementado
      
   // ------------------------>Calculo del literal
   int n = 12;
   if(!strcmp(g_Paises[g_nIndPais], PORTUGAL)) {
        if(argc == 4) { n = 15; } // Enteros más 2 decimales
   }

   for(i = 0; i < n; ++i) {
      if((argv[2][i] != PUNTO) && (argv[2][i] != COMA)) {  
         if((i == 0) || (i == 3) || (i == 6) || (i == 9) || (i == 13)) {
            sNumero3[0] = argv[2][i];
            sNumero3[1] = argv[2][i+1];
            sNumero3[2] = argv[2][i+2];
            sNumero3[3] = NULL;
            if(!strcmp(sNumero3, "000")) { i += 2; }
         }
         else { sNumero3[0] = NULL; }
         if(strcmp(sNumero3, "000")) {
            for(int j = 0; j < 2; ++j) {
               // TURQUIA
               if(!strcmp(g_Paises[g_nIndPais], TURQUIA) &&
                    (nReglaTur[i][j] != -1))
                  strcat(szImporte, arTurquia[nReglaTur[i][j]][argv[2][i]-48]);
               // PORTUGAL
               if(!strcmp(g_Paises[g_nIndPais], PORTUGAL) &&
                    (nReglaPor[i][j] != -1)) {
                  if(i == 13) {
                     strcat(szImporte, "EUROS E ");
                     swCentavos = TRUE;
                  }
                  if(strcmp(arPortugal[nReglaPor[i][j]][argv[2][i]-48], NULL)) {
                     if((nReglaPor[i][j] == 0)  // Regla de uno, dos, tres...
                          && ((nReglaPor[i][j+1] == 5)     // ¿Siguiente regla = mil millones...?
                            || (nReglaPor[i][j+1] == 3))   // ¿... o = mil?
                          && (argv[2][i] == '1')) { // Si el número es un 1, no ejecutamos esta posición de la regla
                          continue;
					 }

                     if(nReglaPor[i][j] == 4) {      // ¿Regla millones?
                        if(argv[2][i] != '1') {        // Si no es la unidad...    
                           strcat(szImporte, arPortugal[4][9]);
                           strcat(szImporte, " ");
                           continue;
                        }
                     }
                     if((nReglaPor[i][j] == 2) // Regla de cien, doscientos...
                          && ((argv[2][i] == '1') && ((argv[2][i+1] != '0') || (argv[2][i+2] != '0'))))
                     {
                        strcat(szImporte, "CENTO");    // Ciento...
                        strcat(szImporte, " ");
                     }
                     else {
                        if(nReglaPor[i][j] == 1) {// Regla de diez, veinte...
                           if(argv[2][i] == '1') { // ¿El carácter es un 1?
                              if(argv[2][i+1] == '0') {   // ¿Siguiente carácter es un 0?
                                 nReglaPor[i+1][0] = -1;    // Modificar comportamiento regla: no hacer nada en sig. car.
                                 strcat(szImporte, arPortugal[nReglaPor[i][j]][argv[2][i]-48]);
                                 strcat(szImporte, " ");
                              }
                              else {
                                 nReglaPor[i][j] = 6;       // Modificar comportamiento regla: doce, trece...
                                 nReglaPor[i+1][0] = -1;    // Modificar comportamiento regla: no hacer nada
                                 strcat(szImporte, arPortugal[nReglaPor[i][j]][argv[2][i+1]-48]);
                                 strcat(szImporte, " ");
                              }
                           }
                           else { // El carácter no es un 1
                              strcat(szImporte, arPortugal[nReglaPor[i][j]][argv[2][i]-48]);
                              strcat(szImporte, " ");
                           }
                        }
                        else { // No es regla de diez, veinte...
                           strcat(szImporte, arPortugal[nReglaPor[i][j]][argv[2][i]-48]);
                           strcat(szImporte, " ");
                        }
                     }
                  }
               }
            }
         }
      }
   }
   
   if(!strcmp(g_Paises[g_nIndPais], TURQUIA)) {
       strcat(szImporte, " YENITURKLIRASI");	   
	   char* p = strchr(argv[2], ',') + 1;
	   if(p != NULL) {
		   p[2] = '\0';
		   const int num = atoi(p); 		   				  
		   const int dd = num/10;
		   const int uu = num%10;
		   if(dd > 0 || uu > 0) {
			   const char* d[] = { "", "ON", "YIRMI", "OTUZ", "KIRK", "ELLI", "ATMIS", "YETMIS", "SEKSEN", "DOKSAN" };
			   const char* u[] = { "", "BIR", "IKI", "UC", "DORT", "BES", "ALTI", "YEDI", "SEKIZ", "DOKUZ" };
			   strcat(strcat(strcat(strcat(szImporte, " "), d[dd]), u[uu]), " KURUS");
		   }
	   }
	   
	   if((p = strstr(szImporte, "BIRBIN")) != NULL) {
		   const int L = strlen(szImporte);
		   if(0 < L && L < N) {
			   char* q = new char[L + 1];
			   if(q  != NULL) {
				   const int m = p - szImporte;
				   strcpy((char *)memcpy(q, szImporte, m) + m, p + 3);
				   strcpy(szImporte, q);
				   delete [] q; q = NULL;
			   }
		   }
	   }
   }
   
   if(swCentavos) { strcat(szImporte, "CENTIMOS"); }
   
   
   // ---------------------->Grabacion del literal en el secuencial PVNLET.TXT   
   cFile* pFichlet = new cFile("PVNLET.TXT", "wb");
   if(pFichlet != NULL) {
       pFichlet->EscribirLinea(szImporte);
       delete pFichlet; pFichlet = NULL;
   }
    
   return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
