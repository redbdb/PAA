#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//.\gustavoassuncao_202300083453_datagrama.exe entradadata.txt saidadata.txt

struct pacote{
    string* lista;
    int tamanho;
    int indice;
};

void troca(pacote* V1, pacote* V2){
    pacote var = *V1;
    *V1 = *V2;
    *V2 = var;
}

void heapify(pacote* V, int T, int i){
    int P = i, E = 2*i + 1, D = 2*i + 2;

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
    maxHeap(V, n);

    for(int i = n - 1; i > 0; i--){
        troca(&V[0], &V[i]);

        heapify(V,i,0);
    }
}

int main(int argc, char *argv[]){
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);
    //pega o total de pacotes e quantidade de pacotes analisados
    string str;
    int totalPacotes, qtdPacotes;
    int cont = 0;

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
    istringstream marcador(str);
    //cout << "linha: " << str << endl; 
    getline(marcador, str, ' ');
    pacotes[i].indice = stoi(str);
    //atribui tamanho n e uma lista com tamanho n
    getline(marcador, str, ' ');
    pacotes[i].tamanho = stoi(str);
    pacotes[i].lista = new string[stoi(str)];
    //pega a lista de valores
    for(int j = 0; j < pacotes[i].tamanho; j++){
        getline(marcador, str, ' ');
        pacotes[i].lista[j] = str;
    }

    if((i+1)%qtdPacotes == 0){

        heapSort(pacotes, i+1);
        if(cont == pacotes[cont].indice){
            while(cont == pacotes[cont].indice){
                saida << "|";//amostrar os valores da lista abaixo;

                for(int j = 0; j < pacotes[cont].tamanho; j++){
                    saida << pacotes[cont].lista[j];
                    if(j!=pacotes[cont].tamanho-1)
                        saida << ",";
                }
                cont++;
            }
            saida << "|" << endl;
        }
        
    }
}
    
    entrada.close();
    saida.close();

    return 0;
}
