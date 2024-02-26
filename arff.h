#ifndef __ARFF__
#define __ARFF__


typedef struct {
  char *rotulo;
  char *tipo;
  char **categorias;//vetor de strings
  int num_categorias;
} atributo;

void exibe_atributos(atributo *infos, int quantidade);
int conta_atributos(FILE *arff);
void processa_categorias(atributo *elemento, char *categorias);
atributo* processa_atributos(FILE *arff, int quantidade);
void valida_arff(FILE *arff, atributo *atributos, int quantidade);

atributo* libera_memoria(atributo *InfAtributo, int tamanho);

void liberar_categorias(atributo *elemento);

int avalia_ataques(FILE *arff, atributo *atributos,FILE* saida);

int avalia_entidades(FILE *arff, atributo *atributos,FILE* saida);

int avalia_tamanho(FILE *arff, atributo *atributos,FILE* saida);

int black_list(FILE *arff, atributo *atributos,FILE* saida);

int avalia_ataques(FILE *arff, atributo *atributos,FILE* saida);

//Avalia_IP* avalia_ips_entidades(Avalia_IP* ips,char* nome_array,char* ip_array,int tamanho_categoria,int tamanho_total);

int avalia_entidades(FILE *arff, atributo *atributos,FILE* saida);

//Avalia_IP* avalia_ips_tam(Avalia_IP* ips,char* nome_array,char* tam_array,int tamanho_total);

int avalia_tamanho(FILE *arff, atributo *atributos,FILE* saida);

//Avalia_IP* avalia_ips_blist(Avalia_IP* ips,char* nome_array,char* ip_array,int tamanho_categoria,int tamanho_total);

int black_list(FILE *arff, atributo *atributos,FILE* saida);



#endif