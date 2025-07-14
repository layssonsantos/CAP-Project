/*
 * Jogo da Forca completo em C
 * Com modularização, temas por arquivo, placar binário atualizado, estatísticas e ASCII art
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

#define MAX_PALAVRAS    500
#define MAX_TEMA        10
#define MAX_NOME_ARQ    64
#define MAX_LINHA       128
#define MAX_PONTOS      100  // agora permite até 100 jogadores únicos
#define MAX_HANGMAN     7

/* Estruturas */
typedef struct {
    char nome_jogador[40];
    int pontuacao;            // soma cumulativa de tentativas_restantes
    time_t ultimo_jogo;
} Recorde;

/* ASCII art dos estágios da forca */
const char *hangman_stages[MAX_HANGMAN] = {
    "  +---+\n      |\n      |\n      |\n     ===\n",
    "  +---+\n  O   |\n      |\n      |\n     ===\n",
    "  +---+\n  O   |\n  |   |\n      |\n     ===\n",
    "  +---+\n  O   |\n /|   |\n      |\n     ===\n",
    "  +---+\n  O   |\n /|\\  |\n      |\n     ===\n",
    "  +---+\n  O   |\n /|\\  |\n /    |\n     ===\n",
    "  +---+\n  O   |\n /|\\  |\n / \\  |\n     ===\n"
};

/* Funções de I/O de texto */
int ler_palavras_tema(const char *arquivo, char lista[][MAX_LINHA]) {
    FILE *f = fopen(arquivo, "r");
    if (!f) return 0;
    int n = 0;
    char linha[MAX_LINHA];
    while (fgets(linha, sizeof linha, f) && n < MAX_PALAVRAS) {
        if (linha[0] == ';' || linha[0] == '\n') continue;
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) > 0)
            strcpy(lista[n++], linha);
    }
    fclose(f);
    return n;
}

/* Funções de placar binário com acumulação por nome */
int carregar_placar(const char *arquivo, Recorde vet[]) {
    FILE *f = fopen(arquivo, "rb");
    if (!f) return 0;
    int n = 0;
    while (fread(&vet[n], sizeof *vet, 1, f) == 1 && n < MAX_PONTOS) {
        n++;
    }
    fclose(f);
    return n;
}

void salvar_todos_recordes(const char *arquivo, Recorde vet[], int n) {
    FILE *f = fopen(arquivo, "wb");
    if (!f) return;
    fwrite(vet, sizeof *vet, n, f);
    fclose(f);
}

void atualiza_ou_insere_recorde(const char *arquivo, Recorde *novo) {
    Recorde vet[MAX_PONTOS];
    int n = carregar_placar(arquivo, vet);
    // busca por nome
    for (int i = 0; i < n; i++) {
        if (strcmp(vet[i].nome_jogador, novo->nome_jogador) == 0) {
            // acumula pontuação e atualiza data
            vet[i].pontuacao += novo->pontuacao;
            vet[i].ultimo_jogo = novo->ultimo_jogo;
            salvar_todos_recordes(arquivo, vet, n);
            return;
        }
    }
    // se não achou, insere novo
    if (n < MAX_PONTOS) {
        vet[n++] = *novo;
        salvar_todos_recordes(arquivo, vet, n);
    }
}

/* Estatísticas de temas (arquivo texto) */
void atualizar_estatistica(const char *arquivo, const char *tema) {
    FILE *f = fopen(arquivo, "r");
    if (!f) return;
    char linha[MAX_TEMA][MAX_LINHA];
    char nomes[MAX_TEMA][MAX_LINHA];
    int contagens[MAX_TEMA];
    int n = 0;
    // Leitura das linhas existentes
    while (fgets(linha[n], MAX_LINHA, f) && n < MAX_TEMA) {
        linha[n][strcspn(linha[n], "\r\n")] = '\0';
        if (sscanf(linha[n], "%[^:]:%d", nomes[n], &contagens[n]) != 2) {
            nomes[n][0] = '\0'; contagens[n] = 0;
        }
        n++;
    }
    fclose(f);
    // Atualiza contagem para o tema selecionado
    for (int i = 0; i < n; i++) {
        if (strcmp(nomes[i], tema) == 0) {
            contagens[i]++;
            break;
        }
    }
    // Reescreve o arquivo com contagens atualizadas
    f = fopen(arquivo, "w");
    if (!f) return;
    for (int i = 0; i < n; i++) fprintf(f, "%s:%d\n", nomes[i], contagens[i]);
    fclose(f);
}

/* Função de ordenação simples de strings */
void ordenar_palavras(char v[][MAX_LINHA], int n) {
    char tmp[MAX_LINHA];
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (strcmp(v[i], v[j]) > 0) {
                strcpy(tmp, v[i]); strcpy(v[i], v[j]); strcpy(v[j], tmp);
            }
        }
    }
}

/* Mecanismo de jogo */
void jogar_forca(const char *palavra, int max_erros) {
    int len = strlen(palavra);
    char pal_exib[len+1];
    for (int i = 0; i < len; i++) pal_exib[i] = '_';
    pal_exib[len] = '\0';
    char usadas[26] = "";
    int erros = 0;
    while (erros < max_erros && strcmp(pal_exib, palavra) != 0) {
        printf("%s\n", hangman_stages[erros]);
        printf("Palavra: %s\nLetras usadas: %s\n", pal_exib, usadas);
        printf("Chute uma letra: ");
        char buffer[16]; fgets(buffer, sizeof buffer, stdin);
        char c = toupper(buffer[0]);
        if (strchr(usadas, c)) continue;
        int acerto = 0;
        for (int i = 0; i < len; i++) {
            if (toupper(palavra[i]) == c) {
                pal_exib[i] = palavra[i]; acerto = 1;
            }
        }
        if (!acerto) erros++;
        int pos = strlen(usadas);
        usadas[pos] = c; usadas[pos+1] = '\0';
    }
    if (strcmp(pal_exib, palavra) == 0)
        printf("\nVocê venceu! Palavra: %s\n", palavra);
    else
        printf("\nVocê foi enforcado! Palavra correta: %s\n", palavra);
}

int main() {
    setlocale(LC_ALL, "");
    srand((unsigned)time(NULL));

    /* 1) Carregar temas disponíveis */
    const char *temas[MAX_TEMA] = {"animais", "frutas", "paises"};
    int qtd_temas = 3;
    printf("Escolha um tema:\n");
    for (int i = 0; i < qtd_temas; i++) printf(" %d) %s\n", i+1, temas[i]);
    int escolha; scanf("%d", &escolha); while (getchar() != '\n');

    /* 2) Carregar palavras do tema */
    char arquivo_pal[MAX_NOME_ARQ];
    snprintf(arquivo_pal, sizeof arquivo_pal, "palavras/%s.txt", temas[escolha-1]);
    char lista[MAX_PALAVRAS][MAX_LINHA];
    int n = ler_palavras_tema(arquivo_pal, lista);
    if (n == 0) { puts("Erro ao carregar palavras"); return 1; }

    /* 3) Sortear palavra aleatória */
    int idx = rand() % n;
    char *palavra = lista[idx];

    /* 4) Escolha dificuldade */
    printf("\nEscolha dificuldade: 1)Fácil 2)Médio 3)Difícil\n");
    int dif; scanf("%d", &dif); while (getchar() != '\n');
    int max_erros = (dif == 1 ? 10 : (dif == 2 ? 7 : 5));

    /* 5) Exibir placar atual */
    Recorde rc[MAX_PONTOS];
    int rcs = carregar_placar("placar.dat", rc);
    printf("\n-- Placar acumulado --\n");
    for (int i = 0; i < rcs; i++) {
        printf("%s - %d\n", rc[i].nome_jogador, rc[i].pontuacao);
    }

    /* 6) Jogar Forca */
    jogar_forca(palavra, max_erros);

    /* 7) Atualizar recorde do jogador */
    Recorde novo;
    printf("Digite seu nome: ");
    fgets(novo.nome_jogador, sizeof novo.nome_jogador, stdin);
    novo.nome_jogador[strcspn(novo.nome_jogador, "\r\n")] = '\0';
    novo.pontuacao = max_erros;
    novo.ultimo_jogo = time(NULL);
    atualiza_ou_insere_recorde("placar.dat", &novo);

    /* 8) Atualizar estatísticas de tema */
    atualizar_estatistica("estatisticas.txt", temas[escolha-1]);

    /* 9) Exibir placar atualizado */
    rcs = carregar_placar("placar.dat", rc);
    printf("\n-- Placar atualizado --\n");
    for (int i = 0; i < rcs; i++) {
        printf("%s - %d\n", rc[i].nome_jogador, rc[i].pontuacao);
    }

    return 0;
}
