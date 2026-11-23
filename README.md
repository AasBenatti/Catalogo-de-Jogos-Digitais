# Catálogo de Jogos Digitais (C)

Projeto prático em dupla da disciplina de Linguagem de Programação — sistema modular em C com TAD, vetor de ponteiros com alocação dinâmica e persistência em arquivo texto.

## 1. Identificação da dupla
Adryeli Assis Benatti — RA: 1630482423002
Nicolas Rufino — RA: 1630482423009

## 2. Motivo da escolha do tema
Escolhemos o tema 1, Catálogo de Jogos Digitais, porque ele nos interessou bastante, 
é um tema de que gostamos muito e reúne os assuntos em que temos mais dificuldade. 
Temos dificuldade com programação em geral e, em C, principalmente com vetores, ponteiros
e struct. Depois de pesquisar e consultar um pouco, vimos que o projeto exige os três ao
mesmo tempo e não de forma muito complicada, e isso nos obrigou a praticar para entender 
e aprender um pouco mais.

## 3. Como modelamos o problema

### Divisão de tarefas

Adryeli fez o TAD, que são os arquivos `jogo.h` e `jogo.c`. 
Nicolas fez o `main.c` (o menu), o `dados.txt`, o `CMakeLists.txt` e o README.

### Arquitetura das funções
O código segue o modelo de TAD, dividido em três arquivos:

- `jogo.h`: a interface. Tem a `struct Jogo`, o `typedef`, as constantes (`MAX` e `ARQUIVO`) e a lista (protótipos) de todas as funções.
- `jogo.c`: o código das funções, separadas por tarefa:
    - Memória: `inicializa` deixa as 100 posições do vetor vazias, e `liberar_memoria` devolve a memória antes de o programa fechar.
    - Entrada de dados: `ler_int`, `ler_float` e `ler_texto` leem o que o usuário digita e conferem se está correto.
    - Menu: `cadastrar`, `imprimir_tudo` (listar), `exibir_busca` (buscar por código) e `relatorio` fazem as opções 1 a 4.
    - Arquivo: `carregar_arquivo` lê o `dados.txt` quando o programa abre, e `salvar_arquivo` grava os jogos quando o usuário escolhe a opção 5.
- `main.c`: cria o vetor `p_jogo v[MAX]`, mostra o menu (`do..while` com `switch`) e chama as funções do `jogo.c`.

### Decisões sobre a organização de arquivos
- Dividimos o código em `jogo.h`, `jogo.c` e `main.c` porque o enunciado pede um TAD e porque assim o menu não precisa saber como os jogos são guardados.
- Deixamos o `dados.txt` na pasta do projeto, para o programa sempre achar o mesmo arquivo.
- Criamos o `CMakeLists.txt` porque sem ele o CLion não libera o botão de executar.
- Criamos o `.gitignore` para a pasta gerada pelo CLion não ir para o GitHub.

### Decisões sobre o tratamento dos dados
- Os jogos ficam em um vetor de ponteiros, com no máximo 100. A memória de um jogo só é reservada quando ele é cadastrado ou lido do arquivo, para gastar memória apenas com os jogos que existem.
- No `dados.txt`, cada linha é um jogo, com os campos separados por ponto e vírgula, porque esse formato é fácil de ler com `fgets` e `sscanf`. Se o arquivo não existir, o programa começa vazio.
- Não aceitamos dois jogos com o mesmo código, para a busca por código sempre achar um único jogo.
- Título e desenvolvedora não podem ficar em branco, porque uma linha com campo vazio não é lida na próxima vez que o programa abre, e o jogo se perderia.
- Se o usuário digitar um ponto e vírgula dentro de um texto, o programa troca por vírgula, porque o ponto e vírgula é o que separa os campos no arquivo.
- Preço e horas aceitam vírgula ou ponto, porque é assim que se digita no Brasil, mas não aceitam valor negativo, que não faz sentido para esses campos.
- Se uma linha do arquivo estiver errada ou com código repetido, o programa ignora a linha e mostra um aviso, para um arquivo editado à mão com erro não derrubar o programa.
- Se o salvamento falhar, o programa não fecha, para não perder os dados que estão na memória.

## 4. Compilação e execução

CLion:

1. Deixe todos os arquivos do projeto juntos na mesma pasta.
2. No CLion, vá em File > Open e escolha essa pasta.
3. Espere o CLion terminar de carregar o projeto.
4. Clique no botão de executar (▷).
5. O menu do catálogo aparece na parte de baixo da tela.

Sem o CLion, no terminal:

1. Abra o terminal dentro da pasta do projeto.
2. Digite `gcc -o catalogo main.c jogo.c` e aperte Enter. Isso cria o programa a partir dos arquivos `main.c` e `jogo.c`.
3. Digite `./catalogo` (no Windows: `catalogo.exe`) e aperte Enter. Isso abre o programa.

- só funciona se o gcc estiver instalado no computador.

## OBS de coisas que utilizamos:
- Usamos `static` em `fim_de_entrada` porque ela só é necessária dentro do `jogo.c`; 
assim ela fica escondida do resto do programa e fora do `jogo.h`.
- Usamos `strchr` porque ele permite detectar se o `fgets` leu a linha inteira; 
se faltou o `'\n'`, descartamos o excedente para ele não ser lido como a resposta seguinte.
- Usamos o `target_compile_definitions` no CMake porque o CLion roda o programa em outra pasta, 
e o caminho completo garante que ele sempre encontre o mesmo `dados.txt`.

## Menu

1. Cadastrar jogo
2. Listar todos
3. Buscar jogo por código
4. Relatório (total, média de preço e jogo com maior tempo jogado)
5. Salvar e sair
