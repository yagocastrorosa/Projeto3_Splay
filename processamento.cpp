//
// Created by yago on 05/09/2021.
//

#include <iostream>
#include <fstream>
#include <string>
#include "splay.h"
#include <unordered_map>
#include "main.h"
#include "processamento.h"

using namespace std;
using std::string;
using namespace splay;

int calculo_posicao(string linha, string coluna) {

    int posicao, contagem = 0;
    string substring;

    //Localização da coluna desejada no arquivo:
    posicao = linha.find(coluna);

    //Caso a coluna não seja encontrada no arquivo, retornará uma mensagem de erro:
    if (posicao == string::npos) {
        cout << "Não foi possível localizar a coluna informada" << endl;
        return 0;
    }

    //É criada uma substring do ínicio da linha até a posição da coluna informada:
    substring = linha.substr(0, posicao);

    //As vírgulas dessa substring são retiradas e contabilizadas para o cálculo da posição da coluna:
    while (posicao != string::npos) {
        posicao = substring.find(',');
        substring[posicao] = ' ';
        contagem++;
    }

    return contagem;
};

string localiza_coluna(int contagem, string linha) {

    int i, posicao, posicao2;
    string conteudo, copia;

    copia = linha;

    if(contagem == 0) {
        posicao = 0;
    } else {
        //Busca da coluna com base no número de vírgulas:
        for (i = 0; i < contagem; i++) {
            posicao = copia.find(',');
            copia[posicao] = ' ';
        }
    }

    //Essa posição é armazenada e é realizada a busca da próxima vírgula:
    posicao2 = posicao;
    posicao = copia.find(',');

    //Caso não seja a última coluna da linha:
    if (posicao != string::npos) {

        //Caso seja a primeira coluna:
        if (posicao2 == 0) {
            conteudo = linha.substr(0, posicao - posicao2);

            //Caso seja uma coluna do meio:
        } else {
            conteudo = linha.substr(posicao2 + 1, posicao - posicao2 - 1);
        }

        //Caso seja a última coluna:
    } else {

        //Caso seja a única coluna do arquivo:
        if (posicao2 == 0) {
            conteudo = linha.substr(0, linha.size());
        } else {
            conteudo = linha.substr(posicao2 + 1, linha.size());
        }
    }

    return conteudo;
}

void armazenamento(char *argv[], int argc, Splay<string> &arv, unordered_map<string,cadastro> & dados) {

    int tamanho=0, tamanholinha1=0, contagem, j;
    string coluna, colunausuario, argumentos, linha, conteudo, valor, copia;

    cadastro inf;

    //Leitura do arquivo:
    ifstream arq(argv[1]);

    //Mensagem de erro caso não seja possível realizar a abertura:
    if (!arq.is_open()) {
        perror("ao abrir o arquivo informado");
        return;
    } else {

        //Leitura das colunas passadas como argumento:
        for (j = 2; j < argc; j++) {

            argumentos += argv[j];

            coluna = argv[j];

            ifstream arq(argv[1]);

            //Leitura da primeira linha do arquivo (em que o nome das colunas estão presentes):
            getline(arq, linha);

            //Cálculo da posição de início da próxima linha:
            tamanho = linha.size() + 1;

            tamanholinha1 = tamanho;

            //Cálculo da posição da coluna desejada:
            contagem = calculo_posicao(linha, coluna);

            if (contagem == 0) {
                return;
            } else {
                contagem--;
            }

            //Leitura de todas as linhas do arquivo:
            while (true) {

                getline(arq, linha);

                //Caso o arquivo chegue ao fim, o programa encerra o loop:
                if (arq.eof()) {
                    break;
                } else {

                    conteudo = localiza_coluna(contagem, linha);

                    if (dados.count(conteudo) > 0) {
                        dados[conteudo].coluna += "_" + to_string(contagem);
                        dados[conteudo].posicao += "_" + to_string(tamanho);
                    } else {
                        dados[conteudo].coluna = to_string(contagem);
                        dados[conteudo].posicao = to_string(tamanho);
                        splay_adiciona(arv, conteudo);
                    }

                    //Cálculo da posição da próxima linha:
                    tamanho = linha.size() + tamanho + 1;

                }
            }
        }
    }
}
