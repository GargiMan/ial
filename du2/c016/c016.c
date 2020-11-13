
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {

	int retval = 1;
	int keylen = strlen(key);
	for (int i = 0; i < keylen; i++) retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	//ptr doesnt exist
	if (ptrht == NULL) return;

	//init table to NULL
	for (int i = 0; i < HTSIZE; i++) (*ptrht)[i] = NULL;

	return;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	//ptr doesnt exist
	if (ptrht == NULL) return NULL;
	tHTItem* ptr = (*ptrht)[hashCode(key)];

	//search for key in ptr
	while (ptr != NULL) {
		if (strcmp(ptr->key, key) == 0) return ptr;
		ptr = ptr->ptrnext;
	}	

	return ptr;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	//ptr doesnt exist
	if (ptrht == NULL) return;

	//search
	tHTItem* ptr = htSearch(ptrht,key);

	//found, set new data
	if (ptr) {
		ptr->data = data;
		return;
	} 

	//not found, allocate new
	ptr = malloc(sizeof(tHTItem));
	if (ptr == NULL) return;

	ptr->key = key;
	ptr->data = data;
	ptr->ptrnext = (*ptrht)[hashCode(key)];
	(*ptrht)[hashCode(key)] = ptr;
	
	return;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	//ptr doesnt exist
	if (ptrht == NULL) return NULL;

	//search
	tHTItem* ptr = htSearch(ptrht,key);

	//return data adr or null
	return (ptr ? &ptr->data : NULL);
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

	//ptr doesnt exist
	if (ptrht == NULL) return;

	tHTItem* ptr = (*ptrht)[hashCode(key)];
	tHTItem* ptrprev = NULL;

	//search for key at index
	while (ptr != NULL) {

		if (strcmp(ptr->key, key) == 0) {
			if (ptrprev) {
				ptrprev->ptrnext = ptr->ptrnext;
			} else {
				(*ptrht)[hashCode(key)] = ptr->ptrnext;
			}
			free(ptr);
			return;
		}
		
		ptrprev = ptr;
		ptr = ptr->ptrnext;
	}	

	return;
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	//ptr doesnt exist
	if (ptrht == NULL) return;

	//clear table array
	for (int i = 0; i < HTSIZE; i++) {

		//clear single index
		while ((*ptrht)[i] != NULL) {
			tHTItem* ptr = (*ptrht)[i]->ptrnext;
			free((*ptrht)[i]);
			(*ptrht)[i] = ptr;
		}
	}

	return;
}
