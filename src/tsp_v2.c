//!  Second solution to the Travelling Salesman Problem - Dynamic Programming
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
#include <stdbool.h>

#include "../lib/cities.h"
#include "../lib/elapsed_time.h"

/**
 * 
 * Global variables
 * 
 */

static int min_length, max_length;
static int min_tour[max_n_cities + 1], max_tour[max_n_cities + 1];
static long n_tours;

/**
 * 
 * This solution to the Travelling Salesman Problem uses Dynamic Programming and Recursion
 * We store the every result we get to in a matrix, following a masking pattern that allows
 * us to effeciently store and fetch the data we want.
 * 
 * For instance, if we calculte the length of every possible permutation for n cities and 
 * store every result, later, when we are solving tsp for n+1 cities, we just need to fetch
 * previously obtained results stored in the matrix, append the new city and calculate it.
 * 
 * This solution has O(nlog(n)) time complexity
 * 
 */
static int distanciasCalculadasMin[18][1 << 18];
static int distanciasCalculadasMax[18][1 << 18];
static int minPath[5][1 << 5];

static int number;

/**
 * 
 * Simple function that returns whether or not val is in arr
 * 
 */
bool contains(int val, int *arr, int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    if (arr[i] == val)
      return true;
  }
  return false;
}

/**
 * 
 * Simple function that returns the distance between two indexes when seen as cities
 * 
 */
int dist(int a, int b)
{
  return cities[a].distance[b];
}

/**
 * 
 * Masking function, called when we need to update the mask after every iteration of
 * a given permutation
 * 
 */
int newMasker(int oldMask, int i)
{
  int newMask;
  int molde = 1 << i;
  newMask = oldMask ^ molde;

  return newMask;
}

// This code does not respect the DRY principle.

/**
 * 
 * Recursive Function that calculates the minimum distance from dest to lastCity
 * that goes through every city contained in mask
 * 
 */
int calc_dist(int dest, int mask, int lastCity)
{
  int i, m = 100000000;
  int newMask;
  if (mask == 0)
  {
    return dist(0, dest);
  }
  if (distanciasCalculadasMin[lastCity][mask] == 0) // calculo da distancia caso nao tenha sido calculada
  {
    for (i = 0; i < 15; i++) // vou calcular chamar a funçao para cada uma das cidades que o caminho tem
    {
      if ((mask & (1 << i))) // se a mascara contiver a cidade i
      {
        newMask = newMasker(mask, i);                     // retiro a cidade que descobri que pertence
        int d = calc_dist(i, newMask, i) + dist(i, dest); // calculo a distacia até a cidade que retirei+essa cidade ate dest
        if (d < m)
        {
          m = d;
          distanciasCalculadasMin[lastCity][mask] = d;

          minPath[i][mask] = i;
          printf("Adicionei à posiçao (%d,%d) o valor %d\n", i, mask, minPath[i][mask]);
          // o caminho que acaba em lastCity passa em i
          // ir do que tem maior mascara para o que tem menor mascara
        }
      }
    }
  }
  else
  {
    m = distanciasCalculadasMin[lastCity][mask];
  }
  return m;
}

/**
 * 
 * Recursive Function that calculates the maximum distance from dest to lastCity
 * that goes through every city contained in mask
 * 
 */
int calc_distv2(int dest, int mask, int lastCity)
{
  int i, m = 0;
  int newMask;
  if (mask == 0)
  {
    return dist(0, dest);
  }
  if (distanciasCalculadasMax[lastCity][mask] == 0) // calculo da distancia caso nao tenha sido calculada
  {
    for (i = 0; i < 14; i++) // vou calcular chamar a funçao para cada uma das cidades que o caminho tem
    {
      if ((mask & (1 << i))) // se a mascara contiver a cidade i
      {
        newMask = newMasker(mask, i);                       // retiro a cidade que descobri que pertence
        int d = calc_distv2(i, newMask, i) + dist(i, dest); // calculo a distacia até a cidade que retirei+essa cidade ate dest
        if (d > m)
        {
          m = d;
          distanciasCalculadasMax[lastCity][mask] = d;
        }
      }
    }
  }
  else
  {
    m = distanciasCalculadasMax[lastCity][mask];
  }
  return m;
}

/**
 * 
 * Driver function of the two above. Calculates the mask of the cities in a
 * and calls the above functions to calculate minimum and maximum paths.
 * 
 */
void tsp_v2(int n, int m, int *a)
{
  int i;

  int mascara = 0;

  for (i = 1; i < n; i++)
  {
    mascara += (1 << a[i]);
  }

  min_length = calc_dist(0, mascara, a[n - 1]);
  max_length = calc_distv2(0, mascara, a[n - 1]);
}

/**
 * 
 * Driver code
 * 
 */
int main(int argc, char **argv)
{
  int min_distances[16];
  int max_distances[16];
  double times[16];
  int n_mec, special, n, i, a[max_n_cities];
  n_mec = 89262;
  special = 0;
  double dt1;
  init_cities_data(n_mec, special);
  printf("data for init_cities_data(%d,%d)\n", n_mec, special);
  fflush(stdout);
#if 1
  print_distances();
#endif
  for (n = 3; n <= 15; n++)
  {

    if (n <= 16)
    {
      int caminho[n];
      number = n;
      for (i = 0; i < n; i++)
        a[i] = i;
      min_length = 1000000000;
      max_length = 0;
      (void)elapsed_time();
      n_tours = 0l;
      tsp_v2(n, 1, a);
      dt1 = elapsed_time();
      min_distances[n - 1] = min_length;
      max_distances[n - 1] = max_length;
      times[n - 1] = dt1;
      printf("tsp_v2 finished in %8.3fs. (calculated paths for %d different cities)\n", dt1, n);
      int mask = 0;
      for (int m = 1; m < n; m++)
      {
        mask += 1 << m;
      }

      printf("Min length: %d\n", min_length);
      printf("Min tour: [");

      printf("]\n");
      i = 0;

      printf("Max length: %d\n", max_length);
    }
  }

  /**
     * 
     * Writing our results to a file. <br>
     * We're printing: <br>
     *      > computing times for each number of cities <br>
     *      > minimum length paths for each number of cities <br>
     *      > maximum length paths for each number of cities <br>
     * 
     */
  FILE *fp = fopen("distances2.m", "w");

  int ai;

  fprintf(fp, "tempos=[");
  for (ai = 0; ai < 15; ai++)
  {
    fprintf(fp, "%f", times[ai]);
    fprintf(fp, "%s", ai == 14 ? "]; \n" : "\n,");
  }

  fprintf(fp, "mins=[");
  for (ai = 0; ai < 15; ai++)
  {
    fprintf(fp, "%d", min_distances[ai]);
    fprintf(fp, "%s", ai == 14 ? "]; \n" : "\n,");
  }

  fprintf(fp, "maxs=[");
  for (ai = 0; ai < 15; ai++)
  {
    fprintf(fp, "%d", max_distances[ai]);
    fprintf(fp, "%s", ai == 14 ? "]; \n" : "\n,");
  }

  fclose(fp);

  return 0;
}
