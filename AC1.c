#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

// Condição de contorno periódica
void insere_borda_periodica(int N, int Aut[N][N], int Aut2[N+2][N+2])
{
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            Aut2[i+1][j+1] = Aut[i][j];
        }
    }
    for (i = 0; i < N; i++) {
        Aut2[0][i+1] = Aut[N-1][i];
        Aut2[N+1][i+1] = Aut[0][i];
        Aut2[i+1][0] = Aut[i][N-1];
        Aut2[i+1][N+1] = Aut[i][0];
    }
    Aut2[0][0] = Aut[N-1][N-1];
    Aut2[0][N+1] = Aut[N-1][0];
    Aut2[N+1][0] = Aut[0][N-1];
    Aut2[N+1][N+1] = Aut[0][0];
}

// Inicialização do Autômato
void inicializa_matriz(int N, int Mat[N][N], float porc)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Mat[i][j] = 0;
        }
    }
    int total_celulas = porc * N * N;
    for (int t = 0; t < total_celulas; t++) {
        int i = rand() % N;
        int j = rand() % N;
        Mat[i][j] = 1;
    }
}

// Matriz no estado final t = 100
void imprime_matriz(int N, int Mat[N][N], const char* arq, int salvar_arquivo) {
    if (salvar_arquivo) {
        FILE *file = fopen(arq, "w");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fprintf(file, "%d ", Mat[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", Mat[i][j]);
        }
        printf("\n");
    }
}

// Contagem de estados vazios e ocupados (zeros e uns) ao longo do tempo
void conta_celulas(int N, int Mat[N][N], int* zeros, int* uns)
{
    *zeros = 0;
    *uns = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (Mat[i][j] == 0)
                (*zeros)++;
            else
                (*uns)++;
        }
    }
}

int main()
{
    setlocale(LC_NUMERIC, "C");
    setlocale(LC_ALL, "Portuguese");

//  srand(time(NULL));
    int seed = 484842;
    srand(seed);

    int N = 200, tmax = 100;                              // Dimensão do autômato, instante de tempo final
    float porc = 0.1, mi = 0.375, lambda = 0.25;         // Proporção inicial de estados ocupados e parâmetros utilizados
    int Aut[N][N], Aut2[N+2][N+2];
    int zeros[tmax], uns[tmax];

    inicializa_matriz(N, Aut, porc);
    printf("Estado inicial:\n");
    imprime_matriz(N, Aut, NULL, 0);

    for (int t = 0; t < tmax; t++) {
        insere_borda_periodica(N, Aut, Aut2);

        int i = rand() % N;
        int j = rand() % N;
        float s = (float)rand() / RAND_MAX;

        if (Aut[i][j] == 0) {
            if (Aut2[i][j+1] == 1 || Aut2[i+2][j+1] == 1 || Aut2[i+1][j] == 1 || Aut2[i+1][j+2] == 1) {
                if (s <= lambda) {
                    Aut[i][j] = 1;
                }
            }
        } else {
            if (s <= mi) {
                Aut[i][j] = 0;
            }
        }

        conta_celulas(N, Aut, &zeros[t], &uns[t]);
    }

    printf("\nEstado final:\n");
    imprime_matriz(N, Aut, "ACSEQ.txt", 1);

    FILE *file = fopen("contagem.txt", "w");
    fprintf(file, "Tempo, Zeros, Uns\n");
    for (int t = 0; t < tmax; t++) {
        fprintf(file, "%d, %d, %d\n", t, zeros[t], uns[t]);
    }
    fclose(file);

    FILE *gnuplot = popen("gnuplot -persistent", "w");
    fprintf(gnuplot, "set title 'Evolução dos Zeros e Uns'\n");
    fprintf(gnuplot, "set xlabel 'Tempo'\n");
    fprintf(gnuplot, "set ylabel 'Quantidade'\n");
    fprintf(gnuplot, "set key outside\n");
    fprintf(gnuplot, "plot 'contagem.txt' using 1:2 with lines title 'Zeros', \\\n");
    fprintf(gnuplot, "     'contagem.txt' using 1:3 with lines title 'Uns'\n");

    pclose(gnuplot);

    return 0;
}
