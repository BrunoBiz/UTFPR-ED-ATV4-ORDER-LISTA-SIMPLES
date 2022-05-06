#include "ListaContatos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINESIZE 128

void MenuPrincipal();
void LerArquivo(ListaEncadeadaContatos* Lista);
void LimparTela();
void chop (char* text);

int main()
{
    ListaEncadeadaContatos* Lista = CriarListaContatos();
    LerArquivo(Lista);
    return 0;
}

void LerArquivo(ListaEncadeadaContatos* Lista){
    FILE *myfile = fopen ( "arquivo.txt", "r" );
    /* FILE *myfile = fopen ( "arquivo - DEBUG.txt", "r" );*/

    remove("log-operacoes.txt"); /* Elimina para gerar um novo */

    char line[LINESIZE];
    int Tamanho = 0;

    while(fgets(line, sizeof(line), myfile)){
        char* Operacao = strtok(line, ",");

        if ((strcmp(Operacao, "inserir_inicio") == 0) ||
            (strcmp(Operacao, "inserir_final") == 0)) {
            int Insercao;
            Contato NovoContato;
            Tamanho++;

            char* Name    = strtok(NULL, ",");
            char* Surname = strtok(NULL, ",");
            char* Email   = strtok(NULL, ",");
            char* Phone   = strtok(NULL, ",");

            char* Date  = strtok(NULL, ",");
            char* Month = strtok(NULL, ",");
            char* Year  = strtok(NULL, ",");

            char* Profession = strtok(NULL, ",");

            strcpy(NovoContato.nome, Name);
            strcpy(NovoContato.sobrenome, Surname);
            strcpy(NovoContato.email, Email);
            strcpy(NovoContato.telefone, Phone);

            NovoContato.diaNascimento = atoi(Date);
            NovoContato.mesNascimento = atoi(Month);
            NovoContato.anoNascimento = atoi(Year);

            strcpy(NovoContato.profissao, Profession);

            if (strcmp(Operacao, "inserir_inicio") == 0){
                Insercao = 1;
            }else {
                Insercao = 2;
            }

            /* Sempre vai visitar apenas um no final devido à propriedade que aponta ao final da lista */
            LogOperacoes(strcat(Operacao, ",1"));

            InserirContato(Lista, NovoContato, Insercao);
        }

        if (strcmp(Operacao, "buscar") == 0){
            char* Email = strtok(NULL, ",");
            chop(Email); /* REMOVE O CARRIAGE RETURN */
            BuscarLista(Lista, 1, Email);
        }

        if (strcmp(Operacao, "remover") == 0){
            char* Email = strtok(NULL, ",");
            chop(Email); /* REMOVE O CARRIAGE RETURN */
            if (RemoverContatoPorEmail(Lista, Email) == 1){
                Tamanho--;
            }
        }

        if (strcmp(Operacao, "obter_tamanho_lista\n") == 0){
            /* Não visita nenhum nodo, já que existe uma variável para controlar o tamanho da lista */
            LogOperacoes("obter_tamanho_lista, 0");
        }

        if (strcmp(Operacao, "ordenar\n") == 0){
            Ordenar(Lista, Tamanho);
        }
    }

    LogDados(Lista);

    printf("Arquivo processado com sucesso!\n");
}

void LimparTela(){
    #ifdef _WIN32
    system("cls");

    #elif __linux__
    system("clear");

    #endif
}
/* REMOVE O CARRIAGE RETURN DA STRING */
void chop (char* text) {
  size_t length= strlen (text);
  if (length && text [length-1] == '\n')
    text [length-1]= '\0';
}
