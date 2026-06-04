typedef struct consulta Consulta;


typedef struct paciente Paciente;


Paciente* criarListaPacientes(void);
Paciente* cadastrarPaciente(Paciente *lista);
void listarPacientes(Paciente *lista);
Paciente* buscarPacientePorCPF(Paciente *lista, const char cpf[]);
void mostrarPaciente(Paciente *p);
void editarProntuario(Paciente *lista);
Paciente* liberarListaPacientes(Paciente *lista);


void registrarConsulta(Paciente *lista);
void excluirConsulta(Paciente *lista);
void listarConsultasPaciente(Paciente *p);
int consultarQuantidadeAtendimentos(Paciente *lista);


Paciente* carregarDados(void);
void salvarDados(Paciente *lista);


void limparBuffer(void);
void lerTexto(char texto[], int tamanho);
int lerInteiro(void);