#include <stdio.h>
#include "fic_wave.h"

FILE *abre_wave (const char *ficWave, float *fm){
  FILE *fpWave;
  int	fm_;

  //comprovar que puc obrir fitxer en mode lectura i escritura
  if ((fpWave = fopen(ficWave, "r")) == NULL) return NULL;

  //fseek: Posa el punter d'un arxiu a la posicio indicada
  if (fseek(fpWave, 44, SEEK_SET) < 0) return NULL;

  //Moure a la posico del fitxer on esta la freq de mostreig
  fseek(fpWave, 24, SEEK_SET);
  //LLegir el valor
  fread(&fm_, sizeof(fm_), 1, fpWave);
  *fm = fm_;

  //*fm = 16000;

  return fpWave;
}

size_t lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave){
  //fread: Llegeix dades de la sequencia. Retorna el numero d'elements llegits correctament
  return fread(x, size, nmemb, fpWave);
}

void cierra_wave(FILE *fpWave){
  fclose(fpWave);
}
