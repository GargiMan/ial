
/* Hlavičkový soubor pro c401.c - rekurzívní implementace operaci nad BVS,
obsahuje jednak nutné knihovny a externí proměnné, ale rovněž
definici datových typů, se kterými se pracuje v jdenotlivých
funkcích. Nemodifikovat! */

/* ********************** SOUBOR S HLAVIČKOU ********************** */
/* ********************** ------------------ ********************** */

/*  vytvořil: Martin Tuček
    předmět: Algoritmy (IAL) - FIT (Fakulta Informacnich Technologií)
    hlavicka pro soubor: c401.c
    datum: září 2005
    upravil: Bohuslav Křena, listopad 2009
             Karel Masařík, říjen 2013
             Radek Hranický, 2014-2018
*/
/* ***************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0

extern int solved;                        /* indikace, zda byla funkce řešena */

/* uzel stromu */

typedef struct tBSTNode {
	char Key;			                                                      /* klíč */
	int BSTNodeCont;                                            /* užitečný obsah uzlu */
	struct tBSTNode * LPtr;                                    /* levý podstrom */
	struct tBSTNode * RPtr;                                   /* pravý podstrom */
} *tBSTNodePtr;

/* prototypy funkcí */

void BSTInit   (tBSTNodePtr *);
int  BSTSearch (tBSTNodePtr  , char, int *);
void BSTInsert (tBSTNodePtr *, char, int);
void BSTDelete (tBSTNodePtr *, char);
void BSTDispose(tBSTNodePtr *);

/* konec c401.h */
