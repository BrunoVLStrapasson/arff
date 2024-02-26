#ifndef __LISTA__
#define __LISTA_

typedef struct {
    char *IP;
    char *nome;
    int qnt_IP;
    int tam_pkt;
}Avalia_Elem;

typedef struct auxElem {
    Avalia_Elem* elem;
    struct auxElem* ant;
    struct auxElem* prox;
}No;

typedef struct {
    No* cabeca;
}Lista;

void InicializarLista(Lista* l);

int InserirElemFim(Lista* f, Avalia_Elem* elem);

int ExcluirElemIni(Lista* f);

void EsvaziarFila(Lista* f);

int Imprimir(Lista* f);


#endif


