#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pessoa {
  char nome[100];
  char cpf[12];
} Pessoa;

typedef struct carro {
  char placa[8];
  int ano;
  char modelo[50];
  char cpf[12];
} Carro;

int tamPessoas = 0;
int tamCarros = 0;
int pessoasComCarro = 0;
Pessoa *pessoas;
Carro *carros;

void menuOpcoes() {
  printf("\n\n");
  printf("---------- MENU DE OPCOES ----------\n\n");
  printf("(1) - Cadastrar Pessoa\n");
  printf("(2) - Cadastrar Carro\n");
  printf("(3) - Imprimir dados\n");
  printf("(0) - Encerrar Programa\n\n");
  printf("Opcao: ");
}

void ordenaPessoas() {
  int i, aux, c;
  Pessoa tmp;
  for(i=0; i<tamPessoas-1; i++)
    for(aux=0; aux<tamPessoas-1; aux++) {
      for(c=0; c<11 && pessoas[aux].cpf[c]==pessoas[aux+1].cpf[c]; c++);
      if(pessoas[aux+1].cpf[c]<pessoas[aux].cpf[c]) {
        tmp = pessoas[aux];
        pessoas[aux] = pessoas[aux+1];
        pessoas[aux+1] = tmp;
      }
    }
}

void ordenaCarros() {
  int i, aux, c;
  Carro tmp;
  for(i=0; i<tamCarros-1; i++)
    for(aux=0; aux<tamCarros-1; aux++) {
      for(c=0; c<11 && carros[aux].cpf[c]==carros[aux+1].cpf[c]; c++);
      if(carros[aux+1].cpf[c]<carros[aux].cpf[c]) {
        tmp = carros[aux];
        carros[aux] = carros[aux+1];
        carros[aux+1] = tmp;
      }
    }
}

void escreveArquivo() {
  int i, j=0;
  int aux;
  FILE *arquivoPessoas;
  FILE *arquivoCarros;
  arquivoPessoas = fopen("pessoas.txt", "w");
  arquivoCarros = fopen("carros.txt", "w");
  ordenaPessoas();
  ordenaCarros();
  for(i=0; i<tamPessoas; i++) {
    aux = 0;
    j = 0;
    while(aux == 0 && j < tamCarros) {
      if(strcmp(pessoas[i].cpf, carros[j].cpf) == 0) {
        pessoasComCarro++;
        aux = 1;
      }
      j++;
    }
  }
  fprintf(arquivoPessoas, "%d\n", pessoasComCarro);
  for(i=0; i<tamPessoas; i++) {
    aux = 0;
    j = 0;
    while(aux == 0 && j < tamCarros) {
      if(strcmp(pessoas[i].cpf, carros[j].cpf) == 0) {
        fprintf(arquivoPessoas, "%s\n", pessoas[i].nome);
        if(i < tamPessoas-1)
          fprintf(arquivoPessoas, "%s\n", pessoas[i].cpf);
        else
          fprintf(arquivoPessoas, "%s", pessoas[i].cpf);
        aux = 1;
      }
      j++;
    }
  }
  for(i=0; i<tamCarros; i++) {
    fprintf(arquivoCarros, "%s\n", carros[i].placa);
    fprintf(arquivoCarros, "%d\n", carros[i].ano);
    fprintf(arquivoCarros, "%s\n", carros[i].modelo);
    if(i < tamPessoas-1)
      fprintf(arquivoCarros, "%s\n", carros[i].cpf);
    else
      fprintf(arquivoCarros, "%s", carros[i].cpf);
  }
  fclose(arquivoPessoas);
  fclose(arquivoCarros);
}

void carregaArquivo() {
  int i=0;
  FILE *arquivoPessoas;
  FILE *arquivoCarros;
  arquivoPessoas = fopen("pessoas.txt", "r");
  arquivoCarros = fopen("carros.txt", "r");
  if(arquivoPessoas != NULL) {
    fscanf(arquivoPessoas, " %d", &tamPessoas);
    pessoas = malloc(tamPessoas * sizeof(Pessoa));
    while(fscanf(arquivoPessoas, " %[^\n]", pessoas[i].nome) > 0) {
      fscanf(arquivoPessoas, " %[^\n]", pessoas[i].cpf);
      i++;
    }
    fclose(arquivoPessoas);
  }
  if(arquivoCarros != NULL) {
    while(fscanf(arquivoCarros, " %[^\n]", carros[tamCarros].placa) > 0) {
      carros = realloc(carros, (tamCarros + 2) * sizeof(Carro));
      fscanf(arquivoCarros, "%d\n", &carros[tamCarros].ano);
      fscanf(arquivoCarros, " %[^\n]", carros[tamCarros].modelo);
      fscanf(arquivoCarros, " %[^\n]", carros[tamCarros].cpf);
      tamCarros++;
    }
    fclose(arquivoCarros);
  }
}

int cpfExiste(char *cpf) {
  int i;
  for(i=0; i<tamPessoas; i++)
    if(strcmp(pessoas[i].cpf, cpf) == 0)
      return 1;
    return 0;
}

int placaExiste(char *placa) {
  int i;
  for(i=0; i<tamCarros; i++) {
    if(strcmp(carros[i].placa, placa) == 0)
      return 1;
    else
      return 0;
  }
}

void cadastraPessoa() {
  Pessoa pessoa;
  pessoas = realloc(pessoas, (tamPessoas+2) * sizeof(Pessoa));
  int verifica;
  printf("\n\n");
  printf("Digite o nome da pessoa: ");
  scanf(" %[^\n]", pessoa.nome);
  do {
    verifica = 0;
    printf("Digite o CPF da pessoa: ");
    scanf(" %[^\n]", pessoa.cpf);
    verifica = cpfExiste(pessoa.cpf);
    if(verifica == 1)
      printf("Pessoa ja cadastrada\n");
    if(strlen(pessoa.cpf) != 11) {
      verifica = 1;
      printf("CPF invalido, digite um CPF com 11 digitos\n");
    }
  } while(verifica == 1);
  pessoas[tamPessoas] = pessoa;
  tamPessoas++;
}

void cadastraCarro() {
  Carro carro;
  carros = realloc(carros, (tamCarros+2) * sizeof(Carro));
  int verifica;
  printf("\n\n");
  do {
    printf("Digite a placa do carro: ");
    scanf(" %[^\n]", carro.placa);
    if(strlen(carro.placa) != 7)
      printf("Placa invalida\n");
    if(placaExiste(carro.placa) == 1)
      printf("Carro ja existente\n");
  } while(strlen(carro.placa) != 7 || placaExiste(carro.placa) == 1);
  do {
    printf("Digite o ano do carro: ");
    scanf(" %d", &carro.ano);
    if(carro.ano > 2021)
      printf("Ano invalido\n");
  } while(carro.ano > 2021);
  printf("Digite o modelo do carro: ");
  scanf(" %[^\n]", carro.modelo);
  do {
    verifica = 1;
    printf("Digite o CPF do proprietario do veiculo: ");
    scanf(" %[^\n]", carro.cpf);
    verifica = cpfExiste(carro.cpf);
    if(verifica == 0)
      printf("Pessoa nao existe\n");
    if(strlen(carro.cpf) != 11) {
      verifica = 0;
      printf("CPF invalido\n");
    }
  } while(verifica == 0);
  carros[tamCarros] = carro;
  tamCarros++;
}

void imprimeDados() {
  int i, j;
  ordenaPessoas();
  ordenaCarros();
  system("cls");
  printf("----- CADASTROS -----\n");
  for(i=0; i<tamPessoas; i++) {
    printf("PESSOA NUMERO %d:\n", (i+1));
    printf("nome: %s\n", pessoas[i].nome);
    printf("cpf: %s\n", pessoas[i].cpf);
    printf("\n\n");
    printf("CARROS DA PESSOA %d:\n", (i+1));
    for(j=0; j<tamCarros; j++) {
      if(strcmp(pessoas[i].cpf, carros[j].cpf) == 0) {
        printf("placa: %s\n", carros[i].placa);
        printf("ano: %d\n", carros[i].ano);
        printf("modelo: %s\n", carros[i].modelo);
      }
    }
    printf("-----------------------\n\n");
  }
  printf("\n\n\n");
}

int main() {
  carros = malloc(sizeof(Carro));
  char opcao;
  carregaArquivo();
  printf("QUANTIDADE DE PESSOAS = %d\n\n", tamPessoas);
  do {
    menuOpcoes();
    scanf(" %c", &opcao);
    switch(opcao) {
      case '1':
        cadastraPessoa();
        break;
      case '2':
        cadastraCarro();
        break;
      case '3':
        imprimeDados();
    }
  } while(opcao != '0');
  escreveArquivo();
  return 0;
}