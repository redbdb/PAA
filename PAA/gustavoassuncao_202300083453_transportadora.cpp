#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>

using namespace std;
//.\gustavoassuncao_202300083453_transportadora.exe entradatransp.txt saidatransp.txt

/*[AAA1234]R$100.00,49KG(98%),10L(10%)->IJ777888999KL
[BBB5C67]R$5265.03,1250KG(63%),12000L(100%)->EF444555666GH,MN000111222OP,QR333444555ST
PENDENTE:R$49.99,2KG,1L->AB111222333CD*/

//testar primeiro com matriz 2d, alterar para 3d depois

//nao amostra casa decimal se o numero for inteiro. corrigir dps

struct Carro{
    string Placa;
    int Peso;
    int Volume;
};

struct Pacote{
    string Codigo;
    float Valor;
    int Peso;
    int Volume;
};

int main(int argc, char const *argv[]){
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);

    int TotalCarros, TotalItens, PesoMax = 0, VolMax = 0;

    entrada >> TotalCarros;
    
    Carro ListaCarros[TotalCarros];

    //enche a lista de carros, pega peso e volume maximo
    for(int i = 0; i < TotalCarros; i++){
        entrada >> ListaCarros[i].Placa >> ListaCarros[i].Peso >> ListaCarros[i].Volume;
        if(ListaCarros[i].Peso > PesoMax)
            PesoMax = ListaCarros[i].Peso;
        if(ListaCarros[i].Volume)
            VolMax = ListaCarros[i].Volume;
    }

    entrada >> TotalItens;

    Pacote ListaItens[TotalItens];
    bool ListaEnviados[TotalItens] = {false};

    //enche lista de itens
    for(int i = 0; i < TotalItens; i++){
        entrada >> ListaItens[i].Codigo >> ListaItens[i].Valor >> ListaItens[i].Peso >> ListaItens[i].Volume;
    }

    float Matriz[TotalItens + 1][PesoMax + 1] = {0};//[VolMax]//de 0 ate o valor

    //procedimento para ordenar a matriz****

    

    //vai maximizar por carro em ordem
    float SomaValor = 0;
    int SomaPeso = 0;
    int SomaVolume = 0;

    //variaveis para percorrer a matriz na hora de ler a programação dinamica
    int x;
    int y;

    for(int i = 0; i < TotalCarros; i++){
        saida << "[" << ListaCarros[i].Placa << "]R$";

        x = TotalItens;
        y = ListaCarros[i].Peso;
        //z = ListaCarros[i].Volume;

        while (x > 0){
           if((Matriz[x][y] != Matriz[x-1][y]) && !ListaEnviados[x-1]){
                ListaEnviados[x-1] = true;
                y = y - ListaItens[x].Peso;
                SomaPeso += ListaItens[x].Peso;
                SomaValor += ListaItens[x].Valor;
                SomaVolume += ListaItens[x].Volume;
                x--;
            }
            else
                x--;
        }
        saida << SomaValor << "," << SomaPeso << "KG(" << SomaPeso*100/ListaCarros[i].Peso << "%)," << SomaVolume << "L(" << SomaVolume*100/ListaCarros[i].Volume << "%)->" << "lista de nomes agui";
        saida << endl;
    } 

    //escreve os pendentes
    for(int i = 0; i < TotalItens; i++){
        if(ListaEnviados[i] == false){
            SomaValor += ListaItens[i].Valor;
            SomaPeso += ListaItens[i].Peso;
            SomaVolume += ListaItens[i].Volume;
        }
    }

    saida << "PENDENTE:R$" << SomaValor << "," << SomaPeso << "KG," << SomaVolume << "L->" << "lista de nomes agui";//como amostrar todos sem percorrer de novo nada??? lista???
    
    return 0;
}
