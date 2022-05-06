#include "ListaContatos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ListaEncadeadaContatos* CriarListaContatos(){
    ListaEncadeadaContatos* NovaLista;
    NovaLista = (ListaEncadeadaContatos*) malloc(sizeof(ListaEncadeadaContatos));
    if(NovaLista!= NULL)
    {
        NovaLista->inicio = NULL;
        NovaLista->final  = NULL;
    }
    return NovaLista;
}

int InserirContato(ListaEncadeadaContatos* lista, Contato contato, int InicioFinal){
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->contato = contato;

    /* INICIO */
    if (InicioFinal == 1){
        novoNodo->proximo = lista->inicio;

        /*
            UNICA MANEIRA DE MUDAR O NODO FINAL NA INSERÇÃO INICIAL É SE
            ESTE SER O PRIMEIRO A SER INSERIDO, CASO CONTRARIO, O NODO
            FINAL SEMPRE SERÁ O MESMO, JÁ QUE ESTE NÃO MUDA NESTA CONDIÇÃO
        */
        if(lista->inicio == NULL){
            lista->final = novoNodo;
        }

        lista->inicio = novoNodo;

        return 1;
    } else {
    /* FINAL */
        /*
            QUANDO É O PRIMEIRO, INSERE PELO MÉTODO DE INICIO, JÁ QUE O RESULTADO SERÁ O MESMO
        */
        if (lista->inicio == NULL){
            InserirContato(lista, contato, 1);
        } else {
            Nodo* UltimoNodoAtual = lista->final;
            UltimoNodoAtual->proximo = novoNodo;
            lista->final = novoNodo;
            novoNodo->proximo = NULL;
        }
    }
}

void ImprimirContato(Contato* contato){
    printf("X---------------------X\n");
    printf("| Informacoes Contato |\n");
    printf("X---------------------X\n");

    if (contato != NULL){
        printf("Nome..........: %s\n", contato->nome);
        printf("Sobrenome.....: %s\n", contato->sobrenome);
        printf("E-mail........: %s\n", contato->email);
        printf("Telefone......: %s\n", contato->telefone);
        printf("Profissao.....: %s", contato->profissao);
        printf("Dt. Nascimento: %d/%d/%d\n", contato->diaNascimento, contato->mesNascimento, contato->anoNascimento);
    }
}

void BuscarLista(ListaEncadeadaContatos* lista, int OpcaoConsulta, char* EmailConsulta){
    /*
        OPCAOCONSULTA
        0 - IMPRESSAO GERAL
        1 - CONSULTA PELO EMAIL
    */
    int EncontrouRegistro = 0;
    int Ctd = 0;

    if (lista != NULL){
        if (lista->inicio != NULL){
            Nodo* NodeLoop = lista->inicio;
            while(NodeLoop != NULL){
                Contato ContatoTestaConsulta = NodeLoop->contato;
                Ctd++;
                if (OpcaoConsulta == 0){
                    ImprimirContato(&NodeLoop->contato);
                }

                if (OpcaoConsulta == 1){
                    if(strcmp(EmailConsulta, ContatoTestaConsulta.email) == 0){
                        EncontrouRegistro = 1;
                        break;
                    }
                }

                NodeLoop = NodeLoop->proximo;
            }
            if ((OpcaoConsulta == 1) && (EncontrouRegistro == 0)){
                LogOperacoes("ERRO,buscar,Contato nao encontrado");
            }
            if ((OpcaoConsulta == 1) && (EncontrouRegistro == 1)){
                char CtdAux[4];
                char Buscar[15];
                strcpy(Buscar, "buscar,");

                itoa(Ctd, CtdAux, 10);

                strcat(Buscar, CtdAux);

                LogOperacoes(Buscar);
            }
        }
    }
}

void DestruirListaEncadeadaContatos(ListaEncadeadaContatos** lista){
    if(*lista != NULL)
    {
        while((*lista)->inicio != NULL)
        {
            Nodo* nodo = (*lista)->inicio;
            (*lista)->inicio = nodo->proximo;
            free(nodo);
        }
        (*lista)->inicio = NULL;
        free(*lista);
    }
    *lista = NULL;
}

int RemoverContatoPorEmail(ListaEncadeadaContatos* lista, char EmailRemocao[]){
    int Removeu = 0;
    int Ctd = 0;

    if (lista != NULL){
        if (lista->inicio != NULL){
            Nodo* NodeLoop     = lista->inicio;
            Nodo* NodoAnterior = lista->inicio;

            while(NodeLoop != NULL){
                Contato ContatoTestaConsulta = NodeLoop->contato;
                Ctd++;
                if(strcmp(EmailRemocao, ContatoTestaConsulta.email) == 0){
                    /*
                        ANTES DE REMOVER, FAZ ALGUMAS VERIFICAÇÕES, COMO, SE O NODO ELIMINADO É O PRIMEIRO OU ULTIMO,
                        JA QUE TEM QUE ATUALIZAR AS PROPRIEDADES NA LISTA
                    */

                    /*
                        QUANDO ELIMINA O PRIMEIRO, SE É O UNICO DA LISTA, RESETA AMBAS AS PROPRIEDADES INICIAL E FINAL,
                        CASO CONTRARIO, SETA O SEGUNDO COMO PRIMEIRO.
                    */
                    if(lista->inicio == NodeLoop){
                        if(NodeLoop->proximo == NULL){  /* UNICO */
                            free(NodeLoop);
                            lista->inicio = NULL;
                            lista->final = NULL;
                        }else{
                            lista->inicio = NodeLoop->proximo;
                            free(NodeLoop);
                        }
                    }

                    /*
                        QUANDO ELIMINA O NODO FINAL, MUDA A PROPRIEDADE 'FINAL' NA LISTA PARA APONTAR AO NODO
                        ANTERIOR AO QUE ESTÁ SENDO ELIMINADO, (NodoAnterior) E FAZ COM QUE ESTE NÃO APONTE
                        A UM PROXIMO;
                    */
                    if (lista->final == NodeLoop){
                        lista->final = NodoAnterior;

                        NodoAnterior->proximo = NULL;

                        free(NodeLoop);

                        break;
                    }
                    /*
                        QUANDO REMOVER UM NODO DO MEIO, O ANTERIOR VAI APONTAR AO SEGUINTE AO NODO REMOVIDO, PREVININDO
                        QUE QUEBRE A LISTA.
                    */
                    /*
                        SE NÃO É O INICIAL NEM O FINAL, LÓGICAMENTE É UM DO MEIO
                    */
                    NodoAnterior->proximo = NodeLoop->proximo;
                    free(NodeLoop);

                    Removeu = 1;
                }
                /*
                    SALVA UMA COPIA DO NODO, QUE VAI SER O ANTERIOR AO ATUAL DO LAÇO, UTILIZA QUANDO A REMOÇÃO É
                    NO MEIO OU FINAL.
                */
                NodoAnterior = NodeLoop;
                NodeLoop = NodeLoop->proximo;
            }
        }
    }
    if (Removeu == 0){
        LogOperacoes("ERRO,remover,Contato nao encontrado");
    }else{
        char CtdAux[4];
        char Remover[15];
        strcpy(Remover, "remover,");

        itoa(Ctd, CtdAux, 10);

        strcat(Remover, CtdAux);

        LogOperacoes(Remover);
    }

    return Removeu;
}

void Ordenar(ListaEncadeadaContatos* lista, int Tamanho){
    Nodo* NodoUltimoOrdenado = lista->final;
    Nodo* NodeAuxTrocaPrimeiro = NULL;
    Nodo* NodeAuxTrocaSegundo  = NULL;

    int CtdOrdenacao = 0, DataComparacao, DataComparacaoAux, i;

    for (i = 0; i < Tamanho; i++) {
        Nodo* NodePrimeiro = lista->inicio;
        Nodo* NodeSegundo  = NodePrimeiro->proximo;
        Nodo* NodoAnterior = NULL;

        while(NodeSegundo != NULL){
            Nodo* NodeAuxTrocaPrimeiro = NULL;
            Nodo* NodeAuxTrocaSegundo  = NULL;

            Contato OrdenacaoAux = NodeSegundo->contato;
            Contato Ordenacao = NodePrimeiro->contato;

            DataComparacao = Ordenacao.diaNascimento +
                             Ordenacao.mesNascimento * 100 +
                             Ordenacao.anoNascimento * 1000;

            DataComparacaoAux = OrdenacaoAux.diaNascimento +
                                OrdenacaoAux.mesNascimento * 100 +
                                OrdenacaoAux.anoNascimento * 1000;
            if(DataComparacao > DataComparacaoAux){
                CtdOrdenacao++;
                if (lista->inicio == NodePrimeiro){
                    lista->inicio = NodeSegundo;
                }

                NodeAuxTrocaPrimeiro = NodePrimeiro;
                NodeAuxTrocaSegundo  = NodeSegundo->proximo;

                NodePrimeiro = NodeSegundo;
                NodeSegundo  = NodeAuxTrocaPrimeiro;

                if (NodoAnterior!= NULL){
                    NodoAnterior->proximo = NodePrimeiro;
                }

                NodePrimeiro->proximo = NodeSegundo;
                NodeSegundo->proximo  = NodeAuxTrocaSegundo;
            }

            NodoAnterior = NodePrimeiro;
            NodePrimeiro = NodeSegundo;
            NodeSegundo  = NodeSegundo->proximo;

            if (NodePrimeiro == NodoUltimoOrdenado){
                break;
            }
        }

        NodoUltimoOrdenado = NodePrimeiro;

        if (i == 0){
            lista->final = NodoUltimoOrdenado;
            NodoUltimoOrdenado->proximo = NULL;
        }
    }

    char CtdAux[10];
    char Ordenacao[50];
    strcpy(Ordenacao, "ordenar,");

    itoa(CtdOrdenacao, CtdAux, 10);

    strcat(Ordenacao, CtdAux);

    LogOperacoes(Ordenacao);
}

void LogDados(ListaEncadeadaContatos* lista){
    FILE *Dados = fopen ( "log-conteudo-lista.txt", "w" );

    int Ctd;

    Nodo* NodeLOG = lista->inicio;
    while(NodeLOG != NULL){
        Contato ContatoLOG = NodeLOG->contato;
        Ctd++;

        char str[10];
        sprintf(str, "%d", Ctd);

        fputs(str, Dados);
        fputs(",EMAIL:", Dados);
        fputs(ContatoLOG.email, Dados);
        fputs("\n", Dados);

        NodeLOG = NodeLOG->proximo;
    }

    fclose(Dados);
}

void LogOperacoes(char* OperacInserir){
    FILE *Operacoes = fopen( "log-operacoes.txt", "a" );
    chop(OperacInserir);
    fputs(OperacInserir, Operacoes);
    fputs("\n", Operacoes);

    fclose(Operacoes);
}
