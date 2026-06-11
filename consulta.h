#include <stdio.h>



typedef struct consulta Consulta;
Consulta* criarConsultaComDados(int codigo,
    const char data[],
    const char horario[],
    const char medico[],
    const char sintomas[],
    const char diagnostico[],
    const char prescricao[]);

Consulta* inserirConsultaInicio(Consulta *lista, Consulta *nova);

Consulta* criarListaConsultas(void);
Consulta* criarConsultaComDados(int codigo, const char data[], const char horario[], const char medico[], const char sintomas[], const char diagnostico[], const char prescricao[]);
Consulta* inserirConsultaInicio(Consulta *lista, Consulta *nova);
Consulta* cadastrarConsulta(Consulta *lista);
Consulta* excluirConsultaPorCodigo(Consulta *lista, int codigo, int *removeu);
void listarConsultas(Consulta *lista);
int contarConsultas(Consulta *lista);
void salvarConsultasArquivo(FILE *arquivo, const char cpfPaciente[], Consulta *lista);
void liberarConsultas(Consulta *lista);
