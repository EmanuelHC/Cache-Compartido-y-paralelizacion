#include "cache.h"

using namespace std;


string Cache::decodificador(char c)
{
  switch(c)
  {
      case '0': return "0000";
      case '1': return "0001";
      case '2': return "0010";
      case '3': return "0011";
      case '4': return "0100";
      case '5': return "0101";
      case '6': return "0110";
      case '7': return "0111";
      case '8': return "1000";
      case '9': return "1001";
      case 'A': return "1010";
      case 'B': return "1011";
      case 'C': return "1100";
      case 'D': return "1101";
      case 'E': return "1110";
      case 'F': return "1111";
      case 'a': return "1010";
      case 'b': return "1011";
      case 'c': return "1100";
      case 'd': return "1101";
      case 'e': return "1110";
      case 'f': return "1111";
      default: return "";
  }
}

void Cache::setAtributos(string asociatividad0, int tamano_cache0, int tamano_bloque0)
{
  asociatividad = asociatividad0;
  tamano_cache = tamano_cache0;
  tamano_bloque= tamano_bloque0;
//tamano_memoria=tamano_memoria0;
}

int Cache::getTAMANO_tag()
{
  return tamano_tag;
}

int Cache::getTAMANO_index()
{
  return tamano_index;
}

int Cache::getTAMANO_offset()
{
  return tamano_offset;
}

int Cache::getNumero_filas()
{
  return n_bloques;
}

void Cache::getDatos(){
  //tamano_memoria;
  int tamano_cacheKB= tamano_cache*1024;
  n_bloques=tamano_cacheKB/tamano_bloque;
  int n_palabaras=tamano_bloque/4;
  tamano_offset=log2(tamano_bloque);
  tamano_index=log2(n_bloques);
  int total_bits= log2(tamano_memoria)+10;
  tamano_tag=(total_bits - tamano_index - tamano_offset);

  cout << "tamano_memoria=" <<tamano_memoria<<" (KB)"<< endl;
  cout << "tamano_cache=" <<tamano_cache<< endl;
  cout << "n_bloques=" <<n_bloques<< endl;
  cout << "tamano_bloque=" <<tamano_bloque<< endl;
  cout << "tamano_offset=" <<tamano_offset<< endl;
  cout << "tamano_index=" <<tamano_index<< endl;
  cout << "tamano_tag=" << tamano_tag<< endl;
}

string Cache::getTAG_bin(string bin)
{
  string tag="";
  int cuenta= 31;
  string::iterator iterador0;
  string Bin=bin;
  for(iterador0=Bin.begin();iterador0<Bin.end();iterador0++)
  {
    if(cuenta>(31-tamano_tag))
    {
      tag=tag+*(iterador0);
    }
    cuenta--;
  }
  return tag;
}

string Cache::getINDEX_bin(string bin0)
{
  string  index_2="";
  string Bin0;
  Bin0=bin0;
  int cuenta= 31;
  string::iterator iterador1;
  for(iterador1=Bin0.begin();iterador1<Bin0.end();iterador1++)
  {
    if(cuenta<=(31-tamano_tag)&&cuenta>(31-tamano_tag-tamano_index))
    {
      index_2=index_2 + *(iterador1);
    }
    cuenta--;
  }
  return index_2;
}

string Cache::getOFFSET_bin(string bin1)
{
  string offset="";
  string Bin1=bin1;
  int cuenta= 31;
  string::iterator iterador2;
  for(iterador2=Bin1.begin();iterador2<Bin1.end();iterador2++)
  {
    if (cuenta<=(31-tamano_tag-tamano_index))
    {
      offset=offset+*(iterador2);
    }
    cuenta--;
  }
  return offset;
}

int Cache::decodificar_decimal(string binario, int tamano)
{
  string::iterator iterador4;
  string codificado;
  codificado=binario;
  int exponente=tamano-1;
  int valor_decimal=0;
  for(iterador4=codificado.begin(); iterador4<codificado.end();iterador4++)
  {
    int base;
    if(exponente>=0){
      if(*(iterador4)=='0'){
        base = 0;
      }else{
        base=2;
      }
      int suma;
      if (*(iterador4)=='0' && exponente==0){
        suma=0;
      }else{
        suma= pow(base,exponente);
      }
      valor_decimal= valor_decimal + suma ;
      exponente--;
    }
  }
  return valor_decimal;
}

int* Cache::proximo_estado(int estadoActual_cpu0 ,int estadoActual_cpu1, char operacion, int  numero_linea )
{
  int proximo_estado_cpu0;
  int proximo_estado_cpu1;
  int*  proximos_estados=(int*) malloc(sizeof(int)*2);
  if (numero_linea%2==0)
  {
    if(operacion=='L'){
      switch (estadoActual_cpu0)
      {
        case 00:
          switch (estadoActual_cpu1) {
            case 00://caso dato invalid en ambos cpus
              proximo_estado_cpu0=01; // se pone en exclusive
              proximo_estado_cpu1=00; // se mantiene en invalidate
              break;
            case 01:// Caso estaba en el otro cpu
              proximo_estado_cpu0=11;// estaba en el otro cpu ahora esta en los 2 y es shared
              proximo_estado_cpu1=11;// dato esta en ambos cpus
              break;
            case 10:  // caso estaba en el otro cpu modificado
              proximo_estado_cpu0=11;//dato ahora esta en ambos cpus
              proximo_estado_cpu1=11;//dato ahora esta en ambos cpus
              break;
          }
          break;
        case 01://caso dato exclusivo en cpu0
          switch (estadoActual_cpu1) {
            case 00://caso dato invalido en el otro cpu
              proximo_estado_cpu0=01; // se mantiene en exlusive
              proximo_estado_cpu1=00; // se mantiene en invalidate
              break;
          }
          break;
        case 10:  //caso dato modificado en cpu0
          switch (estadoActual_cpu1) {
            case 00://caso dato invalido en el otro cpu
              proximo_estado_cpu0=10; // se mantiene en modificado
              proximo_estado_cpu1=00; // se mantiene en invalidate
              break;
          }
          break;
        case 11:
          switch (estadoActual_cpu1) {
            case 11://dato debe estar compartido tambien en el otro cpu
              proximo_estado_cpu0=11; // se mantiene shared
              proximo_estado_cpu1=11; // se mantiene shared
              break;
          }
          break;
      }
    }
    if(operacion=='S'){
      switch (estadoActual_cpu0)
      {
        case 00:
          switch (estadoActual_cpu1) {
            case 00://caso dato invalid en ambos cpus
              proximo_estado_cpu0=10; // **se pone en invalido
              proximo_estado_cpu1=00; // se mantiene en invalidate
              break;
            case 01:// Caso estaba en el otro cpu
              proximo_estado_cpu0=10;// se realiza escritura dato debe estar en modificado
              proximo_estado_cpu1=00;// se debe poner en invalido
              break;
            case 10:  // caso estaba en el otro cpu modificado
              proximo_estado_cpu0=10;//dato ahora esta modificado
              proximo_estado_cpu1=00;//se debe invalidar el dato
              break;
          }
          break;
        case 01://caso dato exclusivo en cpu0
          switch (estadoActual_cpu1) {
            case 00://caso dato invalido en el otro cpu
              proximo_estado_cpu0=10; // se escribe por lo que se modifica
              proximo_estado_cpu1=00; // se mantiene en invalidate
              break;
          }
          break;
        case 10:  //caso dato modificado en cpu0
          switch (estadoActual_cpu1) {
            case 00://caso dato invalido en el otro cpu
              proximo_estado_cpu0=10; // se mantiene en modificado
              proximo_estado_cpu1=00; // se mantiene en invalidate
              break;
          }
          break;
        case 11:
          switch (estadoActual_cpu1) {
            case 11://dato debe estar compartido tambien en el otro cpu
              proximo_estado_cpu0=10; // se escribe por lo que pasa a modificado
              proximo_estado_cpu1=00; // se debe invalidar
              break;
          }
          break;
      }
    }
  }
  if (numero_linea%2!=0)
  {
    if(operacion=='L'){
      switch (estadoActual_cpu1)
      {
        case 00:
          switch (estadoActual_cpu0) {
            case 00://caso dato invalid en ambos cpus
              proximo_estado_cpu0=00; // se mantiene en invalidate
              proximo_estado_cpu1=01; // se pone en exclusive
              break;
            case 01:// Caso estaba en el otro cpu
              proximo_estado_cpu0=11;// estaba en el otro cpu ahora esta en los 2 y es shared
              proximo_estado_cpu1=11;// dato esta en ambos cpus
              break;
            case 10:  // caso estaba en el otro cpu modificado
              proximo_estado_cpu0=11;//dato ahora esta en ambos cpus
              proximo_estado_cpu1=11;//dato ahora esta en ambos cpus
              break;
          }
          break;
        case 01://caso dato exclusivo en cpu0
          switch (estadoActual_cpu0) {
            case 00://caso dato invalido en el otro cpu
              proximo_estado_cpu0=00; // se mantiene en invalidate
              proximo_estado_cpu1=01; // se mantiene en exlusive
              break;
          }
          break;
        case 10:  //caso dato modificado en cpu1
          switch (estadoActual_cpu0) {
            case 00://caso dato invalido en el otro cpu
              proximo_estado_cpu0=00; // se mantiene en invalidate
              proximo_estado_cpu1=10; // se mantiene en modificado
              break;
          }
          break;
        case 11:
          switch (estadoActual_cpu0) {
            case 11://dato debe estar compartido tambien en el otro cpu
              proximo_estado_cpu0=11; // se mantiene shared
              proximo_estado_cpu1=11; // se mantiene shared
              break;
          }
          break;
      }
    }
    if(operacion=='S'){
      switch (estadoActual_cpu1)
      {
        case 00:
          switch (estadoActual_cpu0) {
            case 00://caso dato invalid en ambos cpus
              proximo_estado_cpu1=01; // se pone en exclusive
              proximo_estado_cpu0=00; // se mantiene en invalidate
              break;
            case 01:// Caso estaba en el otro cpu
              proximo_estado_cpu1=10;// se realiza escritura dato debe estar en modificado
              proximo_estado_cpu0=00;// se debe poner en invalido
              break;
            case 10:  // caso estaba en el otro cpu modificado
              proximo_estado_cpu1=10;//dato ahora esta modificado
              proximo_estado_cpu0=00;//se debe invalidar el dato
              break;
          }
          break;
        case 01://caso dato exclusivo en cpu1
          switch (estadoActual_cpu0) {
            case 00://caso dato invalido en el otro cpu
              proximo_estado_cpu1=10; // se escribe por lo que se modifica
              proximo_estado_cpu0=00; // se mantiene en invalidate
              break;
          }
          break;
        case 10:  //caso dato modificado en cpu1
          switch (estadoActual_cpu0) {
            case 00://caso dato invalido en el otro cpu
              proximo_estado_cpu1=10; // se mantiene en modificado
              proximo_estado_cpu0=00; // se mantiene en invalidate
              break;
          }
          break;
        case 11:
          switch (estadoActual_cpu0) {
            case 11://dato debe estar compartido tambien en el otro cpu
              proximo_estado_cpu1=10; // se escribe por lo que pasa a modificado
              proximo_estado_cpu0=00; // se debe invalidar
              break;
          }
          break;
      }
    }
  }
  proximos_estados[0]= proximo_estado_cpu0;
  proximos_estados[1]= proximo_estado_cpu1;
  return proximos_estados;
}

int* Cache::getDigitos(int numero, int tamano){
    //int iNums = 10;
    //int iNumsSize = 2;
   int n = 2;
    int* array = (int*) malloc(sizeof(int)*n);
   for (int i=tamano-1; i>=0; i--) {
       int y = pow(10, i);
       int z = numero/y;
       int x2 = numero / (y * 10);
       array[i]  = z-x2*10;
       cout<< "digito = " << array[i] <<endl;
   }
   return array;

}
