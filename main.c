#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas para armazenar dados dos pacientes
typedef struct Data {
  int dia;
  int mes;
  int ano;
} Data;

typedef struct Registro {
  char nome[50];
  int idade;
  char rg[20];
  Data *entrada;
} Registro;

// Estruturas para a lista encadeada (cadastro de pacientes)
typedef struct Elista {
  Registro dados;
  struct Elista *proximo;
} Elista;
// Estruturas para a lista duplamente encadeada (cadastro de consultas)
typedef struct {
  Elista *inicio;
  int qtde;
} Lista;

// Estruturas para a fila (atendimento)
typedef struct Efila {
  Registro dados;
  struct Efila *proximo;
} Efila;

typedef struct {
  Efila *head;
  Efila *tail;
  int qtde;
} Fila;

// Estruturas para a árvore binária (pesquisa)
typedef struct EABB {
  Registro dados;
  struct EABB *filhoEsq;
  struct EABB *filhoDir;
} EABB;

typedef struct {
  EABB *raiz;
  int qtde;
} ABB;

// Estruturas para a pilha (desfazer operações)
typedef struct Eoperacao {
  Registro dados;
  struct Eoperacao *proximo;
} Eoperacao;

typedef struct {
  Eoperacao *topo;
} Pilha;

// Funções de inicialização
void inicializarLista(Lista *lista) { lista->inicio = NULL; }

void inicializarFila(Fila *fila) {
  fila->head = NULL;
  fila->tail = NULL;
  fila->qtde = 0;
}

void inicializarABB(ABB *arvore) {
  arvore->raiz = NULL;
  arvore->qtde = 0;
}

void inicializarPilha(Pilha *pilha) { pilha->topo = NULL; }

// Funções para cadastrar, enfileirar e mostrar pacientes
void cadastrarPaciente(Lista *lista);
void consultarPaciente(Lista *lista);
void atualizarPaciente(Lista *lista);
void removerPaciente(Lista *lista);
void mostrarLista(Lista *lista);
void submenuCadastro(Lista *lista);

// Funções para enfileirar pacientes, desenfileirar pacientes e mostrar fila
void enfileirarPaciente(Fila *fila, Lista *lista, Pilha *pilha);
void desenfileirarPaciente(Fila *fila);
void mostrarFila(Fila *fila);
void submenuAtendimento(Fila *fila, Lista *lista, Pilha *pilha);

// Funções para pesquisar pacientes na lista
EABB *inserirNaABB(EABB *raiz, Registro paciente, int *qtde);
void coletarDadosABB(EABB *raiz, Registro *array, int *index);
void submenuPesquisa(ABB *arvore, Lista *lista);

// Funções para ordenação e exibição
void exibirRegistrosOrdenados(Registro *array, int tamanho, int criterio);

// Funções de comparação
int compararAno(const void *a, const void *b);
int compararMes(const void *a, const void *b);
int compararDia(const void *a, const void *b);
int compararIdade(const void *a, const void *b);

// Funções para a pilha
void push(Pilha *pilha, Registro dados);
Registro pop(Pilha *pilha);
void desfazerOperacao(Fila *fila, Pilha *pilha);

// Funções para carregar e salvar arquivos
void carregarDados(Lista *lista);
void salvarDados(Lista *lista);
void sobre();

// Funções para liberar memória
void liberarABB(EABB *raiz);
void liberarLista(Lista *lista);
void liberarFila(Fila *fila);
void liberarPilha(Pilha *pilha);

// Função principal
int main() {
  // Declaração de variáveis
  Lista lista;
  inicializarLista(&lista);

  Fila fila;
  inicializarFila(&fila);

  ABB arvore;
  inicializarABB(&arvore);

  Pilha pilha;
  inicializarPilha(&pilha);

  int opcao;
  do {
    printf("\n1. Cadastrar paciente\n");
    printf("2. Atendimento\n");
    printf("3. Pesquisa\n");
    printf("4. Desfazer última operação\n");
    printf("5. Carregar ou salvar\n");
    printf("6. Sobre\n");
    printf("7. Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    getchar(); // Limpar o buffer

    switch (opcao) {
    case 1:
      submenuCadastro(&lista);
      break;
    case 2:
      submenuAtendimento(&fila, &lista, &pilha);
      break;
    case 3:
      submenuPesquisa(&arvore, &lista);
      break;
    case 4:
      desfazerOperacao(&fila, &pilha);
      break;
    case 5:
      // Submenu de carregar ou salvar
      {
        int opcao_carregar_salvar;
        do {
          printf("\n--- Carregar ou Salvar ---\n");
          printf("1. Carregar dados dos pacientes\n");
          printf("2. Salvar dados dos pacientes\n");
          printf("3. Voltar ao menu principal\n");
          printf("Escolha uma opção: ");
          scanf("%d", &opcao_carregar_salvar);
          getchar();

          switch (opcao_carregar_salvar) {
          case 1:
            carregarDados(&lista);
            break;
          case 2:
            salvarDados(&lista);
            break;
          case 3:
            printf("Voltando ao menu principal...\n");
            break;
          default:
            printf("Opção inválida!\n");
          }
        } while (opcao_carregar_salvar != 3);
      }
      break;
    case 6:
      sobre();
      break;
    case 7:
      printf("Encerrando o programa.\n");
      break;
    default:
      printf("Opção inválida!\n");
    }
  } while (opcao != 7);

  // Liberar memória antes de encerrar
  liberarABB(arvore.raiz);
  liberarLista(&lista);
  liberarFila(&fila);
  liberarPilha(&pilha);

  return 0;
}

// Implementação das funções

// Função para cadastrar um novo paciente na lista
void cadastrarPaciente(Lista *lista) {
  // Criar um novo registro de paciente
  Elista *novo = (Elista *)malloc(sizeof(Elista));
  if (!novo) {
    printf("Erro de alocação de memória!\n");
    return;
  }

  // Coletando informações do paciente
  printf("Digite o nome do paciente: ");
  fgets(novo->dados.nome, 50, stdin);
  novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';

  printf("Digite a idade do paciente: ");
  scanf("%d", &novo->dados.idade);
  getchar();

  printf("Digite o RG do paciente: ");
  fgets(novo->dados.rg, 20, stdin);
  novo->dados.rg[strcspn(novo->dados.rg, "\n")] = '\0';

  // Alocar memória para a data de entrada
  novo->dados.entrada = (Data *)malloc(sizeof(Data));
  if (!novo->dados.entrada) {
    printf("Erro de alocação de memória!\n");
    free(novo);
    return;
  }

  printf("Digite a data de entrada (dia mes ano): ");
  scanf("%d %d %d", &novo->dados.entrada->dia, &novo->dados.entrada->mes,
        &novo->dados.entrada->ano);
  getchar();

  // Inserindo no início da lista
  novo->proximo = lista->inicio;
  lista->inicio = novo;
  lista->qtde++;
  printf("Paciente cadastrado com sucesso!\n");
}

// Função para consultar um paciente cadastrado
void consultarPaciente(Lista *lista) {
  if (lista->inicio == NULL) {
    printf("Não há pacientes cadastrados.\n");
    return;
  }

  char nomeBusca[50];
  printf("Digite o nome do paciente que deseja consultar: ");
  fgets(nomeBusca, 50, stdin);
  nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

  // Percorrer a lista para encontrar o paciente
  Elista *atual = lista->inicio;
  while (atual != NULL) {
    if (strcmp(atual->dados.nome, nomeBusca) == 0) {
      printf("Paciente encontrado:\n");
      printf("Nome: %s\n", atual->dados.nome);
      printf("Idade: %d\n", atual->dados.idade);
      printf("RG: %s\n", atual->dados.rg);
      printf("Data de Entrada: %02d/%02d/%04d\n", atual->dados.entrada->dia,
             atual->dados.entrada->mes, atual->dados.entrada->ano);
      return;
    }
    // Percorrer a lista
    atual = atual->proximo;
  }
  printf("Paciente não encontrado.\n");
}

// Função para atualizar os dados de um paciente
void atualizarPaciente(Lista *lista) {
  if (lista->inicio == NULL) {
    printf("Não há pacientes cadastrados.\n");
    return;
  }

  char nomeBusca[50];
  printf("Digite o nome do paciente que deseja atualizar: ");
  fgets(nomeBusca, 50, stdin);
  nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
  // Percorrer a lista para encontrar o paciente
  Elista *atual = lista->inicio;
  while (atual != NULL) {
    if (strcmp(atual->dados.nome, nomeBusca) == 0) {
      printf("Paciente encontrado. Digite os novos dados.\n");

      printf("Digite o novo nome do paciente: ");
      fgets(atual->dados.nome, 50, stdin);
      atual->dados.nome[strcspn(atual->dados.nome, "\n")] = '\0';

      printf("Digite a nova idade do paciente: ");
      scanf("%d", &atual->dados.idade);
      getchar();

      printf("Digite o novo RG do paciente: ");
      fgets(atual->dados.rg, 20, stdin);
      atual->dados.rg[strcspn(atual->dados.rg, "\n")] = '\0';

      printf("Digite a nova data de entrada (dia mes ano): ");
      scanf("%d %d %d", &atual->dados.entrada->dia, &atual->dados.entrada->mes,
            &atual->dados.entrada->ano);
      getchar();

      printf("Dados atualizados com sucesso!\n");
      return;
    }
    atual = atual->proximo;
  }
  printf("Paciente não encontrado.\n");
}

// Função para remover um paciente da lista
void removerPaciente(Lista *lista) {
  if (lista->inicio == NULL) {
    printf("Não há pacientes cadastrados.\n");
    return;
  }

  char nomeBusca[50];
  printf("Digite o nome do paciente que deseja remover: ");
  fgets(nomeBusca, 50, stdin);
  nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
  // Percorrer a lista para encontrar o paciente
  Elista *atual = lista->inicio;
  Elista *anterior = NULL;

  while (atual != NULL) {
    if (strcmp(atual->dados.nome, nomeBusca) == 0) {
      // Remover o nó do início da lista
      if (anterior == NULL) {
        lista->inicio = atual->proximo;
      } else {
        // Remover no meio ou no final da lista
        anterior->proximo = atual->proximo;
      }
      // Liberar memória
      free(atual->dados.entrada);
      free(atual);

      printf("Paciente removido com sucesso!\n");
      return;
    }
    anterior = atual;
    atual = atual->proximo;
  }
  printf("Paciente não encontrado.\n");
}

// Função para mostrar a lista de pacientes
void mostrarLista(Lista *lista) {
  if (lista->inicio == NULL) {
    printf("Não há pacientes cadastrados.\n");
    return;
  }

  Elista *atual = lista->inicio;
  printf("\n--- Lista de Pacientes ---\n");
  while (atual != NULL) {
    printf("Nome: %s, Idade: %d, RG: %s, Data de Entrada: %02d/%02d/%04d\n",
           atual->dados.nome, atual->dados.idade, atual->dados.rg,
           atual->dados.entrada->dia, atual->dados.entrada->mes,
           atual->dados.entrada->ano);
    atual = atual->proximo;
  }
  printf("---------------------------\n");
}
// Submenu de cadastro de pacientes
void submenuCadastro(Lista *lista) {
  int opcao_cadastro;
  do {
    printf("\n--- Cadastro de Pacientes ---\n");
    printf("1. Cadastrar novo paciente\n");
    printf("2. Consultar paciente cadastrado\n");
    printf("3. Mostrar lista completa\n");
    printf("4. Atualizar dados de paciente\n");
    printf("5. Remover paciente\n");
    printf("6. Voltar ao menu principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao_cadastro);
    getchar();

    switch (opcao_cadastro) {
    case 1:
      cadastrarPaciente(lista);
      break;
    case 2:
      consultarPaciente(lista);
      break;
    case 3:
      mostrarLista(lista);
      break;
    case 4:
      atualizarPaciente(lista);
      break;
    case 5:
      removerPaciente(lista);
      break;
    case 6:
      printf("Voltando ao menu principal...\n");
      break;
    default:
      printf("Opção inválida!\n");
    }
  } while (opcao_cadastro != 6);
}

// Função para enfileirar um paciente na fila
void enfileirarPaciente(Fila *fila, Lista *lista, Pilha *pilha) {
  if (lista->inicio == NULL) {
    printf("A lista está vazia. Cadastre um paciente primeiro.\n");
    return;
  }

  // Percorrer a lista de pacientes
  Elista *pacienteAtual = lista->inicio;
  while (pacienteAtual != NULL) {
    // Verificar se o paciente já está na fila
    int jaEnfileirado = 0;
    Efila *filaAtual = fila->head;
    while (filaAtual != NULL) {
      if (strcmp(filaAtual->dados.rg, pacienteAtual->dados.rg) == 0) {
        jaEnfileirado = 1;
        break;
      }
      filaAtual = filaAtual->proximo;
    }

    if (!jaEnfileirado) {
      // Paciente não está na fila, enfileirar
      Efila *novo = (Efila *)malloc(sizeof(Efila));
      if (!novo) {
        printf("Erro de alocação de memória!\n");
        return;
      }

      // Copiar os dados do paciente
      novo->dados = pacienteAtual->dados;

      // Cópia profunda da data
      novo->dados.entrada = (Data *)malloc(sizeof(Data));
      if (!novo->dados.entrada) {
        printf("Erro de alocação de memória!\n");
        free(novo);
        return;
      }
      *(novo->dados.entrada) = *(pacienteAtual->dados.entrada);

      novo->proximo = NULL;

      // Inserir na fila
      if (fila->head == NULL) {
        fila->head = novo;
        fila->tail = novo;
      } else {
        fila->tail->proximo = novo;
        fila->tail = novo;
      }

      // Empilhar a operação realizada
      push(pilha, novo->dados);

      printf("Paciente %s enfileirado com sucesso!\n", novo->dados.nome);
      return;
    }

    // Avançar para o próximo paciente
    pacienteAtual = pacienteAtual->proximo;
  }

  printf("Todos os pacientes já foram enfileirados.\n");
}

// Função para desenfileirar um paciente
void desenfileirarPaciente(Fila *fila) {
  if (fila->head == NULL) {
    printf("A fila está vazia.\n");
    return;
  }
  // Removendo o primeiro paciente da fila
  Efila *removido = fila->head;
  printf("Paciente %s atendido e removido da fila.\n", removido->dados.nome);
  // Atualizando o início da fila
  fila->head = fila->head->proximo;
  // Liberando memória(FILA ACABOU)
  if (fila->head == NULL) {
    fila->tail = NULL;
  }

  // Liberar memória
  free(removido->dados.entrada);
  free(removido);

  fila->qtde--; // Decrementa a quantidade de elementos na fila
  printf("Paciente removido.");
}
void mostrarFila(Fila *fila) {
  if (fila->head == NULL) {
    printf("A fila está vazia.\n");
    return;
  }
  // Mostrando a fila
  Efila *atual = fila->head;
  printf("\n--- Fila de Atendimento ---\n");
  while (atual != NULL) {
    printf("Nome: %s, Idade: %d, RG: %s, Data de Entrada: %02d/%02d/%04d\n",
           atual->dados.nome, atual->dados.idade, atual->dados.rg,
           atual->dados.entrada->dia, atual->dados.entrada->mes,
           atual->dados.entrada->ano);
    atual = atual->proximo;
  }
  printf("---------------------------\n");
}

// Submenu de atendimento
void submenuAtendimento(Fila *fila, Lista *lista, Pilha *pilha) {
  int opcao_atendimento;
  do {
    printf("\n--- Menu de Atendimento ---\n");
    printf("1. Enfileirar paciente\n");
    printf("2. Desenfileirar paciente\n");
    printf("3. Mostrar fila\n");
    printf("4. Voltar ao menu principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao_atendimento);
    getchar();

    switch (opcao_atendimento) {
    case 1:
      enfileirarPaciente(fila, lista, pilha);
      break;
    case 2:
      desenfileirarPaciente(fila);
      break;
    case 3:
      mostrarFila(fila);
      break;
    case 4:
      printf("Voltando ao menu principal...\n");
      break;
    default:
      printf("Opção inválida!\n");
    }
  } while (opcao_atendimento != 4);
}

// Função para inserir na ABB (sem filtro)
EABB *inserirNaABB(EABB *raiz, Registro paciente, int *qtde) {
  if (raiz == NULL) {
    // Alocar memória para um novo nó
    EABB *novo = (EABB *)malloc(sizeof(EABB));
    if (!novo) {
      printf("Erro de alocação de memória!\n");
      return NULL;
    }

    // Inicializar o novo nó
    novo->dados = paciente;

    // Cópia profunda para o campo entrada
    novo->dados.entrada = (Data *)malloc(sizeof(Data));
    if (!novo->dados.entrada) {
      free(novo);
      printf("Erro de alocação de memória!\n");
      return NULL;
    }
    // Cópia profunda para o campo entrada
    *(novo->dados.entrada) = *(paciente.entrada);
    // Inicializar os ponteiros esquerda e direita como NULL
    novo->filhoEsq = NULL;
    novo->filhoDir = NULL;

    // Incrementar a quantidade de nós
    (*qtde)++;

    return novo;
  }

  // Critério de inserção
  if (strcmp(paciente.nome, raiz->dados.nome) < 0) {
    // Inserir na subárvore esquerda se for menor que 0
    raiz->filhoEsq = inserirNaABB(raiz->filhoEsq, paciente, qtde);
  } else {
    // Inserir na subárvore direita se for maior que 0
    raiz->filhoDir = inserirNaABB(raiz->filhoDir, paciente, qtde);
  }

  return raiz;
}

// Função para coletar dados da ABB
void coletarDadosABB(EABB *raiz, Registro *array, int *index) {
  // Verificar se a raiz é válida
  if (raiz != NULL) {
    coletarDadosABB(raiz->filhoEsq, array, index);
    // Copiar os dados da raiz para o array
    if (array != NULL) {
      array[*index] = raiz->dados;
    }
    (*index)++; // Incrementar o índice
    coletarDadosABB(raiz->filhoDir, array, index);
  }
}

// Função para inserir pacientes na ABB
void inserirPacientesABB(ABB *arvore, Lista *lista) {
  Elista *atual = lista->inicio; // Declare a variável
  while (atual != NULL) {
    arvore->raiz = inserirNaABB(arvore->raiz, atual->dados,
                                &arvore->qtde); // Insere na ABB
    atual = atual->proximo;                     // Avança para o próximo nó
  }
  printf("Pacientes inseridos na árvore binária. Total de nós: %d\n",
         arvore->qtde);
}

int buscarNaABB(EABB *raiz, char *nome) {
  // Verificar se a raiz é válida
  if (raiz == NULL)
    return 0;
  if (strcmp(raiz->dados.nome, nome) == 0)
    return 1; // Registro encontrado
  if (strcmp(nome, raiz->dados.nome) < 0)
    return buscarNaABB(raiz->filhoEsq, nome);
  return buscarNaABB(raiz->filhoDir, nome);
}

// Submenu de pesquisa
void submenuPesquisa(ABB *arvore, Lista *lista) {
  int opcao_pesquisa;
  do {
    printf("\n--- Menu de Pesquisa ---\n");
    printf("1. Inserir pacientes na árvore\n");
    printf("2. Mostrar registros ordenados por ano\n");
    printf("3. Mostrar registros ordenados por mês\n");
    printf("4. Mostrar registros ordenados por dia\n");
    printf("5. Mostrar registros ordenados por idade\n");
    printf("6. Voltar ao menu principal\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao_pesquisa);
    getchar();

    switch (opcao_pesquisa) {
    case 1:
      inserirPacientesABB(arvore, lista);
      break;

    case 2:
      // Coletar dados
      {
        int tamanho = 0;
        coletarDadosABB(arvore->raiz, NULL, &tamanho);
        if (tamanho == 0) {
          printf("Nenhum registro para exibir.\n");
          break;
        }
        Registro *array = (Registro *)malloc(tamanho * sizeof(Registro));
        int index = 0;
        coletarDadosABB(arvore->raiz, array, &index);
        // Ordenar por ano
        qsort(array, tamanho, sizeof(Registro), compararAno);
        // Exibir registros
        exibirRegistrosOrdenados(array, tamanho, 1);
        free(array);
      }
      break;
    case 3:
      // Coletar dados
      {
        int tamanho = 0;
        coletarDadosABB(arvore->raiz, NULL, &tamanho);
        if (tamanho == 0) {
          printf("Nenhum registro para exibir.\n");
          break;
        }
        Registro *array = (Registro *)malloc(tamanho * sizeof(Registro));
        int index = 0;
        coletarDadosABB(arvore->raiz, array, &index);
        // Ordenar por mês
        qsort(array, tamanho, sizeof(Registro), compararMes);
        // Exibir registros
        exibirRegistrosOrdenados(array, tamanho, 2);
        free(array);
      }
      break;
    case 4:
      // Coletar dados
      {
        int tamanho = 0;
        coletarDadosABB(arvore->raiz, NULL, &tamanho);
        if (tamanho == 0) {
          printf("Nenhum registro para exibir.\n");
          break;
        }
        Registro *array = (Registro *)malloc(tamanho * sizeof(Registro));
        int index = 0;
        coletarDadosABB(arvore->raiz, array, &index);
        // Ordenar por dia
        qsort(array, tamanho, sizeof(Registro), compararDia);
        // Exibir registros
        exibirRegistrosOrdenados(array, tamanho, 3);
        free(array);
      }
      break;
    case 5:
      // Coletar dados
      {
        int tamanho = 0;
        coletarDadosABB(arvore->raiz, NULL, &tamanho);
        if (tamanho == 0) {
          printf("Nenhum registro para exibir.\n");
          break;
        }
        Registro *array = (Registro *)malloc(tamanho * sizeof(Registro));
        int index = 0;
        coletarDadosABB(arvore->raiz, array, &index);
        // Ordenar por idade
        qsort(array, tamanho, sizeof(Registro), compararIdade);
        // Exibir registros
        exibirRegistrosOrdenados(array, tamanho, 4);
        free(array);
      }
      break;
    case 6:
      printf("Voltando ao menu principal...\n");
      break;
    default:
      printf("Opção inválida!\n");
    }
  } while (opcao_pesquisa != 6);
}

// Função para exibir registros ordenados
void exibirRegistrosOrdenados(Registro *array, int tamanho, int criterio) {
  if (tamanho == 0) {
    printf("Nenhum registro para exibir.\n");
    return;
  }
  printf("\n--- Registros Ordenados ---\n");
  for (int i = 0; i < tamanho; i++) {
    printf("Nome: %s, Idade: %d, Data: %02d/%02d/%04d\n", array[i].nome,
           array[i].idade, array[i].entrada->dia, array[i].entrada->mes,
           array[i].entrada->ano);
  }
  printf("---------------------------\n");
}

// Funções de comparação para qsort
int compararAno(const void *a, const void *b) {
  Registro *dadoA = (Registro *)a;
  Registro *dadoB = (Registro *)b;
  // Comparar os anos
  return (dadoA->entrada->ano - dadoB->entrada->ano);
}

int compararMes(const void *a, const void *b) {
  Registro *dadoA = (Registro *)a;
  Registro *dadoB = (Registro *)b;
  // Comparar meses
  return (dadoA->entrada->mes - dadoB->entrada->mes);
}

int compararDia(const void *a, const void *b) {
  Registro *dadoA = (Registro *)a;
  Registro *dadoB = (Registro *)b;
  // Comparar os dias
  return (dadoA->entrada->dia - dadoB->entrada->dia);
}

int compararIdade(const void *a, const void *b) {
  // Convertendo os ponteiros para Registro
  Registro *dadoA = (Registro *)a;
  Registro *dadoB = (Registro *)b;
  // Comparando as idades
  return (dadoA->idade - dadoB->idade);
}

// Funções para a pilha
void push(Pilha *pilha, Registro dados) {
  Eoperacao *novo = (Eoperacao *)malloc(sizeof(Eoperacao));
  if (!novo) {
    printf("Erro de alocação de memória!\n");
    return;
  }
  novo->dados = dados;
  // Cópia profunda da data
  novo->dados.entrada = (Data *)malloc(sizeof(Data));
  if (!novo->dados.entrada) {
    printf("Erro de alocação de memória!\n");
    free(novo);
    return;
  }
  *(novo->dados.entrada) = *(dados.entrada);
  // Adicionar na pilha
  novo->proximo = pilha->topo;
  // Atualizar o topo da pilha
  pilha->topo = novo;
}

Registro pop(Pilha *pilha) {
  Registro dadosVazio = {0};
  if (pilha->topo == NULL) {
    printf("A pilha está vazia.\n");
    return dadosVazio;
  }
  // Remover do topo
  Eoperacao *removido = pilha->topo;
  // Atualizar o topo da pilha
  pilha->topo = pilha->topo->proximo;
  // Liberar memória
  Registro dadosRemovidos = removido->dados;
  free(removido);

  return dadosRemovidos;
}

void desfazerOperacao(Fila *fila, Pilha *pilha) {
  if (pilha->topo == NULL) {
    printf("Não há operações para desfazer.\n");
    return;
  }

  // Mostrar a operação a ser desfeita
  Registro ultimaOperacao = pilha->topo->dados;
  printf("Operação a ser desfeita: Enfileirar paciente %s\n",
         ultimaOperacao.nome);
  printf("Deseja realmente desfazer esta operação? (s/n): ");
  char resposta;
  scanf(" %c", &resposta);
  getchar();
  // Verificar a resposta
  if (resposta == 's' || resposta == 'S') {
    // Desempilhar a operação
    Registro dadosDesfeitos = pop(pilha);

    // Remover o último paciente da fila
    if (fila->head == NULL) {
      printf("A fila está vazia. Nenhum paciente para remover.\n");
      return;
    }

    // Caso especial: apenas um elemento na fila
    if (fila->head == fila->tail) {
      free(fila->head->dados.entrada);
      free(fila->head);
      fila->head = NULL;
      fila->tail = NULL;
    } else {
      // Percorrer até o penúltimo elemento
      Efila *atual = fila->head;
      while (atual->proximo != fila->tail) {
        atual = atual->proximo;
      }
      // Remover o último
      free(fila->tail->dados.entrada);
      free(fila->tail);
      atual->proximo = NULL;
      fila->tail = atual;
    }

    printf("Operação desfeita com sucesso.\n");
  } else {
    printf("Operação não desfeita.\n");
  }
}

// Funções para carregar e salvar arquivos
void carregarDados(Lista *lista) {
  FILE *arquivo = fopen("dados_pacientes.txt", "r");
  if (arquivo == NULL) {
    printf("Nenhum dado prévio encontrado. Iniciando com lista vazia.\n");
    return;
  }

  // Array temporário para armazenar as linhas
  char linhas[1000][1000]; // Supondo um máximo de 1000 linhas
  int count = 0;

  // Ler todas as linhas do arquivo para o array
  while (fgets(linhas[count], sizeof(linhas[count]), arquivo)) {
    count++;
    if (count >= 1000) {
      printf("Número máximo de registros excedido.\n");
      break;
    }
  }

  fclose(arquivo);

  // Inserir pacientes na lista em ordem inversa para manter a ordem original
  for (int i = count - 1; i >= 0; i--) {
    Elista *novo = (Elista *)malloc(sizeof(Elista));
    if (!novo) {
      printf("Erro de alocação de memória!\n");
      return;
    }

    novo->dados.entrada = (Data *)malloc(sizeof(Data));
    if (!novo->dados.entrada) {
      printf("Erro de alocação de memória!\n");
      free(novo);
      return;
    }

    // Parse da linha
    sscanf(linhas[i], "%[^;];%d;%[^;];%d/%d/%d", novo->dados.nome,
           &novo->dados.idade, novo->dados.rg, &novo->dados.entrada->dia,
           &novo->dados.entrada->mes, &novo->dados.entrada->ano);

    // Inserir no início da lista
    novo->proximo = lista->inicio;
    lista->inicio = novo;
  }

  printf("Dados carregados com sucesso!\n");
}

void salvarDados(Lista *lista) {
  // Abrir o arquivo para escrita
  FILE *arquivo = fopen("dados_pacientes.txt", "w");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo para escrita.\n");
    return;
  }
  // Percorrer a lista e escrever no arquivo
  Elista *atual = lista->inicio;
  while (atual != NULL) {
    fprintf(arquivo, "%s;%d;%s;%02d/%02d/%04d\n", atual->dados.nome,
            atual->dados.idade, atual->dados.rg, atual->dados.entrada->dia,
            atual->dados.entrada->mes, atual->dados.entrada->ano);
    atual = atual->proximo;
  }

  fclose(arquivo);
  printf("Dados salvos com sucesso!\n");
}

// Funções para liberar memória
void liberarABB(EABB *raiz) {
  if (raiz != NULL) {
    liberarABB(raiz->filhoEsq);
    liberarABB(raiz->filhoDir);
    free(raiz->dados.entrada);
    free(raiz);
  }
}
// Função para exibir informações sobre os desenvolvedores
void sobre() {
  printf("\n--- Sobre o Programa ---\n");
  printf("Desenvolvedores:\n");
  printf("  - Matheus Dourado Valle\n");
  printf("  - João Pedro Sabino\n");
  printf("Ciclo: Quarto semestre\n");
  printf("Curso: Ciência da Computação\n");
  printf("Disciplina: CC4652\n");
  printf("Data: 13/11/2024\n");
  printf("-------------------------\n");
}

void liberarLista(Lista *lista) {
  Elista *atual = lista->inicio;
  while (atual != NULL) {
    Elista *temp = atual;
    atual = atual->proximo;
    free(temp->dados.entrada);
    free(temp);
  }
  lista->inicio = NULL;
}

void liberarFila(Fila *fila) {
  // Liberar cada elemento da fila
  Efila *atual = fila->head;
  while (atual != NULL) {
    // Liberar a memória alocada para o registro
    Efila *temp = atual;
    // Atualizar o ponteiro para o próximo elemento
    atual = atual->proximo;
    free(temp->dados.entrada);
    free(temp);
  }
  fila->head = fila->tail = NULL;
}

void liberarPilha(Pilha *pilha) {
  // Liberar cada elemento da pilha
  Eoperacao *atual = pilha->topo;
  while (atual != NULL) {
    // Liberar a memória alocada para o registro
    Eoperacao *temp = atual;
    // Atualizar o ponteiro para o próximo elemento
    atual = atual->proximo;
    free(temp->dados.entrada);
    free(temp);
  }
  pilha->topo = NULL;
}
