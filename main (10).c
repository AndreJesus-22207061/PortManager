#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_EMBARCACOES 10
#define NUM_PILHAS 6
#define EIC "ERROR: invalid command\n"
#define EFFR "ERROR: file format is not recognized\n"
#define ECNOF "ERROR: could not open file\n"
#define SOC "SUCCESS: operation concluded\n"
#define MENU1 "+---- MENU\n| move        [-g grua] [-d ponto] [-p pilha] [-D ponto] [-P pilha] [-n numero_de_contentores]\n| show        [-d ponto] [-e embarc]\n| where        [embarc]\n| navigate    [-e embarc] [-d ponto]\n| load        [-e embarc] [-p pilha] [-c contentor:peso]\n| weight    [embarc]\n| save        [filename]\n| help\n| quit\n+----\n"








typedef struct Contentor {
    char codigo[3];
    int peso;
} Contentor;

typedef struct No {
    Contentor *valor;
    struct No* proximo;
} No;

typedef struct Pilha {
    No* topo;
} Pilha;

typedef struct Barco {
    Pilha *pilhas[NUM_PILHAS];
    char matricula[4];
} Barco;

typedef struct Estaleiro {
    Barco *embarcacoes[NUM_EMBARCACOES];
} Estaleiro;

int pilha_vazia(Pilha* pilha);
void empilhar(Pilha* stack, Contentor *valor);
int pilha_vazia(Pilha* pilha);
Estaleiro *lerficheiro(const char* nome_arquivo);
Estaleiro *criarEstaleiro();
Barco *criarBarco();
Pilha *criarPilha();
void funcaoWhere();
void funcaLoad();
void funcaoShow();
void funcaoWeight();
void funcaoNavigate();
void mostraMenu();





int pilha_vazia(Pilha* pilha) {
    return (pilha->topo == NULL);
}

Pilha *criarPilha(){
    Pilha *stack;
    stack = malloc(sizeof(Pilha));
    stack ->topo = NULL;
    return stack;
}

Barco *criarBarco() {
    Barco *boat;
    int i;
    boat = malloc(sizeof(Barco));
    boat->matricula[0] = '\0';
    for (i = 0; i < NUM_PILHAS; i++)
    {
        boat->pilhas[i] = criarPilha();
    }
    return boat;
}

Estaleiro *criarEstaleiro()
{
    Estaleiro *estaleiro;
    int i;
    estaleiro = malloc(sizeof(Estaleiro));
    for (i = 0; i < NUM_EMBARCACOES; i++)
    {
        estaleiro->embarcacoes[i] = criarBarco();
    }
    return estaleiro;
}

void empilhar(Pilha* stack, Contentor *c) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->valor = c;
    novoNo->proximo = stack->topo;
    stack->topo = novoNo;
}

int comecaCom(const char *str, const char *pre)
{
    size_t letra; 
    size_t linha;
    letra = strlen(pre), linha = strlen(str);
    if(linha < letra){
    return 0;
      
    }else {
    return strncmp(pre, str, letra) == 0;
  }
}


int numeroDeDigitos(int num)
{
    int digitos = 0;
    if (num < 10) return 1;
    while (num != 0)
    {
        num /= 10;
        digitos++;
    }
    return digitos;
}

Estaleiro *lerficheiro(const char* nome_arquivo) {
    Estaleiro *estaleiro;
    FILE* arquivo = fopen(nome_arquivo, "r");
    char *matricula, linha[100], str_contentores[100];
    int pontoAtracagem; 
    int numPilha; 
    int numContentores;
    Pilha *pilha;
    int i;

    estaleiro = criarEstaleiro();
    if (arquivo == NULL) {
        printf("%s", ECNOF);
        exit(1);
    }
    matricula = (char*) malloc(sizeof(char) * 5);
    if (matricula == NULL) {
        exit(1);
    }
    while (fgets(linha, 100, arquivo) != NULL) {
       if (comecaCom(linha, "d")) {
            if (sscanf(linha, "d%d %4[^\n]\n", &pontoAtracagem, matricula) == 2) {
                if (pontoAtracagem < 0 || pontoAtracagem >= NUM_EMBARCACOES) {
                   exit(1);
                }

                strcpy(estaleiro->embarcacoes[pontoAtracagem]->matricula, matricula);
            } else {
                printf("1%s", EFFR);
                exit(1);
            }
        } else if (comecaCom(linha, "\tp")) {
            int sscanf_res = sscanf(linha, "\tp%d %d %4[^\n]", &numPilha, &numContentores, str_contentores);
            if (sscanf_res < 2 || numPilha < 0 || numPilha >= NUM_PILHAS || ((numPilha >= 0 && numPilha < NUM_PILHAS) && strlen(str_contentores) == 0)) {
                
                printf("1%s", EFFR);
                exit(1);
            }
            pilha = estaleiro->embarcacoes[pontoAtracagem]->pilhas[numPilha];
            i = 0;

            if (numContentores < 1)
              continue;
            i += 4 + numeroDeDigitos(numPilha) + numeroDeDigitos(numContentores);
            while (numContentores > 0 && (linha+i)[0] != '\0') {
                Contentor *contentor = malloc(sizeof(Contentor));
                if ((linha+i)[0] == ' ')
                    i++;
                if ((linha+i)[0] == '\n')
                    break;
                sscanf(linha+i, "%4[^:]:%d", contentor->codigo, &contentor->peso);
                i += 1 + strlen(contentor->codigo) + numeroDeDigitos(contentor->peso);
                empilhar(pilha, contentor);
                numPilha--;
            }
       }else {
            if (strlen(linha) > 1) {  
                printf("%s",EFFR);
                exit(1);
            }
       }
    }

    fclose(arquivo);

    return estaleiro;
}


int contentorExiste(Barco* barco, const char* codigo) {
  Pilha* pilha;

  
    if (barco == NULL) {
        return 0;
    }

    for (int i = 0; i < NUM_PILHAS; i++) {
        pilha = barco->pilhas[i];
        if (pilha != NULL) {
            No* atual = pilha->topo;
            while (atual != NULL) {
                if (strcmp(atual->valor->codigo, codigo) == 0 && atual->valor != NULL) {
                    return 1;
                }
                atual = atual->proximo;
            }
        }
    }
    return 0;
}



void funcaoLoad(Estaleiro* estaleiro, const char* comando) {
    char matricula[5];
    Contentor* contentor;
    int numPilha; 
    int peso;
    char codigo[4];
    int i;
    Pilha* pilha;

    int result = sscanf(comando, "load -p %d -c %3[^:]:%d -e %4s", &numPilha, codigo, &peso, matricula);
    if (result != 4) {
        result = sscanf(comando, "load -c %3[^:]:%d -p %d -e %4s", codigo, &peso, &numPilha, matricula);
    }
    if (result == 4) {
        int encontrouEmbarcacao = 0;

        if (estaleiro != NULL) {
            for (i = 0; i < NUM_EMBARCACOES; i++) {
                if (estaleiro->embarcacoes[i] != NULL && strcmp(estaleiro->embarcacoes[i]->matricula, matricula) == 0) {
                    encontrouEmbarcacao = 1;
                    break;
                }
            }
        } else {
            return;
        }

        if (encontrouEmbarcacao) {
            if (numPilha >= 0 && numPilha < NUM_PILHAS) {
                if (estaleiro->embarcacoes[i] != NULL) {
                    pilha = estaleiro->embarcacoes[i]->pilhas[numPilha];
                    if (pilha == NULL) {
                        pilha = criarPilha(); 
                        estaleiro->embarcacoes[i]->pilhas[numPilha] = pilha;
                    }
                    if (pilha != NULL) {
                        No* atual = pilha->topo;
                        while (atual != NULL) {
                            if (atual->valor != NULL && strcmp(atual->valor->codigo, codigo) == 0) {
                                printf("%s", EIC);
                                return;
                            }
                            atual = atual->proximo;
                        }
                        contentor = malloc(sizeof(Contentor));
                        if (contentor != NULL) {
                            strcpy(contentor->codigo, codigo);
                            contentor->peso = peso;
                            if (contentorExiste(estaleiro->embarcacoes[i], codigo)) {
                                printf("%s", EIC);
                                free(contentor);
                            } else {
                                empilhar(pilha, contentor);
                                printf("%s", SOC);
                            }
                            return;
                        } else {
                            printf("%s", EIC);
                            return;
                        }
                    } else {
                        printf("%s", EIC);
                        return;
                    }
                } else {
                    printf("%s", EIC);
                    return;
                }
            } else {
                printf("%s", EIC);
                return;
            }
        } else {
            printf("%s", EIC);
            return;
        }
    } else {
        printf("%s", EIC);
        return;
    }
}









void funcaoWhere(Estaleiro* estaleiro, const char* comando) {
    char matricula[5];
    int comandoUser = sscanf(comando, "where %4s", matricula);
    int encontrouBarco = 0;
    int i;
    if (comandoUser == 1) {
        for (i = 0; i < NUM_EMBARCACOES; i++) {
            if (strcmp(estaleiro->embarcacoes[i]->matricula, matricula) == 0) {
                encontrouBarco = 1;
                break;
            }
        }
        if (encontrouBarco) {
            printf("%d %s\n", i, estaleiro->embarcacoes[i]->matricula);
        } else {
            printf("%s", EIC);
        }
    } else {
        printf("%s", EIC);
    }
}





void mostraMenu() {
    printf("%s", MENU1);
   
}



void exibirContentoresInvertido(Pilha* pilha) {
  int numContentores = 0;
  int index;
  int i;
  No* atual;
  Contentor** contentores;
  
    if (pilha->topo == NULL) {
        return;
    }

    atual = pilha->topo;
    while (atual != NULL) {
        numContentores++;
        atual = atual->proximo;
    }

    contentores = (Contentor**)malloc(numContentores * sizeof(Contentor*));
    atual = pilha->topo;
    index = numContentores - 1;
    while (atual != NULL) {
        contentores[index] = atual->valor;
        atual = atual->proximo;
        index--;
    }

    for (i = 0; i < numContentores; i++) {
        printf("%s:%d ", contentores[i]->codigo, contentores[i]->peso);
    }

    free(contentores); 
}

int contarContentores(Pilha* pilha) {
    int contador = 0;
    if (pilha != NULL && pilha->topo != NULL) {
        No* atual = pilha->topo;
        while (atual != NULL) {
            contador++;
            atual = atual->proximo;
        }
    }
    return contador;
}


void funcaoShow(Estaleiro* estaleiro, const char* comando) {
    char ponto[3];
    char embarcacao[5];
    int comandoUser = sscanf(comando, "show -d %2s", ponto);
    int encontrouBarco = 0;
    int i;
    int pontoAtracagem = atoi(ponto);
    int contagem;

    if (strstr(comando, "-d") != NULL && strstr(comando, "-e") != NULL) {
        printf("%s", EIC);
        return;
    }

    if (comandoUser == 1) {
        if (strlen(comando) != 9) {
            printf("%s", EIC);
            return;
        }
        if (pontoAtracagem >= 0 && pontoAtracagem < NUM_EMBARCACOES) {
            Barco* barco = estaleiro->embarcacoes[pontoAtracagem];
            if (barco != NULL && barco->matricula[0] != '\0') {
                printf("d%d %s ", pontoAtracagem, barco->matricula);
                for (int j = 0; j < NUM_PILHAS; j++) {
                    Pilha* pilha = barco->pilhas[j];
                    if (pilha != NULL && pilha->topo != NULL) {
                        printf("p%d ", j);
                        contagem = contarContentores(pilha); 
                        printf("%d ", contagem);
                        exibirContentoresInvertido(pilha);
                    }
                }
                printf("\n");
            } else {
                printf("%s", EIC);
            }
        } else {
            printf("%s", EIC);
        }
    } else if (sscanf(comando, "show -e %4s", embarcacao) == 1) {
        for (i = 0; i < NUM_EMBARCACOES; i++) {
            if (estaleiro->embarcacoes[i] != NULL && strcmp(estaleiro->embarcacoes[i]->matricula, embarcacao) == 0) {
                encontrouBarco = 1;
                break;
            }
        }
        if (encontrouBarco) {
            Barco* barco = estaleiro->embarcacoes[i];
            printf("d%d %s ", i, barco->matricula);
            for (int j = 0; j < NUM_PILHAS; j++) {
                Pilha* pilha = barco->pilhas[j];
                if (pilha != NULL && pilha->topo != NULL) {
                    printf("p%d ", j);
                    contagem = contarContentores(pilha); 
                    printf("%d ", contagem);
                    exibirContentoresInvertido(pilha);
                }
            }
            printf("\n");
        } else {
            printf("%s", EIC);
        }
    } else if (strcmp(comando, "show") == 0) {
        for (int i = 0; i < NUM_EMBARCACOES; i++) {
            Barco* barco = estaleiro->embarcacoes[i];
            if (barco != NULL && barco->matricula[0] != '\0') {
                printf("d%d %s ", i, barco->matricula);
                for (int j = 0; j < NUM_PILHAS; j++) {
                    Pilha* pilha = barco->pilhas[j];
                    if (pilha != NULL && pilha->topo != NULL) {
                        printf("p%d ", j);
                        contagem = contarContentores(pilha); 
                        printf("%d ", contagem);
                        exibirContentoresInvertido(pilha);
                    }
                }
                printf("\n");
            }
        }
    } else {
        printf("%s", EIC);
    }
}





void funcaoNavigate(Estaleiro* estaleiro, const char* comando) {
    char matricula[5];
    int pontoAtracagemDestino;
    int comandoUser = sscanf(comando, "navigate -d %d -e %s", &pontoAtracagemDestino, matricula);
    int encontrouBarco = 0;
    int pontoDeAtracagemAtual = -1;
    int i;
    Barco* novaEmbarcacao;

    if (comandoUser != 2) {
        comandoUser = sscanf(comando, "navigate -e %s -d %d", matricula, &pontoAtracagemDestino);
        if (comandoUser != 2) {
            printf("%s", EIC);
            return;
        }
    } 
  if (strlen(matricula) != 4) {
        printf("%s", EIC);
        return;
    }

    for (i = 0; i < 4; i++) {
        if (matricula[i] < 'A' || matricula[i] > 'Z') {
            printf("%s", EIC);
            return;
        }
    }

    if (pontoAtracagemDestino < 0 || pontoAtracagemDestino >= NUM_EMBARCACOES) {
        printf("%s", EIC);
        return;
    }

    if (strcmp(estaleiro->embarcacoes[pontoAtracagemDestino]->matricula, "") != 0) {
        printf("%s", EIC);
        return;
    }

    for (int aux = 0; aux < NUM_EMBARCACOES; aux++) {
        if (estaleiro->embarcacoes[aux] != NULL && strcmp(estaleiro->embarcacoes[aux]->matricula, matricula) == 0) {
            encontrouBarco = 1;
            pontoDeAtracagemAtual = aux;
            break;
        }
    }

    if (encontrouBarco) {
        if (pontoAtracagemDestino == pontoDeAtracagemAtual) {
            printf("%s", EIC);
        } else {
            strcpy(estaleiro->embarcacoes[pontoAtracagemDestino]->matricula, matricula);
            estaleiro->embarcacoes[pontoAtracagemDestino] = estaleiro->embarcacoes[pontoDeAtracagemAtual];
            estaleiro->embarcacoes[pontoDeAtracagemAtual] = NULL;

            printf("%s", SOC);
        }
    } else {
        if (strlen(matricula) != 4) {
            printf("%s", EIC);
            return;
        }

        novaEmbarcacao = malloc(sizeof(Barco));
        strcpy(novaEmbarcacao->matricula, matricula);
        for (i = 0; i < NUM_PILHAS; i++) {
            novaEmbarcacao->pilhas[i] = NULL;
        }
        estaleiro->embarcacoes[pontoAtracagemDestino] = novaEmbarcacao;

        printf("%s", SOC);
    }
}


void funcaoWeight(Estaleiro* estaleiro, const char* comando) {
    char matricula[5];
    int comandoUser = sscanf(comando, "weight %4s", matricula);
    int encontrouEmbarcacao = 0; 
    int pesoTotal = 0;
    int i;
    
    
    if (estaleiro == NULL) {
        printf("%s", EIC);
        return;
    }

    
    if (comandoUser != 1) {
        printf("%s", EIC);
        return;
    }

    for (i = 0; i < NUM_EMBARCACOES; i++) {
        if (strcmp(estaleiro->embarcacoes[i]->matricula, matricula) == 0 && estaleiro->embarcacoes[i] != NULL) {
            encontrouEmbarcacao = 1;
            break;
        }
    }

    if (encontrouEmbarcacao) {
        for (int j = 0; j < NUM_PILHAS; j++) {
            Pilha* pilha = estaleiro->embarcacoes[i]->pilhas[j];
            if (pilha != NULL) {
                No* atual = pilha->topo;
                while (atual != NULL) {
                    pesoTotal += atual->valor->peso;
                    atual = atual->proximo;
                }
            }
        }
        printf("%s %d\n", estaleiro->embarcacoes[i]->matricula, pesoTotal);
    } else {
        printf("%s", EIC);
    }
}




int main(int arg1, char* arg2[]) {
    Estaleiro* estaleiro = NULL;
    char* comando = NULL;
    if(arg1>2){
       estaleiro = lerficheiro(arg2[1]);
    }else{
      estaleiro = criarEstaleiro();
    }

  if(estaleiro == NULL){
   return 1;
  }

  comando = malloc(sizeof(char)* 1000);
  
    mostraMenu();
    while (1) {
        printf(">");
        if(fgets(comando, 1000, stdin) == NULL){
          break;
        }
        
    
        comando[strcspn(comando, "\n")] = '\0';
        
        if (strcmp(comando, "quit") == 0) {
            break;
        }else if (comecaCom(comando, "load ")) {
          funcaoLoad(estaleiro,comando);
          continue;
        }else if (strcmp(comando, "help") ==0){
          mostraMenu();
        }else if(comecaCom(comando,"where")){
         if(estaleiro != NULL){
           funcaoWhere(estaleiro, comando); 
         }
        }else if(comecaCom(comando , "navigate")){
          if(estaleiro != NULL){
          funcaoNavigate(estaleiro, comando);
          }
        }else if(comecaCom(comando, "show")){
          if(estaleiro != NULL){
            funcaoShow(estaleiro, comando);
          }
        }else if(comecaCom(comando,"weight")){
          if(estaleiro != NULL){
          funcaoWeight(estaleiro, comando);
          }
        }else{
          printf("%s", EIC);
        }
  }  
    return 0; 
}
