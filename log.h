#ifndef __LOG__
#define __LOG__

//#include "lista.h"

Lista* libera_memoria_IP(Lista *Inf_IPs, int tamanho);

Lista* avalia_nomes(Lista* ips,char* val_array,int tamanho);

int avalia_ataques(FILE *arff, atributo *atributos,FILE* saida);

//Avalia_IP* avalia_ips_entidades(Avalia_IP* ips,char* nome_array,char* ip_array,int tamanho_categoria,int tamanho_total);

int avalia_entidades(FILE *arff, atributo *atributos,FILE* saida);

//Avalia_IP* avalia_ips_tam(Avalia_IP* ips,char* nome_array,char* tam_array,int tamanho_total);

int avalia_tamanho(FILE *arff, atributo *atributos,FILE* saida);

//Avalia_IP* avalia_ips_blist(Avalia_IP* ips,char* nome_array,char* ip_array,int tamanho_categoria,int tamanho_total);

int black_list(FILE *arff, atributo *atributos,FILE* saida);

#endif
