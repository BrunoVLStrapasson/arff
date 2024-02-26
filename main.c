#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

  #include "arff.h"
  #include "lista.h"
  //#include "log.h"

#define HELP "Forma de uso: ./arff -i <arq_in> [-p -v -a -b -c -d]\n"

int main(int argc, char **argv){
  int opt;
  char exibicao = 0;
  char validacao = 0;
  char ataques = 0;
  char entidades = 0;
  char tamanho = 0;
  char firewall = 0;
  char *entrada = 0;

  while ((opt = getopt(argc, argv, "pvabcdi:")) != -1) {
    switch (opt) {
    case 'i':
      entrada = strdup(optarg);
      break;
    case 'p':
      exibicao = 1;
      break;
    case 'v':
      validacao = 1;
      break;
    case 'a': //Relatório de todos os ataques ocorridos e o número de ocorrências no dataset (nome do arquivo de saída: R_ATAQUES.txt)
      ataques = 1;
      break;
    case 'b': //Relatório de endereços de origem maliciosos, pot. maliciosos e benignos (nome do arquivo de saída: R_ENTIDADES.txt)
      entidades = 1;
      break;
    case 'c': //A média da média do tamanho dos pacotes por cada tipo de ataque, considerando cada origem (nome do arquivo de saída: R_TAMANHO.txt)
      tamanho = 1; 
      break;
    case 'd': //Geração de um arquivo de configuração de iptable para bloquear as origens maliciosas (nome do arquivo de saída: C_IPTABLES.txt)
      firewall = 1;
      break;

    default:
      fprintf(stderr, HELP);
      exit(1);
    }
  }

  if (!entrada){
    fprintf(stderr, HELP);
    exit(2);
  }

  FILE *arquivo = fopen(entrada, "r");
  if (!arquivo){
    fprintf(stderr, "Não foi possível abrir o arquivo informado!\n");
    exit(3);
  }

  int qntd_atributos = conta_atributos(arquivo);
  if (!qntd_atributos){
    fprintf(stderr, "Existem erros no arquivo fornecido!\n");
    exit(4);
  }
  atributo *dados_atributos = processa_atributos(arquivo, qntd_atributos);

  if (exibicao){
    exibe_atributos(dados_atributos, qntd_atributos);
  }
  if (validacao){
    rewind(arquivo);
    valida_arff(arquivo, dados_atributos, qntd_atributos);
  }
  if (ataques){
    FILE* saida;
    saida = fopen("R_ATAQUES.txt","w");
    if(!saida){
        fprintf(stderr,"Erro ao abrir o arquivo R_ATAQUES.txt.\n");
        exit(11);
    }
    rewind(arquivo);
    //Chamara função que gera o relatório dos ataques;
    if(!avalia_ataques(arquivo,dados_atributos,saida)){
        fprintf(stderr,"Erro ao gerar relatorio de ataques.\n");
        exit(18);
    }

    fclose(saida);
  }
  if (entidades){
    FILE* saida;
    saida = fopen("R_ENTIDADES.txt","w");
    if(!saida){
        fprintf(stderr,"Erro ao abrir o arquivo R_ENTIDADES.txt.\n");
        exit(12);
    }
    rewind(arquivo);
    //Chamar a função que gera o relatório de entidades;
    if(!avalia_entidades(arquivo,dados_atributos,saida)){
        fprintf(stderr,"Erro ao gerar relatorio de entidades.\n");
        exit(17);
    }

    fclose(saida);
  }

  if (tamanho){
    FILE* saida;
    saida = fopen("R_TAMANHO.txt","w");
    if(!saida){
        fprintf(stderr,"Erro ao abrir o arquivo R_TAMANHO.txt.\n");
        exit(13);
    }
    rewind(arquivo);
    //Chama a função que gera o relatório da media do tamanho dos ataques;
    if(!avalia_tamanho(arquivo,dados_atributos,saida)){
        fprintf(stderr,"Erro ao gerar relatorio da media do tamanho dos ataques.\n");
        exit(16);
    }

    fclose(saida);
  }

  if (firewall){
    FILE* saida;
    saida = fopen("BLACKLIST.bl","w");
    if(!saida){
        fprintf(stderr,"Erro ao abrir o arquivo BLACKLIST.bl.\n");
        exit(14);
    }
    rewind(arquivo);
    //Chama a função que gera a black list;
    if(!black_list(arquivo,dados_atributos,saida)){
        fprintf(stderr,"Erro ao gerar a Black list.\n");
        exit(15);
    }

    fclose(saida);
  }

  //liberar todas as categorias alocadas
  for(int i=0;i<qntd_atributos;i++){
      liberar_categorias(&dados_atributos[i]);
  }
  //libera a memoria alocada em atributos
  libera_memoria(dados_atributos, qntd_atributos);

  //libera o vetor de atributos
  free(dados_atributos);

  //libera a entrada
  free(entrada);

  fclose(arquivo);
  
  return 0 ;
}
