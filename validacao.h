#define TEXTO_NOME 1
#define TEXTO_LIVRE 2
#define TEXTO_CPF 3
#define TEXTO_TELEFONE 4
#define TEXTO_DATA 5
#define TEXTO_HORA 6

int lerInteiro(const char mensagem[]);
int lerInteiroPositivo(const char mensagem[]);
int lerInteiroIntervalo(const char mensagem[], int minimo, int maximo);
int lerTextoValidado(const char mensagem[], char texto[], int tamanho, int tipo);

int somenteDigitos(const char texto[]);
int cpfValido(const char cpf[]);
int dataValida(const char data[]);
int horaValida(const char hora[]);
int dataNaoFutura(const char data[]);
int dataHoraNaoFutura(const char data[], const char hora[]);

