//jogo.h  -  INTERFACE do TAD "Jogo"

/*
 *  REQUISITO (Arquitetura Modular e TAD) - modulo.h (Interface):
 *  "Definição da struct, constantes (#define), definição de tipos (typedef)
 *  e protótipos de todas as funções que operam sobre os dados."

 *  Separamos "o que existe" (este arquivo) de "como funciona" (jogo.c).
 *  Assim o main.c usa as funções sem precisar conhecer o código por dentro.
 */
#ifndef JOGO_H
#define JOGO_H

#include <stdio.h>

/* REQUISITO (Gerenciamento de Memória):
 *   "Definir uma capacidade máxima no .h (ex.: #define MAX 100)."
 *
 * Aqui fizemos a constante MAX = 100, o limite de jogos do catálogo.
 * Tamanho fixo ao vetor de ponteiros. */
#define MAX 100

/* REQUISITO (Persistência em Arquivo Texto): arquivo de dados em texto.
 * FEITO: aqui fizemos a constante com o nome do arquivo.
 * O CMakeLists.txt troca este valor pelo caminho completo da pasta do projeto.
 *
 * Obj: deixar o nome do arquivo em um lugar só, valendo para a leitura e gravação.
 * O CLion roda o programa em outra pasta, e o caminho completo garante que ele sempre use o mesmo dados.txt. */
#ifndef ARQUIVO
#define ARQUIVO "dados.txt"
#endif

/* REQUISITO
 *   "struct contendo ao menos 4 campos de tipos variados: números inteiros,
 *    texto/strings e ponto flutuante." Campos do tema: Código, Título,
 *    Desenvolvedora, Preço, Horas Jogadas.

 * Aqui fizemos a struct Jogo com 5 campos: int (codigo), texto (titulo
 * e desenvolvedora) e float (preco e horas). */
 // PS: consideramos variados e não diferentes, por isso repetimos char e float.
typedef struct Jogo {
    int codigo;               /* identificador único de cada jogo */
    char titulo[101];
    char desenvolvedora[81];
    float preco;
    float horas;              /* horas jogadas */
} jogo;

/* REQUISITO (Arquitetura - typedef): definição de tipos.
 * Aqui fizemos p_jogo como apelido de "jogo *" (ponteiro para jogo). */
typedef jogo *p_jogo;

/* REQUISITO (Gerenciamento de Memória):
 *   "Utilizar um vetor de ponteiros para a struct (ex.: TipoStruct *vetor[MAX])."
 * Aqui declaramos o vetor v, com MAX ponteiros para jogo.
 * Só a declaração (extern) fica aqui, a definição está em main.c.
 */

 /* main.c e jogo.c precisam usar o MESMO vetor, o extern avisa que a
 variável existe sem criar uma segunda cópia dela. */
extern p_jogo v[MAX];

/* Protótipos: o compilador precisa conhecer cada função antes de ela ser
 * chamada, por isso que todas estão listadas aqui. */

//Gerenciamento de Memória (Vetor de Ponteiros)
void inicializa(void);       //põe NULL em todos os ponteiros.
void liberar_memoria(void);  //free() em cada posição não nula.

//Não req: entrada de dados pelo teclado
int ler_linha(char *destino, int tam);  // lê uma linha com fgets retorna 0 se a entrada acabou.
void ler_texto(const char *mensagem, char *destino, int tam); // lê um texto que não pode ser vazio.
void trocar_ponto_e_virgula(char *texto); // troca ; por , para não quebrar o arquivo.
int ler_int(const char *mensagem);      // lê um inteiro, repetindo se for inválido.
float ler_float(const char *mensagem);  // lê um real, repetindo se for inválido.

//Funcionalidades do Menu de Console
void cadastrar(void);                   // 1 Cadastrar.
void imprimir_tudo(void);               // 2 Listar Todos.
void exibir_busca(void);                // 3 Buscar/Exibir Único.
void relatorio(void);                   // 4.Relatório/Estatística.
//5. Salvar e Sair usa salvar_arquivo() e liberar_memoria() (veja abaixo).

int buscar_por_codigo(int codigo);      // usada pela busca retorna a posição ou -1.
void imprimir_unico(int i);             // imprime o jogo da posição i.

//Persistência em Arquivo Texto
int carregar_arquivo(const char *nome); // carga na inicialização retorna quantos jogos leu.
int salvar_arquivo(const char *nome);   // gravação retorna 1 se salvou, 0 se deu erro.

#endif
