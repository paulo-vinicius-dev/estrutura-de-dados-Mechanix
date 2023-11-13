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

void inicializarFila(Fila* fila) {
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanho = 0;
}

void enfileirar(Fila* fila, Carro carro) {
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    if (novoNodo == NULL) {
        printf("\nErro na alocação de memória.\n");
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
        printf("\nO serviço está lotado.\n");
    }
}

// Função para realizar a saída de um carro de um serviço
void saidaDeCarros(Servico* servico, Fila* filaSaida) {
    if (servico->quantidade > 0) {
        Carro carro = servico->carros[0];
        printf("\nCarro %s saindo do serviço de %s.\n", carro.placa,
               carro.servico == 1 ? "rodas" : "higienização");
        enfileirar(filaSaida, carro);

        // Remove o carro do serviço
        for (int i = 0; i < servico->quantidade - 1; i++) {
            servico->carros[i] = servico->carros[i + 1];
        }
        servico->quantidade--;
    } else {
        printf("\nO serviço está vazio.\n");
    }
}

// Função para movimentar carros da fila de entrada para o serviço
void moverParaServico(Fila* filaEntrada, Servico* servicoRodas, Servico* servicoHigienizacao) {
    // Move carros da fila de entrada para o serviço de rodas ou higienização
    while (filaEntrada->frente != NULL) {
        Carro carro = desenfileirar(filaEntrada);

        if (carro.servico == 1 && servicoRodas->quantidade < 2) {  // Carro para serviço de rodas
            adicionarAoServico(servicoRodas, carro);
            printf("\nCarro %s movido do serviço de entrada para o serviço de rodas.\n", carro.placa);
        } else if (carro.servico == 2 && servicoHigienizacao->quantidade < 2) {  // Carro para serviço de higienização
            adicionarAoServico(servicoHigienizacao, carro);
            printf("\nCarro %s movido do serviço de entrada para o serviço de higienização.\n", carro.placa);
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
        printf("\nFila de saída vazia.\n");
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

    printf("\nServiço de Rodas: ");
    for (int i = 0; i < servicoRodas->quantidade; i++) {
        printf("%s -> ", servicoRodas->carros[i].placa);
    }
    printf("FIM\n");

    printf("\nServiço de Higienização: ");
    for (int i = 0; i < servicoHigienizacao->quantidade; i++) {
        printf("%s -> ", servicoHigienizacao->carros[i].placa);
    }
    printf("FIM\n");

    printf("\nFila de Saída: ");
    atual = filaSaida->frente;
    while (atual != NULL) {
        printf("%s -> ", atual->carro.placa);
        atual = atual->proximo;
    }
    printf("FIM\n");
}

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
        printf("2 - Realizar saída de carros dos serviços\n");
        printf("3 - Entregar carros (remover da fila de saída)\n");
        printf("4 - Mostrar filas\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Carro novoCarro;
                printf("Digite a placa do carro: ");
                scanf("%s", novoCarro.placa);
                printf("Digite o modelo do carro: ");
                scanf("%s", novoCarro.modelo);
                printf("Digite o serviço desejado (rodas[1]/higienização[2]): ");
                scanf("%d", &novoCarro.servico);

                if (novoCarro.servico == 1) {
                    if (servicoRodas.quantidade < 2) {
                        adicionarAoServico(&servicoRodas, novoCarro);
                        printf("\nCarro %s registrado no serviço de rodas.\n", novoCarro.placa);
                    } else {
                        enfileirar(&filaEntrada, novoCarro);
                        printf("\nNão há espaço no serviço de rodas, o carro foi para a fila de entrada.\n");
                    }
                } else if (novoCarro.servico == 2) {
                    if (servicoHigienizacao.quantidade < 2) {
                        adicionarAoServico(&servicoHigienizacao, novoCarro);
                        printf("\nCarro %s registrado no serviço de higienização.\n", novoCarro.placa);
                    } else {
                        enfileirar(&filaEntrada, novoCarro);
                        printf("\nNão há espaço no serviço de higienização, o carro foi para a fila de entrada.\n");
                    }
                } else {
                    printf("\nServiço inválido.\n");
                }
                break;
            }
            case 2: {
                int escolhaServico;
                printf("Escolha o serviço para realizar a saída (rodas[1]/higienização[2]): ");
                scanf("%d", &escolhaServico);

                if (escolhaServico == 1) {
                    saidaDeCarros(&servicoRodas, &filaSaida);
                } else if (escolhaServico == 2) {
                    saidaDeCarros(&servicoHigienizacao, &filaSaida);
                } else {
                    printf("\nServiço inválido.\n");
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
                printf("\nOpção inválida.\n");
                break;
            }
        }

    } while (opcao != 0);

    return 0;
}
