#include "gamelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*_____________________________variabili globali_____________________________*/
S_Giocatore* giocatori;
unsigned short quest_da_finire;
S_Stanza* stanza_inizio;
S_Stanza* lista_stanze;
int num_g = 0;
int nImp = 0;
int nAstr = 0;

S_Stanza *botole[20];
S_Stanza *stanze[30];
int contatore_botola = 0;
int contatore_stanze = 0;
/*___________________________________________________________________________*/

/*______________________________prototipi____________________________________*/
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
static int chiamata_emergenza(int nome);
static int Uccidi_astronauta(S_Stanza *stanza, int nome, int col);
static S_Stanza *Elimina(S_Stanza *Node);
static S_Stanza *prossimaStanza(S_Stanza *Node);
static void mischia_turni(int v[]);
static int Defenestra(S_Stanza *stanza, int nome, int col);
static void SetColor(int col);
static void Inizia_gioco();
static int Sabotaggio(int nome);
static int Usa_Botola(int nome);
static void Cerca_G(int nome);
static void FinePartita();
/*___________________________________________________________________________*/


/*____________________________Imposta_Gioco()________________________________*/
void imposta_gioco(){
  int quest;
  clr();  //pulisce lo schermo

  n_giocatori();  //funzione che chiede il numero di giocatori

  giocatori = (S_Giocatore*) calloc(num_g, sizeof(S_Giocatore));  //creo in memoria dinamica la struttura giocatore per il numero di giocatori

  creaNome();
  creaStato();
  sInizio();

  clr();

  //chiedo il numero di quest da fare per il salvataggio sell'astronave
  do{
    printf("\n Quante quest vuoi inserire?\n ");
    scanf("%d", &quest);
    while(getchar() != '\n');
    if(num_g > quest || quest < 0)
    {
      printf("\n Il numero di quest deve essere maggiore o uguale al numero di giocatori\n ");
    }
  }while(num_g > quest || quest < 0);
  quest_da_finire = quest;

  for(int i = 0; i < num_g; i++)
  {
    giocatori[i].pos = stanza_inizio;
  }


  int scelta = 0;
  clr();
  printf(" PREMERE:\n 1. Per visualizzare tutte le informazioni sui giocatori\n 2. Per tornare al menù principale e giocare\n ");

  do{
    scanf("%d", &scelta);
    while(getchar() != '\n');
    switch(scelta)
    {
      case 1:
        for(int i = 0; i < num_g; i++){
          printf("\n Il %d° giocatore è %s ed è un %s nella stanza %p.", i+1 ,StampaN(giocatori[i].nome), StampaS(giocatori[i].stato), stanza_inizio);
        }
        printf("\n\n Premere invio per tornare al menù e avviare il gioco...");
        while(getchar() != '\n');
        break;
      case 2:
        Inizia_gioco();
        break;
      default:
        printf(" Il tasto premuto non corrisponde a nessuna scelta\nReinserisci la tua scelta qui:\t");
    }
  }while(scelta != 1 && scelta != 2);

}

/*_________________________Gioca()_Funzione_principale_______________________*/
void gioca(){

  if(num_g == 0){
    printf("\n Prima di giocare è necessario impostare il gioco, verrai riportato al menu principale\n");
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
  mischia_turni(turno_giocatori);
  nAstr = num_g - nImp;


  while(quest_da_finire != 0 && nImp != 0 && nAstr != 0){
    clr();
    if(count == num_g){
      mischia_turni(turno_giocatori);
      count = 0;
    }
    colore = turno_giocatori[count];
    count++;

    SetColor(colore);

    controllo_scelta = 0;
    if(giocatori[colore].stato == astronauta){
      printf("\n Turno del giocatore \t%s\n",StampaN(giocatori[colore].nome));
      printf("\n Premere invio per iniziare il turno...\n");
      while(getchar()!='\n');
      printf(" Sei nella stanza: \t%p di tipo: %s", giocatori[colore].pos, StampaT(giocatori[colore].pos->tStanza));
      Cerca_G(colore);
      printf("\n Per vincere la partita mancano %u quest da fare\n", quest_da_finire);
      printf("\n In questa stanza puoi:\n");
      printf("\n 1) avanzare");
      printf("\n 2) eseguire la quest");
      printf("\n 3) Chiamata di emergenza\n ");
      scanf("%d", &s);
      while(getchar() != '\n');
      while(s != 1 && s != 2 && s != 3){
        printf("\n Scelta inesistente, riscrivi qui la tua scelta:\n ");
        scanf("%d", &s);
        while(getchar() != '\n');
      }
      do{
        switch(s){
          case 1:
            avanza(colore);
            controllo_scelta = 1;
            break;
          case 2:
            if(esegui_quest(colore) == 0)
              printf("\n Non ci sono quest da fare in questa stanza");
            else
              controllo_scelta = 1;
            break;
          case 3:
            if(chiamata_emergenza(colore) == 1)
              controllo_scelta = 1;
            else
              printf("\n Non è possibile effettuare una chiamata di emergenza in questa stanza \n");
            break;
          default:
            printf("\n Comando sbagliato" );
            break;
          }
        if(controllo_scelta == 0){
          printf("\n Inserisci un'altra voce: ");
          scanf("%d", &s);
          while(getchar() != '\n');
        }
      }while(controllo_scelta == 0);
    }

    if(giocatori[colore].stato == impostore)
    {
      printf("\n Turno del giocatore \t%s\n",StampaN(giocatori[colore].nome));
      printf("\n Premere invio per iniziare il turno...\n");
      while(getchar()!='\n');
      printf(" Sei nella stanza: %p di tipo: %s",giocatori[colore].pos, StampaT(giocatori[colore].pos->tStanza));
      Cerca_G(colore);
      printf("\n Premere:");
      printf("\n 1) avanzare");
      printf("\n 2) Chiamata di emergenza");
      printf("\n 3) Uccidi");
      printf("\n 4) Sabotaggio");
      printf("\n 5) Usa botola\n ");
      scanf("%d", &s);
      while(getchar() != '\n');
      while(s != 1 && s != 2 && s != 3 && s != 4 && s != 5){
        printf("\n Scelta inesistente, riscrivi qui la tua scelta:\n ");
        scanf("%d", &s);
        while(getchar() != '\n');
      }
      do{
        switch(s)
        {
          case 1:
            avanza(colore);
            controllo_scelta = 1;
            break;
          case 2:
            if(chiamata_emergenza(colore) == 1)
              controllo_scelta = 1;
            else
              printf("\n Non è possibile effettuare una chiamata di emergenza in questa stanza \n");
            break;
          case 3:
            if(Uccidi_astronauta(giocatori[colore].pos, giocatori[colore].nome, colore) == 1)
              controllo_scelta = 1;
            else{
              printf("\n Non ci sono giocatori da uccidere in questa stanza");
            }
            break;
            case 4:
                if(Sabotaggio(colore) == 0)
                  printf("\n Non è possibile fare un sabotaggio in questa stanza");
                else
                  controllo_scelta = 1;
                break;
          case 5:
            if(Usa_Botola(colore) == 1)
              controllo_scelta = 1;
            else
              printf("\n Non è presente nessuna botola nella stanza");

            break;
          default:
            printf("\n Comando sbagliato");
            break;
        }
        if(controllo_scelta == 0){
          printf("\n Inserisci un'altra voce: ");
          scanf("%d", &s);
          while(getchar() != '\n');
        }
      }while(controllo_scelta == 0);
    }
  }
  FinePartita();
}

/*_______________________Funzione_che_pulisce_lo_schermo_____________________*/
static void clr(){
  printf("\033[2J");
  printf("\033[0;0f");

}
/*_____________________Setta_il_colore_del_terminale_________________________*/
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
/*_____________________Inizia_gioco()_per_tornare_in_main.c__________________*/
static void Inizia_gioco(){
    printf("\n Premi invio per tornare al menù principale:\n ");
    while(getchar() != '\n');
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
  stanza_inizio->emergenza_chiamata = 1;
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
  if(stanza_inizio-> tStanza == botola){
    botole[contatore_botola] = stanza_inizio;
    contatore_botola++;
  }
  return 1;
}

//funzione che mischia l'array dei turni
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

// funzione che chiede il numero di giocatori
static void n_giocatori(){
  printf("\n Inserisci il numero di giocatori: (Minimo: 4 e Massimo: 10)\n ");
  scanf("%d", &num_g);
  while(getchar() != '\n');
  do{
    if(num_g < 4){
      printf("\n Inserisci più giocatori: ");
      scanf("%d", &num_g);
      while(getchar() != '\n');
    }
    else if(num_g > 10){
      printf("\n Hai inserito troppi giocatori: ");
      scanf("%d", &num_g);
      while(getchar() != '\n');
    }
  }while(num_g < 4 || num_g > 10);
}

//assegna i nomi ai giocatori
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

//assegna ai giocatori lo stato, impostore o astronauta
static void creaStato(){

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

//funzione che legge l'enum del nome del gioatore e lo ritorna in stringa
static char *StampaN(int num){
  switch(num)
  {
    case rosso: return "rosso"; break;
    case rosa: return "rosa"; break;
    case blu: return "blu"; break;
    case viola: return "viola"; break;
    case verde: return "verde"; break;
    case ciano: return "ciano"; break;
    case giallo: return "giallo"; break;
    case arancione: return "arancione"; break;
    case bianco: return "bianco"; break;
    case marrone: return "marrone"; break;
  }
  return "NULL";
}

//funzione che legge l'enum dello stato del giocatore e lo ritorna in stringa
static char *StampaS(int num){
  switch(num)
  {
    case astronauta: return "astronauta"; break;
    case impostore: return "impostore"; break;
  }
  return "null";
}

//funzione che legge l'enum del tipo della stanza e lo ritorna in stringa
static char *StampaT(int s){
  switch(s)
  {
    case vuota: return "vuota"; break;
    case quest_semplice: return "quest_semplice"; break;
    case quest_complicata: return "quest_complicata"; break;
    case botola: return "botola"; break;
  }
  return "null";
}

//funzione avanza eseguibile sia dagli astronauti che dagli impostori
static void avanza(int num){
  int s;
  lista_stanze = giocatori[num].pos;
  printf(" Dove vuoi andare:\n 1.Avanti: %p\n 2.Destra: %p\n 3.Sinistra: %p\n 4.Resta fermo: %p\n ", lista_stanze->avanti, lista_stanze->destra, lista_stanze->sinistra, lista_stanze);//giocatori[num].pos->avanti, giocatori[num].pos->destra, giocatori[num].pos->sinistra);
  scanf("%d", &s);
  while(getchar()!= '\n');
  while(s != 1 && s != 2 && s != 3 && s != 4){
    printf("\n Direzone inesistente, ritenta\n 1)avanti\n2) destra\n3) sinistra\n ... ");
    scanf("%d", &s);
    while(getchar() != '\n');
  }
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
        lista_stanze->emergenza_chiamata = 1;

        if(giocatori[num].pos -> tStanza == botola){
          botole[contatore_botola] = giocatori[num].pos;
          contatore_botola++;
        }
        else{
          stanze[contatore_stanze] = giocatori[num].pos;
          contatore_stanze++;
        }
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
        lista_stanze->emergenza_chiamata = 1;


        if(giocatori[num].pos -> tStanza == botola){
          botole[contatore_botola] = giocatori[num].pos;
          contatore_botola++;
        }
        else{
          stanze[contatore_stanze] = giocatori[num].pos;
          contatore_stanze++;
        }
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
          lista_stanze->emergenza_chiamata = 1;


          if(giocatori[num].pos -> tStanza == botola){
            botole[contatore_botola] = giocatori[num].pos;
            contatore_botola++;
          }
          else{
            stanze[contatore_stanze] = giocatori[num].pos;
            contatore_stanze++;
          }
        }
        else if(lista_stanze->sinistra != NULL)
          giocatori[num].pos = lista_stanze->sinistra;
        break;

        case 4:
          break;

  }
}

//funizone pre gestire la probabilità del tipo delle stanze
static int probabilita(){
  int n;
  n = rand()%100;
  if(n <= 29)
    return 0;
  else if(n <= 59 && n >= 30)
    return 1;
  else if(n <= 74 && n >= 60)
    return 2;
  else if(n <= 100 && n >= 75)
    return 3;
  return 0;
}

//funzione esugui_quest eseguibile solo dagli astronauti
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

static void Cerca_G(int nome){
  printf("\n Nella tua stanza sono presenti questi giocatori: ");
  for(int i = 0; i < num_g; i++){
    if(giocatori[i].pos == giocatori[nome].pos && giocatori[i].nome != giocatori[nome].nome)
      if(giocatori[i].stato == astronauta || giocatori[i].stato == impostore)
        printf(" %s, ", StampaN(giocatori[i].nome));
  }
  for(int i = 0; i < num_g; i++){
    if(giocatori[i].pos == giocatori[nome].pos && giocatori[i].nome != giocatori[nome].nome)
       if(giocatori[i].pos == giocatori[nome].pos && giocatori[i].stato == assassinato)
        printf("\n Il giocatore %s è stato trovato assassinato nella stanza\n",StampaN(giocatori[i].nome) );
  }
}

static void FinePartita(){
  if(quest_da_finire == 0 || nImp == 0){
    clr();
    printf("   _____                                   _                  _       _ _ _\n");
    printf("  / ____|                                 | |                (_)     | | | |\n");
    printf(" | |     _ __ _____      ___ __ ___   __ _| |_ ___  __      ___ _ __ | | | |\n");
    printf(" | |    | '__/ _ \\ \\ /\\ / / '_ ` _ \\ / _` | __/ _ \\ \\ \\ /\\ / / | '_ \\| | | |\n");
    printf(" | |____| | |  __/\\ V  V /| | | | | | (_| | ||  __/  \\ V  V /| | | | |_|_|_|\n");
    printf("  \\_____|_|  \\___| \\_/\\_/ |_| |_| |_|\\__,_|\\__\\___|   \\_/\\_/ |_|_| |_(_|_|_)\n");
    printf("\n Premere invio per tornare al menu e terminare il gioco\n");
    getchar();
    while(getchar() != '\n');
  }else if(nAstr == 0){
    clr();
    printf("\n   _____                           _               __    __ _         _  _  _");
    printf("\n   \\_   \\_ __ ___  _ __   ___  ___| |_ ___  _ __  / / /\\ \\ (_)_ __   / \\/ \\/ \\");
    printf("\n    / /\\/ '_ ` _ \\| '_ \\ / _ \\/ __| __/ _ \\| '__| \\ \\/  \\/ / | '_ \\ /  /  /  /");
    printf("\n /\\/ /_ | | | | | | |_) | (_) \\__ \\ || (_) | |     \\  /\\  /| | | | /\\_/\\_/\\_/");
    printf("\n \\____/ |_| |_| |_| .__/ \\___/|___/\\__\\___/|_|      \\/  \\/ |_|_| |_\\/ \\/ \\/");
    printf("\n                |_|");
    printf("\n Premere invio per tornare al menu e terminare il gioco\n");
    getchar();
    while(getchar() != '\n');
  }
}

//funzione uccidi_astronauta eseguibile solo dagli impostori
static int Uccidi_astronauta(S_Stanza *stanza, int nome, int col){
  int n = 0;
  int s;
  int v[num_g];
  int count = 0;
  int controllo = 0;

  for(int i = 0; i < num_g; i++){
    if(giocatori[i].pos == stanza && giocatori[i].nome != nome && giocatori[i].stato == astronauta){
      n++;
    }
  }
  if(n == 0)
    return 0;
  else{
    printf("\n Puoi uccidere i seguenti giocatori:\n");
    for(int i = 0; i < num_g; i++){
      if(giocatori[i].pos == stanza && giocatori[i].nome != nome && giocatori[i].stato == astronauta){
        v[count] = i;
        printf(" %d) %s\n ",i, StampaN(giocatori[i].nome));
        count++;
      }
    }
    do{
      printf("\n Inserisci il numero del giocatore da uccidere\n ");
      scanf("%d", &s);
      while(getchar() != '\n');
      for(int i = 0; i < count; i++){
        if(s == v[i])
          controllo = 1;
      }
    }while(controllo  == 0);
    while(getchar() != '\n');
    giocatori[s].stato = assassinato;
    nAstr--;
    if(count >  1){
      if(Defenestra(stanza, nome, col) == 1){
        giocatori[s].stato = defenestrato;
      }
    }
    return 1;
  }
}

//funzione chiamata da Uccidi_astronauta che gestisce la probabilita dell'impostore di essere defenestrato
static int Defenestra(S_Stanza *stanza, int nome, int col){
  int n = 0;
  int d = 0;
  for(int i = 0; i < num_g; i++){
    if(giocatori[i].pos == stanza && giocatori[i].nome != nome && giocatori[i].stato == astronauta)
      n+=5;
    if(giocatori[i].pos == giocatori[col].pos->stanza_precedente && giocatori[i].stato == astronauta)
      n+=2;
  }
  if(n == 0)
    return 0;
  d = rand()%10;
  if(d < n){
    giocatori[col].stato = defenestrato;
    nImp--;
    clr();
    fflush(stdin);
    printf("\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n　　　.　　　 　　.　　　　　。　　 。　. 　\n\t\t.　　 。　　　　　 ඞ 。 . 　　 • 　　　　•\n\t\t　　ﾟ　　il %s è stato defenestrato.　 。　.\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n　　　.　　　 　　.　　　　　。　　 。　. ", StampaN(giocatori[col].nome));
    getchar();
    while(getchar() != '\n');
    return 1;
  }
  return 0;
}

static int Sabotaggio(int nome){
  if(giocatori[nome].pos -> tStanza == quest_semplice || giocatori[nome].pos -> tStanza == quest_complicata){
    giocatori[nome].pos -> tStanza = vuota;
    return 1;
  }
  else if(giocatori[nome].pos -> tStanza != quest_semplice && giocatori[nome].pos -> tStanza != quest_complicata)
    return 0;
  return 0;
}

static int Usa_Botola(int nome){
  int n;
  S_Stanza *appoggio = giocatori[nome].pos;

  if(giocatori[nome].pos->tStanza != botola)
    return 0;
  if(contatore_botola > 1){
    do{
      n = rand()%contatore_botola;
      giocatori[nome].pos = botole[n];
    }while(appoggio == botole[n]);
    return 1;
  }
  else if(contatore_stanze > 0){
    do{
      n = rand()%contatore_stanze;
      giocatori[nome].pos = stanze[n];
    }while(appoggio == stanze[n]);
    return 1;
  }
  return 0;
}

static S_Stanza *prossimaStanza(S_Stanza *Node){
  if(Node->avanti != NULL)
    Node = Node->avanti;
  else if(Node->destra != NULL)
    Node = Node->destra;
  else if(Node->sinistra != NULL)
    Node = Node->sinistra;
  return Node;
}

static S_Stanza *Elimina(S_Stanza*Node){
  printf("Elimina è entrata nel nodo %p.\n", Node);
  while(prossimaStanza(Node) != Node){
    if((Node->avanti) != NULL)
      Node->avanti = Elimina(Node->avanti);
    if((Node->destra) != NULL)
      Node->destra = Elimina(Node->destra);
    if((Node->sinistra) != NULL)
      Node->sinistra = Elimina(Node->sinistra);
  }
  if(Node == NULL){
    return NULL;
  }
  else{
    printf("Elimina distruggerà ora il nodo %p.\n", Node);
    free(Node);
  }
  return NULL;
}

static int chiamata_emergenza(int nome){

  int controllo = 0;
  int arr_g[num_g];
  int count = 0;
  int p_imp;
  int p_ast;
  int def = 0;

  if(giocatori[nome].pos -> emergenza_chiamata == 0)
    return 0;
  for(int i = 0; i < num_g; i++){
    if(giocatori[nome].pos == giocatori[i].pos && giocatori[i].stato == assassinato)
      controllo = 1;
  }
  if(controllo == 0)
    return 0;

  for(int i = 0; i < num_g; i++){
    if(giocatori[i].pos == giocatori[nome].pos){
      arr_g[count] = i;
      count++;
    }
  }
  if(count == 0)
    return 0;

  for(int i = 0; i < count; i++){
    p_imp = 3;
    p_ast = 3;
    if(giocatori[arr_g[i]].stato == impostore){
      for(int j = 0; j < count; j++){
        if(giocatori[arr_g[j]].stato == impostore && giocatori[arr_g[j]].nome != giocatori[arr_g[i]].nome)
          p_imp -= 3;
        else if(giocatori[arr_g[j]].stato == astronauta)
          p_imp += 2;
      }
    }
    else if(giocatori[arr_g[i]].stato == astronauta){
      for(int j = 0; j < count; j++){
        if(giocatori[arr_g[j]].stato == astronauta && giocatori[arr_g[j]].nome != giocatori[arr_g[i]].nome)
          p_ast -= 3;
        else if(giocatori[arr_g[j]].stato == impostore)
          p_ast += 2;
      }
    }
      if(giocatori[i].stato == impostore){
        def = rand()%10;
        if(def <= p_imp){
          giocatori[i].stato = defenestrato;
          clr();
          printf("\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n　　　.　　　 　　.　　　　　。　　 。　. 　\n\t\t.　　 。　　　　　 ඞ 。 . 　　 • 　　　　•\n\t\t　　ﾟ　　il %s è stato defenestrato.　 。　.\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n\t\t　　　.　　　 　　.　　　　　。　　 。　. ", StampaN(giocatori[i].nome));
          while(getchar() != '\n');
          controllo = 0;
          nImp--;
        }
      }
      else if(giocatori[i].stato == astronauta){
        def = rand()%10;
        if(def <= p_ast){
          giocatori[i].stato = defenestrato;
          clr();
          printf("\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n　　　.　　　 　　.　　　　　。　　 。　. 　\n\t\t.　　 。　　　　　 ඞ 。 . 　　 • 　　　　•\n\t\t　　ﾟ　　il %s è stato defenestrato.　 。　.\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n\t\t　　　.　　　 　　.　　　　　。　　 。　. ", StampaN(giocatori[i].nome));
          while(getchar() != '\n');
          controllo = 0;
          nAstr--;
        }
      }
      giocatori[nome].pos->emergenza_chiamata = 0;
      if(controllo == 0)
        return 1;
    }
    clr();
    printf("\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n　　　.　　　 　　.　　　　　。　　 。　. 　\n\t\t.　　 。　　　　　 ඞ 。 . 　　 • 　　　　•\n\t\t　　ﾟ　　Nessuno è stato defenestrato　 。　.\n\t\t. 　　　。　　　　•　 　ﾟ　　。 　　.\n\t\t　　　.　　　 　　.　　　　　。　　 。　. ");
    while(getchar() != '\n');
    return 1;
  }

void Termina_Gioco(){
  if(stanza_inizio != NULL){
    lista_stanze = stanza_inizio;
    Elimina(lista_stanze);
  }
  free(giocatori);
}
