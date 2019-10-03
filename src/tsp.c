//!  First solution of the Travelling Salesman Problem - Brute Force
/*!

  Joao Nogueira
  Tiago Melo
  ..

  AED, 2018/2019
  Documented @ CSLP, 2019/2020

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <assert.h>

#include "../lib/cities.h"
#include "../lib/elapsed_time.h"

/**
 * 
 * Global variables
 * 
 */

static int min_length,max_length;
static int min_tour[max_n_cities + 1],max_tour[max_n_cities + 1];
static long int n_tours;

/**
 * 
 * This variable will store the amount of paths of i distance.
 * If hist[1000] == 7 then we found 7 paths that had a distance that summed up to 1000
 * 
 */

long hist[10001]; 



/**
 * 
 * First solution - brute force, distance computed at the end, compute best and worst tours
 * Generate (n-1)! different permutations for each a[different cities] of length 
 * and compute the length for each of those
 * This has O(n!) time complexity
 * 
 */

void tsp_v1(int n, int m, int *a)
{
  int i,t,length;
  length=0;

  if(m < n - 1)
    for(i = m;i < n;i++){
      t = a[m];
      a[m] = a[i];
      a[i] = t;
      tsp_v1(n,m + 1,a);
      t = a[m];
      a[m] = a[i];
      a[i] = t;
    }else{

      /**
       * 
       * Visiting a given permutation of cities and calculating its cummulative distance
       * 
       */


      length=0;

      for(i = 0;i < n;i++){
        if(i != n-1){
          length += cities[a[i]].distance[a[i+1]];
        }else{
          length += cities[a[i]].distance[a[0]];
        }
      }

      if(length < min_length){
        min_length = length;
        for(i = 0; i < n; i++)
          min_tour[i]=a[i];
      }

      if(length > max_length){
        max_length = length;
        for(i = 0; i < n; i++)
          max_tour[i]=a[i];
      }

      /**
       * 
       * Updating our data
       * 
       */

      hist[length]++;
      n_tours++;


  }
}

/**
 * 
 * We are also generating random paths for each path a with length n
 * 
 */

void rand_perm(int n,int a[])
{
  int i,j,k;
  for(i = 0;i < n;i++)
    a[i] = i;
  for(i = n - 1;i > 0;i--)
  {
    j = (int)floor((double)(i + 1) * (double)rand() / (1.0 + (double)RAND_MAX)); // range 0...i
    assert(j >= 0 && j <= i);
    k = a[i];
    a[i] = a[j];
    a[j] = k;
  }
}

/**
 * 
 * Function that returns the cummulative distance of transversing the path a
 * of length n
 * 
 */

int distance_calc(int n, int a[]){
  int length=0;
  for(int i = 0;i < n;i++){
    if(i != n-1){
      length += cities[a[i]].distance[a[i+1]];
    }else{
      length += cities[a[i]].distance[a[0]];
    }
  }
  return length;
}

/**
 * 
 * Driver code
 * 
 */

int main(int argc,char **argv)
{
  int n_mec,special,n,a[max_n_cities], j, randMaxDistance, randMinDistance;
  double tempos[16];
  int min_distances[16];
  int max_distances[16];
  long int i;
  char file_name[32];
  double dt1;
  
  randMaxDistance = 0;
  n_mec = 89005;
  special = 0;
  init_cities_data(n_mec,special);
  printf("data for init_cities_data(%d,%d)\n",n_mec,special);
  fflush(stdout);
#if 1
  print_distances();
#endif
  for(n = 3;n <= 15;n++){     // n => numero de cidades visitadas (n_cities)

    dt1 = -1.0;
    if(n <= 16)
    {
      randMinDistance = 1000000;
      (void)elapsed_time();
      for(i = 0;i < n;i++)
        a[i] = i;
      min_length = 1000000000;
      max_length = 0;
      n_tours = 0l;
      tsp_v1(n,1,a); // no need to change the starting city, as we are making a tour
      dt1 = elapsed_time();
      tempos[n]=dt1;
      min_distances[n]=min_length;
      max_distances[n]=max_length;
      printf("tsp_v1() finished in %8.3fs (%ld tours generated for %d different cities)\n",dt1,n_tours,n);
      printf("  min %5d [",min_length);
      for(i = 0;i < n;i++)
        printf("%2d%s",min_tour[i],(i == n - 1) ? "]\n" : ",");
      printf("  max %5d [",max_length);
      for(i = 0;i < n;i++)
        printf("%2d%s",max_tour[i],(i == n - 1) ? "]\n" : ",");
      fflush(stdout);

     int d;

      for(j=0;j < 100000; j++){   // geraçao de 100000 caminhos aleatorios
        rand_perm(n,a);
        d = distance_calc(n,a); // calculo da distancia de um caminho aleatorio
        if(d < randMinDistance){
          randMinDistance = d;
          }

        if(d > randMaxDistance){
          randMaxDistance = d;
        }

      }


      printf("Random Min Distance: %d\n", randMinDistance);
      printf("Random Max Distance: %d\n", randMaxDistance);


      // Criaçao de um array com D lugares, onde D e o maximo de distancia possivel
      // E armazeno, no index distance_calc(n,a) o numero de vezes que apareceu essa distancia
      // Assim, vou ter um array que me da a frequencia relativa da distancia em funçao da distancia  (<---- Opçao escolhida)

      // OR

      // Criaçao de 2 arrays, um que servira de eixo das abcissas que tera as distancias
      // e outro que servira de eixo das ordenadas que tera as frequencias.
      // Sempre que calculo uma distancia irei adicionar essa mesma distancia ao array.
      // Na posiçao onde adiciono essa distancia adiciono 1 no outro array.

      // OR

      // Criaçao de um unico array com todas as distancias que sao calculadas com a geraçao
      // de todos os caminhos possiveis. A funçao hist() do matlab faz automaticamente a frequencia
      // relativa de cada um!!

      if(argc == 2 && strcmp(argv[1],"-f") == 0)
      {
        min_tour[n] = -1;
        sprintf(file_name,"min_%02d.svg",n);
        make_map(file_name,min_tour);
        max_tour[n] = -1;
        sprintf(file_name,"max_%02d.svg",n);
        make_map(file_name,max_tour);
      }
    }
  }
    /**
     * 
     * Writing our results to a file.
     * We're printing:
     *      > computed distance
     *      > computing times for each number of cities
     *      > minimum length paths for each number of cities
     *      > maximum length paths for each number of cities
     * 
     */ 

   FILE *fp = fopen("distances.m","w");

   long int ai;
   fprintf(fp,"distancias=[");
   for(ai = 0; ai < 10001; ai++){
     fprintf(fp,"%ld",hist[ai]);
     fprintf(fp,"%s", ai == 10000 ? "]; \n": "\n,");
   }

   fprintf(fp,"tempos=[");
   for(ai = 0; ai < 15; ai++){
     fprintf(fp,"%f",tempos[ai]);
     fprintf(fp,"%s", ai == 14 ? "]; \n": "\n,");
   }

   fprintf(fp,"mins=[");
   for(ai = 0; ai < 15; ai++){
     fprintf(fp,"%d",min_distances[ai]);
     fprintf(fp,"%s", ai == 14 ? "]; \n": "\n,");
   }

   fprintf(fp,"maxs=[");
   for(ai = 0; ai < 15; ai++){
     fprintf(fp,"%d",max_distances[ai]);
     fprintf(fp,"%s", ai == 14 ? "]; \n": "\n,");
   }

   fclose(fp);



  return 0;
}
