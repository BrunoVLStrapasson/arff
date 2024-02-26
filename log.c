#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "log.h"
#include "lista.h"
#include "arff.h"

Lista* libera_memoria_IP(Lista *Inf_IPs, int tamanho){
    if(Inf_IPs == NULL){
        fprintf(stderr,"Erro ao liberar a memoria.\n");
        exit(5);
    }

    for(int i=0;i<tamanho;i++){
        //free(Inf_IPs[i].nome);
        //free(Inf_IPs[i].IP);
        //free(Inf_IPs[i].tam_pkt);
        //free(Inf_IPs[i].ocupados);
        //free(Inf_IPs[i].qnt_IP);
    }
    //tudo ok, memoria liberada
    return Inf_IPs;
}

int Ajusta_Ponteiro(FILE *arff){
    char linha[1024];
        while(fgets(linha,sizeof(linha),arff)){
        if(strncmp(linha,"@data",5) == 0){
            //@data encontrado
            break; 
        }
    }

    return 1;
}

Lista* avalia_nomes(Lista* ips,char* val_array){
    int achou = 0;
    char* string;
    char* ip = "vazio\0";

    No* aux = ips->cabeca->prox;
    while(aux != ips->cabeca){
        if(strcmp(aux->elem->nome,val_array) == 0){
            achou = 1;
            aux->elem->qnt_IP++;    
            break;
        }
        aux = aux->prox;
    }

    if(!achou){
        Avalia_Elem *IPs;
        IPs = (Avalia_Elem*)malloc(sizeof(Avalia_Elem));
        if(!IPs){
            printf("Erro ao alocar memoria.\n");
            exit(10);
        }
        IPs->IP = malloc(16 * sizeof(char));
        IPs->nome = malloc(16 * sizeof(char));
        for(int j = 0; j< 16; j++){
            IPs->IP[j] = ' ';
            IPs->IP[j] = ' ';
        }
        IPs->qnt_IP = 0;
        IPs->tam_pkt = 0; 
        //atribui os valores ao novo nodo que sera inserido na lista
        IPs->qnt_IP = 1;
        strcpy(IPs->IP,ip);
        string = strdup(val_array);
        strcpy(IPs->nome,string); 

        InserirElemFim(ips,IPs);
        free(string);
    }

    return ips;
}

int avalia_ataques(FILE *arff, atributo *atributos,FILE* saida){
    char linha[2048],teste[2048],**val_array = NULL;
    int cont = 0 ;
    char *separador;
    Lista l;
    InicializarLista(&l);

    //acho a posicao de PKT_CLASS no meu vetor de atributos 
    int j=0;
    while(strcmp(atributos[j].rotulo,"PKT_CLASS") != 0){j++;}
    
    //ajusto o ponteiro do arquivo parando em @data
    Ajusta_Ponteiro(arff);

    //comeca a ler todas as linhas do arff apartir do @data, uma linha completa por vez
    while(fgets(teste,sizeof(teste),arff)){      
        //separa tudo que esta entre virgulas e salva em val_array para a verificacao que eh feita
        //em seguida no corpo dessa mesma funcao
        separador = strtok(teste,"{,");
        cont = 0;
        while(separador){
            val_array = realloc(val_array, (cont + 1) * sizeof(char *));
            val_array[cont] = strdup(separador);
            cont++;
            separador = strtok(NULL,",");
        }
        int tam = strlen(val_array[cont-1]);
        //ajusta o fim da string com o /0
        val_array[cont-1][tam-1] = '\0';

        //comparo o nome do ataque que esta em val_array com todas a posicoes posiveis do atributo PKT_CLASS
        for(int i=0;i<atributos[j].num_categorias;i++){
            if(strncmp(val_array[j],"Normal",5) == 0){
                //apenas sai do laço "Normal" nao eh um ataque
                break;
            }else if(strcmp(val_array[j],atributos[j].categorias[i]) == 0){
                avalia_nomes(&l,val_array[j]);
            }
        }
        //Libera o vetor alocado 
        for(int i =0;i<cont;i++){
            free(val_array[i]);
        }
    }

    //registra os ataques no arquivo de saida
    No* end = l.cabeca->prox; 
    while (end != l.cabeca){
        fprintf(saida,"%s;%d\n",end->elem->nome,end->elem->qnt_IP);
        end = end->prox;
    }
    
    free(val_array);

    EsvaziarFila(&l);

    printf("Ataques processados com sucesso.\n");

    return 1;
}

Lista* avalia_ips_entidades(Lista* ips,char* nome_array,char* ip_array){
    int achou = 0;
    char* string_N;
    char* string_P;

    No* aux = ips->cabeca->prox;
    while(aux != ips->cabeca){
        if(strcmp(aux->elem->IP,ip_array) == 0){
            achou = 1;
            if(strcmp(aux->elem->nome,nome_array) == 0){
                aux->elem->qnt_IP++;
            }
            break;
        }
        aux = aux->prox;
    }

    if (!achou) {
        Avalia_Elem *IPs;
        IPs = (Avalia_Elem*)malloc(sizeof(Avalia_Elem));
        if(!IPs){
            printf("Erro ao alocar memoria.\n");
            exit(10);
        }
        IPs->IP = malloc(16 * sizeof(char));
        IPs->nome = malloc(16 * sizeof(char));
        for(int j = 0; j< 16; j++){
            IPs->IP[j] = ' ';
            IPs->IP[j] = ' ';
        }
        IPs->qnt_IP = 0;
        IPs->tam_pkt = 0; 
        // Não é um IP duplicado, adicione à lista
        string_P = strdup(ip_array);
        strcpy(IPs->IP, string_P);
        string_N = strdup(nome_array);
        strcpy(IPs->nome, string_N);
        IPs->qnt_IP = 1;
        InserirElemFim(ips,IPs);
        //libera a memoria alocada nos strings
        free(string_P);
        free(string_N);
        //free(IPs);
    }

    return ips;
}

int avalia_entidades(FILE *arff, atributo *atributos,FILE* saida){
    char linha[2048],teste[2048],**val_array = NULL;
    int cont = 0;
    char *separador;
    Lista l;
    InicializarLista(&l);
    
    //acho que posicao de PKT_CLASS no meu vetor de atributos 
    int j=0;
    while(strcmp(atributos[j].rotulo,"PKT_CLASS") != 0){j++;}
    
    //acho que posiccao SRC_ADD no meu vetor de atributos
    int k = 0;
    while(strcmp(atributos[k].rotulo,"SRC_ADD") != 0){k++;}   
    
    //Posiciono o ponteiro em @data
    Ajusta_Ponteiro(arff);

    //comeco a ler linha a linha a parte de dados do arquivo
    while(fgets(teste,sizeof(teste),arff)){      
        //separa tudo que esta entre virgulas (dados do arquivo) e salva em val_array, 
        //em seguida chamo a funcao para avaliar e classificar a entidade dessa linha
        separador = strtok(teste,"{,");
        cont = 0;
        while(separador){
            val_array = realloc(val_array, (cont + 1) * sizeof(char *));
            val_array[cont] = strdup(separador);
            cont++;
            separador = strtok(NULL,",");
        }
        int tam = strlen(val_array[cont-1]);
        //ajusta o fim da string com o /0
        val_array[cont-1][tam-1] = '\0';

        avalia_ips_entidades(&l,val_array[j],val_array[k]);

        //Libera o vetor alocado 
        for(int i =0;i<cont;i++){
            free(val_array[i]);
        }
    }   

    //registra as entidades no arquivo de saida
    No* end = l.cabeca->prox; 
    while (end != l.cabeca) {
        if(strcmp(end->elem->nome,"Normal") == 0){
            fprintf(saida,"%s;benigna\n",end->elem->IP);
        }else if(end->elem->qnt_IP >= 1 && end->elem->qnt_IP <= 5){
            fprintf(saida,"%s;potencialmente maliciosa\n",end->elem->IP);
        }else{ //(end->elem->IP > 5){ 
            fprintf(saida,"%s;maliciosa\n",end->elem->IP);
        }
        end = end->prox;
    }

    free(val_array);

    EsvaziarFila(&l);

    printf("Entidades processadas com sucesso.\n");

    return 1;
}

Lista* avalia_ips_tam(Lista* ips,char* nome_array,char* tam_array){
    int achou = 0;
    int valor = 0 ;
    char *string;

    No* aux = ips->cabeca->prox;
    while(aux != ips->cabeca){
        if(strcmp(aux->elem->nome,nome_array) == 0){
            valor = atoi(tam_array);
            aux->elem->tam_pkt+=valor;
            aux->elem->qnt_IP++;
            achou = 1;
        }
        aux = aux->prox;
    }

    if (!achou){
        Avalia_Elem *IPs;
        IPs = (Avalia_Elem*)malloc(sizeof(Avalia_Elem));
        if(!IPs){
            printf("Erro ao alocar memoria.\n");
            exit(10);
        }
        IPs->qnt_IP = 1;
        IPs->tam_pkt = 0;
        IPs->IP = malloc(16 * sizeof(char));
        IPs->nome = malloc(16 * sizeof(char));
        for(int j = 0; j< 16; j++){
            IPs->IP[j] = ' ';
            IPs->IP[j] = ' ';
        }
        //pos = ips[0].ocupados;
        if(strcmp(nome_array,"Normal") != 0){
            valor = atoi(tam_array);
            IPs->tam_pkt += valor;
            string = strdup(nome_array);
            strcpy(IPs->nome,string);
            IPs->qnt_IP = 1;
            InserirElemFim(ips,IPs);
            free(string);
        }
    }

    return ips;
}

int avalia_tamanho(FILE *arff, atributo *atributos,FILE* saida){
    char linha[2048],teste[2048],**val_array = NULL;
    int cont = 0;
    char *separador;
    Lista l;
    InicializarLista(&l);

    //acho que posicao de PKT_CLASS no meu vetor de atributos 
    int j=0;
    while(strcmp(atributos[j].rotulo,"PKT_CLASS") != 0){j++;}
    
    //acho que posiccao PKT_AVG_SIZE. no meu vetor de atributos(tamanho do pacote)
    int k = 0;
    while(strcmp(atributos[k].rotulo,"PKT_AVG_SIZE") != 0){k++;}
    
    //ajusto o ponteiro para o @data
    Ajusta_Ponteiro(arff);

    //comeco a ler os dados do arquivo, linha a linha
    while(fgets(teste,sizeof(teste),arff)){      
        //separa tudo que esta entre virgulas e salva em val_array para a verificacao que eh feita
        //em seguida no corpo dessa mesma funcao
        separador = strtok(teste,"{,");
        cont = 0;
        while(separador){
            val_array = realloc(val_array, (cont + 1) * sizeof(char *));
            val_array[cont] = strdup(separador);
            cont++;
            separador = strtok(NULL,",");
        }
        int tam = strlen(val_array[cont-1]);
        //ajusta o fim da string com o /0
        val_array[cont-1][tam-1] = '\0';

        for(int i=0;i<atributos[j].num_categorias;i++){
            if(strncmp(val_array[j],"Normal",5) == 0){
                //apenas sai do laço "Normal" nao eh um ataque
                break;
            }else if(strcmp(val_array[j],atributos[j].categorias[i]) == 0){
                avalia_ips_tam(&l,val_array[j],val_array[k]);
            }
        }
        //Libera o vetor alocado 
        for(int i =0;i<cont;i++){
            free(val_array[i]);
        }
    }

    //registra a media do tamanho dos ataques no arquivo de saida
    No* end = l.cabeca->prox; 
    while (end != l.cabeca) {
        //printf("%s;%d\n",end->elem->nome,end->elem->tam_pkt/end->elem->qnt_IP);
        fprintf(saida,"%s;%d\n",end->elem->nome,end->elem->tam_pkt/end->elem->qnt_IP);
        end = end->prox;
    }

    free(val_array);

    EsvaziarFila(&l);

    printf("Tamanhos processados com sucesso.\n");

    return 1;
}

Lista* avalia_ips_blist(Lista* ips,char* nome_array,char* ip_array){
    int achou = 0;
    char* string_N;
    char* string_P;

    No* aux = ips->cabeca->prox;
    while(aux != ips->cabeca){
        if(strcmp(aux->elem->IP,ip_array) == 0){
            achou = 1;
            if(strcmp(aux->elem->nome,nome_array) == 0){
                aux->elem->qnt_IP++;
            }
            break;
        }
        aux = aux->prox;
    }

    if (!achou){
        Avalia_Elem *IPs;
        IPs = (Avalia_Elem*)malloc(sizeof(Avalia_Elem));
        if(!IPs){
            printf("Erro ao alocar memoria.\n");
            exit(10);
        }
        IPs->qnt_IP = 1;
        IPs->tam_pkt = 0;
        IPs->IP = malloc(16 * sizeof(char));
        IPs->nome = malloc(16 * sizeof(char));
        for(int j = 0; j< 16; j++){
            IPs->IP[j] = ' ';
            IPs->IP[j] = ' ';
        }
        // Não é um IP duplicado, adicione à lista
        string_P = strdup(ip_array);
        string_N = strdup(nome_array);
        strcpy(IPs->IP, string_P);
        strcpy(IPs->nome, string_N);
        IPs->qnt_IP = 1;
        InserirElemFim(ips,IPs);
        //libera a memoria dos strings
        free(string_N);
        free(string_P);
    }

    return ips;
}

int black_list(FILE *arff, atributo *atributos,FILE* saida){
    char linha[2048],teste[2048],**val_array = NULL;
    int cont = 0;
    char *separador;
    Lista l;

    InicializarLista(&l);

    //acho que posicao de PKT_CLASS no meu vetor de atributos 
    int j=0;
    while(strcmp(atributos[j].rotulo,"PKT_CLASS") != 0){j++;}
    
    //acho que posiccao SRC_ADD no meu vetor de atributos
    int k = 0;
    while(strcmp(atributos[k].rotulo,"SRC_ADD") != 0){k++;}
    
    //ajusta o ponteiro para @data
    Ajusta_Ponteiro(arff);

    //comeco a ler os dados do arquivo, linha a linha
    while(fgets(teste,sizeof(teste),arff)){      
        //separa tudo que esta entre virgulas e salva em val_array para a verificacao que eh feita
        //em seguida no corpo dessa mesma funcao
        separador = strtok(teste,"{,");
        cont = 0;
        while(separador){
            val_array = realloc(val_array, (cont + 1) * sizeof(char *));
            val_array[cont] = strdup(separador);
            cont++;
            separador = strtok(NULL,",");
        }
        int tam = strlen(val_array[cont-1]);
        //ajusta o fim da string com o /0
        val_array[cont-1][tam-1] = '\0';

        avalia_ips_blist(&l,val_array[j],val_array[k]);

        //Libera o vetor alocado 
        for(int i =0;i<cont;i++){
            free(val_array[i]);
        }
    }   

    //registra a Blist no arquivo de saida
    No* end = l.cabeca->prox; 
    while (end != l.cabeca) {
        if(end->elem->qnt_IP > 5 && strcmp(end->elem->nome,"Normal") !=0){
            fprintf(saida,"%s\n",end->elem->IP);
        }
        end = end->prox;
    }

    free(val_array);

    EsvaziarFila(&l);

    printf("Black list criada com sucesso.\n");

    return 1;
}
