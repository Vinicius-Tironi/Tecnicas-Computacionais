#include <stdio.h>
#include<math.h>
#include<locale.h>
#include <stdlib.h>

double H = 10000;                                          // Parâmetros
double n = 0.01;
double K = powf(10,6);
double r = 0.2;
double nb = 0.33;
double mb = 0.66;
double e = 10;
double a = 1.0;

double lambda(double B)
{
    return B / (K + B);
}

double dSdt(double t, double S, double I, double B)        // Humanos Susceptíveis
{
    double A = 0.5 + 0.5 * sin(2 * M_PI * t / 365);        // Taxa de contato modificada por uma função periódica
    return n * (H - S) - a * lambda(B) * S;
}

double dIdt(double t, double S, double I, double B)        // Humanos Infectados
{
    double A = 0.5 + 0.5 * sin(2 * M_PI * t / 365);
    return a * lambda(B) * S - r * I;
}

double dBdt(double t, double S, double I, double B)        // Toxina V. cholerae presente na água
{
    double E = e + e * sin(2 * M_PI * t / 365);
    return B * (nb - mb) + e * I;
}

void runge_kutta(double *S, double *I, double *B, double *t, double h)       // Runge-Kutta de Quarta Ordem
{
    double k1, l1, j1;
    double k2, l2, j2;
    double k3, l3, j3;
    double k4, l4, j4;

    k1 = h * dSdt(*t, *S, *I, *B);
    l1 = h * dIdt(*t, *S, *I, *B);
    j1 = h * dBdt(*t, *S, *I, *B);

    k2 = h * dSdt(*t + h / 2, *S + k1 / 2, *I + l1 / 2, *B + j1 / 2);
    l2 = h * dIdt(*t + h / 2, *S + k1 / 2, *I + l1 / 2, *B + j1 / 2);
    j2 = h * dBdt(*t + h / 2, *S + k1 / 2, *I + l1 / 2, *B + j1 / 2);

    k3 = h * dSdt(*t + h / 2, *S + k2 / 2, *I + l2 / 2, *B + j2 / 2);
    l3 = h * dIdt(*t + h / 2, *S + k2 / 2, *I + l2 / 2, *B + j2 / 2);
    j3 = h * dBdt(*t + h / 2, *S + k2 / 2, *I + l2 / 2, *B + j2 / 2);

    k4 = h * dSdt(*t + h, *S + k3, *I + l3, *B + j3);
    l4 = h * dIdt(*t + h, *S + k3, *I + l3, *B + j3);
    j4 = h * dBdt(*t + h, *S + k3, *I + l3, *B + j3);

    *S += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    *I += (l1 + 2 * l2 + 2 * l3 + l4) / 6;
    *B += (j1 + 2 * j2 + 2 * j3 + j4) / 6;
    *t += h;
}

void plot_dados()                                                   // Plotagem dos dados
{
    FILE *gp = popen("gnuplot -persistent", "w");

    fprintf(gp, "set xlabel 'Tempo' font ', 18'\n");
    fprintf(gp, "set ylabel 'Populações' font ', 18'\n");
    fprintf(gp, "set title 'S, I, e B ao longo do tempo' font ', 18'\n");
    fprintf(gp, "set key right top font ', 12'\n\n");
    fprintf(gp, "set xrange [0:360]\n");
    fprintf(gp, "set xtics font ', 12'\n");
    fprintf(gp, "set ytics font ', 12'\n");
   // fprintf(gp, "set yrange [0:13000]\n");
    fprintf(gp, "set style line 1 lt 2 lw 1 lc rgb 'black' dt 2\n");

    fprintf(gp, "plot 'dados.txt' using 1:2 with lines title 'S', \\\n");
    fprintf(gp, "     'dados.txt' using 1:3 with lines title 'I', \\\n");
    fprintf(gp, "     'dados.txt' using 1:4 with lines title 'B', \\\n");
    fprintf(gp, "     6600 with lines title 'Sc = 6600' linestyle 1 , 0 with lines title '0' linestyle 1\n");

    fclose(gp);
}

int main()
{
setlocale(LC_ALL, "Portuguese");
setlocale(LC_NUMERIC, "C");

    double S = H;                                           // Variáveis de estado
    double I = 100.0;
    double B = 50.0;
    double h = 0.01;                                        // Tempo e passo de tempo
    double t = 0.0;
    double T = 365.0;

    double Sc = ((r*K*(mb-nb))/e);                          // Quantidade crítica de indivíduos susceptíveis
    double R = (S*(a * e))/(r*K*(mb-nb));                   // Número básico de reprodução R_0

    printf("Sc = %lf\n" , Sc);
    printf("R = %lf\n" , R);
    printf("nb-mb = %lf\n" , (nb - mb));

    FILE *fp = fopen("dados.txt", "w");

    while (t <= T)
    {
        fprintf(fp, "%f %f %f %f\n", t, S, I, B);
        runge_kutta(&S, &I, &B, &t, h);
    }

    fclose(fp);
    plot_dados();
}

