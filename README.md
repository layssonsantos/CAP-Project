# Jogo da Forca em C

**Jogo da Forca Modular em C** é um terminal game de adivinhação de palavras com temas configuráveis. Fácil de usar e com funcionalidades avançadas para gerenciar estatísticas e placares.

## Funcionalidades

* **Temas Dinâmicos**: palavras em arquivos texto (`palavras/animais.txt`, etc.)
* **Jogo Interativo**: gráficos ASCII, 7 estágios da forca, controle de tentativas e letras usadas
* **Matrizes**: (arrays 2D) como char lista[`MAX_PALAVRAS`][`MAX_LINHA`] para armazenar listas de palavras e hangman_stages para ASCII art
* **Dificuldades**: Fácil (10 tentativas), Médio (7), Difícil (5)
* **Placar Persistente**: armazena e acumula pontuação por jogador em `placar.dat` (binário)
* **Estatísticas de Tema**: conta partidas jogadas por tema em `estatisticas.txt`

## Tecnologias e Recursos Usados

* **Arrays** para palavras e registros de jogadores
* **Modularização** em funções: leitura de arquivos, lógica do jogo, gestão de placar/estatísticas
* **Manipulação de Strings**: `fgets`, `strcpy`, `strcmp`, `sscanf`
* **Arquivos Texto e Binário**: `fopen`, `fgets`, `fprintf`, `fread`, `fwrite`
* **Controle de Fluxo**: `for`, `while`, `if`
* **Aleatoriedade e Tempo**: `rand()`, `srand(time(NULL))`, `time()`

## Instalação e Execução

```bash
gcc -o forca forca.c
./forca
```
*Desenvolvido em C para praticar conhecimentos adquiridos na disciplina de Construção de Algoritmos e Programação.*
