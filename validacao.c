#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "validacao.h"

static void removerQuebraLinha(char texto[]) {
    texto[strcspn(texto, "\n")] = '\0';
}

static void limparRestoLinha(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static int contemPontoEVirgula(const char texto[]) {
    return strchr(texto, ';') != NULL;
}

static int stringVazia(const char texto[]) {
    return texto[0] == '\0';
}

static int somenteLetrasEspacos(const char texto[]) {
    int i;

    if (stringVazia(texto)) {
        return 0;
    }

    for (i = 0; texto[i] != '\0'; i++) {
        unsigned char c = (unsigned char) texto[i];

        if (!(isalpha(c) || isspace(c) || c >= 128)) {
            return 0;
        }
    }

    return 1;
}

int somenteDigitos(const char texto[]) {
    int i;

    if (stringVazia(texto)) {
        return 0;
    }

    for (i = 0; texto[i] != '\0'; i++) {
        if (!isdigit((unsigned char) texto[i])) {
            return 0;
        }
    }

    return 1;
}

static int textoLivreValido(const char texto[]) {
    if (stringVazia(texto)) {
        return 0;
    }

    if (contemPontoEVirgula(texto)) {
        return 0;
    }

    return 1;
}

static int anoBissexto(int ano) {
    if ((ano % 400) == 0) {
        return 1;
    }

    if ((ano % 100) == 0) {
        return 0;
    }

    if ((ano % 4) == 0) {
        return 1;
    }

    return 0;
}

int dataValida(const char data[]) {
    int dia, mes, ano;
    char extra;
    int diasMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (strlen(data) != 10) {
        return 0;
    }

    if (sscanf(data, "%2d/%2d/%4d%c", &dia, &mes, &ano, &extra) != 3) {
        return 0;
    }

    if (data[2] != '/' || data[5] != '/') {
        return 0;
    }

    if (ano < 1900 || mes < 1 || mes > 12) {
        return 0;
    }

    if (anoBissexto(ano)) {
        diasMes[2] = 29;
    }

    if (dia < 1 || dia > diasMes[mes]) {
        return 0;
    }

    return 1;
}

int horaValida(const char hora[]) {
    int h, m;
    char extra;

    if (strlen(hora) != 5) {
        return 0;
    }

    if (hora[2] != ':') {
        return 0;
    }

    if (sscanf(hora, "%2d:%2d%c", &h, &m, &extra) != 2) {
        return 0;
    }

    if (h < 0 || h > 23 || m < 0 || m > 59) {
        return 0;
    }

    return 1;
}

int dataNaoFutura(const char data[]) {
    int dia, mes, ano;
    time_t agora;
    struct tm *dataAtual;

    if (!dataValida(data)) {
        return 0;
    }

    sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano);

    agora = time(NULL);
    dataAtual = localtime(&agora);

    if (ano > dataAtual->tm_year + 1900) {
        return 0;
    }

    if (ano == dataAtual->tm_year + 1900 && mes > dataAtual->tm_mon + 1) {
        return 0;
    }

    if (ano == dataAtual->tm_year + 1900 && mes == dataAtual->tm_mon + 1 && dia > dataAtual->tm_mday) {
        return 0;
    }

    return 1;
}

int dataHoraNaoFutura(const char data[], const char hora[]) {
    int dia, mes, ano, h, m;
    time_t agora;
    struct tm consulta;
    time_t tempoConsulta;

    if (!dataValida(data) || !horaValida(hora)) {
        return 0;
    }

    sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano);
    sscanf(hora, "%2d:%2d", &h, &m);

    memset(&consulta, 0, sizeof(consulta));
    consulta.tm_mday = dia;
    consulta.tm_mon = mes - 1;
    consulta.tm_year = ano - 1900;
    consulta.tm_hour = h;
    consulta.tm_min = m;
    consulta.tm_sec = 0;
    consulta.tm_isdst = -1;

    tempoConsulta = mktime(&consulta);
    agora = time(NULL);

    if (tempoConsulta == (time_t) -1) {
        return 0;
    }

    if (difftime(tempoConsulta, agora) > 0) {
        return 0;
    }

    return 1;
}

int lerInteiro(const char mensagem[]) {
    char linha[100];
    char *fim;
    long valor;

    while (1) {
        printf("%s", mensagem);

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        if (strchr(linha, '\n') == NULL) {
            limparRestoLinha();
        }

        removerQuebraLinha(linha);

        if (stringVazia(linha)) {
            printf("Digite um numero inteiro.\n");
            continue;
        }

        valor = strtol(linha, &fim, 10);

        if (*fim == '\0') {
            return (int) valor;
        }

        printf("Entrada invalida. Digite apenas numeros inteiros.\n");
    }
}

int lerInteiroPositivo(const char mensagem[]) {
    int valor;

    do {
        valor = lerInteiro(mensagem);

        if (valor <= 0) {
            printf("Digite um numero inteiro positivo.\n");
        }
    } while (valor <= 0);

    return valor;
}

int lerInteiroIntervalo(const char mensagem[], int minimo, int maximo) {
    int valor;

    do {
        valor = lerInteiro(mensagem);

        if (valor < minimo || valor > maximo) {
            printf("Digite uma opcao entre %d e %d.\n", minimo, maximo);
        }
    } while (valor < minimo || valor > maximo);

    return valor;
}

int cpfValido(const char cpf[]) {
    int i;
    int soma;
    int resto;
    int d10;
    int d11;
    int todosIguais = 1;

    if (strlen(cpf) != 11) {
        return 0;
    }

    if (!somenteDigitos(cpf)) {
        return 0;
    }

    for (i = 1; i < 11; i++) {
        if (cpf[i] != cpf[0]) {
            todosIguais = 0;
            break;
        }
    }

    if (todosIguais) {
        return 0;
    }

    soma = 0;

    for (i = 0; i < 9; i++) {
        soma += (cpf[i] - '0') * (10 - i);
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1) {
        d10 = 0;
    } else {
        d10 = 11 - resto;
    }

    if (d10 != cpf[9] - '0') {
        return 0;
    }

    soma = 0;

    for (i = 1; i < 10; i++) {
        soma += (cpf[i] - '0') * (11 - i);
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1) {
        d11 = 0;
    } else {
        d11 = 11 - resto;
    }

    if (d11 != cpf[10] - '0') {
        return 0;
    }

    return 1;
}

int lerTextoValidado(const char mensagem[], char texto[], int tamanho, int tipo) {
    char entrada[500];
    int valido;

    while (1) {
        valido = 1;

        printf("%s", mensagem);
        printf("\nDigite 0 para cancelar.\n> ");

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }

        if (strchr(entrada, '\n') == NULL) {
            limparRestoLinha();
        }

        removerQuebraLinha(entrada);

        if (strcmp(entrada, "0") == 0) {
            return 0;
        }

        if (stringVazia(entrada)) {
            printf("Campo vazio. Digite novamente.\n");
            continue;
        }

        if ((int) strlen(entrada) >= tamanho) {
            printf("Texto muito grande. Limite de %d caracteres.\n", tamanho - 1);
            continue;
        }

        if (contemPontoEVirgula(entrada)) {
            printf("O caractere ';' nao e permitido.\n");
            continue;
        }

        switch (tipo) {
            case TEXTO_NOME:
                if (!somenteLetrasEspacos(entrada)) {
                    printf("Digite apenas letras e espacos.\n");
                    valido = 0;
                }
                break;

            case TEXTO_CPF:
                if (!cpfValido(entrada)) {
                    printf("CPF invalido. Digite um CPF real com 11 numeros.\n");
                    valido = 0;
                }
                break;

            case TEXTO_TELEFONE:
                if (!somenteDigitos(entrada) || strlen(entrada) < 8 || strlen(entrada) > 15) {
                    printf("Telefone invalido. Digite apenas numeros, entre 8 e 15 digitos.\n");
                    valido = 0;
                }
                break;

            case TEXTO_DATA:
                if (!dataValida(entrada)) {
                    printf("Data invalida. Use o formato dd/mm/aaaa.\n");
                    valido = 0;
                }
                break;

            case TEXTO_HORA:
                if (!horaValida(entrada)) {
                    printf("Hora invalida. Use o formato hh:mm.\n");
                    valido = 0;
                }
                break;

            case TEXTO_LIVRE:
                if (!textoLivreValido(entrada)) {
                    printf("Texto invalido. O campo nao pode estar vazio nem conter ';'.\n");
                    valido = 0;
                }
                break;

            default:
                printf("Tipo de validacao desconhecido.\n");
                valido = 0;
        }

        if (valido) {
            strcpy(texto, entrada);
            return 1;
        }
    }
}
