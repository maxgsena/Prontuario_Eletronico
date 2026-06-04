#include <stdio.h>
#include "paciente.h"

int main(void) {
    Paciente *lista;
    Paciente *pacienteEncontrado;
    int opcao;
    char cpf[15];

    lista = carregarDados();

    do {
        printf("SISTEMA DE PRONTUARIO MEDICO\n");
        printf("1. Cadastrar paciente\n");
        printf("2. Registrar consulta\n");
        printf("3. Listar pacientes\n");
        printf("4. Buscar paciente por CPF\n");
        printf("5. Editar prontuario\n");
        printf("6. Excluir consulta\n");
        printf("7. Consultar quantitativo de atendimentos\n");
        printf("8. Sair\n");
        printf("Opcao: ");
        opcao = lerInteiro();

        switch (opcao) {
            case 1:
                lista = cadastrarPaciente(lista);
                break;

            case 2:
                registrarConsulta(lista);
                break;

            case 3:
                listarPacientes(lista);
                break;

            case 4:
                printf("\nDigite o CPF do paciente: ");
                lerTexto(cpf, 15);
                pacienteEncontrado = buscarPacientePorCPF(lista, cpf);
                mostrarPaciente(pacienteEncontrado);
                break;

            case 5:
                editarProntuario(lista);
                break;

            case 6:
                excluirConsulta(lista);
                break;

            case 7:
                printf("\nQuantidade total de atendimentos: %d\n", consultarQuantidadeAtendimentos(lista));
                break;

            case 8:
                salvarDados(lista);
                lista = liberarListaPacientes(lista);
                printf("Sistema encerrado. Dados salvos.\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 8);

    return 0;
}
