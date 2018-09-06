/*
CEFET-RJ | BCC | ORGANIZAÇÃO DE ESTRUTURAS DE ARQUIVOS
PROFESSOR: RENATO MAURO
TRABALHO: IMPLEMENTAÇÃO DE BUSCA BINÁRIA NO ARQUIVO DE CEPS
ALUNA: THAÍS DE OLIVEIRA GONÇALVES
*/

#include <stdio.h>
// biblioteca string.h elimina o warning relacionado à função strncmp durante a compilação
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

/*Definição da função buscaCep, que recebe como parâmetros o tamanho do arquivo em
bytes (variável fim), um ponteiro (para o arquivo .dat que contém todos os
CEPS ordenados) e um número (variável cep) a ser pesquisado nesse arquivo.
*/
void buscaCep (long fim_busca, FILE *arquivo, char* cep_pesquisado){

	int quantidade;
	int i = 0;
	long inicio_busca = 0;
	long meio;
	Endereco endereco;

	/*a variável meio será o determinante do ponto de partida da busca no arquivo,
	por isso é calculada dentro do while, mudando de posição a cada loop
	*/
	while (inicio_busca <= fim_busca){
		i++;
		meio = (inicio_busca + fim_busca)/2;
		// Função fseek coloca a cabeça de leitura numa determinada posição do arquivo.
		fseek(arquivo, meio*sizeof(Endereco), SEEK_SET);
		quantidade = fread(&endereco, sizeof(Endereco), 1, arquivo);
		/* O if abaixo compara, ataravés da função strcnmp, duas strings até o oitavo caracter, no
		caso, cep_pesquisado e endereco.cep na posicao corrente do arquivo.
		Se forem iguais, imprime logradouro, bairro, cidade, uf, sigla e cep. Senão, continua a busca.
		*/
		if(strncmp(cep_pesquisado, endereco.cep, 8) == 0){
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",endereco.logradouro,endereco.bairro,endereco.cidade,endereco.uf,endereco.sigla,endereco.cep);
			break;
		}else{
			/* Senão, se o cep pesquisado for menor que o cep da posição corrente, quer dizer que o fim da busca
			passará a ser a posição imediatamente anterior ao meio no próximo loop. Reduzindo pela metade o tempo de busca.
			*/
			if (strncmp(cep_pesquisado, endereco.cep, 8) < 0){
				fim_busca = meio - 1;
			}else{
				/* Senão, o cep pesquisado será maior que o cep da posição corrente. Então o início da busca
				passará a ser a posição imediatamente posterior ao meio no próximo loop. Reduzindo pela metade o tempo de busca.
				*/
				inicio_busca = meio + 1;
			}
		}
	}
	printf("Quantidade de passagens: %d", i);

}

int main(int argc, char**argv)
{
	FILE *f;
	Endereco e;
	long posicao, tamanho;


	if(argc != 2)
	{
		fprintf(stderr, "EXECUÇÃO CORRETA: ./buscabin [CEP] %s", argv[0]);
		return 1;
	}

	f = fopen("cep_ordenado.dat","r");
	// fseek percorre o arquivo até a última posição.
	fseek(f,0,SEEK_END);
	// variável tamanho recebe o valor da última posição, que será o tamanho do arquivo
	tamanho = ftell(f);
	// função rewind retorna a posição de leitura ao início do arquivo
	rewind(f);

	/* A variável ultimo determina a posição final de leitura, que será sempre o tamanho do arquivo dividido pelo tamanho
		do bloco de informação e subtraído em 1, já que o índice inicial é zero.
	*/
	posicao = (tamanho/sizeof(Endereco)) - 1;

	buscaCep(tamanho, f, argv[1]);

	fclose(f);

	return 0;

}
