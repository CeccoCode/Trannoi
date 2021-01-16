#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

int main()
{
  int s = 0;



  do{
    //printf("____  ____  _____ _      _      ____  _____  _____   _      _  _     \n/   _\/  __\/  __// \  /|/ \__/|/  _ \/__ __\/  __/  / \  /|/ \/ \  /|\n|  /  |  \/||  \  | |  ||| |\/||| / \|  / \  |  \    | |  ||| || |\ ||\n|  \_ |    /|  /_ | |/\||| |  ||| |-||  | |  |  /_   | |/\||| || | \||\n\____/\_/\_\\____\\_/  \|\_/  \|\_/ \|  \_/  \____\  \_/  \|\_/\_/  \|");
    //printf("                                        _                  _       \n                                       | |                (_)      \n  ___ _ __ _____      ___ __ ___   __ _| |_ ___  __      ___ _ __  \n / __| '__/ _ \ \ /\ / / '_ ` _ \ / _` | __/ _ \ \ \ /\ / / | '_ \ \n| (__| | |  __/\ V  V /| | | | | | (_| | ||  __/  \ V  V /| | | | |\n \___|_|  \___| \_/\_/ |_| |_| |_|\__,_|\__\___|   \_/\_/ |_|_| |_|\n");
    printf("\033[2J");
    printf("\033[0;0f");
    printf("\033[0;97m");
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
        Termina_Gioco();
        break;

      default:
        printf("comando sbagliato\n");
        break;
    }

  }while(1);
  return 1;
}
