PROJETO: Sistema de Prontuario Eletronico em C

1) Divisao em 2 TADs principais:
   - TAD Paciente: paciente.h e paciente.c
   - TAD Consulta: consulta.h e consulta.c

2) Arquivo auxiliar de validacao:
   - validacao.h e validacao.c
   Esse arquivo centraliza as leituras validadas de inteiros, textos, CPF, telefone, data e hora.

3) Validacoes implementadas:
   - O menu aceita apenas numeros inteiros dentro do intervalo permitido.
   - Codigo de consulta aceita apenas inteiro positivo.
   - Nome aceita apenas letras e espacos.
   - CPF aceita exatamente 11 numeros, sem pontos ou tracos.
   - Telefone aceita apenas numeros, entre 8 e 15 digitos.
   - Data aceita apenas formato dd/mm/aaaa e verifica se a data realmente existe.
   - Hora aceita apenas formato hh:mm, entre 00:00 e 23:59.
   - Campos de texto livre nao podem ficar vazios e nao podem conter ponto e virgula (;).

4) Bloqueio de datas e horarios futuros:
   - Data de nascimento nao pode ser futura.
   - Consulta nao pode ser registrada em data ou horario futuro.
   - Se a data for hoje, um horario maior que o horario atual tambem e bloqueado.

5) Arquivos de dados:
   - pacientes.txt
   - consultas.txt

6) Estrutura usada:
   - Lista encadeada de pacientes.
   - Cada paciente possui uma lista encadeada de consultas.

COMO COMPILAR:

gcc main.c paciente.c consulta.c validacao.c -o prontuario

COMO EXECUTAR NO WINDOWS:

prontuario.exe

COMO EXECUTAR NO LINUX/MAC:

./prontuario
