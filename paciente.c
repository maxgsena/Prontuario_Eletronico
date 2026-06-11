#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.h"
#include "consulta.h"
#include "validacao.h"

struct paciente {
    int numeroProntuario;
    char nome[100];
    char cpf[15];
    char dataNascimento[11];
    char telefone[20];
    Consulta *consultas;
    struct paciente *prox;
};

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

Paciente* buscarPacientePorCPF(Paciente *lista, const char cpf[]) {
    while (lista != NULL) {
        if (strcmp(lista->cpf, cpf) == 0) {
            return lista;
        }

        lista = lista->prox;
    }

    return NULL;
}

static Paciente* buscarPacientePorCPFExceto(Paciente *lista, const char cpf[], Paciente *ignorar) {
    while (lista != NULL) {
        if (lista != ignorar && strcmp(lista->cpf, cpf) == 0) {
            return lista;
        }

        lista = lista->prox;
    }

    return NULL;
}

Paciente* cadastrarPaciente(Paciente *lista) {
    Paciente *novo = (Paciente*) malloc(sizeof(Paciente));

    if (novo == NULL) {
        printf("Erro ao alocar memoria para paciente.\n");
        return lista;
    }

    novo->numeroProntuario = proximoNumeroProntuario(lista);

    printf("\n--- CADASTRO DE PACIENTE ---\n");
    lerTextoValidado("Nome completo: ", novo->nome, sizeof(novo->nome), TEXTO_NOME);

    do {
        lerTextoValidado("CPF, somente numeros: ", novo->cpf, sizeof(novo->cpf), TEXTO_CPF);

        if (buscarPacientePorCPF(lista, novo->cpf) != NULL) {
            printf("Ja existe um paciente cadastrado com esse CPF.\n");
        }
    } while (buscarPacientePorCPF(lista, novo->cpf) != NULL);

    int dataOk;

    do {
        if (!lerTextoValidado("Data de nascimento (dd/mm/aaaa): ", 
            novo->dataNascimento, 
            sizeof(novo->dataNascimento), 
            TEXTO_DATA)) {
            printf("Cadastro de paciente cancelado.\n");
            free(novo);
            return lista;
        }

    dataOk = dataNaoFutura(novo->dataNascimento);

    if (!dataOk) {
        printf("A data de nascimento nao pode ser futura.\n");
    }

} while (!dataOk);

    lerTextoValidado("Telefone, somente numeros: ", novo->telefone, sizeof(novo->telefone), TEXTO_TELEFONE);

    novo->consultas = criarListaConsultas();
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
        printf("Data de nascimento: %s\n", lista->dataNascimento);
        printf("Telefone: %s\n", lista->telefone);
        printf("Quantidade de consultas: %d\n", contarConsultas(lista->consultas));

        lista = lista->prox;
    }
}

void mostrarPaciente(Paciente *p) {
    if (p == NULL) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    printf("\nDados do paciente\n");
    printf("Numero do prontuario: %d\n", p->numeroProntuario);
    printf("Nome completo: %s\n", p->nome);
    printf("CPF: %s\n", p->cpf);
    printf("Data de nascimento: %s\n", p->dataNascimento);
    printf("Telefone: %s\n", p->telefone);

    printf("\nConsultas do paciente\n");
    listarConsultas(p->consultas);
}

void editarProntuario(Paciente *lista) {
    char cpf[15];
    char novoCPF[15];
    Paciente *p;
    int opcao;

    printf("\nEditar prontuario\n");
    lerTextoValidado("CPF do paciente: ", cpf, sizeof(cpf), TEXTO_CPF);

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

        opcao = lerInteiroIntervalo("Opcao: ", 1, 5);

        switch (opcao) {
            case 1:
                lerTextoValidado("Novo nome: ", p->nome, sizeof(p->nome), TEXTO_NOME);
                printf("Nome alterado com sucesso.\n");
                break;

            case 2:
                do {
                    lerTextoValidado("Novo CPF, somente numeros: ", novoCPF, sizeof(novoCPF), TEXTO_CPF);

                    if (buscarPacientePorCPFExceto(lista, novoCPF, p) != NULL) {
                        printf("Ja existe outro paciente com esse CPF.\n");
                    }
                } while (buscarPacientePorCPFExceto(lista, novoCPF, p) != NULL);

                strcpy(p->cpf, novoCPF);
                printf("CPF alterado com sucesso.\n");
                break;

            case 3:
                do {
                    lerTextoValidado("Nova data de nascimento (dd/mm/aaaa): ", p->dataNascimento, sizeof(p->dataNascimento), TEXTO_DATA);

                    if (!dataNaoFutura(p->dataNascimento)) {
                        printf("A data de nascimento nao pode ser futura.\n");
                    }
                } while (!dataNaoFutura(p->dataNascimento));

                printf("Data de nascimento alterada com sucesso.\n");
                break;

            case 4:
                lerTextoValidado("Novo telefone, somente numeros: ", p->telefone, sizeof(p->telefone), TEXTO_TELEFONE);
                printf("Telefone alterado com sucesso.\n");
                break;

            case 5:
                printf("Encerrando edicao.\n");
                break;
        }
    } while (opcao != 5);

    salvarDados(lista);
}

void registrarConsultaPaciente(Paciente *lista) {
    char cpf[15];
    Paciente *p;

    printf("\nRegistrar consulta\n");
    lerTextoValidado("CPF do paciente: ", cpf, sizeof(cpf), TEXTO_CPF);

    p = buscarPacientePorCPF(lista, cpf);

    if (p == NULL) {
        printf("Paciente nao encontrado. Cadastre o paciente antes da consulta.\n");
        return;
    }

    p->consultas = cadastrarConsulta(p->consultas);
    salvarDados(lista);
}

void excluirConsultaPaciente(Paciente *lista) {
    char cpf[15];
    int codigo;
    int removeu;
    Paciente *p;

    printf("\nExcluir consulta\n");
    lerTextoValidado("CPF do paciente: ", cpf, sizeof(cpf), TEXTO_CPF);

    p = buscarPacientePorCPF(lista, cpf);

    if (p == NULL) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    listarConsultas(p->consultas);

    if (contarConsultas(p->consultas) == 0) {
        return;
    }

    codigo = lerInteiroPositivo("Codigo da consulta que deseja excluir: ");
    p->consultas = excluirConsultaPorCodigo(p->consultas, codigo, &removeu);

    if (removeu) {
        printf("Consulta excluida com sucesso.\n");
        salvarDados(lista);
    } else {
        printf("Consulta nao encontrada.\n");
    }
}

int consultarQuantidadeAtendimentos(Paciente *lista) {
    int total = 0;

    while (lista != NULL) {
        total += contarConsultas(lista->consultas);
        lista = lista->prox;
    }

    return total;
}

void salvarDados(Paciente *lista) {
    FILE *fp = fopen("pacientes.txt", "w");
    FILE *fc = fopen("consultas.txt", "w");

    if (fp == NULL || fc == NULL) {
        printf("Erro ao abrir arquivos para salvar.\n");

        if (fp != NULL) {
            fclose(fp);
        }

        if (fc != NULL) {
            fclose(fc);
        }

        return;
    }

    while (lista != NULL) {
        fprintf(fp, "Paciente %d\n", lista->numeroProntuario);
        fprintf(fp, "Nome: %s\n", lista->nome);
        fprintf(fp, "CPF: %s\n", lista->cpf);
        fprintf(fp, "Data de nascimento: %s\n", lista->dataNascimento);
        fprintf(fp, "Telefone: %s\n", lista->telefone);
        fprintf(fp, "-----------------------------\n\n");

        salvarConsultasArquivo(fc, lista->cpf, lista->consultas);

        lista = lista->prox;
    }

    fclose(fp);
    fclose(fc);
}

static void removerQuebraLinhaArquivo(char linha[]) {
    linha[strcspn(linha, "\r\n")] = '\0';
}

static void copiarValorRotulo(const char linha[], const char rotulo[], char destino[], int tamanho) {
    const char *inicio;
    int i;

    destino[0] = '\0';

    if (strncmp(linha, rotulo, strlen(rotulo)) != 0) {
        return;
    }

    inicio = linha + strlen(rotulo);

    while (*inicio == ' ') {
        inicio++;
    }

    strncpy(destino, inicio, tamanho - 1);
    destino[tamanho - 1] = '\0';

    for (i = strlen(destino) - 1; i >= 0; i--) {
        if (destino[i] == ' ') {
            destino[i] = '\0';
        } else {
            break;
        }
    }
}

Paciente* carregarDados(void) {
    FILE *fp;
    FILE *fc;
    Paciente *lista = NULL;
    Paciente *novo;
    Paciente *p;
    Consulta *novaConsulta;

    char linha[1500];

    char cpfPaciente[15];
    char data[11];
    char horario[6];
    char medico[100];
    char sintomas[300];
    char diagnostico[300];
    char prescricao[300];

    int codigoConsulta;

    fp = fopen("pacientes.txt", "r");

    if (fp != NULL) {
        while (fgets(linha, sizeof(linha), fp) != NULL) {
            removerQuebraLinhaArquivo(linha);

            if (strncmp(linha, "Paciente ", 9) == 0) {
                novo = (Paciente*) malloc(sizeof(Paciente));

                if (novo == NULL) {
                    printf("Erro ao alocar memoria ao carregar paciente.\n");
                    fclose(fp);
                    return lista;
                }

                if (sscanf(linha, "Paciente %d", &novo->numeroProntuario) != 1) {
                    free(novo);
                    continue;
                }

                if (fgets(linha, sizeof(linha), fp) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Nome:", novo->nome, sizeof(novo->nome));
                }

                if (fgets(linha, sizeof(linha), fp) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "CPF:", novo->cpf, sizeof(novo->cpf));
                }

                if (fgets(linha, sizeof(linha), fp) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Data de nascimento:", novo->dataNascimento, sizeof(novo->dataNascimento));
                }

                if (fgets(linha, sizeof(linha), fp) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Telefone:", novo->telefone, sizeof(novo->telefone));
                }

                novo->consultas = criarListaConsultas();

                novo->prox = lista;
                lista = novo;
            }
        }

        fclose(fp);
    }

    fc = fopen("consultas.txt", "r");

    if (fc != NULL) {
        while (fgets(linha, sizeof(linha), fc) != NULL) {
            removerQuebraLinhaArquivo(linha);

            if (strncmp(linha, "Paciente CPF:", 13) == 0) {
                copiarValorRotulo(linha, "Paciente CPF:", cpfPaciente, sizeof(cpfPaciente));

                if (fgets(linha, sizeof(linha), fc) != NULL) {
                    removerQuebraLinhaArquivo(linha);

                    if (sscanf(linha, "Consulta %d", &codigoConsulta) != 1) {
                        continue;
                    }
                } else {
                    break;
                }

                if (fgets(linha, sizeof(linha), fc) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Data:", data, sizeof(data));
                }

                if (fgets(linha, sizeof(linha), fc) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Horario:", horario, sizeof(horario));
                }

                if (fgets(linha, sizeof(linha), fc) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Medico:", medico, sizeof(medico));
                }

                if (fgets(linha, sizeof(linha), fc) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Sintomas:", sintomas, sizeof(sintomas));
                }

                if (fgets(linha, sizeof(linha), fc) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Diagnostico:", diagnostico, sizeof(diagnostico));
                }

                if (fgets(linha, sizeof(linha), fc) != NULL) {
                    removerQuebraLinhaArquivo(linha);
                    copiarValorRotulo(linha, "Prescricao:", prescricao, sizeof(prescricao));
                }

                p = buscarPacientePorCPF(lista, cpfPaciente);

                if (p != NULL) {
                    novaConsulta = criarConsultaComDados(
                        codigoConsulta,
                        data,
                        horario,
                        medico,
                        sintomas,
                        diagnostico,
                        prescricao
                    );

                    p->consultas = inserirConsultaInicio(p->consultas, novaConsulta);
                }
            }
        }

        fclose(fc);
    }

    return lista;
}

Paciente* liberarListaPacientes(Paciente *lista) {
    Paciente *aux;

    while (lista != NULL) {
        aux = lista;
        lista = lista->prox;
        liberarConsultas(aux->consultas);
        free(aux);
    }

    return NULL;
}
