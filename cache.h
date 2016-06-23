#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <cstdlib>
#include <tgmath.h>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <ctime>
#include <new>


using namespace std;

class Cache{
public:
  int n_ways;
  int n_sets;
  int tamano_cache;
  int tamano_bloque;
  int tamano_offset;
  int tamano_tag;
  int tamano_index;
  int n_bloques;
  int tamano_memoria=4194304;
  string asociatividad;
  //FUNCIONES
  string decodificador(char c);
  void setAtributos(string asociatividad0, int tamano_cache0, int tamano_bloque0);
  int getTAMANO_tag();
	void getDatos();
  int getTAMANO_index();
  int getTAMANO_offset();
  int getNumero_filas();
  string getTAG_bin(string bin);
  string getINDEX_bin(string bin0);
  string getOFFSET_bin(string bin1);
  int decodificar_decimal(string binario, int tamano);
  int* proximo_estado(int a,int b, char c, int d);
  int* getDigitos(int numero, int tamano);
};
