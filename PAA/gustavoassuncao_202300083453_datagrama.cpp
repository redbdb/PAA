#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//.\gustavoassuncao_202300083453_datagrama.exe entradadata.txt saidadata.txt

struct pacote{
    string* lista;//pode fazer?
    int tamanho;
    int indice;
};

void troca(pacote* V1, pacote* V2){
    pacote var = *V1;
    *V1 = *V2;
    *V2 = var;
}

void heapify(pacote* V, int T, int i){
    int P = i, E = 2*i + 1, D = 2*i + 2;//arrumar isso pra tirar funções esquerdo e direito

    if(E < T && V[E].indice > V[P].indice)
        P = E;
    if(D < T && V[D].indice > V[P].indice)
        P = D;
    
    if(P != i) {
        troca(&V[i], &V[P]);
        heapify(V, T, P);
    }
}

void maxHeap(pacote* V, int n){
    for(int i = n / 2 - 1; i >=0; i--)
        heapify(V, n, i);
}

void heapSort(pacote* V, int n){
    maxHeap(V, n);//fazer heap maximo

    for(int i = n - 1; i > 0; i--){
        troca(&V[0], &V[i]);

        heapify(V,1,0);
    }
}

int main(int argc, char *argv[]){
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);
    //pega o total de pacotes e quantidade de pacotes analisados
    string str;
    int totalPacotes, qtdPacotes;

    getline(entrada, str);
    istringstream marcador(str);

    getline(marcador, str,' ');
    totalPacotes = stoi(str);

    getline(marcador, str,' ');
    qtdPacotes = stoi(str);

    pacote pacotes[totalPacotes];
    
    for(int i = 0; i < totalPacotes; i++){//percorre a lista de pacotes
    //atribui o indice
    getline(entrada, str);
    getline(marcador, str, ' ');
    pacotes[i].indice = stoi(str);
    //atribui tamanho n e uma lista com tamanho n
    getline(marcador, str, ' ');
    pacotes[i].tamanho = stoi(str);
    pacotes[i].lista = new string[stoi(str)];
    //pega a lista de valores
    for(int j = 0; j < stoi(str); j++){
        getline(marcador, str, ' ');
        pacotes[i].lista[j] = str;
    }

        if(i%qtdPacotes==0){//condicional para fazer algo a cada n pacotes
            //organiza lista de struct
            //se estiver em ordem amostra
        }
    }

    entrada.close();
    saida.close();

    return 0;
}
