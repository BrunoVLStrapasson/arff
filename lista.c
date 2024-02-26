#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
//#include "arff.h"

void InicializarLista(Lista* f) {
    f->cabeca = (No*) malloc(sizeof(No));
    f->cabeca->prox = f->cabeca;
    f->cabeca->ant = f->cabeca;
    f->cabeca->elem = NULL;
    //f->cabeca->elem->qnt_IP = 0;
    //f->cabeca->elem->tam_pkt = 0;
}

int InserirElemFim(Lista* l, Avalia_Elem* elem) {
    No* novo = (No*) malloc(sizeof(No));
    novo->elem = elem;
    novo->prox = l->cabeca;
    novo->ant = l->cabeca->ant;
    l->cabeca->ant = novo;
    novo->ant->prox = novo;
    return 0;
}


int ExcluirElemIni(Lista* f) {
    if (f->cabeca->prox == f->cabeca) return 1;
    No* apagar = f->cabeca->prox;
    f->cabeca->prox = apagar->prox;
    apagar->prox->ant = f->cabeca;
    free(apagar);
    return 0;
}

void EsvaziarFila(Lista* f) {
    No* end = f->cabeca->prox;
    while (end != f->cabeca) {
    No* apagar = end;
    end = end->prox;
    free(apagar->elem->IP);
    free(apagar->elem->nome);
    free(apagar->elem);
    free(apagar);
    }
    f->cabeca->prox = f->cabeca;
    f->cabeca->ant = f->cabeca;
}

int Imprimir(Lista* f) {
    No* end = f->cabeca->prox; 
    while (end != f->cabeca) {
        //printf("%s;%d\n",end->elem->nome,end->elem->tam_pkt/end->elem->qnt_IP);
        if(end->elem->qnt_IP > 5 && strcmp(end->elem->nome,"Normal") !=0){
            printf("%s\n",end->elem->IP);
        }
        end = end->prox;
    }
    return 0;
}