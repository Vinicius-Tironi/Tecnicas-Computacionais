#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <time.h>


int main()
{
setlocale(LC_ALL, "Portuguese");
setlocale(LC_NUMERIC, "C");

    char dados_1[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Dolphins\\soc-dolphins.mtx";
    char dados_2[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Macaques\\mammalia-macaque-dominance.edges";
    char dados_3[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Insects\\insecta-ant-colony2-day31.edges";
    char dados_4[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Kangaroos\\mammalia-kangaroo-interactions.edges";
    char dados_5[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Jazz\\jazz.mtx";
    char dados_6[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Tribes\\soc-tribes.edges";
    char dados_7[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Metabolic\\bio-celegans.mtx";
    char dados_8[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Enzymes\\ENZYMES_g1.edges";

    // Tribes modificado
    char m_dados_6[] = "C:\\Users\\Vinicius\\Documents\\Vinícius\\Mestrado\\2º Semestre\\Técnicas Computacionais com Aplicação em Biomatemática\\Projeto 3 - Redes Complexas\\Data\\Tribes\\modificado_soc-tribes.edges";
    // modifica-se tribes para que os pesos de suas arestas sejam -4,4 no lugar de -1,1, a fim de se melhorar a visualização no gráfico da Rede.
    // os caracterizadores topológicos são calculados utilizando o conjunto de dados original, e não o modificado.

    FILE *entrada = fopen(dados_1, "r");

    int n_linhas = 0;
    int n_colunas = 0;
    char Linha[256];
    int i, j, k;

    // quantidade de linhas
    while (fgets(Linha, sizeof(Linha), entrada))
    {
        n_linhas++;
    }
    printf("Número de linhas no arquivo: %d\n", n_linhas);
    rewind(entrada); // retorna o ponteiro no começo do arquivo lido

    // quantidade de colunas
    if (fgets(Linha, sizeof(Linha), entrada)) {
        char *token = strtok(Linha, " \t\n");
        while (token != NULL) {
            n_colunas++;
            token = strtok(NULL, " \t\n");
        }
    }
    printf("Número de colunas no arquivo: %d\n", n_colunas);
    rewind(entrada); // nao fechamos entrada porque ele vai ser lido dentro das funções

    if (n_colunas == 2) // se o arquivo tem duas colunas, é uma rede sem peso
    {
        nao_direcional_sem_peso(entrada, n_linhas, n_colunas);
    }
    if(n_colunas == 3) // se o arquivo três colunas, é uma rede com peso
    {
        nao_direcional_com_peso(entrada, n_linhas, n_colunas);
    }
}

void nao_direcional_sem_peso(FILE *entrada, int n_linhas, int n_colunas)
{
    printf("Rede Neural Sem Peso\n");
    int i, j, k;

    // o maior valor da rede é o tamanho da matriz
    int LA[n_linhas][n_colunas];
    int N = 0;
    for (i = 0; i < n_linhas; i++)
        {
        for (j = 0; j < n_colunas; j++)
        {
            fscanf(entrada, "%d", &LA[i][j]);
            if (LA[i][j] > N)
            {
                N = LA[i][j];
            }
        }
    }
    printf("Maior valor (dimensão da matriz): %d\n", N);

        // Construção da matriz de adjacência
        int MA[N][N];
        int n_conexoes = 0;

        // Inicializa a matriz de adjacência com zeros
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                MA[i][j] = 0;
            }
        }

        // Preenche a matriz de adjacência com base nos dados lidos
        for (i = 0; i < n_linhas; i++)
        {
            MA[LA[i][0] - 1][LA[i][1] - 1] = 1;
            MA[LA[i][1] - 1][LA[i][0] - 1] = 1;
        }

        // Calcula o número de conexões
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                n_conexoes += MA[i][j];
            }
        }
        printf("Número de conexões: %d\n", n_conexoes / 2);

        // Salva a matriz de adjacência em um arquivo
        FILE *saida_matriz_adjacencia = fopen("matriz_adjacencia.mtx", "w");
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                fprintf(saida_matriz_adjacencia, "%d ", MA[i][j]);
            }
            fprintf(saida_matriz_adjacencia, "\n");
        }
        fclose(saida_matriz_adjacencia);

        // Densidade de conexões
        float d_conexoes = (float)n_conexoes / (N * (N - 1));
        printf("Densidade de Conexões: %f \n", d_conexoes);

        // Grau de conectividade para cada vértice
        int grau_vertices[N];
        for (i = 0; i < N; i++)
        {
            grau_vertices[i] = 0;
            for (j = 0; j < N; j++)
            {
                grau_vertices[i] += MA[i][j];
            }
        }

        // Motra o grau de conectividade de cada vértice
        for (i = 0; i < N; i++) {
            printf("Vértice: %d - Grau de conectividade: %d\n", i + 1, grau_vertices[i]);
        }

        // Determina o maior e o menor grau de conectividade
        int menor_grau = grau_vertices[0], maior_grau = grau_vertices[0];
        int menor_vertice = 1, maior_vertice = 1;
        float soma_grau_conectividade = 0;

        for (i = 0; i < N; i++)
            {
            soma_grau_conectividade += grau_vertices[i];
            if (grau_vertices[i] < menor_grau)
            {
                menor_grau = grau_vertices[i];
                menor_vertice = i + 1;
            }
            if (grau_vertices[i] > maior_grau)
            {
                maior_grau = grau_vertices[i];
                maior_vertice = i + 1;
            }
        }

        float media_grau_conectividade = soma_grau_conectividade / N;
        printf("Menor Grau: %d (Vértice %d), Maior Grau: %d (Vértice %d)\n", menor_grau, menor_vertice, maior_grau, maior_vertice);
        printf("Média Grau de Conectividade: %f\n", media_grau_conectividade);


        // distribuiçao de conectividade

        int max_grau = 0;
        for (i = 0; i < N; i++)
        {
            if (grau_vertices[i] > max_grau) {
                max_grau = grau_vertices[i];
            }
        }

        int frequencia[max_grau + 1];
        for (i = 0; i <= max_grau; i++)
        {
            frequencia[i] = 0;
        }

        for (i = 0; i < N; i++)
        {
            frequencia[grau_vertices[i]]++;
        }

        for (i = 0; i <= max_grau; i++)
        {
            if (frequencia[i] > 0)
            {
                printf("Grau %d: %d vértices\n", i, frequencia[i]);
            }
        }

       // Salvar os dados em um arquivo para o gnuplot
        FILE *arquivo_dados = fopen("distribuicao_conectividade.dat", "w");
        for (i = 0; i <= max_grau; i++)
        {
            if (frequencia[i] > 0)
            {
                fprintf(arquivo_dados, "%d %d\n", i, frequencia[i]);
            }
        }
        fclose(arquivo_dados);

        // plot da distribuição de conectividade
        FILE *gnuplot = popen("gnuplot -persistent", "w");
        fprintf(gnuplot, "set title 'Distribuição de Conectividade' font ', 18'\n");
        fprintf(gnuplot, "set xlabel 'Grau de Conectividade' font ', 18'\n");
        fprintf(gnuplot, "set ylabel 'Número de Vértices' font ', 18'\n");
        fprintf(gnuplot, "set key right top font ', 12'\n\n");
        fprintf(gnuplot, "set xtics font ', 12'\n");
        fprintf(gnuplot, "set ytics font ', 12'\n");
        fprintf(gnuplot, "set grid\n");
        fprintf(gnuplot, "set yrange [0:15]\n");

        fprintf(gnuplot, "plot 'distribuicao_conectividade.dat' with linespoints linestyle 1 title 'Distribuição de Conectividade'\n");
        fclose(gnuplot);


        // triâgulos e agrupamento global
        float C_total = 0;
        for (i = 0; i < N; i++)
            {
            int e_i = 0; // triângulos envolvendo o vértice i
            int k_i = 0; // grau de conectividade do vértice i

            // número de vizinhos de i
            for (j = 0; j < N; j++)
            {
                if (MA[i][j] == 1)
                {
                    k_i++;
                    for (k = j + 1; k < N; k++)
                    {
                        if (MA[i][k] == 1 && MA[j][k] == 1)
                        {
                            e_i++; // acréscimo no contador de triângulos
                        }
                    }
                }
            }

            // se o vértice tem k > 1, calcula o coeficiente
            if (k_i > 1)
            {
                float C_i = (float)(2 * e_i) / (k_i * (k_i - 1));
                C_total += C_i;
             //   printf("Vértice %d - Coeficiente de Agrupamento: %f\n", i + 1, C_i);
            } else
            {
             //   printf("Vértice %d - Coeficiente de Agrupamento: 0 (não forma triângulo)\n", i + 1);
            }
        }

        // Coeficiente de agrupamento global
        float C_global = C_total / N;
        printf("Coeficiente de Agrupamento Global: %f\n", C_global);

        // menor caminho médio
         int inf =99999;  // vértices não conectados apresentam distância infinita
         float soma_mc = 0 , D[N][N];
         float cm;
         for(i =0; i < N;i ++)      // matriz de distâncias
         {
            for(j =0; j < N; j ++)
            {
                if( i != j && MA[i][j]==0)
                {
                    D[i][j]= inf ;  // vértices não conectados => distância infinita
                }
                else
                {
                    D[i][j]= MA[i][j];
                }
            }
        }

        // Floyd-Warshall
        for(i =0; i < N;i ++)
            {
                for(j =0; j < N; j ++)
                {
                    for(k =0; k < N; k ++)
                    {
                        if( D[j][k] > (D[j][i] + D[i][k]) )      // considera o vértice k intermediário
                        {
                            D[j][k]= D [j][i]+ D [i][k];         // determina os menores caminhos
                        }
                    }
                }
            }

         for(i =0; i < N;i ++)
         {
            for(j =0; j < N; j ++)
            {
                if( i != j)
                {
                    soma_mc = soma_mc + D [i][j];
                }
            }
        }
         cm = soma_mc /( N *( N -1.0) ); // menor caminho médio
         printf ("Menor Caminho: %f\n", cm);
}


void nao_direcional_com_peso(FILE *entrada, int n_linhas, int n_colunas)
{
    printf("Rede Neural com Peso\n");
    int i, j, k;

        int LA[n_linhas][n_colunas];
        int N = 0;
        for (i = 0; i < n_linhas; i++)
            {
            for (j = 0; j < n_colunas; j++)
            {
                fscanf(entrada, "%d", &LA[i][j]);
                // Considera apenas as duas primeiras colunas para determinar N - a terceira coluna são os pesos
                if (j < 2 && LA[i][j] > N)
                {
                    N = LA[i][j];
                }
            }
        }

        fclose(entrada);
        printf("Maior valor (dimensão da matriz): %d\n", N);


        int MA[N][N];
        int n_conexoes = 0;

        // Inicializa a matriz de adjacência com zeros
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                MA[i][j] = 0;
            }
        }

        // a matriz de adjacência é construída com os pesos das conexões
        for (i = 0; i < n_linhas; i++)
            {
            int origem = LA[i][0] - 1;
            int destino = LA[i][1] - 1;
            int peso = LA[i][2];

            MA[origem][destino] = peso;
            MA[destino][origem] = peso;
            n_conexoes++;
        }

        printf("Matriz de Adjacência Ponderada construída com sucesso.\n");
        printf("Número de conexões: %d\n", n_conexoes);


        // Salva a matriz de adjacência com peso em um arquivo
        FILE *saida_matriz_adjacencia_peso = fopen("matriz_adjacencia_peso.edges", "w");
        for (i = 0; i < N; i++)
            {
            for (j = 0; j < N; j++)
            {
                fprintf(saida_matriz_adjacencia_peso, "%d ", MA[i][j]);
            }
            fprintf(saida_matriz_adjacencia_peso, "\n");
        }
        fclose(saida_matriz_adjacencia_peso);




        // Densidade de conexões
        float d_conexoes = (float)(2 * n_conexoes) / (N * (N - 1));
        printf("Densidade de Conexões: %f\n", d_conexoes);


        // Grau de conectividade para cada vértice
        int grau_vertices[N];
        for (i = 0; i < N; i++)
        {
            grau_vertices[i] = 0;
            for (j = 0; j < N; j++)
            {
                if (MA[i][j] != 0)
                {
                    grau_vertices[i] += 1;
                }
            }
        }

        // Mostra o grau de conectividade de cada vértice
        for (i = 0; i < N; i++)
        {
            printf("Vértice: %d - Grau de conectividade: %d\n", i + 1, grau_vertices[i]);
        }

        // Determina o maior e o menor grau de conectividade
        int menor_grau = grau_vertices[0], maior_grau = grau_vertices[0];
        int menor_vertice = 1, maior_vertice = 1;
        float soma_grau_conectividade = 0;

        for (i = 0; i < N; i++)
            {
            soma_grau_conectividade += grau_vertices[i];
            if (grau_vertices[i] < menor_grau)
            {
                menor_grau = grau_vertices[i];
                menor_vertice = i + 1;
            }
            if (grau_vertices[i] > maior_grau)
            {
                maior_grau = grau_vertices[i];
                maior_vertice = i + 1;
            }
        }

        // Média do grau de conectividade
        float media_grau_conectividade = soma_grau_conectividade / N;
        printf("Menor Grau: %d (Vértice %d), Maior Grau: %d (Vértice %d)\n", menor_grau, menor_vertice, maior_grau, maior_vertice);
        printf("Média Grau de Conectividade: %f\n", media_grau_conectividade);

        //  distribuiçao de conectividade
        int max_grau = 0;
        for (i = 0; i < N; i++)
            {
            if (grau_vertices[i] > max_grau)
            {
                max_grau = grau_vertices[i];
            }
        }

        int frequencia[max_grau + 1];
        for (i = 0; i <= max_grau; i++)
        {
            frequencia[i] = 0;
        }

        for (i = 0; i < N; i++)
        {
            frequencia[grau_vertices[i]]++;
        }

        for (i = 0; i <= max_grau; i++)
            {
                if (frequencia[i] > 0)
            {
                printf("Grau %d: %d vértices\n", i, frequencia[i]);
            }
        }

       // Salvar os dados em um arquivo para o gnuplot
        FILE *arquivo_dados = fopen("distribuicao_conectividade_peso.dat", "w");
        for (i = 0; i <= max_grau; i++)
            {
            if (frequencia[i] > 0)
            {
                fprintf(arquivo_dados, "%d %d\n", i, frequencia[i]);
            }
        }
        fclose(arquivo_dados);

        // plot da distribuição de conectividade
        FILE *gnuplot = popen("gnuplot -persistent", "w");
        fprintf(gnuplot, "set title 'Distribuição de Conectividade' font ', 18'\n");
        fprintf(gnuplot, "set xlabel 'Grau de Conectividade' font ', 18'\n");
        fprintf(gnuplot, "set ylabel 'Número de Vértices' font ', 18'\n");
        fprintf(gnuplot, "set key right top font ', 12'\n\n");
        fprintf(gnuplot, "set xtics font ', 12'\n");
        fprintf(gnuplot, "set ytics font ', 12'\n");
        fprintf(gnuplot, "set grid\n"); // Adiciona a grade ao gráfico
        fprintf(gnuplot, "set yrange [0:5]\n");
       // fprintf(gnuplot, "set xrange [9:72]\n");

        fprintf(gnuplot, "plot 'distribuicao_conectividade_peso.dat' with linespoints linestyle 1 title 'Distribuição de Conectividade'\n");
        fclose(gnuplot);

        // Triângulos e agrupamento global
        float C_total = 0;
        for (i = 0; i < N; i++)
            {
            int e_i = 0; // triângulos envolvendo o vértice i
            int k_i = 0; // Grau do vértice i
            // número de vizinhos de i
            for (j = 0; j < N; j++) {
                if (abs(MA[i][j]) > 0)               // nesta função utilizo abs() para o cálculo dos caracterizadores topológicos
                    {                                // por conta do Conjunto de Dados 6 - Tribes, que apresenta pesos negativos
                    k_i++;
                    for (k = j + 1; k < N; k++) {
                        if (abs(MA[i][k]) > 0 && abs(MA[j][k]) > 0)
                        {
                            e_i++; // contador de triângulos
                        }
                    }
                }
            }

            // Se o vértice k > 1, calcula o coeficiente
            if (k_i > 1)
                {
                float C_i = (float)(2 * e_i) / (k_i * (k_i - 1));
                C_total += C_i;
                // printf("Vértice %d - Coeficiente de Agrupamento: %f\n", i + 1, C_i);
            } else
            {
                // printf("Vértice %d - Coeficiente de Agrupamento: 0 (Menor que 2 vizinhos)\n", i + 1);
            }
        }

        // Coeficiente de agrupamento global
        float C_global = C_total / N;
        printf("Coeficiente de Agrupamento Global: %f\n", C_global);

        int inf = 99999;
        float soma_mc = 0, D[N][N];
        float cm;

        // menor caminho
        for (i = 0; i < N; i++)    //   matriz de distâncias
            {
            for (j = 0; j < N; j++)
            {
                if (i != j && MA[i][j] == 0) {
                    D[i][j] = inf;  // Sem conexão , distância infinita
                } else {
                    D[i][j] = abs(MA[i][j]);  // abs() por conta de Tribes
                }
            }
        }

        // Floyd-Warshall
        for (k = 0; k < N; k++)
            {
            for (i = 0; i < N; i++)
                {
                for (j = 0; j < N; j++)
                {
                    if (D[i][j] > D[i][k] + D[k][j])
                    {
                        D[i][j] = D[i][k] + D[k][j];
                    }
                }
            }
        }

        // menor caminho médio
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                if (i != j && D[i][j] < inf)
                {
                    soma_mc += D[i][j];
                }
            }
        }

        // Calcula o menor caminho médio
        cm = soma_mc / (N * (N - 1.0));
        printf("Menor Caminho: %f\n", cm);

}
