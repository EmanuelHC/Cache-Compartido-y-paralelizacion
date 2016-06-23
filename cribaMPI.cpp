#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <mpi.h>
#include <math.h>
#include <string.h>      /* time */
#include <fstream>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <ctime>
#include <ratio>
#include <chrono>
#define TIME_SECS 25 
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_SIZE(id,p,n) (BLOCK_LOW(((id)+1),p,n)-BLOCK_LOW(id,p,n))

//#include <random>
using namespace std;
using namespace std::chrono;


bool criba(int tam,int  argc, char* argv[]){

    int m[48615], resultado[48615]; //Se inicia el buffer m con el numero minimo requerido para calcular los primeroso 5000 primos
    //Se inicia el buffer de resultados asimismo
    int a=0;    //a es el contador de numeros primos totales encontrados 
    m[0] = 0;
    m[1] = 0;
    int id, proc;     
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD); //Se inicia una barrier para que todos los procesos inicien en sincronia
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Comm_size(MPI_COMM_WORLD,&proc);
    
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    resultado[0] = 1;
    resultado[1] = 1; 
    for(int i = 2; i < tam; ++i){ //se rellena el arreglo como sin marcar
        m[i] = 1;
	resultado[i] = 1;
    }
    
    
    for(int i = floor(2+id*(tam/proc)); i < (id+1)*(sqrt(tam)); i+=1) {  //se tachan los multiplos h del numero
        if(m[i]==1) {
            for(int h = 2; i*h < tam; ++h)
                m[i*h] = 0;
        }
      
    }

    MPI_Allreduce(m, resultado, 48615, MPI_INT, MPI_PROD, MPI_COMM_WORLD); //Se reducen todos los m locales a resultado, que es 
                                                                           //la variable global
      
    MPI_Finalize();
    	// para imprimir se usa el id del proceso master
     if(!id){
      high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  cout << "Tiempo del ejecucion " << time_span.count()/TIME_SECS << " segundos." << endl;
     for (int j = 2; j <=tam; j++) {
      if (resultado[j]==1){
        a++;

       }
      
      } // Se imprime la cantidad total de numeros primos
      cout << "A = " << a <<endl;
     }
    return m;
}

int main(int  argc, char* argv[]){
  
   
  criba(48615,argc,argv);
  
//-------------------------------------------------------------------------------
 // high_resolution_clock::time_point t2 = high_resolution_clock::now();
 // duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
 // cout << "Tiempo del ejecucion " << time_span.count() << " segundos." << endl;
//------------------------------------------------------------------------------
 
  return 0;
 
 
  //MPI_Finalize();
}
