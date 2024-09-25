#include <stdio.h>
#include<math.h>
#include<locale.h>
#include <stdlib.h>


int dia;
int parasitemia;

int main()
{
setlocale(LC_ALL, "Portuguese");
setlocale(LC_NUMERIC, "C");
FILE *arquivo;
FILE *gnuplot;

    char dados_1[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 1 - Interpolação\\s457.dat";
    char dados_2[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 1 - Interpolação\\s457a.dat";
    char dados_3[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 1 - Interpolação\\s607.dat";
    char dados_4[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 1 - Interpolação\\s607a.dat";
    char dados_5[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 1 - Interpolação\\s731.dat";
    char dados_6[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 1 - Interpolação\\s731a.dat";

    arquivo = fopen(dados_6, "r");

    int n;
    char linha[256];
    while (fscanf(arquivo, "%d %d", &dia, &parasitemia) == 2) // Os .dat têm tamanhos diferentes...
    {
        n = n + 1;
    }

    gnuplot = popen("gnuplot -persistent", "w");
    fprintf(gnuplot, "set title 'Gráfico Parasitemia (sem Log)' font ', 15'\n");
    fprintf(gnuplot, "set xlabel 'Dias' font ', 15'\n");
    fprintf(gnuplot, "set ylabel 'Parasitemia' font ', 15'\n");
    fprintf(gnuplot, "plot '-' using 1:2 with linespoints title 'Dados'\n");


rewind(arquivo);                     // volta ao início do arquivo: evita problemas de contagem com o índice n.
int i = 0;
int Dias[n], Dados[n];
float LogDados[n];

while (fscanf(arquivo, "%d %d", &dia, &parasitemia) == 2)
{
    if (i < n)
    {
        Dias[i] = dia;
        Dados[i] = parasitemia;
        i++;
    }
    else
    {
        Dias[i] = i;               // no caso de arquivos de tamanhos diferentes (sexuada, assexuada), os dias são preenchidos com o índice i (n+1,n+2...)
        Dados[i] = 0;              // e a parasitemia com zeros.
    }
    fprintf(gnuplot, "%d %d\n", dia, parasitemia);
}
pclose(gnuplot);


// Escala Log
for (i = 0; i < n; i++)
{
    LogDados[i] = log10(Dados[i] + 1);          // forma de melhor visualizar os dados faltantes e evitar indeterminações com o logaritmo.
    printf("Dia: %d; Parasitemia: %d => Log(Parasitemia) = %f\n", Dias[i], Dados[i], LogDados[i]);
}


FILE *log_gnuplot;

    log_gnuplot = popen("gnuplot -persistent", "w");
    fprintf(log_gnuplot, "set title 'Gráfico Parasitemia (escala Log)' font ', 15'\n");
    fprintf(log_gnuplot, "set xlabel 'Dias' font ', 15'\n");
    fprintf(log_gnuplot, "set ylabel 'Parasitemia' font ', 15'\n");
    fprintf(log_gnuplot, "set yrange [0:6]\n");

    fprintf(log_gnuplot, "plot '-' using 1:2 with linespoints title 'Dados'\n");
    for (int i = 0; i < n; i++)
    {
        fprintf(log_gnuplot, "%d %f\n", Dias[i], LogDados[i]);
    }
pclose(log_gnuplot);


// Interpolação Linear
for (int j = 0; j < i; j++)
{
    if (LogDados[j] == 5.0)                          // identifica os valores faltantes, agora marcados como log(p+1) = 5.
    {
        int k;
        for (k = j - 1; k >= 0 && LogDados[k] == 5.0; k--);           // estima estes utilizando interpolação linear.
        if (k >= 0)
        {
            int l;
            for (l = j + 1; l < i && LogDados[l] == 5.0; l++);
            if (l < i)
            {
                LogDados[j] = LogDados[k] + (float)(Dias[j] - Dias[k]) * (LogDados[l] - LogDados[k]) / (Dias[l] - Dias[k]);
            }
        }
    }
}

FILE *interp_gnuplot;

    interp_gnuplot = popen("gnuplot -persistent", "w");
    fprintf(interp_gnuplot, "set title 'Gráfico Parasitemia (Interpolado)' font ', 15'\n");
    fprintf(interp_gnuplot, "set xlabel 'Dias' font ', 15'\n");
    fprintf(interp_gnuplot, "set ylabel 'Parasitemia' font ', 15'\n");
    fprintf(interp_gnuplot, "set yrange [0:6]\n");

    fprintf(interp_gnuplot, "plot '-' using 1:2 with linespoints title 'Dados'\n");
    for (i = 0; i < n; i++)
    {
        fprintf(interp_gnuplot, "%d %f\n", Dias[i], LogDados[i]);
    }
pclose(interp_gnuplot);

printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * *\n");

// Máximos Locais e Máximo Global
float maximo = 0;
float primeiro_maximo;
int posicao_l;
int posicao_g;
int m = 0;

for (i = 0 ; i < n ; i ++)     // no if abaixo... m: contador de máximos locais => se m < 1, o máximo local identificado é o primeiro máximo local.
{
    if (LogDados[i] > LogDados[i-3] && LogDados[i] > LogDados[i-2] && LogDados[i] > LogDados[i-1] && LogDados[i] > LogDados[i+1] && LogDados[i] > LogDados[i+2] && LogDados[i] > LogDados[i+3] && m < 1)
    {
        primeiro_maximo = LogDados[i];                                              // identifica e armazena o primeiro máximo local e sua posição.
        posicao_l = i+1;
        maximo = LogDados[i];
        printf("Primeiro Máximo Local: %f, observado no dia %d;\n" , maximo, i+1);
        m = m + 1;
    }
    else if (LogDados[i] > LogDados[i-3] && LogDados[i] > LogDados[i-2] && LogDados[i] > LogDados[i-1] && LogDados[i] > LogDados[i+1] && LogDados[i] > LogDados[i+2] && LogDados[i] > LogDados[i+3] && m >= 1)
    {
        maximo = LogDados[i];                                                       // identifica os demais máximos locais e sua posição; não os armazena.
        printf("Máximo Local: %f, observado no dia %d;\n" , maximo, i+1);
        m = m + 1;
    }
}
printf("São observados %d máximos locais.\n" , m);

for (i = 0 ; i < n ; i ++)
{
    if (LogDados[i] > LogDados[i-3] && LogDados[i] > LogDados[i-2] && LogDados[i] > LogDados[i-1] && LogDados[i] > LogDados[i+1] && LogDados[i] > LogDados[i+2] && LogDados[i] > LogDados[i+3] && LogDados[i] > maximo)
    {
        maximo = LogDados[i];                   // identifica e armazena o máximo global e sua posição.
        posicao_g = i+1;
    }
}
printf("Máximo Global: %f, observado no dia %d.\n" , maximo, posicao_g);

printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * *\n");

// Dias Positivos

int dias_p = 0;

for (i = 0 ; i < n ; i ++)  // parasitemia = 10 => log10(parasitemia) = 1
{
    if (LogDados[i] >= 1)
    {
        dias_p = dias_p + 1;           // contador de dias positivos
    }
}
printf("São observados %d dias positivos.\n" , dias_p);
int Vet_dias_p[dias_p];
int j = 0;

for (i = 0 ; i < n ; i ++)           // armazena os dias positivos; utilizados para determinar o primeiro e o último dia positivo,
{                                    // o tamanho e a metade da série positiva.
    if (LogDados[i] >= 1)
    {
        Vet_dias_p[j++] = i+1;
    }
}

int primeiro_dia = Vet_dias_p[0];
printf("Primeiro dia positivo: %d;\n" , primeiro_dia);
int ultimo_dia = Vet_dias_p[dias_p - 1];
printf("Último dia positivo: %d;\n" , ultimo_dia);
int tam = ultimo_dia - primeiro_dia;
printf("Tamanho da série: %d - %d = %d dias;\n" , ultimo_dia, primeiro_dia, tam);
int metade_serie = primeiro_dia+(tam/2);
printf("Metade da série: dia %d.\n" , metade_serie);


// Proporção de observações positivas
int c_primeira_metade = 0;
int c_segunda_metade = 0;
float prop_primeira_metade;
float prop_segunda_metade;

for (int k = primeiro_dia ; k <= metade_serie; k ++)
{
    if (LogDados[k] >= 1)
    {
        c_primeira_metade = c_primeira_metade + 1;
    }
}
for (int k = metade_serie ; k <= ultimo_dia; k ++)
{
    if (LogDados[k] >= 1)
    {
        c_segunda_metade = c_segunda_metade + 1;
    }
}

prop_primeira_metade = (float)(c_primeira_metade)/(dias_p);               // calculam-se as proporções na primeira e segunda metade da série positiva.
prop_segunda_metade = (float)(c_segunda_metade)/(dias_p);

printf("Quantidade de dias positivos na primeira metade: %d\n" , c_primeira_metade);
printf("=> proporção primeira metade = %d/%d = %f;\n" , c_primeira_metade, dias_p, prop_primeira_metade);
printf("Quantidade de dias positivos na segunda metade: %d\n" , c_segunda_metade);
printf("=> proporção segunda metade = %d/%d = %f.\n" , c_segunda_metade, dias_p, prop_segunda_metade);

printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * *\n");


// Suavização da curva
float suav_LogDados[n];

suav_LogDados[0] = LogDados[0];            // primeiro e último pontos não são suavizados, pois, para estes, não existem pontos adjacentes.
for (int i = 1; i < n - 1; i++)
{
    suav_LogDados[i] = (LogDados[i-1] + LogDados[i] + LogDados[i+1]) / 3.0;        // média movel.
}
suav_LogDados[n-1] = LogDados[n-1];

FILE *suav_gnuplot;

    suav_gnuplot = popen("gnuplot -persistent", "w");
    fprintf(suav_gnuplot, "set title 'Gráfico Parasitemia (Curva Suavizada)' font ', 15'\n");
    fprintf(suav_gnuplot, "set xlabel 'Dias' font ', 15'\n");
    fprintf(suav_gnuplot, "set ylabel 'Parasitemia' font ', 15'\n");
    fprintf(suav_gnuplot, "set yrange [0:5]\n");

    fprintf(suav_gnuplot, "plot '-' using 1:2 with linespoints title 'Dados'\n");
    for (i = 0; i < n; i++)
    {
        fprintf(suav_gnuplot, "%d %f\n", Dias[i], suav_LogDados[i]);
    }
pclose(suav_gnuplot);


// Regressão Linear Sobre os Dados Suavizados    // primeiro dia positivo até o primeiro maximo local ; utiliza os dados suavizados
printf("Regressão Linear do dia %d (primeiro dia positivo) ao dia %d (primeiro máximo local):\n" , primeiro_dia, posicao_l);


int N = (posicao_l - primeiro_dia)+1;                                       // cálculo das estatísticas utilizadas na regressão linear.

float soma_x = 0;                                                          // cálculo das médias.
float media_x;
for (int i = (primeiro_dia)-1; i < (posicao_l); i++)
{
    soma_x = soma_x + Dias[i];
}
media_x = (soma_x)/(N);
//printf("Média Dias = %f\n" , media_x);

float soma_y = 0;
float media_y;
for (int i = (primeiro_dia)-1; i <= (posicao_l); i++)
{
    soma_y = soma_y + suav_LogDados[i];
}
media_y = (soma_y)/(N);
//printf("Média Parasitemia = %f\n" , media_y);

float s_x = 0, s_y = 0, r = 0;
for (int i = 0; i < N; i++)                                                   // cálculo dos desvios-padrão.
{
    s_x = s_x + (Dias[i] - media_x) * (Dias[i] - media_x);
    s_y = s_y + (suav_LogDados[i] - media_y) * (suav_LogDados[i] - media_y);
    r = r + (Dias[i] - media_x) * (suav_LogDados[i] - media_y);
}
s_x = sqrt(s_x / (N));
s_y = sqrt(s_y / (N));
r = r / ((N) * s_x * s_y);
printf("Dias: Média = %f; Desvio Padrão = %f;\n", media_x, s_x);
printf("Parasitemia: Média = %f; Desvio Padrão = %f;\n", media_y, s_y);
printf("Correlação Dias/Parasitemia = %f;\n" , r);

float beta = r * (s_y / s_x);                                                     // beta_1.
float intercepto = media_y - beta * media_x;                                      // beta_0.

printf("Coeficiente Angular da Regressão Linear (Beta): %f;\n", beta);
printf("Intercepto Vertical: %f;\n", intercepto);
printf("Equação da Reta de Regressão: y = %f + %f * x\n", intercepto, beta);     // reta da regressão linear.

printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * *\n");

// Área sob a curva de parasitemia  -   Método dos trapézios
float Area = 0;

for (int i = 1; i < n; i++)
{
    Area = Area + 0.5 * (suav_LogDados[i-1] + suav_LogDados[i]) * (Dias[i] - Dias[i-1]);
}
printf("Área sob a curva: %f\n", Area);

fclose(arquivo);
printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * *\n");

// Correlação Cruzada

// Correlação realizada entre os dados da série assexuada e sexuada;
// os arquivos .dat dados_1,_3 e _5 representam a série sexuada e .dat dados_2,_4 e 6 a série assexuada;
// paciente 1: dados_1,_2 ; paciente 2: dados_3,_4 e paciente 3: dados_5,_6.
// Já usamos x,y antes: S <- sexuada ; A <- assexuada.

FILE *sex_arquivo;                                 // a correlação cruzada considera pares de arquivos: .dat sexuado e seu .dat assexuado correspondente.
FILE *as_arquivo;                                  // consideram-se na correlação os dados na escala logarítmica; aqui repetem-se os passos feitos no início do código.
sex_arquivo = fopen(dados_5, "r");                 // se este código fosse modularizado, haveria muito menos esforço nesta parte!
as_arquivo = fopen(dados_6, "r");

int cr_n = 0;
char cr_linha[256];
while (fscanf(sex_arquivo, "%d %d", &dia, &parasitemia) == 2)     // tamanho do .dat
{
    cr_n = cr_n + 1;
}

rewind(sex_arquivo);

int i_sex = 0;
// int cr_Dias[cr_n];
int sex_Dados[cr_n];
float sex_LogDados[cr_n];

while (fscanf(sex_arquivo, "%d %d", &dia, &parasitemia) == 2 && i_sex < cr_n)
{
    sex_Dados[i_sex] = parasitemia;
    sex_LogDados[i_sex] = log10(sex_Dados[i_sex] + 1);
    i_sex++;
}

fclose(sex_arquivo);

int i_as = 0;
int as_Dados[cr_n];
float as_LogDados[cr_n];

while (fscanf(as_arquivo, "%d %d", &dia, &parasitemia) == 2 && i_as < cr_n)
{
    as_Dados[i_as] = parasitemia;
    as_LogDados[i_as] = log10(as_Dados[i_as] + 1);
    i_as++;
}
fclose(as_arquivo);

for (int i = 0; i < 1; i++)
{
//  printf("Dia: %d; Parasitemia: %d => Log(Parasitemia) = %f\n", cr_Dias[i], as_Dados[i], as_LogDados[i]);
}

float soma_S = 0;                                 // cálculo das médias.
float media_S;
for (int i = 0 ; i <= cr_n ; i++)
{
    soma_S = soma_S + sex_LogDados[i];
}
media_S = (soma_S)/(cr_n);

float soma_A = 0;
float media_A;
for (int i = 0 ; i <= cr_n ; i++)
{
    soma_A = soma_A + as_LogDados[i];
}
media_A = (soma_A)/(cr_n);

float sigma_S = 0, sigma_A = 0, sigma_SA = 0, r_d;        // cálculo dos valores de sigma.
for (int i = 0; i < cr_n; i++)
{
    sigma_S = sigma_S + (sex_LogDados[i] - media_S) * (sex_LogDados[i] - media_S);
    sigma_A = sigma_A + (as_LogDados[i] - media_A) * (as_LogDados[i] - media_A);
}
sigma_S = sqrt(sigma_S);
sigma_A = sqrt(sigma_A);

printf("Sexuada: Média = %f; sigma_S = %f;\n", media_S, sigma_S);
printf("Assexuada: Média = %f; sigma_A = %f;\n", media_A, sigma_A);


int d;
printf("Defina o atraso considerado na série sexuada:\n");                   // aqui, pode-se definir especificamente para qual valor de atraso se deseja
scanf("%d", &d);                                                             // calcular a correlação cruzada.
for (int i = 0; i < cr_n - d; i++)
{
    sigma_SA = sigma_SA + (as_LogDados[i] - media_A) * (sex_LogDados[i+d] - media_S);
}

r_d = sigma_SA / (sigma_S * sigma_A);
printf("Correlação Cruzada = %f, obtida ao considerar um atraso de %d dias na série sexuada.\n", r_d, d);

double correlacao_cruzada[cr_n];
int atraso[cr_n];

for (int D = 0; D < cr_n; D++)                                              // aqui, os valores de correlação cruzada e seus respectivos atrasos são armazenados
{                                                                           // em vetores; Assim, pode-se plotá-los.
    float sigma_SA = 0;
    for (int i = 0; i < cr_n - D; i++)
    {
        sigma_SA = sigma_SA + (as_LogDados[i] - media_A) * (sex_LogDados[i + D] - media_S);
    }
    correlacao_cruzada[D] = sigma_SA / (sigma_S * sigma_A);
    atraso[D] = D;
}

FILE *corr_gnuplot;

    corr_gnuplot = popen("gnuplot -persistent", "w");
    fprintf(corr_gnuplot, "set title 'Correlação Cruzada' font ', 15'\n");
    fprintf(corr_gnuplot, "set xlabel 'Atraso' font ', 15'\n");
    fprintf(corr_gnuplot, "set ylabel 'Correlação' font ', 15'\n");
    fprintf(corr_gnuplot, "set yrange [-1:1]\n");
    fprintf(corr_gnuplot, "set style line 1 lt 2 lw 1 lc rgb 'black' dt 2\n");

    fprintf(corr_gnuplot, "plot '-' using 1:2 with points title 'Correlação Cruzada' , \ 0 with lines linestyle 1\n");
    for (int i = 0; i < cr_n ; i++)
    {
      //  printf("Atraso: %d, Correlação Cruzada: %.6lf\n", atraso[i], correlacao_cruzada[i]);
        fprintf(corr_gnuplot, "%d %.6f\n", atraso[i], correlacao_cruzada[i]);
    }
pclose(corr_gnuplot);

printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * *\n");

}

