#include "cache.h"
#include <stdlib.h>
#include <sstream>

using namespace std;


int main (int argc, char *argv[])
{
  string asociatividad = "Directo";
  int tamano_cache = 8;
  int tamano_bloque = 16;
  int tamano_shared = 64;

  Cache CPU0, CPU1, Shared;
  CPU0.setAtributos(asociatividad,tamano_cache,tamano_bloque );
  CPU0.getDatos();
  CPU1.setAtributos(asociatividad,tamano_cache,tamano_bloque );
  CPU1.getDatos();
  Shared.setAtributos(asociatividad,tamano_shared,tamano_bloque );
  Shared.getDatos();

//___________________________________________________________________________________________________________________
//Se crean los tres caches con el tamano indicado por el index correspondiente
   //Se llena inicialmente  de ceros
   int CPU0_filas = CPU0.getNumero_filas();
   int CPU0_col     = tamano_bloque+2;
   int cacheCPU0[CPU0_filas][CPU0_col];//crea un cache con el numero de filas determinado
   int CPU1_filas = CPU1.getNumero_filas();
   int CPU1_col     = tamano_bloque+2;
   int cacheCPU1[CPU1_filas][CPU1_col];//crea un cache con el numero de filas determinado
   int Shared_filas = Shared.getNumero_filas();
   int Shared_col     = tamano_bloque+2;
   int Shared_L2[Shared_filas][Shared_col];

   for (int x=0;x<=CPU0_filas;x++) { //asigna Invalidate = 00 para el estado inicial de todos los datos de las instrucciones
     for(int y=0;y<CPU0_col;y++){
     }
     cacheCPU0[x][CPU0_col-1]=0;
     cacheCPU0[x][CPU0_col-2]=0;
    }
    for (int x=0;x<=CPU1_filas;x++) { //asigna Invalidate = 00 para el estado inicial de todos los datos de las instrucciones
      for(int y=0;y<CPU1_col;y++){
      }
      cacheCPU1[x][CPU1_col-1]=0;
      cacheCPU1[x][CPU1_col-2]=0;
     }
        // for (int x=0;x<=CPU1_filas;x++) { //asigna Invalidate = 00 para el estado inicial de todos los datos de las instrucciones
    //   for(int y=0;y<CPU1_col;y++){
    //   }
    //    Shared[x][CPU1_col-1]=0;
    //    Shared[x][CPU1_col-2]=0;
    //  }
  std::cout << "________________________________________________________________________________" << std::endl;
//___________________________________________________________________________________________________________________
//Se lee las direcciones
  ifstream ficheroEntrada;
  string instruccion_full;
  string::iterator it;
  string::iterator it2;
  string instruccion_hexa="";
  string instruccion_bin="";
  string bin="";
  bool flag = false;
  char LS;
  unsigned long int rmiss=0;
  unsigned long int wmiss = 0;
  unsigned long int whit = 0;
  unsigned long int rhit=0;
  unsigned long int tHitMiss = 0;
  int linea = 1;
  ficheroEntrada.open ( "aligned.trace" , ios::in);
  if (ficheroEntrada.is_open()) {
    while (! ficheroEntrada.eof() ) {
      int n=0;
      getline (ficheroEntrada,instruccion_full);
        for(it=instruccion_full.begin();it<=instruccion_full.end();it++){//recorre la instrucion
          if(*(it)=='L'|| *(it)=='S')
          {
            LS = *(it);
          }
          if(*(it)!='L' && *(it)!='S' && *(it)!=' ' ){
            instruccion_hexa=instruccion_hexa + *it;//recorre la linea para devolver la instrucion en hexadecimal
          }
        }
      for(it2=instruccion_hexa.begin();it2<=instruccion_hexa.end();it2++){
        char c=*it2;
        instruccion_bin= instruccion_bin+ CPU0.decodificador(c);//devuelve la instrucion en binario
      }
  //Se optienen index tag y offet en binario
    string tag2,index2,offset2;
    tag2=CPU0.getTAG_bin(instruccion_bin);
    index2=CPU0.getINDEX_bin(instruccion_bin);
    offset2=CPU0.getOFFSET_bin(instruccion_bin);
//___________________________________________________________________________________________________________________________________
//Se pasan las instrucciones a decimal
    int tag10,index10,offset10;
    int tamano_tag;
    int* estado_cpu0 =(int*) malloc(sizeof(int)*2);
    int* estado_cpu1 =(int*) malloc(sizeof(int)*2);
    tamano_tag=CPU0.getTAMANO_tag();
    int tamano_offset=CPU0.getTAMANO_offset();
    int tamano_index=CPU0.getTAMANO_index();
    tag10=CPU0.decodificar_decimal(tag2, tamano_tag);
    index10=CPU0.decodificar_decimal(index2, tamano_index);
    offset10=CPU0.decodificar_decimal(offset2,tamano_offset);



//____________________________________________________________________________________________________________________________________________
//Lectura de direcciones para CPU0
      if(LS == 'L'){
        if(linea%2==0){
           // if (Icpu0)
           if(!cacheCPU0[index10][CPU0_col-1] && !cacheCPU0[index10][CPU0_col-2]){
             rmiss++;
             // if (Icpu0 && Icpu1)
             if(!cacheCPU0[index10][CPU0_col-1] && !cacheCPU0[index10][CPU0_col-2] &&
                 !cacheCPU1[index10][CPU1_col-1] && !cacheCPU1[index10][CPU1_col-2]){// habia un dato invalido
                 for (int z=0;z<tamano_bloque;z++){ //se trae el dato de memoria principal
                    cacheCPU0[index10][z]=tag10;
                //    std::cout << "debug" << std::endl;
                 }

              }
             //CASO ESPECIAL: if (Mcpu1 && Icpu0)
             if(!cacheCPU0[index10][CPU0_col-1] && !cacheCPU0[index10][CPU0_col-2] &&
             !cacheCPU1[index10][CPU1_col-1] && cacheCPU1[index10][CPU1_col-2]){
             for (int z=0;z<tamano_bloque;z++){
                Shared_L2[index10][z]=cacheCPU1[index10][z]; //se guarda el dato en L2
                cacheCPU0[index10][z] = Shared_L2[index10][z]; //se lee el dato de L2
              //  std::cout << "debug" << std::endl;
                }
              }
              //CASO ESPECIAL: if (Ecpu1 && Icpu0)
              if(!cacheCPU0[index10][CPU0_col-1] && !cacheCPU0[index10][CPU0_col-2] &&
              !cacheCPU1[index10][CPU1_col-2] && cacheCPU1[index10][CPU1_col-1]){
              for (int z=0;z<tamano_bloque;z++){
                 cacheCPU0[index10][z] = tag10; //se lee el dato de la memoria principal
                 }
               }
           }else{
              if(cacheCPU0[index10][offset10]==tag10)
              rhit++;
           }
          }else{
            // if (Icpu1)
            if(!cacheCPU1[index10][CPU1_col-1] && !cacheCPU1[index10][CPU1_col-2]){
              rmiss++;
              // if (Icpu0 && Icpu1)
              if(!cacheCPU0[index10][CPU0_col-1] && !cacheCPU0[index10][CPU0_col-2] &&
                  !cacheCPU1[index10][CPU1_col-1] && !cacheCPU1[index10][CPU1_col-2]){// habia un dato invalido
                  for (int z=0;z<tamano_bloque;z++){ //se trae el dato de memoria principal
                     cacheCPU1[index10][z]=tag10;
                  }
               }
              //CASO ESPECIAL: if (Mcpu0 && Icpu1)
              if(cacheCPU0[index10][CPU0_col-2] && !cacheCPU0[index10][CPU0_col-1] &&
              !cacheCPU1[index10][CPU1_col-1] && !cacheCPU1[index10][CPU1_col-2]){
              for (int z=0;z<tamano_bloque;z++){
                 Shared_L2[index10][z]=cacheCPU0[index10][z]; //se guarda el dato en L2
                 cacheCPU1[index10][z] = Shared_L2[index10][z]; //se lee el dato de L2
                 //std::cout << "debug2" << std::endl;
                 }
               }
               //CASO ESPECIAL: if (Ecpu0 && Icpu1)
               if(cacheCPU0[index10][CPU0_col-1] && !cacheCPU0[index10][CPU0_col-2] &&
               !cacheCPU1[index10][CPU1_col-1] && !cacheCPU1[index10][CPU1_col-2]){
               for (int z=0;z<tamano_bloque;z++){
                  cacheCPU1[index10][z] = tag10; //se lee de memoria principal
                  }
                }
            }else{
              if(cacheCPU0[index10][offset10]==tag10)
               rhit++;
            }
          }
      }else if(LS == 'S'){
    //Escritura de datos para CPU0 y CPU1
        if(linea%2==0){
           if(!cacheCPU0[index10][CPU0_col-1] && !cacheCPU0[index10][CPU0_col-2]){
             wmiss++;
             for (int z=0;z<tamano_bloque;z++){ //se trae el dato de memoria principal
                    cacheCPU0[index10][z]=tag10;
              }

           }else{
             if(cacheCPU0[index10][offset10]==tag10)
              whit++;
           }
          }else{
            if(!cacheCPU1[index10][CPU1_col-1] && !cacheCPU1[index10][CPU1_col-2]){
              wmiss++;
                  for (int z=0;z<tamano_bloque;z++){ //se trae el dato de memoria principal
                     cacheCPU1[index10][z]=tag10;
                  }
            }else{
              if(cacheCPU1[index10][offset10]==tag10)
               whit++;
            }
          }
      }

      //----------------------------------------------------------------------------------------------------------------
    
      //---------------------------------------------------------------------------------------------------------------


    //SE ACTUALIZA MAQUINA DE ESTADOS
    int currentStateCPU0[2] = {cacheCPU0[index10][CPU0_col-2], cacheCPU0[index10][CPU0_col-1]};
    int currentStateCPU1[2] = {cacheCPU1[index10][CPU1_col-2], cacheCPU1[index10][CPU1_col-1]};
    int estado_actual_cpu0 = currentStateCPU0[1]*10+currentStateCPU0[0];
    int estado_actual_cpu1 = currentStateCPU1[1]*10+currentStateCPU1[0];
    int* proximos_estados = CPU0.proximo_estado(estado_actual_cpu0, estado_actual_cpu1, LS,linea);

    // std::cout << "estado_actual_cpu0=" <<estado_actual_cpu0 <<std::endl;
    // std::cout << "estado_actual_cpu1=" <<estado_actual_cpu0 <<std::endl;
    //Se debe codificar los estados segun el codigo definido para la maquina de estados
    if(proximos_estados[0]==1){
      estado_cpu0[1] = 1;
      estado_cpu0[0] = 0;
    }else if(proximos_estados[0]==0)
    {
      estado_cpu0[0] = 0;
      estado_cpu0[1] = 0;
    }else if(proximos_estados[0]==10){
      estado_cpu0[0] = 1;
      estado_cpu0[1] = 0;
    }else if(proximos_estados[0]==11){
      estado_cpu0[0] = 1;
      estado_cpu0[1] = 1;
    }
    if(proximos_estados[1]==1){
      estado_cpu1[1] = 1;
      estado_cpu1[0] = 0;
    }else if(proximos_estados[1]==0)
    {
      estado_cpu1[0] = 0;
      estado_cpu1[1] = 0;
    }else if(proximos_estados[1]==10)
    {
      estado_cpu1[0] = 1;
      estado_cpu1[1] = 0;
    }else if(proximos_estados[1]==11)
    {
      estado_cpu1[0] = 1;
      estado_cpu1[1] = 1;
    }
    cacheCPU0[index10][CPU0_col-1]  = estado_cpu0[1]; //lo que devuelve la maquina
    cacheCPU0[index10][CPU0_col-2]  = estado_cpu0[0];
    cacheCPU1[index10][CPU0_col-1]  = estado_cpu1[1]; //lo que devuelve la maquina
    cacheCPU1[index10][CPU0_col-2]  = estado_cpu1[0];

    //  std::cout << "proximos_estados[0] ()=" <<proximos_estados[0]<< std::endl;
    //  std::cout << "proximos_estados[1] ()=" <<proximos_estados[1]<< std::endl;
    //  std::cout << "Siguiente estado cpu0=" <<cacheCPU0[index10][CPU0_col-2]<<cacheCPU0[index10][CPU0_col-1]<< std::endl;
    //  std::cout << "Siguiente estado cpu1=" <<cacheCPU1[index10][CPU1_col-2]<<cacheCPU1[index10][CPU1_col-1]<< std::endl;
    //  std::cout << "-----------------------------------------------------------------------------------------------" << std::endl;


//____________________________________________________________________________________________________________________________________________

if(linea>=49642118){
  std::cout << "Linea:" <<linea<< std::endl;
  //std::cout << "estado CPU0:" <<cacheCPU0[index10][CPU0_col-2]<< cacheCPU0[index10][CPU0_col-1]<< std::endl;
  //std::cout << "estado CPU1:" <<cacheCPU1[index10][CPU1_col-2]<< cacheCPU1[index10][CPU1_col-1]<< std::endl;
  std::cout << "Estado cpu0:" <<cacheCPU0[index10][CPU0_col-2]<<cacheCPU0[index10][CPU0_col-1]<< std::endl;
  std::cout << "Estado cpu1:" <<cacheCPU1[index10][CPU1_col-2]<<cacheCPU1[index10][CPU1_col-1]<< std::endl;
}



    instruccion_hexa="";
    instruccion_bin="";
    linea+=1;

        }
    if(ficheroEntrada.eof()){
     // std::cout << "Final del archivo" << std::endl;
    }
    rmiss= rmiss-1;
    tHitMiss=rhit+whit+rmiss+wmiss;
    float miss_rate = (float)(rmiss)/((float)(rhit+rmiss));
    // cout << "HITS = " <<rhit<< endl;
    // cout << "R MISS = " <<rmiss<< endl;
    // cout << "W MISS = " <<wmiss<< endl;
    // std::cout << "total" <<tHitMiss<< std::endl;
    // cout << "Miss rate = "<< miss_rate <<endl;



    ficheroEntrada.close();
  }
  else{
  cout << "Fichero inexistente o faltan permisos para abrirlo" << endl;
}

std::cout << "fin" << std::endl;
return 0;



}
