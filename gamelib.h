#include <stdio.h>

//gamelib.h contiene le dichiarazioni delle funzioni definite in game.c
//e le definizioni dei tipi utilizzati

//Stato_giocatore definisce il tipo di giocatore
typedef enum {astronauta, impostore, assassinato, defenestrato}Stato_giocatore;

//Nome_giocatore definisce il nome in base al colore assegnato
typedef enum {rosso, rosa, blu, viola, verde, ciano, giallo, arancione, bianco, nero}Nome_giocatore;

//Tipo_Stanza descrive la stanza e cosa contiene
typedef enum {vuota, quest_semplice, quest_complicata, botola}Tipo_Stanza;

typedef enum{true, false}bool;
//struct per la creazione e descrizione delle stanza
typedef struct Stanza
{
  struct Stanza* avanti;
  struct Stanza* sinistra;
  struct Stanza* destra;
  struct Stanza* stanza_precedente;
  Tipo_Stanza tStanza;
  bool emergenza_chiamata;
}S_Stanza;

//struct per la creazione dei giocatori
typedef struct Giocatore
{
  S_Stanza *pos;
  Stato_giocatore stato;
  Nome_giocatore nome;
}S_Giocatore;

//prototipi delle funzioni
void imposta_gioco();
void gioca();
//void termina_gioco();
