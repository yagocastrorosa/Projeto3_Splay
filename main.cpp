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

int main(int argc, char *argv[]) {
    string colunadigitada, valor, achou, posicaolinha, posicaocoluna, linha, verifica, verifica2;
    int k, posicaonum, pos=0, pos1;

    ifstream arq(argv[1]);

    //Caso o arquivo não possa ser aberto, o programa retornará uma mensagem de erro
    if (! arq.is_open()){
        cerr << "ao abrir o arquivo" << endl;
        return errno;
    }

    unordered_map<string,cadastro> dados;

    Splay<string> arv = cria_splay<string>();

    armazenamento(argv, argc, arv, dados);

    //Loop responsável por implementar o menu
    while(true) {

        cout << "coluna> ";
        getline(cin, colunadigitada);

        //Caso seja digitado ENTER o loop será encerrado e o programa será finalizado
        if (colunadigitada.empty()){
            break;
        }

        //Armazena o conteúdo presente na linha de comando (exceto nome do arquivo)
        for (k=2; k < argc; k++) {
            verifica = argv[k];
            verifica2 += verifica + "_";
        }

        //Caso a coluna digitada não tenha sido indexada, o programa informará isso e será encerrado
        if (verifica2.find(colunadigitada) == string::npos) {
            cout << "Coluna digitada não está foi indexada pela linha de comando." << endl;
            break;
        }

        while (true) {
            cout << "valor> ";
            getline(cin, valor);

            //Caso seja digitado ENTER o programa voltará a apresentar o prompt da coluna.
            if (valor.empty())
                break;

            //Loop para buscar as informações na árvore

            try {
                achou = splay_acessa(arv, valor);
            } catch (...) {
                cout << "Conteúdo digitado não foi encontrado na coluna." << endl;
                break;
            }

            cadastro mapa;

            //Mapa será uma struct que atribuirá os valores da coluna e posição do valor encontrado
            mapa = dados[achou];

            posicaolinha = mapa.posicao;


            //Caso seja encontrado apenas uma coluna com esta informação
            if (posicaolinha.find("_") == string::npos) {
                posicaonum = stoi(posicaolinha);

                //Será lida a linha do arquivo em que a palavra foi encontrada
                arq.seekg(posicaonum);
                getline (arq, linha);
                cout << linha << endl;

                //Se um dado existir em mais de uma linha ou coluna
            } else {
                while (true){

                    pos1 = posicaolinha.find("_", pos);

                    if (pos1 == string::npos){
                        break;
                    }

                    posicaonum = stoi(posicaolinha.substr(pos,pos1));

                    //Apresentará todas as linhas com o valor encontrado, exceto a última
                    arq.seekg(posicaonum);
                    getline (arq, linha);
                    cout << linha << endl;

                    pos = pos1 + 1;
                }

                //Apresentará a última linha que possui o valor encontrado
                pos1 = posicaolinha.size();

                posicaonum = stoi(posicaolinha.substr(pos,pos1));
                arq.seekg(posicaonum);
                getline (arq, linha);
                cout << linha << endl;
            }
        }
    }
    return 0;
}