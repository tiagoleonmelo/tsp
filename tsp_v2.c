//
// Joao Nogueira
// Tiago Melo
// ...
//
// AED, 2018/2019
//
// solution of the traveling salesman problem
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <assert.h>
#include <stdbool.h>

#include "cities.h"
#include "../P01/elapsed_time.h"


//
// record best solutions
//

static int min_length,max_length;
static int min_tour[max_n_cities + 1],max_tour[max_n_cities + 1];
static long n_tours;
static int distanciasCalculadasMin[18][1<<18];
static int distanciasCalculadasMax[18][1<<18];
static int minPath[5][1<<5];
static int NUMEROFIXE;

//
// second solution
//

bool contains(int val, int *arr, int size){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return true;
    }
    return false;
}

int dist(int a, int b){
  return cities[a].distance[b];
}

int newMasker(int oldMask, int i){   // funçao que ira decrementar a ultima cidade
  int newMask;
  int molde = 1<<i;
  newMask=oldMask^molde;

  return newMask;
}

int calc_dist(int dest, int mask, int lastCity)
{
  int i,m=100000000;
  int newMask;
  if(mask==0){
    return dist(0,dest);
  }
  if(distanciasCalculadasMin[lastCity][mask]==0) // calculo da distancia caso nao tenha sido calculada
  {
    for(i=0; i < 15; i++) // vou calcular chamar a funçao para cada uma das cidades que o caminho tem
    {
      if((mask&(1<<i)))   // se a mascara contiver a cidade i
      {
        newMask=newMasker(mask, i);    // retiro a cidade que descobri que pertence
        int d=calc_dist(i,newMask,i)+dist(i,dest);  // calculo a distacia até a cidade que retirei+essa cidade ate dest
        if(d < m){
          m=d;
          distanciasCalculadasMin[lastCity][mask]=d;

          minPath[i][mask]=i;
          printf("Adicionei à posiçao (%d,%d) o valor %d\n",i,mask,minPath[i][mask]);
          // o caminho que acaba em lastCity passa em i
          // ir do que tem maior mascara para o que tem menor mascara
        }
      }
    }
  }else{
    m=distanciasCalculadasMin[lastCity][mask];
  }
  return m;
}

int calc_distv2(int dest, int mask, int lastCity)
{
  int i,m=0;
  int newMask;
  if(mask==0){
    return dist(0,dest);
  }
  if(distanciasCalculadasMax[lastCity][mask]==0) // calculo da distancia caso nao tenha sido calculada
  {
    for(i=0; i < 14; i++) // vou calcular chamar a funçao para cada uma das cidades que o caminho tem
    {
      if((mask&(1<<i)))   // se a mascara contiver a cidade i
      {
        newMask=newMasker(mask, i);    // retiro a cidade que descobri que pertence
        int d=calc_distv2(i,newMask,i)+dist(i,dest);  // calculo a distacia até a cidade que retirei+essa cidade ate dest
        if(d > m){
          m=d;
          distanciasCalculadasMax[lastCity][mask]=d;
        }
      }
    }
  }else{
    m=distanciasCalculadasMax[lastCity][mask];
  }
  return m;
}

void tsp_v2(int n,int m,int *a)
{
  int i;

  int mascara=0;

  for(i = 1; i < n; i++){
    mascara+=(1<<a[i]); //todos os elementos menos o primeiro
  }

  min_length=calc_dist(0,mascara,a[n-1]); // como temos um ciclo, o destino ira ser sempre a cidade de partida (0) e a mascara sera as cidades por onde temos de passar!
  max_length=calc_distv2(0,mascara,a[n-1]);
}

//
// main program
//

int main(int argc,char **argv)
{
  int min_distances[16];
  int max_distances[16];
  double times[16];
  int n_mec,special,n,i,a[max_n_cities];
  n_mec = 89262;
  special = 0;
  double dt1;
  init_cities_data(n_mec,special);
  printf("data for init_cities_data(%d,%d)\n",n_mec,special);
  fflush(stdout);
#if 1
  print_distances();
#endif
  for(n = 3;n <= 15;n++)     // n => numero de cidades visitadas (n_cities)
  {
    //
    // try tsp_v2
    //
    if(n <= 16)
    {
      int caminho[n];
      NUMEROFIXE=n;
      for(i = 0;i < n;i++)
        a[i] = i;
      min_length = 1000000000;
      max_length = 0;
      (void)elapsed_time();
      n_tours = 0l;
      tsp_v2(n,1,a);
      dt1 = elapsed_time();
      min_distances[n-1]=min_length;
      max_distances[n-1]=max_length;
      times[n-1]=dt1;
      printf("tsp_v2 finished in %8.3fs. (calculated paths for %d different cities)\n",dt1,n);
      int mask=0;
      for(int m=1; m < n; m++)
      {
        mask+=1<<m;
      }

      printf("Min length: %d\n",min_length);
      printf("Min tour: [");
      /*for(i = n-1; i > 0; i--){
        printf("Estou a aceder à posiçao (%d,%d), onde esta o valor %d\n",i,mask,minPath[i][mask]);
        (i!=n)? printf("%d; ",minPath[i][mask]) : printf("%d",minPath[i][mask]);
        //retirar a ultima cidade da mascara
        mask=newMasker(mask,minPath[i][mask]);  // retirar a cidade que acabei de visitar
        //i=minPath[i][mask]+1;
      }*/

      printf("]\n");
      i=0;

      printf("Max length: %d\n",max_length);
      /*printf("Max tour: [");
      for(i = 0; i < n; i++)
        (i!=n-1)? printf("%d; ",max_tour[i]) : printf("%d",max_tour[i]);
      printf("]\n");*/

    }
  }

  FILE *fp = fopen("distances2.m","w");

  int ai;

  fprintf(fp,"tempos=[");
  for(ai = 0; ai < 15; ai++){
    fprintf(fp,"%f",times[ai]);
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
