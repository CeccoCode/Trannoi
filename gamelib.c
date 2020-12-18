#include "gamelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// dichiarazioni delle variabili globali
S_Giocatore* giocatori;
unsigned short quest_da_finire;
S_Stanza* stanza_inizio;
S_Stanza* lista_stanze;
int num_g;


// prototipi
static void n_giocatori();
static void creaNome();
static void creaStato();
static void clr();
static char *StampaN(int num);
static char* StampaS(int num);
static int sInizio();
static char *StampaT(int num);
static void avanza(int num);
static unsigned short esegui_quest(int num);
static int probabilita();





/*_________________________________________________________________________________________________________*/

// funzione imposta_gioco
void imposta_gioco()
{
  int s;
  //pulisco lo schermo
  clr();

  //funzione che chiede il numero di giocatori
  n_giocatori();
  //creo in memoria dinamica la struttura giocatore per il numero di giocatori
  giocatori = (S_Giocatore*) calloc(num_g, sizeof(S_Giocatore));

  creaNome();
  creaStato();
  sInizio();

  //pulicsco lo schermo
  clr();

  // chiedo quante quest servono per la vittoria degli astronauiti
  do{
    printf("\nquante quest vuoi inserire?\t");
    scanf("%hu", &quest_da_finire);
    if(num_g > (int)quest_da_finire)
    {
      printf("\nIl numero di quest deve essere maggiore o uguale al numero di giocatori\n");
    }
  }while(num_g > (int)quest_da_finire);

  //pulicsco lo schermo
  clr();
  printf(" PREMERE:\n 1. Per visualizzare tutte le informazioni sui giocatori\n 2. Per tornare al menù principale e giocare\n");
  scanf("%d", &s);
  switch(s)
  {
    case 1:
    clr();
      for(int i = 0; i < num_g; i++)
      {
        giocatori[i].pos = stanza_inizio;
        printf("\nIl %d° giocatore è %s ed è un %s %s", i+1 ,StampaN(giocatori[i].nome), StampaS(giocatori[i].stato), StampaT(giocatori[i].pos->tStanza));
      }
      break;
    case 2:
    return;
      break;
  }

  free(giocatori);
  free(stanza_inizio);
}

/*_________________________________________________________________________________________________________*/

void gioca()
{
  int num;
  int s;
  unsigned short quest_fatta;
  lista_stanze = stanza_inizio;
  //printf("\nper vincere la partita mancano %ud quest da fare\n", quest_da_finire);
  while(quest_da_finire != 0)
  {
    clr();
    num = rand()%num_g;
    printf("\nTurno del giocatore \t%s\n",StampaN(giocatori[num].nome));
    printf("Sei in una stanza %s:\t", StampaT(giocatori[num].pos->tStanza));
    if(giocatori[num].stato == 0)
    {
      //printf("\nper vincere la partita mancano %ud quest da fare\n", quest_da_finire);
      printf("Premere:\n1. Avanza\n2. Esegui quest\n3. Chiamata di emergenza\n");
      scanf("%d", &s);
      switch(s)
      {
        case 1:
          avanza(num);
          break;
        case 2:
          quest_fatta = esegui_quest(giocatori[num].pos->tStanza);
          quest_da_finire = quest_da_finire - quest_fatta;
          giocatori[num].pos->tStanza = 0;
          break;
        case 3:
          //chiamata_emergenza();
          break;
      }
    }
    else if(giocatori[num].stato == 1)
    {
      printf("Premere:\n1. Avanza\n2. Chiamata di emergenza\n3. Uccidi astronauta\n4. Usa botola\n5. Sabotaggio");
      scanf("%d", &s);
      switch(s)
      {
        case 1:
          avanza(num);
        case 2:
        case 3:
        case 4:
        case 5: break;
      }
    }
  }
}

/*_________________________________________________________________________________________________________*/

// funzione che pulisce lo schermo
static void clr()
{
  printf("\033[2J");
  printf("\033[0;0f");
}

// prende il numero di giocatori e lo scrive in n
static void n_giocatori(int *n)
{
  printf("\nInserisci il numero dei giocatori: (minimo 4, massimo 10)\n");
  scanf("%d", &num_g);
  while(num_g < 4 || num_g > 10)
  {
    if(num_g < 4)
    {
      printf("Pochi giocatori: (minimo 4, massimo 10)\n");
      scanf("%d", &num_g);
    }
    else if(*n > 10)
    {
      printf("Troppi giocatori: (minimo 4, massimo 10)\n");
      scanf("%d",  &num_g);
    }
  }

  return;
}


//assegna ai giocatori i nomi
static void creaNome()
{
  time_t t;

  srand((unsigned) time(&t));
  if(giocatori == NULL)
  {
    printf("Struttura non allocata in memoria");
    return;
  }else
  {
    for(int i = 0; i < num_g; i++)
    {
      giocatori[i].nome = rand()%10;

      for(int j = 0; j < i; j++)
      {
        if(giocatori[i].nome == giocatori[j].nome)
        {
          i--;
          break;
        }
      }
    }
    return;
  }
}

//assegna i giocatori lo stato, impostore o astronauta
static void creaStato(int n)
{
  int nImp = 0;
  int num;

  if(num_g == 4 || num_g == 5)
  {
    do{
      nImp = 0;
      for(int i = 0; i < num_g; i++)
      {
        giocatori[i].stato = rand()%num_g;
        if(giocatori[i].stato > 1)
        {
          giocatori[i].stato = 0;
        }
        else{
          giocatori[i].stato = 1;
          nImp += 1;
        }
      }
    }while(nImp < 1 || nImp > 1);
  }
  else if(num_g == 6 || num_g == 7 || num_g == 8)
    {
      do{
        nImp = 0;
        for(int i = 0; i < num_g; i++)
        {
          giocatori[i].stato = rand()%num_g;
          if(giocatori[i].stato > 2)
          {
            giocatori[i].stato = 0;
          }
          else{
            giocatori[i].stato = 1;
            nImp += 1;
          }
        }
      }while(nImp < 1 || nImp > 2);
    }else{


    do{
      nImp = 0;
      for(int i = 0; i < num_g; i++)
      {
        giocatori[i].stato = rand()%num_g;
        if(giocatori[i].stato > 3)
        {
          giocatori[i].stato = 0;
        }
        else{
          giocatori[i].stato = 1;
          nImp += 1;
        }
      }
    }while(nImp < 1 || nImp > 3);

  }
}

//creo la stanza di partenza
static int sInizio()
{
  stanza_inizio = (S_Stanza*) calloc(1, sizeof(S_Stanza));
  if( stanza_inizio == NULL )
  {
    printf("Stanza non allocata in memoria");
    return 0;
  }

  stanza_inizio->avanti = NULL;
  stanza_inizio->destra = NULL;
  stanza_inizio->sinistra = NULL;
  stanza_inizio->stanza_precedente = NULL;

  stanza_inizio->tStanza = rand()%100;
  if(stanza_inizio->tStanza <= 29)
  {
    stanza_inizio->tStanza = 0;
  }
  if(stanza_inizio->tStanza <= 59 && stanza_inizio->tStanza >= 30)
  {
    stanza_inizio->tStanza = 1;
  }
  if(stanza_inizio->tStanza <= 74 && stanza_inizio->tStanza >= 60)
  {
    stanza_inizio->tStanza = 2;
  }
  if(stanza_inizio->tStanza <= 100 && stanza_inizio->tStanza >= 75)
  {
    stanza_inizio->tStanza = 3;
  }
}

static int probabilita()
{
  int n;
  lista_stanze->tStanza = rand()%100;
  if(lista_stanze->tStanza <= 29)
  {
    lista_stanze->tStanza = 0;
    n = 0;
  }
  if(lista_stanze->tStanza <= 59 && lista_stanze->tStanza >= 30)
  {
    lista_stanze->tStanza = 1;
    n = 1;
  }
  if(lista_stanze->tStanza <= 74 && lista_stanze->tStanza >= 60)
  {
    lista_stanze->tStanza = 2;
    n = 2;
  }
  if(lista_stanze->tStanza <= 100 && lista_stanze->tStanza >= 75)
  {
    lista_stanze->tStanza = 3;
    n = 3;
  }
  return n;
}


// stampa i nomi dei giocatori
static char *StampaN(int num)
{
  switch(num)
  {
    case 0: return "rosso";
    case 1: return "rosa";
    case 2: return "blu";
    case 3: return "viola";
    case 4: return "verde";
    case 5: return "ciano";
    case 6: return "giallo";
    case 7: return "arancione";
    case 8: return "bianco";
    case 9: return "nero";
  }
}

//funzione che ritorna se astronauta o impostore
static char *StampaS(int num)
{
  switch(num)
  {
    case 0: return "astronauta";
    case 1: return "impostore";
  }
}

//funzione che prende il valore dell'enum di Tipo_Stanza e ritorna la stringa associata
static char *StampaT(int num)
{
  switch(num)
  {
    case 0: return "vuota";
    case 1: return "quest_semplice";
    case 2: return "quest_complicata";
    case 3: return "botola";
  }
}

static void avanza(int num)
{
  int s;
  S_Stanza *nuova_stanza;
  printf("Dove vuoi andare:\n1.Avanti\n2.Destra\n3.Sinistra\n");
  scanf("%d", &s);
  switch(s)
  {
    case 1:
      while(lista_stanze->avanti != NULL)
      {
        giocatori[num].pos = lista_stanze->avanti;
      }
      if(lista_stanze->avanti == NULL){
        //nuova_stanza = (S_Stanza*) calloc(1, sizeof(S_Stanza));
        lista_stanze->avanti = malloc(sizeof(lista_stanze));
        lista_stanze->stanza_precedente = lista_stanze;
        lista_stanze = lista_stanze->avanti;
        //lista_stanze->avanti = nuova_stanza;
        //nuova_stanza->stanza_precedente = lista_stanze;
        //lista_stanze = nuova_stanza;
        lista_stanze->avanti = NULL;
        lista_stanze->destra = NULL;
        lista_stanze->sinistra = NULL;
        lista_stanze->tStanza = probabilita();
        giocatori[num].pos = lista_stanze->avanti;
        //printf("%s\n", StampaT(lista_stanze->tStanza));
      } break;

    case 2:
      while(lista_stanze->destra != NULL)
      {
        giocatori[num].pos = lista_stanze->destra;
      }
      if(lista_stanze->destra == NULL){
        //nuova_stanza = (S_Stanza*) calloc(1, sizeof(S_Stanza));
        lista_stanze->destra = malloc(sizeof(S_Stanza*));
        lista_stanze->stanza_precedente = lista_stanze;
        lista_stanze = lista_stanze->destra;
        //lista_stanze->destra = nuova_stanza;
        //nuova_stanza->stanza_precedente = lista_stanze;
        //lista_stanze = nuova_stanza;
        lista_stanze->avanti = NULL;
        lista_stanze->destra = NULL;
        lista_stanze->sinistra = NULL;
        lista_stanze->tStanza = probabilita();
        giocatori[num].pos = lista_stanze->destra;
        //printf("%s\n", StampaT(lista_stanze->tStanza));
      } break;

    case 3:
      while(lista_stanze->sinistra != NULL)
      {
        giocatori[num].pos = lista_stanze->sinistra;
      }
      if(lista_stanze->sinistra == NULL){
        //nuova_stanza = (S_Stanza*) calloc(1, sizeof(S_Stanza));
        lista_stanze->sinistra = malloc(sizeof(S_Stanza*));
        lista_stanze->stanza_precedente = lista_stanze;
        lista_stanze = lista_stanze->sinistra;
        //lista_stanze->sinistra = nuova_stanza;
        //nuova_stanza->stanza_precedente = lista_stanze;
        //lista_stanze = nuova_stanza;
        lista_stanze->avanti = NULL;
        lista_stanze->destra = NULL;
        lista_stanze->sinistra = NULL;
        lista_stanze->tStanza = probabilita();
        giocatori[num].pos = lista_stanze->avanti;
        //printf("%s\n", StampaT(lista_stanze->tStanza));
      } break;

  }
}

static unsigned short esegui_quest(int num)
{
  if(num == 1)
  {
    return 1;
  }
  else if(num == 2)
  {
    return 2;
  }
  else{
    printf("\nNon ci sono quest da fare in questa stanza\n");
    return 0;
  }
}

/*static chiamata_emergenza(S_Stanza *p)
{
  for(int i = 0; i < num_g; i++)
  {
    if()
  }
}*/
