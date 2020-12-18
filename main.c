#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

void main()
{
  int s = 0;



  do{
    printf("\n1) Imposta gioco,\n");
    printf("2) Gioca,\n");
    printf("3) Termina gioco.\n");
    scanf("%i", &s);
    switch (s) {
      case 1:
        imposta_gioco();
        break;

      case 2:
        gioca();
        break;

      case 3:
        //termina_gioco();
        break;

      default:
        printf("comando sbagliato\n");
        break;
    }

  }while(1);
}
