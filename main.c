#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

int main()
{
  int s = 0;



  do{
    printf("\033[2J");
    printf("\033[0;0f\n");
    printf("\t\t████████╗██████╗  █████╗ ███╗   ██╗███╗   ██╗ ██████╗ ██╗\n");
    printf("\t\t╚══██╔══╝██╔══██╗██╔══██╗████╗  ██║████╗  ██║██╔═══██╗██║\n");
    printf("\t\t   ██║   ██████╔╝███████║██╔██╗ ██║██╔██╗ ██║██║   ██║██║\n");
    printf("\t\t   ██║   ██╔══██╗██╔══██║██║╚██╗██║██║╚██╗██║██║   ██║██║\n");
    printf("\t\t   ██║   ██║  ██║██║  ██║██║ ╚████║██║ ╚████║╚██████╔╝██║\n");
    printf("\t\t   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝\n");
    printf("\033[0;97m");
    printf("\n\t\t\t\t1) Imposta gioco,\n");
    printf("\t\t\t\t2) Gioca,\n");
    printf("\t\t\t\t3) Termina gioco.\n");
    scanf("%i", &s);
    switch (s) {
      case 1:
        imposta_gioco();
        break;

      case 2:
        gioca();
        break;

      case 3:
        Termina_Gioco();
        break;

      default:
      fflush(stdin);
        printf("comando sbagliato...premi invio per inserire una nuova opzione\n");
        getchar();
        while(getchar() != '\n');
        break;
    }

  }while(s != 3);
  return 1;
}





/*printf("  _____                                   _                  _       _ _ _\n");
printf(" / ____|                                 | |                (_)     | | | |\n");
printf("| |     _ __ _____      ___ __ ___   __ _| |_ ___  __      ___ _ __ | | | |\n");
printf("| |    | '__/ _ \\ \\ /\\ / / '_ ` _ \\ / _` | __/ _ \\ \\ \\ /\\ / / | '_ \\| | | |\n");
printf("| |____| | |  __/\\ V  V /| | | | | | (_| | ||  __/  \\ V  V /| | | | |_|_|_|\n");
printf(" \\_____|_|  \\___| \\_/\\_/ |_| |_| |_|\\__,_|\\__\\___|   \\_/\\_/ |_|_| |_(_|_|_)\n");*/
