#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// ============================================================
// CÓDIGO DA ILHA – EDIÇÃO FREE FIRE
// NÍVEL: MESTRE
//
// Sistema de gerenciamento de inventário utilizando:
// - Structs
// - Vetor de structs
// - Funções
// - Menu interativo
// - Busca sequencial
// - Busca binária
// - Insertion Sort
// - Enumeração
// - Controle de estado
// ============================================================


// ============================================================
// CONSTANTE
// Define a capacidade máxima da mochila.
// ============================================================

#define MAX_ITENS 10


// ============================================================
// STRUCT ITEM
//
// A struct Item representa cada objeto armazenado na mochila.
//
// nome       -> nome do item
// tipo       -> categoria do item
// quantidade -> quantidade disponível
// prioridade -> importância do item, de 1 a 5
// ============================================================

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade;
} Item;


// ============================================================
// ENUM CRITERIOORDENACAO
//
// Define os critérios disponíveis para ordenar a mochila.
//
// ORDENAR_NOME       -> ordem alfabética pelo nome
// ORDENAR_TIPO       -> ordem alfabética pelo tipo
// ORDENAR_PRIORIDADE -> maior prioridade primeiro
// ============================================================

typedef enum {
    ORDENAR_NOME = 1,
    ORDENAR_TIPO,
    ORDENAR_PRIORIDADE
} CriterioOrdenacao;


// ============================================================
// VARIÁVEIS GLOBAIS
//
// mochila       -> vetor que armazena os itens
// numItens      -> quantidade atual de itens cadastrados
// comparacoes   -> quantidade de comparações realizadas
// ordenadaPorNome -> indica se a mochila está ordenada por nome
// ============================================================

Item mochila[MAX_ITENS];

int numItens = 0;

int comparacoes = 0;

bool ordenadaPorNome = false;


// ============================================================
// limparTela()
//
// Simula a limpeza da tela imprimindo várias linhas vazias.
//
// Essa função ajuda a deixar o terminal mais organizado.
// ============================================================

void limparTela() {

    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}


// ============================================================
// exibirMenu()
//
// Apresenta as opções disponíveis para o jogador.
//
// Também informa se a mochila está ordenada por nome,
// requisito necessário para utilizar a busca binária.
// ============================================================

void exibirMenu() {

    printf("\n");
    printf("=============================================\n");
    printf("       CODIGO DA ILHA - MOCHILA\n");
    printf("=============================================\n");

    printf("Itens na mochila: %d/%d\n", numItens, MAX_ITENS);

    if (ordenadaPorNome) {
        printf("Status: Mochila ordenada por NOME\n");
    } else {
        printf("Status: Mochila NAO ordenada por nome\n");
    }

    printf("---------------------------------------------\n");
    printf("1. Adicionar item\n");
    printf("2. Remover item\n");
    printf("3. Listar itens\n");
    printf("4. Ordenar itens\n");
    printf("5. Buscar item por nome\n");
    printf("0. Sair\n");
    printf("=============================================\n");
    printf("Escolha uma opcao: ");
}


// ============================================================
// inserirItem()
//
// Adiciona um novo item ao vetor mochila.
//
// O sistema verifica primeiro se existe espaço disponível.
//
// Depois solicita:
// - Nome
// - Tipo
// - Quantidade
// - Prioridade
//
// Ao inserir um novo item, a mochila deixa de ser considerada
// ordenada por nome, pois o novo item pode estar fora da ordem.
// ============================================================

void inserirItem() {

    if (numItens >= MAX_ITENS) {

        printf("\nA mochila esta cheia!\n");
        printf("Nao e possivel adicionar novos itens.\n");

        return;
    }

    Item novoItem;

    printf("\n=============================================\n");
    printf("             ADICIONAR ITEM\n");
    printf("=============================================\n");

    // Limpa o buffer antes de utilizar fgets.
    getchar();

    printf("Nome do item: ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);

    // Remove o '\n' inserido pelo fgets.
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0';


    printf("Tipo do item (arma, municao, cura, ferramenta): ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);

    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';


    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);


    // Validação da quantidade.
    while (novoItem.quantidade <= 0) {

        printf("A quantidade deve ser maior que zero.\n");
        printf("Digite novamente: ");

        scanf("%d", &novoItem.quantidade);
    }


    printf("Prioridade (1 a 5): ");
    scanf("%d", &novoItem.prioridade);


    // Validação da prioridade.
    while (novoItem.prioridade < 1 || novoItem.prioridade > 5) {

        printf("A prioridade deve estar entre 1 e 5.\n");
        printf("Digite novamente: ");

        scanf("%d", &novoItem.prioridade);
    }


    // Armazena o novo item no próximo espaço disponível.
    mochila[numItens] = novoItem;

    numItens++;


    // Como um novo item foi inserido, não podemos garantir
    // que a mochila continue ordenada por nome.
    ordenadaPorNome = false;


    printf("\nItem adicionado com sucesso!\n");

    printf("\nPressione ENTER para continuar...");
    getchar();
    getchar();
}


// ============================================================
// removerItem()
//
// Remove um item da mochila utilizando seu nome.
//
// Quando o item é encontrado, todos os itens posteriores
// são deslocados uma posição para a esquerda.
//
// Dessa forma, não fica um espaço vazio no meio do vetor.
// ============================================================

void removerItem() {

    if (numItens == 0) {

        printf("\nA mochila esta vazia.\n");

        return;
    }


    char nomeBusca[30];

    printf("\n=============================================\n");
    printf("              REMOVER ITEM\n");
    printf("=============================================\n");

    getchar();

    printf("Digite o nome do item que deseja remover: ");

    fgets(nomeBusca, sizeof(nomeBusca), stdin);

    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';


    int posicao = -1;


    // Busca sequencial pelo nome.
    for (int i = 0; i < numItens; i++) {

        if (strcmp(mochila[i].nome, nomeBusca) == 0) {

            posicao = i;

            break;
        }
    }


    // Verifica se o item foi encontrado.
    if (posicao == -1) {

        printf("\nItem nao encontrado na mochila.\n");

        return;
    }


    // Desloca os elementos seguintes uma posição para a esquerda.
    for (int i = posicao; i < numItens - 1; i++) {

        mochila[i] = mochila[i + 1];
    }


    // Diminui a quantidade de itens.
    numItens--;


    printf("\nItem \"%s\" removido com sucesso!\n", nomeBusca);


    // A remoção mantém a ordem dos demais itens.
    // Portanto, se já estava ordenada por nome,
    // continua ordenada.
}


// ============================================================
// listarItens()
//
// Exibe todos os itens atualmente armazenados.
//
// Os dados apresentados são:
// - Nome
// - Tipo
// - Quantidade
// - Prioridade
// ============================================================

void listarItens() {

    printf("\n=============================================\n");
    printf("              ITENS DA MOCHILA\n");
    printf("=============================================\n");


    if (numItens == 0) {

        printf("A mochila esta vazia.\n");

        return;
    }


    printf("%-25s %-15s %-10s %-10s\n",
           "NOME",
           "TIPO",
           "QUANTIDADE",
           "PRIORIDADE");

    printf("----------------------------------------------------------------\n");


    for (int i = 0; i < numItens; i++) {

        printf("%-25s %-15s %-10d %-10d\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }


    printf("----------------------------------------------------------------\n");
}


// ============================================================
// compararItens()
//
// Compara dois itens de acordo com o critério escolhido.
//
// Retorno:
//
// Negativo -> primeiro item deve ficar antes do segundo
// Zero     -> itens equivalentes no critério
// Positivo -> primeiro item deve ficar depois do segundo
//
// Essa função é utilizada pelo Insertion Sort.
// ============================================================

int compararItens(Item a, Item b, CriterioOrdenacao criterio) {

    comparacoes++;


    // Ordenação por nome.
    if (criterio == ORDENAR_NOME) {

        return strcmp(a.nome, b.nome);
    }


    // Ordenação por tipo.
    if (criterio == ORDENAR_TIPO) {

        return strcmp(a.tipo, b.tipo);
    }


    // Ordenação por prioridade.
    //
    // Quanto maior a prioridade, primeiro o item aparece.
    if (criterio == ORDENAR_PRIORIDADE) {

        if (a.prioridade > b.prioridade) {
            return -1;
        }

        if (a.prioridade < b.prioridade) {
            return 1;
        }

        return 0;
    }


    return 0;
}


// ============================================================
// insertionSort()
//
// Implementa o algoritmo Insertion Sort.
//
// O algoritmo percorre o vetor e insere cada elemento
// na posição correta dentro da parte já ordenada.
//
// O algoritmo funciona para os três critérios:
// - Nome
// - Tipo
// - Prioridade
// ============================================================

void insertionSort(CriterioOrdenacao criterio) {

    comparacoes = 0;


    for (int i = 1; i < numItens; i++) {

        Item chave = mochila[i];

        int j = i - 1;


        // Desloca os itens maiores para a direita.
        while (j >= 0 &&
               compararItens(mochila[j], chave, criterio) > 0) {

            mochila[j + 1] = mochila[j];

            j--;
        }


        // Insere o item na posição correta.
        mochila[j + 1] = chave;
    }


    // Atualiza o estado da mochila.
    //
    // Somente a ordenação por nome permite utilizar
    // posteriormente a busca binária.
    if (criterio == ORDENAR_NOME) {

        ordenadaPorNome = true;

    } else {

        ordenadaPorNome = false;
    }
}


// ============================================================
// menuDeOrdenacao()
//
// Apresenta um segundo menu para o jogador escolher
// o critério de ordenação.
//
// Opções:
// 1 -> Nome
// 2 -> Tipo
// 3 -> Prioridade
// ============================================================

void menuDeOrdenacao() {

    if (numItens == 0) {

        printf("\nNao existem itens para ordenar.\n");

        return;
    }


    int opcao;

    printf("\n=============================================\n");
    printf("              ORDENAR MOCHILA\n");
    printf("=============================================\n");

    printf("1. Ordenar por nome\n");
    printf("2. Ordenar por tipo\n");
    printf("3. Ordenar por prioridade\n");
    printf("0. Voltar\n");

    printf("---------------------------------------------\n");
    printf("Escolha uma opcao: ");

    scanf("%d", &opcao);


    switch (opcao) {

        case 1:

            insertionSort(ORDENAR_NOME);

            printf("\nMochila ordenada por NOME.\n");
            printf("Comparacoes realizadas: %d\n", comparacoes);

            break;


        case 2:

            insertionSort(ORDENAR_TIPO);

            printf("\nMochila ordenada por TIPO.\n");
            printf("Comparacoes realizadas: %d\n", comparacoes);

            break;


        case 3:

            insertionSort(ORDENAR_PRIORIDADE);

            printf("\nMochila ordenada por PRIORIDADE.\n");
            printf("Comparacoes realizadas: %d\n", comparacoes);

            break;


        case 0:

            printf("\nVoltando ao menu principal...\n");

            break;


        default:

            printf("\nOpcao invalida!\n");
    }
}


// ============================================================
// buscaBinariaPorNome()
//
// Realiza uma busca binária pelo nome do item.
//
// IMPORTANTE:
//
// A busca binária somente funciona corretamente quando
// o vetor está previamente ordenado pelo mesmo critério
// utilizado na busca.
//
// Por isso verificamos a variável ordenadaPorNome antes
// de iniciar a busca.
// ============================================================

void buscaBinariaPorNome() {

    if (numItens == 0) {

        printf("\nA mochila esta vazia.\n");

        return;
    }


    // Verifica o pré-requisito da busca binária.
    if (!ordenadaPorNome) {

        printf("\nNao e possivel realizar a busca binaria.\n");
        printf("Primeiro ordene a mochila por NOME.\n");

        return;
    }


    char nomeBusca[30];


    printf("\n=============================================\n");
    printf("             BUSCA BINARIA\n");
    printf("=============================================\n");

    getchar();

    printf("Digite o nome do item: ");

    fgets(nomeBusca, sizeof(nomeBusca), stdin);

    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';


    int inicio = 0;
    int fim = numItens - 1;

    int encontrado = -1;


    // Enquanto houver uma parte válida do vetor para pesquisar.
    while (inicio <= fim) {

        int meio = (inicio + fim) / 2;


        int resultado = strcmp(mochila[meio].nome, nomeBusca);


        // Item encontrado.
        if (resultado == 0) {

            encontrado = meio;

            break;
        }


        // O nome procurado vem depois do item do meio.
        if (resultado < 0) {

            inicio = meio + 1;

        } else {

            // O nome procurado vem antes do item do meio.
            fim = meio - 1;
        }
    }


    // Exibe o resultado da busca.
    if (encontrado != -1) {

        printf("\nItem encontrado!\n");

        printf("---------------------------------------------\n");
        printf("Nome:       %s\n", mochila[encontrado].nome);
        printf("Tipo:       %s\n", mochila[encontrado].tipo);
        printf("Quantidade: %d\n", mochila[encontrado].quantidade);
        printf("Prioridade: %d\n", mochila[encontrado].prioridade);
        printf("---------------------------------------------\n");

    } else {

        printf("\nItem \"%s\" nao foi encontrado na mochila.\n",
               nomeBusca);
    }
}


// ============================================================
// FUNÇÃO PRINCIPAL - MAIN
//
// Controla o fluxo principal do programa.
//
// O menu continua sendo exibido enquanto o jogador
// não escolher a opção 0.
// ============================================================

int main() {

    int opcao;


    // Estrutura do-while garante que o menu seja exibido
    // pelo menos uma vez.
    do {

        limparTela();

        exibirMenu();

        scanf("%d", &opcao);


        switch (opcao) {

            case 1:

                inserirItem();

                break;


            case 2:

                removerItem();

                break;


            case 3:

                listarItens();

                printf("\nPressione ENTER para continuar...");
                getchar();
                getchar();

                break;


            case 4:

                menuDeOrdenacao();

                printf("\nPressione ENTER para continuar...");
                getchar();
                getchar();

                break;


            case 5:

                buscaBinariaPorNome();

                printf("\nPressione ENTER para continuar...");
                getchar();
                getchar();

                break;


            case 0:

                printf("\n=============================================\n");
                printf("   Obrigado por jogar! Boa sorte na ilha!\n");
                printf("=============================================\n");

                break;


            default:

                printf("\nOpcao invalida! Tente novamente.\n");

                printf("\nPressione ENTER para continuar...");
                getchar();
                getchar();
        }


    } while (opcao != 0);


    return 0;
}
