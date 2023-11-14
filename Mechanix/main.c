#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct para representar um carro
typedef struct {
    char placa[8];
    char modelo[20];
    int servico;
} Carro;

// Nodo de uma fila
typedef struct Nodo {
    Carro carro;
    struct Nodo* proximo;
} Nodo;

// Fila para armazenar carros
typedef struct {
    Nodo* frente;
    Nodo* tras;
    int tamanho;
} Fila;

// Estrutura para representar um serviço
typedef struct {
    Carro carros[2];
    int quantidade;
} Servico;

// Função responsanvel por inicializar as filas
void inicializarFila(Fila* fila);

// coloca o carro na sua posição na fila
void enfileirar(Fila* fila, Carro carro);

// retirar o carro da sua posicao na fila
Carro desenfileirar(Fila* fila);

// Função responsavel por inicializar os serviços
void inicializarServico(Servico* servico);

// Função para adicionar um carro ao serviço
void adicionarAoServico(Servico* servico, Carro carro);

// Função para retirar um carro alocado em um serviço
void saidaDeCarros(Servico* servico, Fila* filaSaida);

// Função responsavel por mover automaticamente um carro sempre seu servico estiver com vaga disponivel
void moverParaServico(Fila* filaEntrada, Servico* servicoRodas, Servico* servicoHigienizacao);

// Função responsável por tirar o carro da fila de saida e entregar ele ao seu dono
void entregaCarros(Fila* filaSaida);

// Função para visualizar as filas(feita para visualizar o comportamento delas no programa)
void mostrarFilas(Fila* filaEntrada, Servico* servicoRodas, Servico* servicoHigienizacao, Fila* filaSaida);

// Função principal
int main() {
    // Iniciliazação das filas
    Fila filaEntrada;
    Fila filaSaida;

    inicializarFila(&filaEntrada);
    inicializarFila(&filaSaida);

    Servico servicoRodas;
    Servico servicoHigienizacao;

    inicializarServico(&servicoRodas);
    inicializarServico(&servicoHigienizacao);

    // Loop do menu principal
    int opcao;

    do {
        printf("\nMenu:\n");
        printf("1 - Registrar entrada de carro\n");
        printf("2 - Realizar saida de carros dos servicos\n");
        printf("3 - Entregar carros (remover da fila de saida)\n");
        printf("4 - Mostrar filas\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Pegar as informações necessárias para preencher a strict Carro
                Carro novoCarro;
                printf("Digite a placa do carro: ");
                scanf("%s", novoCarro.placa);
                printf("Digite o modelo do carro: ");
                scanf("%s", novoCarro.modelo);
                printf("Digite o servico desejado (rodas[1]/higienizacao[2]): ");
                scanf("%d", &novoCarro.servico);

                // Verifica se o serviço escolhido pelo carro há vagas, caso haja adiciona ele a fila do respectivo serviço, caso contrário coloca o carro na fila de entrada(esperando por uma vaga do seu serviço abrir)
                if (novoCarro.servico == 1) {
                    if (servicoRodas.quantidade < 2) {
                        adicionarAoServico(&servicoRodas, novoCarro);
                        printf("\nCarro %s registrado no servico de rodas.\n", novoCarro.placa);
                    } else {
                        enfileirar(&filaEntrada, novoCarro);
                        printf("\nNao ha espaco no servico de rodas, o carro foi para a fila de entrada.\n");
                    }
                } else if (novoCarro.servico == 2) {
                    if (servicoHigienizacao.quantidade < 2) {
                        adicionarAoServico(&servicoHigienizacao, novoCarro);
                        printf("\nCarro %s registrado no servico de higienizacao.\n", novoCarro.placa);
                    } else {
                        enfileirar(&filaEntrada, novoCarro);
                        printf("\nNao ha espaco no servico de higienizacao, o carro foi para a fila de entrada.\n");
                    }
                } else {
                    printf("\nServico invalido.\n");
                }
                break;
            }
            case 2: {
                int escolhaServico;
                printf("Escolha o servico para realizar a saida (rodas[1]/higienizacao[2]): ");
                scanf("%d", &escolhaServico);

                if (escolhaServico == 1) {
                    saidaDeCarros(&servicoRodas, &filaSaida);
                } else if (escolhaServico == 2) {
                    saidaDeCarros(&servicoHigienizacao, &filaSaida);
                } else {
                    printf("\nServico invalido.\n");
                }

                // Movimenta carros da fila de entrada para o serviço
                for (int i = 0; i < filaEntrada.tamanho + filaEntrada.tamanho; i++) {
                    moverParaServico(&filaEntrada, &servicoRodas, &servicoHigienizacao);
                }
                break;
            }
            case 3: {
                entregaCarros(&filaSaida);
                break;
            }
            case 4: {
                mostrarFilas(&filaEntrada, &servicoRodas, &servicoHigienizacao, &filaSaida);
                break;
            }
            case 0: {
                printf("\nSaindo do programa.\n");
                break;
            }
            default: {
                printf("\nOpcao invalida.\n");
                break;
            }
        }

    } while (opcao != 0);

    return 0;
}

void inicializarFila(Fila* fila) {
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanho = 0;
}

void enfileirar(Fila* fila, Carro carro) {
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    if (novoNodo == NULL) {
        printf("\nErro na alocacao de memoria.\n");
        exit(EXIT_FAILURE);
    }

    novoNodo->carro = carro;
    novoNodo->proximo = NULL;

    if (fila->frente == NULL) {
        fila->frente = novoNodo;
        fila->tras = novoNodo;
    } else {
        fila->tras->proximo = novoNodo;
        fila->tras = novoNodo;
    }

    fila->tamanho++;  // Incrementa o tamanho
}

Carro desenfileirar(Fila* fila) {
    if (fila->frente == NULL) {
        printf("\nFila vazia.\n");
        exit(EXIT_FAILURE);
    }

    Nodo* nodoRemovido = fila->frente;
    Carro carro = nodoRemovido->carro;

    fila->frente = fila->frente->proximo;
    free(nodoRemovido);

    if (fila->frente == NULL) {
        fila->tras = NULL;
    }

    fila->tamanho--;

    return carro;
}

// Função para inicializar um serviço
void inicializarServico(Servico* servico) {
    servico->quantidade = 0;
}

// Função para adicionar um carro a um serviço
void adicionarAoServico(Servico* servico, Carro carro) {
    if (servico->quantidade < 2) {
        servico->carros[servico->quantidade] = carro;
        servico->quantidade++;
    } else {
        printf("\nO servico esta lotado.\n");
    }
}

// Função para realizar a saída de um carro de um serviço
void saidaDeCarros(Servico* servico, Fila* filaSaida) {
    if (servico->quantidade > 0) {
        Carro carro = servico->carros[0];
        printf("\nCarro %s saindo do servico de %s.\n", carro.placa,
               carro.servico == 1 ? "rodas" : "higienizacao");
        enfileirar(filaSaida, carro);

        // Remove o carro do serviço
        for (int i = 0; i < servico->quantidade - 1; i++) {
            servico->carros[i] = servico->carros[i + 1];
        }
        servico->quantidade--;
    } else {
        printf("\nO servico esta vazio.\n");
    }
}

// Função para movimentar carros da fila de entrada para o serviço
void moverParaServico(Fila* filaEntrada, Servico* servicoRodas, Servico* servicoHigienizacao) {
    // Move carros da fila de entrada para o serviço de rodas ou higienização
    while (filaEntrada->frente != NULL) {
        Carro carro = desenfileirar(filaEntrada);

        if (carro.servico == 1 && servicoRodas->quantidade < 2) {  // Carro para serviço de rodas
            adicionarAoServico(servicoRodas, carro);
            printf("\nCarro %s movido do servico de entrada para o servico de rodas.\n", carro.placa);
        } else if (carro.servico == 2 && servicoHigienizacao->quantidade < 2) {  // Carro para serviço de higienização
            adicionarAoServico(servicoHigienizacao, carro);
            printf("\nCarro %s movido do servico de entrada para o servico de higienizacao.\n", carro.placa);
        } else {
            enfileirar(filaEntrada, carro);
            break;
        }
    }
}

// Função para entrega de carros (remover da fila de saída)
void entregaCarros(Fila* filaSaida) {
    if (filaSaida->frente != NULL) {
        Carro carro = desenfileirar(filaSaida);
        printf("\nCarro %s entregue.\n", carro.placa);
    } else {
        printf("\nFila de saida vazia.\n");
    }
}

// Função para mostrar as filas
void mostrarFilas(Fila* filaEntrada, Servico* servicoRodas, Servico* servicoHigienizacao, Fila* filaSaida) {
    Nodo* atual;

    printf("\nFila de Entrada: ");
    atual = filaEntrada->frente;
    while (atual != NULL) {
        printf("%s -> ", atual->carro.placa);
        atual = atual->proximo;
    }
    printf("FIM\n");

    printf("\nServico de Rodas: ");
    for (int i = 0; i < servicoRodas->quantidade; i++) {
        printf("%s -> ", servicoRodas->carros[i].placa);
    }
    printf("FIM\n");

    printf("\nServico de Higienizacao: ");
    for (int i = 0; i < servicoHigienizacao->quantidade; i++) {
        printf("%s -> ", servicoHigienizacao->carros[i].placa);
    }
    printf("FIM\n");

    printf("\nFila de Saida: ");
    atual = filaSaida->frente;
    while (atual != NULL) {
        printf("%s -> ", atual->carro.placa);
        atual = atual->proximo;
    }
    printf("FIM\n");
}
