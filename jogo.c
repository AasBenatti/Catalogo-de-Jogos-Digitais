//  jogo.c  -  IMPLEMENTAÇÃO do TAD "Jogo"

 /* REQUISITO (Arquitetura Modular e TAD) - modulo.c (Implementação):
  * Implementação do código de cada protótipo definido no .h.

 *Aqui fizemos o código de todas as funções declaradas em jogo.h.
 *  com o objetivo de guardar o "como funciona" do módulo em um arquivo só, separado da
 *  interface (jogo.h) e do menu (main.c).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h"

// GERENCIAMENTO DE MEMÓRIA (VETOR DE PONTEIROS)

/* REQUISITO (Gerenciamento de Memória):
 *   "Ao iniciar a aplicação, todos os ponteiros do vetor devem apontar
 *    explicitamente para NULL."

 * Aqui fizemos um laço que passa pelas MAX posições e coloca NULL em cada uma.
 * objetivo = um ponteiro que não foi inicializado guarda um valor aleatório (lixo).
 * Com NULL, o programa sabe que a posição está vazia e nunca tenta acessar um jogo que não existe.
 */

void inicializa(void) {
    int i;
    for (i = 0; i < MAX; i++) {
        v[i] = NULL;
    }
}

/* REQUISITO (Gerenciamento de Memória):
 *   "Antes de encerrar o programa, liberar toda a memória alocada
 *    dinamicamente utilizando a função free() para cada posição não nula
 *    do vetor."
 *
 * Aqui percorremos o vetor e, onde o ponteiro não é NULL, fazemos
 * free() e voltamos a posição para NULL.
 * O objetivo foi de devolver ao sistema a memória pedida com malloc, evitando
 * vazamento. O NULL depois do free evita usar um ponteiro que já foi liberado. */
void liberar_memoria(void) {
    int i;
    for (i = 0; i < MAX; i++) {
        if (v[i] != NULL) {
            free(v[i]);
            v[i] = NULL;
        }
    }
}

// Aqui não foi um requisito, mas fizemos para evitar travar ou gravar dados inválidos.

/* Aqui tratamos o caso do teclado ser fechado (Ctrl+D no Linux, Ctrl+Z no Windows)
 * liberamos a memória e saímos sem salvar.
 * sem isso, o programa ficaria repetindo a pergunta para sempre, porque nunca mais conseguiria ler uma resposta. */
static void fim_de_entrada(void) {
    printf("\nEntrada encerrada. Dados nao salvos.\n");
    liberar_memoria();
    exit(1);
}

/* Aqui também não foi requisito, lemos uma linha do teclado com fgets, tiramos o '\n' do final e
 * descartamos o que passar do limite, isso retorna 0 se a entrada acabou e 1 se leu certinho.
 * Usamos fgets em vez de scanf para nunca escrever além do tamanho do vetor (estouro de buffer)
 * e para conseguir ler textos com espaços, como "The Witcher 3" exemplo nos dados.txt. */
int ler_linha(char *destino, int tam) {
    int c;
    if (fgets(destino, tam, stdin) == NULL) {
        destino[0] = '\0';
        return 0;
    }
    if (strchr(destino, '\n') == NULL) {
        //linha maior que o espaço disponível: descarta o resto dela.
        while ((c = getchar()) != '\n' && c != EOF);
    }
    destino[strcspn(destino, "\r\n")] = '\0';
    return 1;
}

/* Não requisito tb, aqui perguntamos de novo até o usuário digitar um número inteiro válido.
 * Com o objetivo de o programa não travar nem usar lixo se o usuário digitar letras onde deveria ir um número. */
int ler_int(const char *mensagem) {
    char linha[64];
    int valor;
    while (1) {
        printf("%s", mensagem);
        if (!ler_linha(linha, sizeof(linha))) fim_de_entrada();
        if (sscanf(linha, "%d", &valor) == 1) {
            return valor;
        }
        printf("Valor invalido. Digite um numero inteiro.\n");
    }
}

/* Não req, aqui perguntamos de novo até o usuário digitar um número real válido >=0.
 * Aceita vírgula ou ponto como separador decimal.
 * o objetivo aqui foi fazer aceitar tanto 24,90 quanto 24.90
 * e recusar valor negativo, que não faz sentido para preço (nesse caso) ou horas. */
float ler_float(const char *mensagem) {
    char linha[64];
    float valor;
    int i;
    while (1) {
        printf("%s", mensagem);
        if (!ler_linha(linha, sizeof(linha))) fim_de_entrada();
        /* troca vírgula por ponto, que é o que o sscanf entende */
        for (i = 0; linha[i] != '\0'; i++) {
            if (linha[i] == ',') linha[i] = '.';
        }
        if (sscanf(linha, "%f", &valor) == 1 && valor >= 0) {
            return valor;
        }
        printf("Valor invalido. Digite um numero maior ou igual a zero.\n");
    }
}

/* Não req, aqui lemos um texto que não pode ficar vazio.
 * um campo vazio seria gravado como "20;;Dev;..." e o jogo se perderia na próxima
 * abertura do programa, porque o sscanf não aceita campo de texto vazio. */
void ler_texto(const char *mensagem, char *destino, int tam) {
    int i, vazio;
    while (1) {
        printf("%s", mensagem);
        if (!ler_linha(destino, tam)) fim_de_entrada();
        vazio = 1;
        for (i = 0; destino[i] != '\0'; i++) {
            if (destino[i] != ' ' && destino[i] != '\t') {
                vazio = 0;
                break;
            }
        }
        if (!vazio) break;
        printf("Este campo nao pode ficar vazio.\n");
    }
    trocar_ponto_e_virgula(destino);
}

/* Não req, aqui trocamos cada ';' digitado dentro de um texto por ','.
 * o arquivo usa ';' para separar os campos de cada linha.
 * Um ';'dentro do título bagunçaria a separação e quebraria a leitura. */
void trocar_ponto_e_virgula(char *texto) {
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ';') texto[i] = ',';
    }
}

//FUNCIONALIDADES DO MENU DE CONSOLE

/* REQUISITO (Menu - 3. Buscar/Exibir Único):
 *   "Localizar um registro por código/identificador único."
 *
 * Aqui percorremos o vetor procurando o jogo com o código informado.
 * Retorna a posição dele no vetor, ou -1 se não encontrar.
 * Objetivo: ter uma única função que acha um jogo pelo código.
 * Ela é usada na busca, no cadastro (para não repetir código) e na carga do arquivo. */
int buscar_por_codigo(int codigo) {
    int i;
    for (i = 0; i < MAX; i++) {
        if (v[i] != NULL && v[i]->codigo == codigo) {
            return i;
        }
    }
    return -1;
}

/* REQUISITO (Menu - 1. Cadastrar):
 *   "Ler os dados do teclado, alocar o ponteiro via malloc e armazenar no vetor."

 * 1 achamos uma posição livre.
 * 2 lemos o código e conferimos que é novo.
 * 3 alocamos o jogo com malloc.
 * 4 lemos os outros campos.

 * No fim o jogo fica guardado em v[i].
 * Nosso objetivo aqui foi incluir um jogo novo no catálogo gastando memória só com jogos que
 * realmente existem, em vez de reservar espaço para os 100 de uma vez. */
void cadastrar(void) {
    int i, codigo;

    /* 1 Procuramos a primeira posição livre (ponteiro NULL).
          Para saber onde guardar o novo jogo e avisar se o catálogo
          estiver cheio. */
    for (i = 0; i < MAX && v[i] != NULL; i++);
    if (i == MAX) {
        printf("Catalogo cheio (maximo de %d jogos).\n", MAX);
        return;
    }

    /* 2 Lemos o código e recusamos se for <=0 ou se já existir.
         O código é o identificador único, dois jogos com
          o mesmo código tornariam a busca ambígua. */
    codigo = ler_int("Codigo: ");
    if (codigo <= 0) {
        printf("O codigo deve ser maior que zero.\n");
        return;
    }
    if (buscar_por_codigo(codigo) != -1) {
        printf("Ja existe um jogo com o codigo %d.\n", codigo);
        return;
    }

    /* 3 REQUISITO (Gerenciamento de Memória):
     *      "alocar memória sob demanda usando malloc(sizeof(TipoStruct))
     *       apenas para o registro que efetivamente existe."
     * Aqui fazemos o malloc, só quando o usuário realmente cadastra um jogo.
     * Se o malloc falhar (NULL), avisamos e cancelamos.
     * Nosso obj: usar memória sob demanda e não usar um ponteiro NULL, o que derrubaria o programa. */
    v[i] = (p_jogo) malloc(sizeof(jogo));
    if (v[i] == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }

    // 4 Lemos os demais campos do jogo. Obj: preencher o jogo que acabamos de alocar.
    v[i]->codigo = codigo;
    ler_texto("Titulo: ", v[i]->titulo, sizeof(v[i]->titulo));
    ler_texto("Desenvolvedora: ", v[i]->desenvolvedora, sizeof(v[i]->desenvolvedora));
    v[i]->preco = ler_float("Preco (R$): ");
    v[i]->horas = ler_float("Horas jogadas: ");

    printf("Jogo cadastrado com sucesso!\n");
}

/* Não req, aqui mostramos na tela os dados do jogo que está na posição i.
 * isso para não repetir os mesmos printf em três lugares:
 * Listar Todos, pela Busca e pelo Relatório. */
void imprimir_unico(int i) {
    if (i >= 0 && i < MAX && v[i] != NULL) {
        printf("Codigo.........: %d\n", v[i]->codigo);
        printf("Titulo.........: %s\n", v[i]->titulo);
        printf("Desenvolvedora.: %s\n", v[i]->desenvolvedora);
        printf("Preco..........: R$ %.2f\n", v[i]->preco);
        printf("Horas jogadas..: %.1f\n", v[i]->horas);
        printf("\n");
    }
}

/* REQUISITO (Menu - 2. Listar Todos):
 *   "Percorrer o vetor e exibir os dados de todos os ponteiros não nulos."
 * Aqui fizemos um laço que passa por todas as posições e mostra só as
 * que têm jogo (ponteiro diferente de NULL). Se não houver nenhum,avisamos.
 * queremos mostrar ao usuário tudo o que está cadastrado, pular os NULL evita
 * acessar um jogo que não existe, o que derrubaria o programa. */
void imprimir_tudo(void) {
    int i, total = 0;
    for (i = 0; i < MAX; i++) {
        if (v[i] != NULL) {
            imprimir_unico(i);
            total++;
        }
    }
    if (total == 0) {
        printf("Nenhum jogo cadastrado.\n");
    }
}

/* REQUISITO (Menu - 3. Buscar/Exibir Único):
 *   "Localizar um registro por código/identificador único e imprimir seus
 *    detalhes."
 * Aqui pedimos o código, usamos buscar_por_codigo() para achar o jogo e
 * imprimimos os detalhes. Se não existir, avisamos.
 * Obj: consultar um jogo específico sem precisar listar todos. */
void exibir_busca(void) {
    int codigo, pos;
    codigo = ler_int("Codigo a buscar: ");
    pos = buscar_por_codigo(codigo);
    if (pos == -1) {
        printf("Jogo com codigo %d nao encontrado.\n", codigo);
    } else {
        imprimir_unico(pos);
    }
}

/* REQUISITO (Menu - 4. Relatório/Estatística):
 *   "Exibir ao menos um cálculo analítico sobre a base cadastrada."
 *   Sugerido média de preço dos jogos ou o jogo com maior tempo jogado.
 * Aqui calculamos e mostramos os dois: a média de preço e o jogo com mais horas jogadas.
 */
void relatorio(void) {
    int i, total = 0, pos_maior = -1;
    float soma = 0;

    /* Somamos os preços, contamos os jogos e guardamos a posição do mais
       jogado. Objetivo: fazer todos os cálculos em uma única passada. */
    for (i = 0; i < MAX; i++) {
        if (v[i] != NULL) {
            soma += v[i]->preco;
            total++;
            if (pos_maior == -1 || v[i]->horas > v[pos_maior]->horas) {
                pos_maior = i;
            }
        }
    }

    /* Sem jogos não existe média, e dividir por zero seria um erro. */
    if (total == 0) {
        printf("Nenhum jogo cadastrado. Nao ha dados para o relatorio.\n");
        return;
    }

    printf("Total de jogos.............: %d\n", total);
    printf("Media de preco.............: R$ %.2f\n", soma / total);
    printf("Jogo com maior tempo jogado:\n\n");
    imprimir_unico(pos_maior);
}

//PERSISTÊNCIA EM ARQUIVO TEXTO

/* REQUISITO (Persistência - Carga de Dados na Inicialização):
 * "Ao abrir o programa, verificar a existência do arquivo de dados texto
 *    (modo "r" via fopen). Se o arquivo existir, realizar a leitura linha a
 *    linha (usando fgets e sscanf), alocando cada registro no vetor de
 *    ponteiros. Se o arquivo não existir, o sistema deve iniciar vazio sem
 *    gerar erro em tempo de execução."
 *
 * Aqui abrimos com fopen("r"); se não existir, devolvemos 0 e o sistema começa vazio.
 * Se existir, lemos cada linha com fgets, separamos os campos com sscanf e guardamos
 * cada jogo em uma posição do vetor (com malloc).
 * Formato de cada linha: codigo;titulo;desenvolvedora;preco;horas
 * Assim os jogos cadastrados em uma execução vão continuar existindo na próxima.
 * Sem isso, tudo se perderia ao fechar o programa. */
int carregar_arquivo(const char *nome) {
    FILE *fp;
    char linha[300];
    int i, carregados = 0;
    jogo temp;

    /* Abrimos em modo leitura seguindo dica "validar se a abertura do
       arquivo retornou NULL". NULL = primeira execução,
       então o sistema começa vazio. */
    fp = fopen(nome, "r");
    if (fp == NULL) {
        return 0;
    }

    // fgets lê uma linha por vez, até o fim do arquivo.
    while (fgets(linha, sizeof(linha), fp) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        if (linha[0] == '\0') continue; //ignora linhas em branco.

        /* O sscanf separa os 5 campos da linha, se não conseguir ler os 5, a
           linha está com formato errado e é ignorada, para não carregar dado
           quebrado nem derrubar o programa. */
        if (sscanf(linha, "%d;%100[^;];%80[^;];%f;%f",
                   &temp.codigo, temp.titulo, temp.desenvolvedora,
                   &temp.preco, &temp.horas) != 5) {
            printf("Aviso: linha com formato invalido ignorada: '%s'\n", linha);
            continue;
        }

        /* Não req, ignoramos código inválido ou repetido (por ex, se
           alguém editou o dados.txt à mão), porque o código é o identificador
           único. */
        if (temp.codigo <= 0 || buscar_por_codigo(temp.codigo) != -1) {
            printf("Aviso: codigo invalido ou repetido ignorado: '%s'\n", linha);
            continue;
        }

        /* Procuramos uma posição livre no vetor, se não houver, o arquivo
           tem mais jogos do que o MAX permite, e o resto é ignorado. */
        for (i = 0; i < MAX && v[i] != NULL; i++);
        if (i == MAX) {
            printf("Aviso: capacidade maxima atingida. Demais registros ignorados.\n");
            break;
        }

        /* REQUISITO (Gerenciamento de Memória): "Ao cadastrar um registro ou
         *   carregar do arquivo, alocar memória sob demanda usando malloc."
         * Aqui fazemos o malloc só para o registro que existe, e testamos o retorno.
         * Obj: gastar memória só com os jogos que estão no arquivo. */
        v[i] = (p_jogo) malloc(sizeof(jogo));
        if (v[i] == NULL) {
            printf("Erro: memoria insuficiente ao carregar o arquivo.\n");
            break;
        }
        *v[i] = temp;
        carregados++;
    }

    /* REQUISITO (Persistência - Fechamento de Streams):
     *   "Garantir que todo ponteiro FILE * aberto seja devidamente fechado
     *    com fclose()."
     * Feito, aqui fechamos o arquivo que foi aberto no início da função.
     * Obj: liberar o arquivo para o sistema e evitar que fique aberto sem necessidade. */
    fclose(fp);
    return carregados;
}

/* REQUISITO (Persistência - Gravação de Dados):
 *   "Implementar uma rotina (chamada ao salvar ou antes do encerramento) que
 *    percorra o vetor de ponteiros e salve todos os registros existentes no
 *    arquivo texto formatado (modo "w" via fopen e escrita formatada com
 *    fprintf)."
 *
 * Aqui abrimos com fopen("w"), percorremos o vetor e escrevemos uma
 * linha por jogo com fprintf, no mesmo formato lido na carga.
 * Retorna 1 se salvou e 0 se não conseguiu abrir o arquivo.
 * Obj: guardar em disco o que está na memória, que some quando o programa
 *  fecha. Usar o mesmo formato da carga garante que o que foi salvo possa ser lido de novo. */
int salvar_arquivo(const char *nome) {
    FILE *fp;
    int i;

    /* Abrimos em modo escrita seguindo a dica "validar se a abertura do
       arquivo retornou NULL". Se for NULL (por exemplo, pasta sem permissão),
       avisamos e não gravamos nada. */
    fp = fopen(nome, "w");
    if (fp == NULL) {
        printf("Erro: nao foi possivel abrir '%s' para escrita.\n", nome);
        return 0;
    }

    /* Uma linha por jogo existente (ponteiro diferente de NULL). */
    for (i = 0; i < MAX; i++) {
        if (v[i] != NULL) {
            fprintf(fp, "%d;%s;%s;%.2f;%.1f\n",
                    v[i]->codigo, v[i]->titulo, v[i]->desenvolvedora,
                    v[i]->preco, v[i]->horas);
        }
    }

    /* REQUISITO (Fechamento de Streams): todo FILE * aberto é fechado com
       fclose().

       Aqui fechamos o arquivo aberto no início da função.
       Obj: o fclose também garante que tudo foi realmente gravado no disco,
       sem ele, os últimos dados poderiam ficar de fora. */
    fclose(fp);
    return 1;
}
