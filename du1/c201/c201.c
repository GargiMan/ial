// c201.c
// Řešení IAL-DU1, 4.10.2020
// Autor: Marek Gergel, FIT
// Přeloženo: gcc 7.5.0

/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      DisposeList ... zrušení všech prvků seznamu,
**      InitList ...... inicializace seznamu před prvním použitím,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/

    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */

    return;
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

    //kontrola
    if (L == NULL) return;
	
    //inicializacia
    L->First = NULL;
    L->Act = NULL;

    return;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/

    //kontrola
    if (L == NULL) return;

    //mazanie prvkov
    while (L->First != NULL) {
        tElemPtr temp = L->First->ptr;
        free(L->First);
        L->First = temp;
    }

    //nastavenie neaktivity
    L->Act = NULL;

    return;
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    
    //kontrola
    if (L == NULL) return;

    //alokacia
    tElemPtr element = malloc(sizeof(struct tElem));
    if (element == NULL) {
        Error();
        return;
    }

    //vkladanie do zoznamu a priradenie hodnoty
    element->data = val;
    element->ptr = L->First;
    L->First = element;

    return;
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/

    //kontrola
    if (L == NULL) return;

    //aktivita na prvy prvok
    L->Act = L->First;

    return;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->First == NULL) {
        Error();
        return;
    }

    //navrat hodnoty
    *val = L->First->data;

    return;
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->First == NULL) return;
    if (L->First == L->Act) L->Act = NULL;

    //mazanie prvku
    tElemPtr temp = L->First->ptr;
    free(L->First);
    L->First = temp;

    return;
}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL || L->Act->ptr == NULL) return;

    //mazanie prvku
    tElemPtr temp = L->Act->ptr;
    L->Act->ptr = L->Act->ptr->ptr;
    free(temp);

    return;
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL) return;

    //alokacia
    tElemPtr element = malloc(sizeof(struct tElem));
    if (element == NULL) {
        Error();
        return;
    }

    //vkladanie do zoznamu a priradenie hodnoty
    element->data = val;
    element->ptr = L->Act->ptr;
    L->Act->ptr = element;
	
    return;
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
	
    //kontrola
    if (L == NULL) return;
	if (L->Act == NULL) {
        Error();
        return;
    }

    //navrat hodnoty
    *val = L->Act->data;

    return;
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL) return;
    
    //zmena hodnoty
	L->Act->data = val;

    return;
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
	
    //kontrola
    if (L == NULL) return;
	if (L->Act == NULL) return;

    //presun aktivity na dalsi prvok
    L->Act = L->Act->ptr;

    return;
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
	
    //kontrola
    if (L == NULL) return 0;

    //navrat 1 aktivny , 0 neaktivny
    return (L->Act == NULL ? 0 : 1);
}

/* Konec c201.c */
