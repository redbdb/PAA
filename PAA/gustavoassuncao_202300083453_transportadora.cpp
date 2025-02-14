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

//calcular todas as possibilidades mais eficientes, percorrer matriz

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

    for(int i = 0; i < TotalCarros; i++){
        entrada >> ListaCarros[i].Placa >> ListaCarros[i].Peso >> ListaCarros[i].Volume;
        //botar procedimento pra pegar maior peso e vol
    }

    entrada >> TotalItens;

    Pacote ListaItens[TotalItens];

    for(int i = 0; i < TotalItens; i++){
        entrada >> ListaItens[i].Codigo >> ListaItens[i].Valor >> ListaItens[i].Peso >> ListaItens[i].Volume;
    }

    float Matriz[TotalItens + 1][PesoMax + 1];//[VolMax]
    
    //vai maximizar por carro em ordem 
    //procedimento para ordenar a matriz****
    //copiar o prox/anterior simula o efeito de nao mudar ou lista com cada um faltando ou nao(provavelmente melhor visto que tem que amostrar pendentes)
    //formatar saida
    
    return 0;
}
