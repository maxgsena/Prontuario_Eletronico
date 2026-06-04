#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.h"

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

struct paciente {
    int numeroProntuario;
    char nome[100];
    char cpf[15];
    char dataNascimento[11];
    char telefone[20];
    Consulta *consultas;
    struct paciente *prox;
};

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerTexto(char texto[], int tamanho) {
    fgets(texto, tamanho, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

int lerInteiro(void) {
    int valor;
    scanf("%d", &valor);
    limparBuffer();
    return valor;
}

Paciente* criarListaPacientes(void) {
    return NULL;
}

static int proximoNumeroProntuario(Paciente *lista) {
    int maior = 0;
    while (lista != NULL) {
        if (lista->numeroProntuario > maior) {
            maior = lista->numeroProntuario;
        }
        lista = lista->prox;
    }
    return maior + 1;
}

static int proximoCodigoConsulta(Paciente *p) {
    int maior = 0;
    Consulta *c = p->consultas;
    while (c != NULL) {
        if (c->codigo > maior) {
            maior = c->codigo;
        }
        c = c->prox;
    }
    return maior + 1;
}

Paciente* buscarPacientePorCPF(Paciente *lista, const char cpf[]) {
    while (lista != NULL) {
        if (strcmp(lista->cpf, cpf) == 0) {
            return lista;
        }
        lista = lista->prox;
    }
    return NULL;
}

void mostrarPaciente(Paciente *p) {
    if (p == NULL) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    printf("Dados do paciente\n");
    printf("Numero do prontuario: %d\n", p->numeroProntuario);
    printf("Nome completo: %s\n", p->nome);
    printf("CPF: %s\n", p->cpf);
    printf("Data de nascimento: %s\n", p->dataNascimento);
    printf("Telefone: %s\n", p->telefone);
    listarConsultasPaciente(p);
}

Paciente* cadastrarPaciente(Paciente *lista) {
    Paciente *novo = (Paciente*) malloc(sizeof(Paciente));

    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        return lista;
    }

    novo->numeroProntuario = proximoNumeroProntuario(lista);

    printf("Cadastrar paciente\n");
    printf("Nome completo: ");
    lerTexto(novo->nome, 100);

    printf("CPF: ");
    lerTexto(novo->cpf, 15);

    if (buscarPacientePorCPF(lista, novo->cpf) != NULL) {
        printf("Ja existe um paciente cadastrado com esse CPF.\n");
        free(novo);
        return lista;
    }

    printf("Data de nascimento (dd/mm/aaaa): ");
    lerTexto(novo->dataNascimento, 11);

    printf("Telefone para contato: ");
    lerTexto(novo->telefone, 20);

    novo->consultas = NULL;
    novo->prox = lista;

    printf("Paciente cadastrado com sucesso. Numero do prontuario: %d\n", novo->numeroProntuario);
    salvarDados(novo);
    return novo;
}

void listarPacientes(Paciente *lista) {
    if (lista == NULL) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    printf("\n--- LISTA DE PACIENTES ---\n");
    while (lista != NULL) {
        printf("\nProntuario: %d\n", lista->numeroProntuario);
        printf("Nome: %s\n", lista->nome);
        printf("CPF: %s\n", lista->cpf);
        printf("Nascimento: %s\n", lista->dataNascimento);
        printf("Telefone: %s\n", lista->telefone);
        lista = lista->prox;
    }
}

void listarConsultasPaciente(Paciente *p) {
    Consulta *c;

    if (p == NULL) {
        return;
    }

    c = p->consultas;

    if (c == NULL) {
        printf("Nenhuma consulta registrada para este paciente.\n");
        return;
    }

    printf("Consultas do paciente\n");
    while (c != NULL) {
        printf("\nCodigo da consulta: %d\n", c->codigo);
        printf("Data: %s\n", c->data);
        printf("Horario: %s\n", c->horario);
        printf("Medico responsavel: %s\n", c->medico);
        printf("Sintomas relatados: %s\n", c->sintomas);
        printf("Diagnostico: %s\n", c->diagnostico);
        printf("Prescricao medica: %s\n", c->prescricao);
        c = c->prox;
    }
}

void registrarConsulta(Paciente *lista) {
    char cpf[15];
    Paciente *p;
    Consulta *nova;

    printf("Registrar consulta\n");
    printf("CPF do paciente: ");
    lerTexto(cpf, 15);

    p = buscarPacientePorCPF(lista, cpf);
    if (p == NULL) {
        printf("Paciente nao encontrado. Cadastre o paciente antes da consulta.\n");
        return;
    }

    nova = (Consulta*) malloc(sizeof(Consulta));
    if (nova == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    nova->codigo = proximoCodigoConsulta(p);

    printf("Data da consulta (dd/mm/aaaa): ");
    lerTexto(nova->data, 11);

    printf("Horario (hh:mm): ");
    lerTexto(nova->horario, 6);

    printf("Medico responsavel: ");
    lerTexto(nova->medico, 100);

    printf("Sintomas relatados: ");
    lerTexto(nova->sintomas, 300);

    printf("Diagnostico: ");
    lerTexto(nova->diagnostico, 300);

    printf("Prescricao medica: ");
    lerTexto(nova->prescricao, 300);

    nova->prox = p->consultas;
    p->consultas = nova;

    printf("Consulta registrada com sucesso. Codigo: %d\n", nova->codigo);
    salvarDados(lista);
}

void editarProntuario(Paciente *lista) {
    char cpf[15];
    Paciente *p;
    int opcao;

    printf("Editar prontuario\n");
    printf("CPF do paciente: ");
    lerTexto(cpf, 15);

    p = buscarPacientePorCPF(lista, cpf);
    if (p == NULL) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    do {
        printf("\n1. Editar nome\n");
        printf("2. Editar CPF\n");
        printf("3. Editar data de nascimento\n");
        printf("4. Editar telefone\n");
        printf("5. Sair da edicao\n");
        printf("Opcao: ");
        opcao = lerInteiro();

        switch (opcao) {
            case 1:
                printf("Novo nome: ");
                lerTexto(p->nome, 100);
                break;
            case 2:
                printf("Novo CPF: ");
                lerTexto(p->cpf, 15);
                break;
            case 3:
                printf("Nova data de nascimento: ");
                lerTexto(p->dataNascimento, 11);
                break;
            case 4:
                printf("Novo telefone: ");
                lerTexto(p->telefone, 20);
                break;
            case 5:
                printf("Encerrando edicao.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 5);

    salvarDados(lista);
}

void excluirConsulta(Paciente *lista) {
    char cpf[15];
    int codigo;
    Paciente *p;
    Consulta *atual;
    Consulta *anterior = NULL;

    printf("Excluir consulta\n");
    printf("CPF do paciente: ");
    lerTexto(cpf, 15);

    p = buscarPacientePorCPF(lista, cpf);
    if (p == NULL) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    listarConsultasPaciente(p);

    printf("\nDigite o codigo da consulta que deseja excluir: ");
    codigo = lerInteiro();

    atual = p->consultas;
    while (atual != NULL && atual->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Consulta nao encontrada.\n");
        return;
    }

    if (anterior == NULL) {
        p->consultas = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    printf("Consulta excluida com sucesso.\n");
    salvarDados(lista);
}

int consultarQuantidadeAtendimentos(Paciente *lista) {
    int total = 0;
    Consulta *c;

    while (lista != NULL) {
        c = lista->consultas;
        while (c != NULL) {
            total++;
            c = c->prox;
        }
        lista = lista->prox;
    }

    return total;
}

Paciente* liberarListaPacientes(Paciente *lista) {
    Paciente *auxPaciente;
    Consulta *auxConsulta;

    while (lista != NULL) {
        auxPaciente = lista;
        lista = lista->prox;

        while (auxPaciente->consultas != NULL) {
            auxConsulta = auxPaciente->consultas;
            auxPaciente->consultas = auxPaciente->consultas->prox;
            free(auxConsulta);
        }

        free(auxPaciente);
    }

    return NULL;
}

void salvarDados(Paciente *lista) {
    FILE *fp = fopen("pacientes.txt", "w");
    FILE *fc = fopen("consultas.txt", "w");
    Consulta *c;

    if (fp == NULL || fc == NULL) {
        printf("Erro ao abrir arquivos para salvar.\n");
        if (fp != NULL) fclose(fp);
        if (fc != NULL) fclose(fc);
        return;
    }

    while (lista != NULL) {
        fprintf(fp, "%d;%s;%s;%s;%s\n",
                lista->numeroProntuario,
                lista->nome,
                lista->cpf,
                lista->dataNascimento,
                lista->telefone);

        c = lista->consultas;
        while (c != NULL) {
            fprintf(fc, "%s;%d;%s;%s;%s;%s;%s;%s\n",
                    lista->cpf,
                    c->codigo,
                    c->data,
                    c->horario,
                    c->medico,
                    c->sintomas,
                    c->diagnostico,
                    c->prescricao);
            c = c->prox;
        }

        lista = lista->prox;
    }

    fclose(fp);
    fclose(fc);
}

Paciente* carregarDados(void) {
    FILE *fp = fopen("pacientes.txt", "r");
    FILE *fc;
    Paciente *lista = NULL;
    Paciente *novo;
    char linha[1200];

    if (fp != NULL) {
        while (fgets(linha, sizeof(linha), fp) != NULL) {
            novo = (Paciente*) malloc(sizeof(Paciente));
            if (novo == NULL) {
                printf("Erro ao alocar memoria ao carregar pacientes.\n");
                fclose(fp);
                return lista;
            }

            linha[strcspn(linha, "\n")] = '\0';

            sscanf(linha, "%d;%99[^;];%14[^;];%10[^;];%19[^\n]",
                   &novo->numeroProntuario,
                   novo->nome,
                   novo->cpf,
                   novo->dataNascimento,
                   novo->telefone);

            novo->consultas = NULL;
            novo->prox = lista;
            lista = novo;
        }
        fclose(fp);
    }

    fc = fopen("consultas.txt", "r");
    if (fc != NULL) {
        while (fgets(linha, sizeof(linha), fc) != NULL) {
            char cpf[15];
            Paciente *p;
            Consulta *nova = (Consulta*) malloc(sizeof(Consulta));

            if (nova == NULL) {
                printf("Erro ao alocar memoria ao carregar consultas.\n");
                fclose(fc);
                return lista;
            }

            linha[strcspn(linha, "\n")] = '\0';

            sscanf(linha, "%14[^;];%d;%10[^;];%5[^;];%99[^;];%299[^;];%299[^;];%299[^\n]",
                   cpf,
                   &nova->codigo,
                   nova->data,
                   nova->horario,
                   nova->medico,
                   nova->sintomas,
                   nova->diagnostico,
                   nova->prescricao);

            p = buscarPacientePorCPF(lista, cpf);
            if (p != NULL) {
                nova->prox = p->consultas;
                p->consultas = nova;
            } else {
                free(nova);
            }
        }
        fclose(fc);
    }

    return lista;
}
