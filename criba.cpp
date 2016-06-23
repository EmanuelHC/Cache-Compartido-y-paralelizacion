#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <string.h>      /* time */
#include <fstream>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <ctime>
#include <ratio>
#include <chrono>

#define N 48615

//#include <random>
using namespace std;
using namespace std::chrono;
 
bool criba(int tam){

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    bool m[N];
    int a = 0;
    m[0] = false;
    m[1] = false;
    for(int i = 2; i <= tam; ++i)
        m[i] = true;

    for(int i = 2; i*i <= tam; ++i) {
        if(m[i]) {
            for(int h = 2; i*h <= tam; ++h)
                m[i*h] = false;
        }
    }
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout << "Tiempo del ejecucion " << time_span.count() << " segundos." << endl;
    
    for (int j = 2; j <=tam; j++) {
      if (m[j]){
        a++;
     //   cout << j << endl;
       }  
    }
    cout << "A = " << a <<endl;
      
     
    return m;
}

int main(int  argc, char* argv[]){
  
  int tamano=N;
  //std::cout << "Numero primos entre 0 y N:" << std::endl;
  criba(tamano);

  return 0;
}
