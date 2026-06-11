typedef struct paciente Paciente;

Paciente* criarListaPacientes(void);
Paciente* cadastrarPaciente(Paciente *lista);
void listarPacientes(Paciente *lista);
Paciente* buscarPacientePorCPF(Paciente *lista, const char cpf[]);
void mostrarPaciente(Paciente *p);
void editarProntuario(Paciente *lista);
void registrarConsultaPaciente(Paciente *lista);
void excluirConsultaPaciente(Paciente *lista);
int consultarQuantidadeAtendimentos(Paciente *lista);
Paciente* carregarDados(void);
void salvarDados(Paciente *lista);
Paciente* liberarListaPacientes(Paciente *lista);
