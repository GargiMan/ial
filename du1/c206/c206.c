// c206.c
// Řešení IAL-DU1, 4.10.2020
// Autor: Marek Gergel, FIT
// Přeloženo: gcc 7.5.0

/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	

    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */

    return;
}

void DLInitList (tDLList *L) {
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
    L->Last = NULL;

    return;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

    //kontrola
    if (L == NULL) return;

    //mazanie prvkov
    while (L->First != NULL) {
        tDLElemPtr temp = L->First->rptr;
        free(L->First);
        L->First = temp;
    }

    //nastavenie neaktivity a posledneho prvku
    L->Act = NULL;
    L->Last = NULL;

    return;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
    //kontrola
    if (L == NULL) return;

    //alokacia
    tDLElemPtr element = malloc(sizeof(struct tDLElem));
    if (element == NULL) {
        DLError();
        return;
    }

    //vkladanie do zoznamu a priradenie hodnoty
    element->data = val;
    element->rptr = L->First;
    element->lptr = NULL;
    if (element->rptr != NULL) {
        element->rptr->lptr = element;
    } else {
        L->Last = element;
    }
    L->First = element;

    return;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	

    //kontrola
    if (L == NULL) return;

    //alokacia
	tDLElemPtr element = malloc(sizeof(struct tDLElem));
    if (element == NULL) {
        DLError();
        return;
    }

    //vkladanie do zoznamu a priradenie hodnoty
    element->data = val;
    element->rptr = NULL;
    element->lptr = L->Last;
    if (element->lptr != NULL) {
        element->lptr->rptr = element;
    } else {
        L->First = element;
    }
    L->Last = element;

    return;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
    //kontrola
    if (L == NULL) return;

    //aktivita na prvy prvok
    L->Act = L->First;

    return;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
    //kontrola
    if (L == NULL) return;

    //aktivita na posledny prvok
    L->Act = L->Last;

    return;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    //kontrola
    if (L == NULL) return;
    if (L->First == NULL) {
        DLError();
        return;
    }

    //navrat hodnoty
	*val = L->First->data;

	return;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Last == NULL) {
        DLError();
        return;
    }

    //navrat hodnoty
    *val = L->Last->data;

	return;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->First == NULL) return;
    if (L->First == L->Act) L->Act = NULL;

    //mazanie prvku
    tDLElemPtr temp = L->First->rptr;
    if (temp == NULL) {
        L->Last = NULL;
    } else {
        temp->lptr = NULL;
    }
    free(L->First);
    L->First = temp;

    return;
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/ 
	
    //kontrola
    if (L == NULL) return;
    if (L->Last == NULL) return;
    if (L->Last == L->Act) L->Act = NULL;

    //mazanie prvku
    tDLElemPtr temp = L->Last->lptr;
    if (temp == NULL) {
        L->First = NULL;
    } else {
        temp->rptr = NULL;
    }
    free(L->Last);
    L->Last = temp;

    return;
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL || L->Act->rptr == NULL) return;

    //mazanie prvku
    tDLElemPtr temp = L->Act->rptr;
    L->Act->rptr = L->Act->rptr->rptr;
    if (L->Act->rptr != NULL) {
        L->Act->rptr->lptr = L->Act;
    } else {
        L->Last = L->Act;
    }
    free(temp);

    return;
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL || L->Act->lptr == NULL) return;

    //mazanie prvku
    tDLElemPtr temp = L->Act->lptr;
    L->Act->lptr = L->Act->lptr->lptr;
    if (L->Act->lptr != NULL) {
        L->Act->lptr->rptr = L->Act;
    } else {
        L->First = L->Act;
    }
    free(temp);

    return;
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
    //kontrola
    if (L == NULL) return;
	if (L->Act == NULL) return;

    //alokacia
    tDLElemPtr element = malloc(sizeof(struct tDLElem));
    if (element == NULL) {
        DLError();
        return;
    }

    //vkladanie do zoznamu a priradenie hodnoty
    element->data = val;
    element->rptr = L->Act->rptr;
    element->lptr = L->Act;
    element->lptr->rptr = element;
    if (element->rptr != NULL) {
        element->rptr->lptr = element;
    } else {
        L->Last = element;
    }

    return;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL) return;

    //alokacia
    tDLElemPtr element = malloc(sizeof(struct tDLElem));
    if (element == NULL) {
        DLError();
        return;
    }

    //vkladanie do zoznamu a priradenie hodnoty
    element->data = val;
    element->rptr = L->Act;
    element->lptr = L->Act->lptr;
    element->rptr->lptr = element;
    if (element->lptr != NULL) {
        element->lptr->rptr = element;
    } else {
        L->First = element;
    }

    return;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    
    //kontrola
    if (L == NULL) return;	
	if (L->Act == NULL) {
        DLError();
        return;
    }

    //navrat hodnoty
    *val = L->Act->data;

    return;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL) return; 

    //zmena hodnoty
    L->Act->data = val;

	return;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL) return;

    //presun aktivity na dalsi prvok
    L->Act = L->Act->rptr;

    return;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
    //kontrola
    if (L == NULL) return;
    if (L->Act == NULL) return;

    //presun aktivity na predchadzajuci prvok
    L->Act = L->Act->lptr;

    return;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
    //kontrola
    if (L == NULL) return 0;

    //navrat 1 aktivny , 0 neaktivny
    return (L->Act == NULL ? 0 : 1);
}

/* Konec c206.c*/
