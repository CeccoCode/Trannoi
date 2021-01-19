#include "gamelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// dichiarazioni delle variabili globali
S_Giocatore* giocatori;
unsigned short quest_da_finire;
S_Stanza* stanza_inizio;
S_Stanza* lista_stanze;
int num_g = 0;
int nImp = 1;


// prototipi
static void n_giocatori();
static void creaNome();
static void creaStato();
static void clr();
static char *StampaN(int num);
static char* StampaS(int num);
static int sInizio();
static char *StampaT(int s);
static void avanza(int num);
static unsigned short esegui_quest(int num);
static int probabilita();
static void emergenza(S_Stanza *p);
static int Uccidi_astronauta(S_Stanza *stanza, int nome, int col);
static S_Stanza *Elimina(S_Stanza*Node);
static S_Stanza *prossimaStanza(S_Stanza *stanza);
static void mischia_turni(int v[]);
static int Defenestra(S_Stanza *stanza, int nome, int col);
static void SetColor(int col);
static void Inizia_gioco();








// funzione imposta_gioco
void imposta_gioco(){
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
  int scelta = 0;
  printf(" PREMERE:\n 1. Per visualizzare tutte le informazioni sui giocatori\n 2. Per tornare al menù principale e giocare\n");
  scanf("%d", &scelta);
  //while(getchar() != '\n')
  switch(scelta)
  {
    case 1:
    clr();
      for(int i = 0; i < num_g; i++)
      {
        giocatori[i].pos = stanza_inizio;
        printf("\nIl %d° giocatore è %s ed è un %s %s", i+1 ,StampaN(giocatori[i].nome), StampaS(giocatori[i].stato), StampaT(giocatori[i].pos->tStanza));
      }
      fflush(stdin);
      printf("\nPremere invio per tornare al menù e avviare il gioco...");
      getchar();
      while(getchar() != '\n');
      break;
    case 2:
      Inizia_gioco();
      break;
    default:
      printf("Il tasto premuto non corrisponde a nessuna scelta");
  }

}

//funzione principale
void gioca(){

  if(num_g == 0)
  {
    printf("\nprima di giocare è necessario impostare il gioco, verrai riportato al menu principale\n");
    return;
  }

  int colore;
  int s;
  int controllo_scelta = 0;
  int turno_giocatori[num_g];
  int count = 0;
  time_t t;
  srand((unsigned) time(&t));

  lista_stanze = (S_Stanza*) malloc(sizeof(S_Stanza));
  lista_stanze = stanza_inizio;
  printf("\nper vincere la partita servono %u quest da fare\n", quest_da_finire);
  mischia_turni(turno_giocatori);


  while(quest_da_finire != 0 && nImp != 0)
  {
    clr();
    if(count == num_g){
      mischia_turni(turno_giocatori);
      count = 0;
    }
    colore = turno_giocatori[count];
    count++;
    SetColor(colore);

    if(giocatori[colore].stato == astronauta)
    {
      printf("\nTurno del giocatore \t%s\n",StampaN(giocatori[colore].nome));
      printf("\nPremere invio per iniziare il turno...\n");
      getchar();
      while(getchar()!='\n');
      printf("Sei nella stanza: \t%p di tipo: \t%s", giocatori[colore].pos, StampaT(giocatori[colore].pos->tStanza));
      printf("\nPer vincere la partita mancano %u quest da fare\n", quest_da_finire);
      printf("\nSei nella stanza insieme ai giocatori:");
      printf("\nIn questa stanza puoi:\n");
      printf("\n1) avanzare");
      printf("\n2) eseguire la quest");
      printf("\n3) Chiamata di emergenza\n");
      scanf("%d", &s);
      controllo_scelta = 0;
      do{
        switch(s){
          case 1:
            avanza(colore);
            controllo_scelta = 1;
            break;
            case 2:
              if(esegui_quest(colore) == 0)
                printf("\nNon ci sono quest da fare in questa stanza");
              else
                controllo_scelta = 1;
              break;
              case 3:
              //emergenza(giocatori[colore].pos); break;
              break;
            default:
              printf("\ncomando sbagliato" );
            break;
          }
        if(controllo_scelta == 0){
          printf("\nInserisci un'altra voce: ");
          scanf("%d", &s);
        }
      }while(controllo_scelta == 0);
    }

    if(giocatori[colore].stato == impostore)
    {
      printf("\nTurno del giocatore \t%s\n",StampaN(giocatori[colore].nome));
      printf("\nPremere invio per iniziare il turno...\n");
      getchar();
      while(getchar()!='\n');
      printf("Sei nella stanza: %p di tipo: %s",giocatori[colore].pos, StampaT(giocatori[colore].pos->tStanza));
      printf("\nPremere:");
      printf("\n1) avanzare");
      printf("\n2) Chiamata di emergenza");
      printf("\n3) Uccidi\n");
      scanf("%d", &s);

      do{
        switch(s)
        {
          case 1:
            avanza(colore);
            controllo_scelta = 1;
            break;
          case 2:
            //emergenza(giocatori[colore].pos);
            controllo_scelta = 1;
            break;
          case 3:
            if(Uccidi_astronauta(giocatori[colore].pos, giocatori[colore].nome, colore) == 1)
              controllo_scelta = 1;
            else{
              printf("\nNon ci sono giocatori da uccidere in questa stanza");
            }
            break;
          case 4: break;
          case 5: break;
          default:
            printf("\ncomando sbagliato");
            break;
        }
        if(controllo_scelta == 0){
          printf("\nInserisci un'altra voce: ");
          scanf("%d", &s);
        }
      }while(controllo_scelta == 0);
    }

    nImp = 0;
    for(int i = 0; i < num_g; i++)
    {
      if(giocatori[i]. stato == impostore)
        nImp++;
    }
  }
  //Termina_Gioco();
}

static void Inizia_gioco(){
    printf("\nStai per essere rimandato al menu principale");
    for(int i = 0; i < num_g; i++)
    {
      giocatori[i].pos = stanza_inizio;
    }
}

static void mischia_turni(int v[]){
  for(int i = 0; i < num_g; i++)
  {
    v[i] = rand()%num_g;
    for(int j = 0; j < i; j++)
    {
      if(v[i] == v[j])
      {
        i--;
        break;
      }
    }
  }
}
// funzione che pulisce lo schermo
static void clr(){
  printf("\033[2J");
  printf("\033[0;0f");

}

static void SetColor(int col){
  switch(giocatori[col].nome)
  {
    case rosso:   printf("\033[1;31m"); break;
    case rosa:   printf("\033[0;95m"); break;
    case blu:   printf("\033[0;34m"); break;
    case viola:   printf("\033[0;35m"); break;
    case verde:   printf("\033[1;32m"); break;
    case ciano:   printf("\033[0;96m"); break;
    case giallo:   printf("\033[0;93m"); break;
    case arancione:   printf("\033[1;33m"); break;
    case bianco:   printf("\033[0;97m"); break;
    case marrone:   printf("\033[0;33m"); break;
  }
}

// prende il numero di giocatori e lo scrive in n
static void n_giocatori(){
  printf("\nInserisci il numero di giocatori: (Minimo: 4 e Massimo: 10)\n");
  scanf("%d", &num_g);
  do{
    if(num_g < 4){
      printf("\nInserisci più giocatori:");
      scanf("%d", &num_g);
    }
    else if(num_g > 10){
      printf("\nHai inserito troppi giocatori:");
      scanf("%d", &num_g);
    }
  }while(num_g < 4 || num_g > 10);
}

//assegna ai giocatori i nomi
static void creaNome(){
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
static void creaStato(int n){
  int nImp = 0;
  //int num;

  if(num_g == 4 || num_g == 5)
  {
    do{
      nImp = 0;
      for(int i = 0; i < num_g; i++)
      {
        giocatori[i].stato = rand()%num_g;
        if(giocatori[i].stato > 1)
        {
          giocatori[i].stato = astronauta;
        }
        else{
          giocatori[i].stato = impostore;
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
            giocatori[i].stato = astronauta;
          }
          else{
            giocatori[i].stato = impostore;
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
          giocatori[i].stato = astronauta;
        }
        else{
          giocatori[i].stato = impostore;
          nImp += 1;
        }
      }
    }while(nImp < 1 || nImp > 3);

  }
}

//creo la stanza di partenza
static int sInizio(){
  stanza_inizio = (S_Stanza*) malloc(sizeof(S_Stanza));
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
    stanza_inizio->tStanza = vuota;
  }
  if(stanza_inizio->tStanza <= 59 && stanza_inizio->tStanza >= 30)
  {
    stanza_inizio->tStanza = quest_semplice;
  }
  if(stanza_inizio->tStanza <= 74 && stanza_inizio->tStanza >= 60)
  {
    stanza_inizio->tStanza = quest_complicata;
  }
  if(stanza_inizio->tStanza <= 100 && stanza_inizio->tStanza >= 75)
  {
    stanza_inizio->tStanza = botola;
  }
  return 1;
}

static int probabilita(){
  int n;
  lista_stanze->tStanza = rand()%100;
  if(lista_stanze->tStanza <= 29)
  {
    lista_stanze->tStanza = vuota;
    n = 0;
  }
  if(lista_stanze->tStanza <= 59 && lista_stanze->tStanza >= 30)
  {
    lista_stanze->tStanza = quest_semplice;
    n = 1;
  }
  if(lista_stanze->tStanza <= 74 && lista_stanze->tStanza >= 60)
  {
    lista_stanze->tStanza = quest_complicata;
    n = 2;
  }
  if(lista_stanze->tStanza <= 100 && lista_stanze->tStanza >= 75)
  {
    lista_stanze->tStanza = botola;
    n = 3;
  }
  return n;
}

// stampa i nomi dei giocatori
static char *StampaN(int num){
  switch(num)
  {
    case 0: return "rosso"; break;
    case 1: return "rosa"; break;
    case 2: return "blu"; break;
    case 3: return "viola"; break;
    case 4: return "verde"; break;
    case 5: return "ciano"; break;
    case 6: return "giallo"; break;
    case 7: return "arancione"; break;
    case 8: return "bianco"; break;
    case 9: return "marrone"; break;
  }
  return "NULL";
}

//funzione che ritorna se astronauta o impostore
static char *StampaS(int num){
  switch(num)
  {
    case 0: return "astronauta"; break;
    case 1: return "impostore"; break;
  }
  return "null";
}

//funzione che prende il valore dell'enum di Tipo_Stanza e ritorna la stringa associata
static char *StampaT(int s){
  switch(s)
  {
    case 0: return "vuota"; break;
    case 1: return "quest_semplice"; break;
    case 2: return "quest_complicata"; break;
    case 3: return "botola"; break;
  }
  return "null";
}

static void avanza(int num){
  int s;
  lista_stanze = giocatori[num].pos;
  printf("Dove vuoi andare:\n1.Avanti: %p\n2.Destra: %p\n3.Sinistra: %p\n4.Resta fermo: %p\n", lista_stanze->avanti, lista_stanze->destra, lista_stanze->sinistra, lista_stanze);//giocatori[num].pos->avanti, giocatori[num].pos->destra, giocatori[num].pos->sinistra);
  scanf("%d", &s);
  switch(s){
    case 1:
      if (lista_stanze->avanti == NULL){
        lista_stanze->avanti = (S_Stanza*)malloc(sizeof(S_Stanza));
        giocatori[num].pos = lista_stanze->avanti;
        lista_stanze->stanza_precedente = lista_stanze;
        lista_stanze = lista_stanze->avanti;
        lista_stanze->avanti = NULL;
        lista_stanze->destra = NULL;
        lista_stanze->sinistra = NULL;
        lista_stanze->tStanza = probabilita();
      }
      else if(lista_stanze->avanti != NULL)
        giocatori[num].pos = lista_stanze->avanti;
      break;

    case 2:
      if (lista_stanze->destra == NULL){
        lista_stanze->destra = (S_Stanza*)malloc(sizeof(S_Stanza));
        giocatori[num].pos = lista_stanze->destra;
        lista_stanze->stanza_precedente = lista_stanze;
        lista_stanze = lista_stanze->destra;
        lista_stanze->avanti = NULL;
        lista_stanze->destra = NULL;
        lista_stanze->sinistra = NULL;
        lista_stanze->tStanza = probabilita();
      }
      else if(lista_stanze->destra != NULL)
        giocatori[num].pos = lista_stanze->destra;
      break;

      case 3:
        if (lista_stanze->sinistra == NULL){
          lista_stanze->sinistra = (S_Stanza*)malloc(sizeof(S_Stanza));
          giocatori[num].pos = lista_stanze->sinistra;
          lista_stanze->stanza_precedente = lista_stanze;
          lista_stanze = lista_stanze->sinistra;
          lista_stanze->avanti = NULL;
          lista_stanze->destra = NULL;
          lista_stanze->sinistra = NULL;
          lista_stanze->tStanza = probabilita();
        }
        else if(lista_stanze->sinistra != NULL)
          giocatori[num].pos = lista_stanze->sinistra;
        break;

        case 4:
          break;

  }
}

static unsigned short esegui_quest(int num){
  if(giocatori[num].pos->tStanza == quest_semplice){
    quest_da_finire = quest_da_finire - 1;
    giocatori[num].pos->tStanza = vuota;
    return 1;
  }
  else if(giocatori[num].pos->tStanza == quest_complicata && quest_da_finire == 1){
    quest_da_finire = 0;
    giocatori[num].pos->tStanza = vuota;
    return 1;
  }
  else if(giocatori[num].pos->tStanza == quest_complicata && quest_da_finire >= 2){
    quest_da_finire = quest_da_finire - 2;
    giocatori[num].pos->tStanza = vuota;
    return 1;
  }
  else
    return 0;
}

static void emergenza(S_Stanza *p){
  for(int i = 0; i < num_g; i++)
  {
    if(giocatori[i].pos == p && giocatori[i].stato == 2 && p->emergenza_chiamata == 1)
    {
      printf("\nIl giocatore %s è stato trovato morto in questa stanza",StampaN(giocatori[i].nome));
    }
    if(giocatori[i].pos == p && p->emergenza_chiamata == 1)
    {
      printf("\nI giocatori presenti nella stanza sono: %s",StampaN(giocatori[i].nome));
    }
  }
}

static int Uccidi_astronauta(S_Stanza *stanza, int nome, int col){
  int count = 1;
  int n = 0;
  int s;

  for(int i = 0; i < num_g; i++){
    if(giocatori[i].pos == stanza && giocatori[i].nome != nome && giocatori[i].stato == astronauta){
      n++;
    }
  }
  if(n == 0)
    return 0;
  else{
    printf("\nPuoi uccidere i seguenti giocatori:");
    for(int i = 0; i < num_g; i++){
      if(giocatori[i].pos == stanza && giocatori[i].nome != nome && giocatori[i].stato == 0){
        printf("\n%d) %s",i, StampaN(giocatori[i].nome));
      }
    }
    scanf("%d", &s);
    giocatori[s].stato = assassinato;
    if(Defenestra(stanza, nome, col) == 1){
      giocatori[s].stato = defenestrato;
      return 1;
    }
    else
      return 0;
  }
}

static int Defenestra(S_Stanza *stanza, int nome, int col){
  int n = 0;
  int d = 0;
  for(int i = 0; i < num_g; i++){
    if(giocatori[i].pos == stanza && giocatori[i].nome != nome && giocatori[i].stato == 0)
      n+=5;
    if(giocatori[i].pos == giocatori[col].pos->stanza_precedente && giocatori[i].stato == 0)
      n+=2;
  }
  d = rand()%10;
  if(d <= n)
    giocatori[col].stato = defenestrato;
  if(giocatori[col].stato == defenestrato){
      clr();
      fflush(stdin);
      printf("\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n　　　.　　　 　　.　　　　　。　　 。　. 　\n\t\t.　　 。　　　　　 ඞ 。 . 　　 • 　　　　•\n\t\t　　ﾟ　　il %s è stato defenestrato.　 。　.\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n　　　.　　　 　　.　　　　　。　　 。　. ", StampaN(giocatori[col].nome));
      getchar();
      while(getchar() != '\n');
      return 1;
  }
  else
    return 0;
}

static S_Stanza *prossimaStanza(S_Stanza *stanza){
  printf("ciao\n");
  if(stanza->avanti != NULL)
    stanza = stanza->avanti;
  else if(stanza->destra != NULL)
    stanza = stanza->destra;
  else if(stanza->sinistra != NULL)
    stanza = stanza->sinistra;
  return stanza;
}

static S_Stanza *Elimina(S_Stanza*Node){
  S_Stanza *appoggio;
  while(lista_stanze->stanza_precedente != NULL){
    printf("\nSto eliminando il nodo %p", lista_stanze);
    appoggio = lista_stanze->stanza_precedente;
    free(lista_stanze);
    lista_stanze = appoggio;
  }

  printf("\nelimino il nodo %p",Node);
  free(Node);
  return NULL;
}

void Termina_Gioco(){
  Elimina(lista_stanze);
  free(giocatori);
  free(stanza_inizio);
}
