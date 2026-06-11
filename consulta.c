#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consulta.h"
#include "validacao.h"

struct consulta {
    int codigo;
    char data[11];
    char horario[6];
    char medico[100];
    char sintomas[300];
    char diagnostico[300];
    char prescricao[300];
    struct consulta *prox;
};

Consulta* criarListaConsultas(void) {
    return NULL;
}

static int proximoCodigoConsulta(Consulta *lista) {
    int maior = 0;

    while (lista != NULL) {
        if (lista->codigo > maior) {
            maior = lista->codigo;
        }

        lista = lista->prox;
    }

    return maior + 1;
}

Consulta* criarConsultaComDados(int codigo, const char data[], const char horario[], const char medico[], const char sintomas[], const char diagnostico[], const char prescricao[]) {
    Consulta *nova = (Consulta*) malloc(sizeof(Consulta));

    if (nova == NULL) {
        printf("Erro ao alocar memoria para consulta.\n");
        return NULL;
    }

    nova->codigo = codigo;
    strcpy(nova->data, data);
    strcpy(nova->horario, horario);
    strcpy(nova->medico, medico);
    strcpy(nova->sintomas, sintomas);
    strcpy(nova->diagnostico, diagnostico);
    strcpy(nova->prescricao, prescricao);
    nova->prox = NULL;

    return nova;
}

Consulta* inserirConsultaInicio(Consulta *lista, Consulta *nova) {
    if (nova == NULL) {
        return lista;
    }

    nova->prox = lista;
    return nova;
}

Consulta* cadastrarConsulta(Consulta *lista) {
    int codigo;
    int dataOk;
    int dataHoraOk;
    char data[11];
    char horario[6];
    char medico[100];
    char sintomas[300];
    char diagnostico[300];
    char prescricao[300];
    Consulta *nova;

    codigo = proximoCodigoConsulta(lista);

    do {
        if (!lerTextoValidado("Data da consulta (dd/mm/aaaa): ",
            data, sizeof(data), TEXTO_DATA)) {
            printf("Cadastro de consulta cancelado.\n");
            return lista;
        }

        dataOk = dataNaoFutura(data);

        if (!dataOk) {
            printf("Nao e permitido cadastrar consulta em data futura.\n");
        }

    } while (!dataOk);

    do {
        if (!lerTextoValidado("Horario da consulta (hh:mm): ",
            horario, sizeof(horario), TEXTO_HORA)) {
            printf("Cadastro de consulta cancelado.\n");
            return lista;
        }

        dataHoraOk = dataHoraNaoFutura(data, horario);

        if (!dataHoraOk) {
            printf("Nao e permitido cadastrar consulta em horario futuro.\n");
        }

    } while (!dataHoraOk);

    if (!lerTextoValidado("Medico responsavel: ",
        medico, sizeof(medico), TEXTO_NOME)) {
        printf("Cadastro de consulta cancelado.\n");
        return lista;
    }

    if (!lerTextoValidado("Sintomas relatados: ",
        sintomas, sizeof(sintomas), TEXTO_LIVRE)) {
        printf("Cadastro de consulta cancelado.\n");
        return lista;
    }

    if (!lerTextoValidado("Diagnostico: ",
        diagnostico, sizeof(diagnostico), TEXTO_LIVRE)) {
        printf("Cadastro de consulta cancelado.\n");
        return lista;
    }

    if (!lerTextoValidado("Prescricao medica: ",
        prescricao, sizeof(prescricao), TEXTO_LIVRE)) {
        printf("Cadastro de consulta cancelado.\n");
        return lista;
    }

    nova = criarConsultaComDados(codigo, data, horario, medico, sintomas, diagnostico, prescricao);

    if (nova == NULL) {
        printf("Erro ao cadastrar consulta.\n");
        return lista;
    }

    lista = inserirConsultaInicio(lista, nova);

    printf("Consulta registrada com sucesso. Codigo: %d\n", codigo);

    return lista;
}

void listarConsultas(Consulta *lista) {
    if (lista == NULL) {
        printf("Nenhuma consulta registrada.\n");
        return;
    }

    while (lista != NULL) {
        printf("\nConsulta\n");
        printf("Codigo: %d\n", lista->codigo);
        printf("Data: %s\n", lista->data);
        printf("Horario: %s\n", lista->horario);
        printf("Medico responsavel: %s\n", lista->medico);
        printf("Sintomas relatados: %s\n", lista->sintomas);
        printf("Diagnostico: %s\n", lista->diagnostico);
        printf("Prescricao medica: %s\n", lista->prescricao);

        lista = lista->prox;
    }
}

Consulta* excluirConsultaPorCodigo(Consulta *lista, int codigo, int *removeu) {
    Consulta *atual = lista;
    Consulta *anterior = NULL;

    *removeu = 0;

    while (atual != NULL && atual->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        return lista;
    }

    if (anterior == NULL) {
        lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    *removeu = 1;

    return lista;
}

int contarConsultas(Consulta *lista) {
    int total = 0;

    while (lista != NULL) {
        total++;
        lista = lista->prox;
    }

    return total;
}

void salvarConsultasArquivo(FILE *arquivo, const char cpfPaciente[], Consulta *lista) {
    while (lista != NULL) {
        fprintf(arquivo, "Paciente CPF: %s\n", cpfPaciente);
        fprintf(arquivo, "Consulta %d\n", lista->codigo);
        fprintf(arquivo, "Data: %s\n", lista->data);
        fprintf(arquivo, "Horario: %s\n", lista->horario);
        fprintf(arquivo, "Medico: %s\n", lista->medico);
        fprintf(arquivo, "Sintomas: %s\n", lista->sintomas);
        fprintf(arquivo, "Diagnostico: %s\n", lista->diagnostico);
        fprintf(arquivo, "Prescricao: %s\n", lista->prescricao);
        fprintf(arquivo, "-----------------------------\n\n");

        lista = lista->prox;
    }
}

void liberarConsultas(Consulta *lista) {
    Consulta *aux;

    while (lista != NULL) {
        aux = lista;
        lista = lista->prox;
        free(aux);
    }
}
