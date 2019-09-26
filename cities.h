//
// Tomás Oliveira e Silva, AED, October 2018
//
// City data for the traveling salesman problem
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define max_n_cities  18

static struct
{
  char name[32];
  double x,y;
  int distance[max_n_cities]; // we use integer distances, to avoid rounding errors
}
cities[max_n_cities];

static int n_cities = 0;

static int find_city(char *name)
{
  int i;

  for(i = 0;i < n_cities;i++)
    if(strcmp(name,cities[i].name) == 0)
      return i;
  return -1;
}

static void new_city(char *name,double x,double y)
{
  int i;

  if(find_city(name) >= 0)
  {
    fprintf(stderr,"City %s already exists\n",name);
    exit(1);
  }
  if(n_cities == max_n_cities)
  {
    fprintf(stderr,"Too many cites (unable to add city %s)\n",name);
    exit(1);
  }
  if(strlen(name) >= sizeof(cities[n_cities].name))
  {
    fprintf(stderr,"The city name (%s) is too large\n",name);
    exit(1);
  }
  (void)strcpy(cities[n_cities].name,name);
  cities[n_cities].x = x;
  cities[n_cities].y = y;
  for(i = 0;i < max_n_cities;i++)
    cities[n_cities].distance[i] = 1000001; // very large distance
  n_cities++;
}

static void new_distance(char *name1,char *name2,int distance,int special)
{
  int i,j;

  i = find_city(name1);
  j = find_city(name2);
  if(i < 0 || j < 0 || i == j || distance <= 0 || distance >= 1000000)
  {
    fprintf(stderr,"Bad info (name1=%s, name2=%s, distance=%d\n",name1,name2,distance);
    exit(1);
  }
  if(special == 0)
    cities[i].distance[j] = cities[j].distance[i] = distance;
  else
  {
    int c = strcoll(cities[i].name,cities[j].name);
    if(c < 0)
    {
      cities[i].distance[j] = round(0.8 * (double)distance);
      cities[j].distance[i] = round(1.2 * (double)distance);
    }
    else
    {
      cities[i].distance[j] = round(1.2 * (double)distance);
      cities[j].distance[i] = round(0.8 * (double)distance);
    }
  }
}

static void init_cities_data(int n_mec,int special)
{
  int i,n,m;

  //
  // make sure we compare the city names using Portuguese rules (Évora ...)
  //
  if(setlocale(LC_ALL,"pt_PT.UTF-8") == NULL)
  {
    fprintf(stderr,"Unable to set the locale to pt_PT.UTF-8\n");
    exit(1);
  }
  srandom((unsigned int)n_mec);
  n_cities = 0;
  //
  // the city coordinates for the pt_master.svg custom map
  //
  for(n = i = m = 0;n < 18;i = (int)random() % 18)
    if((m & (1 << i)) == 0)
    {
      m |= 1 << i;
      n++;
      switch(i)
      {
        case  0: new_city("Aveiro"          , 491.9807,1591.8580); break;
        case  1: new_city("Beja"            , 811.2355,2823.4144); break;
        case  2: new_city("Braga"           , 593.4659,1166.3139); break;
        case  3: new_city("Bragança"        ,1117.9928,1152.0246); break;
        case  4: new_city("Castelo Branco"  , 927.4659,1968.3850); break;
        case  5: new_city("Coimbra"         , 573.4659,1784.0002); break;
        case  6: new_city("Évora"           , 756.4659,2573.3139); break;
        case  7: new_city("Faro"            , 788.4659,3278.6276); break;
        case  8: new_city("Guarda"          ,1009.7085,1600.5739); break;
        case  9: new_city("Leiria"          , 418.4659,2011.3139); break;
        case 10: new_city("Lisboa"          , 332.5259,2476.5579); break;
        case 11: new_city("Portalegre"      , 942.7086,2194.6448); break;
        case 12: new_city("Porto"           , 505.4659,1355.3139); break;
        case 13: new_city("Santarém"        , 505.7086,2257.7281); break;
        case 14: new_city("Setúbal"         , 416.5370,2580.3850); break;
        case 15: new_city("Viana do Castelo", 440.4659,1091.1423); break;
        case 16: new_city("Vila Real"       , 813.8801,1275.8997); break;
        case 17: new_city("Viseu"           , 784.4659,1573.3139); break;
      }
    }
  //
  // inter-city distances (source: ACP road map)
  //
  new_distance("Aveiro","Beja",438,special);
  new_distance("Aveiro","Braga",129,special);
  new_distance("Aveiro","Bragança",282,special);
  new_distance("Aveiro","Castelo Branco",251,special);
  new_distance("Aveiro","Coimbra",77,special);
  new_distance("Aveiro","Évora",360,special);
  new_distance("Aveiro","Faro",496,special);
  new_distance("Aveiro","Guarda",155,special);
  new_distance("Aveiro","Leiria",143,special);
  new_distance("Aveiro","Lisboa",273,special);
  new_distance("Aveiro","Portalegre",284,special);
  new_distance("Aveiro","Porto",74,special);
  new_distance("Aveiro","Santarém",205,special);
  new_distance("Aveiro","Setúbal",306,special);
  new_distance("Aveiro","Viana do Castelo",143,special);
  new_distance("Aveiro","Vila Real",172,special);
  new_distance("Aveiro","Viseu",83,special);

  new_distance("Beja","Braga",540,special);
  new_distance("Beja","Bragança",554,special);
  new_distance("Beja","Castelo Branco",282,special);
  new_distance("Beja","Coimbra",374,special);
  new_distance("Beja","Évora",78,special);
  new_distance("Beja","Faro",149,special);
  new_distance("Beja","Guarda",371,special);
  new_distance("Beja","Leiria",314,special);
  new_distance("Beja","Lisboa",179,special);
  new_distance("Beja","Portalegre",183,special);
  new_distance("Beja","Porto",485,special);
  new_distance("Beja","Santarém",240,special);
  new_distance("Beja","Setúbal",144,special);
  new_distance("Beja","Viana do Castelo",557,special);
  new_distance("Beja","Vila Real",550,special);
  new_distance("Beja","Viseu",453,special);
 
  new_distance("Braga","Bragança",221,special);
  new_distance("Braga","Castelo Branco",353,special);
  new_distance("Braga","Coimbra",184,special);
  new_distance("Braga","Évora",462,special);
  new_distance("Braga","Faro",612,special);
  new_distance("Braga","Guarda",256,special);
  new_distance("Braga","Leiria",250,special);
  new_distance("Braga","Lisboa",380,special);
  new_distance("Braga","Portalegre",386,special);
  new_distance("Braga","Porto",64,special);
  new_distance("Braga","Santarém",312,special);
  new_distance("Braga","Setúbal",406,special);
  new_distance("Braga","Viana do Castelo",67,special);
  new_distance("Braga","Vila Real",103,special);
  new_distance("Braga","Viseu",183,special);

  new_distance("Bragança","Castelo Branco",269,special);
  new_distance("Bragança","Coimbra",333,special);
  new_distance("Bragança","Évora",467,special);
  new_distance("Bragança","Faro",693,special);
  new_distance("Bragança","Guarda",177,special);
  new_distance("Bragança","Leiria",365,special);
  new_distance("Bragança","Lisboa",490,special);
  new_distance("Bragança","Portalegre",361,special);
  new_distance("Bragança","Porto",217,special);
  new_distance("Bragança","Santarém",422,special);
  new_distance("Bragança","Setúbal",529,special);
  new_distance("Bragança","Viana do Castelo",278,special);
  new_distance("Bragança","Vila Real",119,special);
  new_distance("Bragança","Viseu",215,special);

  new_distance("Castelo Branco","Coimbra",187,special);
  new_distance("Castelo Branco","Évora",204,special);
  new_distance("Castelo Branco","Faro",430,special);
  new_distance("Castelo Branco","Guarda",93,special);
  new_distance("Castelo Branco","Leiria",169,special);
  new_distance("Castelo Branco","Lisboa",227,special);
  new_distance("Castelo Branco","Portalegre",99,special);
  new_distance("Castelo Branco","Porto",298,special);
  new_distance("Castelo Branco","Santarém",158,special);
  new_distance("Castelo Branco","Setúbal",274,special);
  new_distance("Castelo Branco","Viana do Castelo",421,special);
  new_distance("Castelo Branco","Vila Real",274,special);
  new_distance("Castelo Branco","Viseu",177,special);

  new_distance("Coimbra","Évora",296,special);
  new_distance("Coimbra","Faro",446,special);
  new_distance("Coimbra","Guarda",156,special);
  new_distance("Coimbra","Leiria",75,special);
  new_distance("Coimbra","Lisboa",206,special);
  new_distance("Coimbra","Portalegre",221,special);
  new_distance("Coimbra","Porto",121,special);
  new_distance("Coimbra","Santarém",138,special);
  new_distance("Coimbra","Setúbal",239,special);
  new_distance("Coimbra","Viana do Castelo",193,special);
  new_distance("Coimbra","Vila Real",220,special);
  new_distance("Coimbra","Viseu",88,special);

  new_distance("Évora","Faro",226,special);
  new_distance("Évora","Guarda",293,special);
  new_distance("Évora","Leiria",236,special);
  new_distance("Évora","Lisboa",131,special);
  new_distance("Évora","Portalegre",105,special);
  new_distance("Évora","Porto",407,special);
  new_distance("Évora","Santarém",162,special);
  new_distance("Évora","Setúbal",96,special);
  new_distance("Évora","Viana do Castelo",479,special);
  new_distance("Évora","Vila Real",472,special);
  new_distance("Évora","Viseu",375,special);

  new_distance("Faro","Guarda",519,special);
  new_distance("Faro","Leiria",386,special);
  new_distance("Faro","Lisboa",281,special);
  new_distance("Faro","Portalegre",331,special);
  new_distance("Faro","Porto",557,special);
  new_distance("Faro","Santarém",312,special);
  new_distance("Faro","Setúbal",248,special);
  new_distance("Faro","Viana do Castelo",629,special);
  new_distance("Faro","Vila Real",698,special);
  new_distance("Faro","Viseu",598,special);

  new_distance("Guarda","Leiria",258,special);
  new_distance("Guarda","Lisboa",316,special);
  new_distance("Guarda","Portalegre",188,special);
  new_distance("Guarda","Porto",206,special);
  new_distance("Guarda","Santarém",247,special);
  new_distance("Guarda","Setúbal",382,special);
  new_distance("Guarda","Viana do Castelo",273,special);
  new_distance("Guarda","Vila Real",185,special);
  new_distance("Guarda","Viseu",79,special);

  new_distance("Leiria","Lisboa",146,special);
  new_distance("Leiria","Portalegre",172,special);
  new_distance("Leiria","Porto",187,special);
  new_distance("Leiria","Santarém",78,special);
  new_distance("Leiria","Setúbal",179,special);
  new_distance("Leiria","Viana do Castelo",259,special);
  new_distance("Leiria","Vila Real",286,special);
  new_distance("Leiria","Viseu",197,special);

  new_distance("Lisboa","Portalegre",230,special);
  new_distance("Lisboa","Porto",317,special);
  new_distance("Lisboa","Santarém",80,special);
  new_distance("Lisboa","Setúbal",47,special);
  new_distance("Lisboa","Viana do Castelo",389,special);
  new_distance("Lisboa","Vila Real",416,special);
  new_distance("Lisboa","Viseu",327,special);

  new_distance("Portalegre","Porto",331,special);
  new_distance("Portalegre","Santarém",161,special);
  new_distance("Portalegre","Setúbal",192,special);
  new_distance("Portalegre","Viana do Castelo",397,special);
  new_distance("Portalegre","Vila Real",367,special);
  new_distance("Portalegre","Viseu",270,special);

  new_distance("Porto","Santarém",249,special);
  new_distance("Porto","Setúbal",353,special);
  new_distance("Porto","Viana do Castelo",76,special);
  new_distance("Porto","Vila Real",102,special);
  new_distance("Porto","Viseu",130,special);

  new_distance("Santarém","Setúbal",113,special);
  new_distance("Santarém","Viana do Castelo",322,special);
  new_distance("Santarém","Vila Real",348,special);
  new_distance("Santarém","Viseu",257,special);

  new_distance("Setúbal","Viana do Castelo",434,special);
  new_distance("Setúbal","Vila Real",463,special);
  new_distance("Setúbal","Viseu",374,special);

  new_distance("Viana do Castelo","Vila Real",154,special);
  new_distance("Viana do Castelo","Viseu",234,special);

  new_distance("Vila Real","Viseu",99,special);
  //
  // revert to standard behaviour
  //
  if(setlocale(LC_ALL,"C") == NULL)
  {
    fprintf(stderr,"Unable to set the locale to C\n");
    exit(1);
  }
}

static void print_distances(void)
{
  int i,j;

  for(i = 0;i < n_cities;i++)
  {
    for(j = 0;cities[i].name[j] != '\0';j++)
      if(((int)cities[i].name[j] & 0xFF) > 127)
        break; // found an accented letter
    printf("// %s%16s",(cities[i].name[j] != '\0') ? " " : "",cities[i].name); // some city names have ONE accented letter...
    for(j = 0;j < n_cities;j++)
      if(i == j)
        printf(" ---");
      else
        printf(" %3d",cities[i].distance[j]);
    printf("\n");
  }
}

static void make_map(char *file_name,int *cities_idx)
{
  FILE *fp_in,*fp_out;
  char *s,line[65536];
  int i,modify;

  if(strcmp(file_name,"pt_master.svg") == 0)
  {
    fprintf(stderr,"Bad file name (%s)\n",file_name);
    exit(1);
  }
  for(i = 0;i <= 4 * max_n_cities && cities_idx[i] >= 0;i++)
    if(cities_idx[i] >= n_cities)
    {
      fprintf(stderr,"City index (%d) too large\n",cities_idx[i]);
      exit(1);
    }
  fp_in = fopen("pt_master.svg","rb");
  if(fp_in == NULL)
  {
    fprintf(stderr,"Cannot open file pt_master.svg\n");
    exit(1);
  }
  fp_out = fopen(file_name,"wb");
  if(fp_out == NULL)
  {
    fprintf(stderr,"Cannot create file %s\n",file_name);
    exit(1);
  }
  modify = 0;
  while(fgets(line,sizeof(line),fp_in) != NULL)
  {
    if(strstr(line,"id=\"ClosedPath\"") != NULL)
      modify = 1; // modify the next line
    else if(modify != 0)
    {
      modify = 0;
      s = strstr(line,"d=\"");
      if(s == NULL)
      {
        fprintf(stderr,"Unexpected contents in the file pr_master.svg\n");
        exit(1);
      }
      s += 3;
      for(i = 0;i <= 4 * max_n_cities && cities_idx[i] >= 0;i++)
        s += sprintf(s,"%s %.4f,%.4f",(i == 0) ? "M" : ((i == 1) ? " L" : ""),cities[cities_idx[i]].x,cities[cities_idx[i]].y);
      sprintf(s," Z\"\n");
    }
    fprintf(fp_out,"%s",line);
  }
}

#ifdef include_main
//
// stand alone version; compile with "cc -Wall -O2 -Dinclude_main -xc cities.h"
//
int main(void)
{
  int idx[max_n_cities + 1] = { 0,13,5,3,10,6,9,4,1,12,16,11,14,15,2,17,8,7,-1 };

  init_cities_data(0,0);
  print_distances();
  make_map("pt.svg",idx);
  return 0;
}
#endif
