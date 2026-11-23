//main.c  -  APLICAÇÃO (ponto de entrada, menu e navegação)

/*
 *  REQUISITO (Arquitetura Modular e TAD) - main.c (Aplicação):
 *  "Contém a função main(), laço principal do menu no console (switch-case
 *  com do..while) e chamada das rotinas do TAD."

 * Aqui fizemos o main() e o menu, com do..while e switch-case. Cada opção
 * chama uma função do TAD (declarada em jogo.h, escrita em jogo.c).
 * Assim mantemos o main enxuto, ele só decide O QUE fazer (o menu) e entrega o
 * trabalho para as funções do TAD.
 */
#include <stdio.h>
#include "jogo.h"

/* REQUISITO (Gerenciamento de Memória) vetor de ponteiros para a struct.
 * Aqui fizemos a definição do vetor v (em jogo.h ele só foi declarado).
 * Obj: criar o vetor uma única vez no programa inteiro. */
p_jogo v[MAX];

int main(void) {
    int opcao = 0;
    int carregados;

    /* REQUISITO (Gerenciamento de Memória):
     *   "Ao iniciar a aplicação, todos os ponteiros do vetor devem apontar
     *    explicitamente para NULL."
     * Aqui chamamos inicializa(), que coloca NULL em todas as posições.

     * Isso para nenhuma posição começar com lixo de memória.
     * O NULL marca a posição como vazia, e o resto do programa usa isso para saber
     * onde existe jogo. */
    inicializa();

    /* REQUISITO (Persistência - Carga de Dados na Inicialização):
     *   "Ao abrir o programa, verificar a existência do arquivo de dados."
     * Aqui chamamos carregar_arquivo(), que lê o arquivo e devolve
     * quantos jogos carregou (0 se o arquivo não existir).
     * Isso para recuperar os jogos salvos na última execução, sem isso o
     * catálogo começaria vazio toda vez que o programa abrisse. */
    carregados = carregar_arquivo(ARQUIVO);
    printf("Jogos carregados do arquivo: %d\n", carregados);

    /* REQUISITO (Funcionalidades do Menu de Console): menu com switch-case
     * dentro de do..while.
     * Aqui fizemos o menu se repetir até o usuário escolher 5.*/
    do {
        printf("\n CATALOGO DE JOGOS DIGITAIS \n");
        printf("1. Cadastrar jogo\n");
        printf("2. Listar todos\n");
        printf("3. Buscar jogo por codigo\n");
        printf("4. Relatorio\n");
        printf("5. Salvar e sair\n");
        opcao = ler_int("Digite sua opcao: ");
        printf("\n");

        switch (opcao) {
            case 1:
                /* 1. Cadastrar: "Ler os dados do teclado, alocar o ponteiro
                   via malloc e armazenar no vetor."
                   FEITO: chamamos cadastrar(). OBJETIVO: incluir um novo
                   jogo no catálogo. */
                cadastrar();
                break;
            case 2:
                /* 2. Listar Todos: "Percorrer o vetor e exibir os dados de
                   todos os ponteiros não nulos."
                   FEITO: chamamos imprimir_tudo(). OBJETIVO: o usuário ver
                   tudo o que está cadastrado. */
                imprimir_tudo();
                break;
            case 3:
                /* 3. Buscar/Exibir Único: "Localizar um registro por código/
                   identificador único e imprimir seus detalhes."
                   FEITO: chamamos exibir_busca(). OBJETIVO: consultar um
                   jogo específico sem precisar listar todos. */
                exibir_busca();
                break;
            case 4:
                /* 4. Relatório/Estatística: "Exibir ao menos um cálculo
                   analítico sobre a base cadastrada."
                   FEITO: chamamos relatorio(). OBJETIVO: transformar os dados
                   cadastrados em informação (média de preço e jogo mais
                   jogado). */
                relatorio();
                break;
            case 5:
                /* 5. Salvar e Sair: "Gravar o estado atual no arquivo texto,
                   liberar a memória alocada e finalizar a execução."
                   FEITO: aqui gravamos no arquivo; a memória é liberada logo
                   depois do laço (liberar_memoria) e o programa termina.
                   OBJETIVO: guardar em disco o que está na memória, que some
                   quando o programa fecha. */
                if (salvar_arquivo(ARQUIVO)) {
                    printf("Dados salvos em '%s'.\n", ARQUIVO);
                } else {
                    /* Não requisito, se não conseguiu gravar, não sai, para não
                       perder os dados que estão na memória. */
                    opcao = 0;
                }
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 5);

    /* REQUISITO (Gerenciamento de Memória):
     *   "Antes de encerrar o programa, liberar toda a memória alocada
     *    dinamicamente utilizando a função free()."
     *
     * Chamamos liberar_memoria(), que dá free() em cada posição não nula do vetor.
     * Obj: devolver ao sistema a memória que pedimos com malloc.
     * Sem isso haveria vazamento de memória. */
    liberar_memoria();
    printf("Programa encerrado.\n");
    return 0;
}
