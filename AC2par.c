#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

// Condição de contorno fixa
void insere_borda_fixa(int N, int Aut[N][N], int Aut2[N+2][N+2], int valor_fixo)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Aut2[i+1][j+1] = Aut[i][j];
        }
    }

    for (int i = 0; i < N + 2; i++) {
        Aut2[i][0] = valor_fixo;
        Aut2[i][N+1] = valor_fixo;
    }
    for (int j = 0; j < N + 2; j++) {
        Aut2[0][j] = valor_fixo;
        Aut2[N+1][j] = valor_fixo;
    }
}

// Condição de contorno periódica
void insere_borda_periodica(int N, int Aut[N][N], int Aut2[N+2][N+2])
{
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            Aut2[i+1][j+1] = Aut[i][j];
        }
        Aut2[i+1][0] = Aut[i][N-1];
        Aut2[i+1][N+1] = Aut[i][0];
    }

    for (j = 0; j < N; j++) {
        Aut2[0][j+1] = Aut[N-1][j];
        Aut2[N+1][j+1] = Aut[0][j];
    }

    Aut2[0][0] = Aut[N-1][N-1];
    Aut2[0][N+1] = Aut[N-1][0];
    Aut2[N+1][0] = Aut[0][N-1];
    Aut2[N+1][N+1] = Aut[0][0];
}

// Inicialização do autômato
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
void imprime_matriz(int N, int Mat[N][N], const char* arq)
{
    FILE *file = fopen(arq, "w");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(file, "%d ", Mat[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

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

// Atualização do autômato
void atualiza_automato(int N, int Aut[N][N], int Aut2[N+2][N+2], float lambda, float mi)
{
    int Aut_temp[N][N];
    float s;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Aut_temp[i][j] = Aut[i][j];
            if (Aut[i][j] == 0) {
                if (Aut2[i][j+1] == 1 || Aut2[i+2][j+1] == 1 || Aut2[i+1][j] == 1 || Aut2[i+1][j+2] == 1) {
                    s = (float)rand() / RAND_MAX;
                    if (s <= lambda) {
                        Aut_temp[i][j] = 1;
                    }
                }
            } else {
                s = (float)rand() / RAND_MAX;
                if (s <= mi) {
                    Aut_temp[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Aut[i][j] = Aut_temp[i][j];
        }
    }
}

int main()
{
    setlocale(LC_NUMERIC, "C");
    setlocale(LC_ALL, "Portuguese");

 // srand(time(NULL));
    int seed = 484842;
    srand(seed);

    int N = 100, tmax = 100;                                 // Dimensão do autômato, instante de tempo final
    float porc = 0.05, mi = 0.375, lambda = 0.25;            // Proporção inicial de estados ocupados e parâmetros utilizados
    int Aut[N][N], Aut2[N+2][N+2];
    int zeros[tmax], uns[tmax];
    int valor_borda = 0;

    inicializa_matriz(N, Aut, porc);
    printf("Estado inicial:\n");
    imprime_matriz(N, Aut, NULL);

    FILE *contagem = fopen("contagem_ACPAR.txt", "w");
    fprintf(contagem, "Tempo, Zeros, Uns\n");

    for (int t = 0; t < tmax; t++) {
     // insere_borda_periodica(N, Aut, Aut2);
        insere_borda_fixa(N, Aut, Aut2, valor_borda);
        atualiza_automato(N, Aut, Aut2, lambda, mi);

        conta_celulas(N, Aut, &zeros[t], &uns[t]);
        fprintf(contagem, "%d, %d, %d\n", t, zeros[t], uns[t]);
    }
    fclose(contagem);

    printf("\nEstado final:\n");
    imprime_matriz(N, Aut, "ACPAR.txt");

    FILE *gnuplot = popen("gnuplot -persistent", "w");
    fprintf(gnuplot, "set title 'Evolução dos Zeros e Uns - ACPAR'\n");
    fprintf(gnuplot, "set xlabel 'Tempo'\n");
    fprintf(gnuplot, "set ylabel 'Quantidade'\n");
    fprintf(gnuplot, "set key outside\n");
    fprintf(gnuplot, "plot 'contagem_ACPAR.txt' using 1:2 with lines title 'Zeros', \\\n");
    fprintf(gnuplot, "     'contagem_ACPAR.txt' using 1:3 with lines title 'Uns'\n");
    pclose(gnuplot);

    return 0;
}
