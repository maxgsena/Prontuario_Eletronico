PROJETO 06 - SISTEMA DE PRONTUARIO ELETRONICO EM C

Arquivos:
- main.c: arquivo principal com o menu do sistema.
- paciente.h: interface do TAD Paciente e Consulta.
- paciente.c: implementação do TAD, listas encadeadas e arquivos.

Estrutura escolhida:
- Lista encadeada de pacientes.
- Cada paciente possui uma lista encadeada de consultas.

Por que lista?
A lista é mais adequada porque o sistema precisa cadastrar, buscar, editar e excluir registros.
Pilha e fila não são ideais para armazenar prontuários, pois limitam a ordem de acesso.

Arquivos gerados automaticamente pelo programa:
- pacientes.txt
- consultas.txt

Compilação:
gcc main.c paciente.c -o prontuario

Execução no Windows:
prontuario.exe

Execução no Linux/Mac:
./prontuario
