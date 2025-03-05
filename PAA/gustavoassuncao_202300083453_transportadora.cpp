#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <chrono>

using namespace std;

//.\gustavoassuncao_202300083453_transportadora.exe entradatranspf.txt saidatransp.txt
//.\gustavoassuncao_202300083453_transportadora.exe entradatransp.txt saidatransp.txt

/*
[AAA1234]R$100.00,49KG(98%),10L(10%)->IJ777888999KL
[BBB5C67]R$5265.03,1250KG(63%),12000L(100%)->EF444555666GH,MN000111222OP,QR333444555ST
PENDENTE:R$49.99,2KG,1L->AB111222333CD
*/

//reduzir tempo

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
    bool Enviado = false;
};

int main(int argc, char const *argv[]){
    auto inicio = std::chrono::high_resolution_clock::now();
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);
    
    int TotalCarros, TotalItens, PesoMax = 0, VolMax = 0;

    entrada >> TotalCarros;
    
    Carro ListaCarros[TotalCarros];

    for(int i = 0; i < TotalCarros; i++){
        entrada >> ListaCarros[i].Placa >> ListaCarros[i].Peso >> ListaCarros[i].Volume;
        if(ListaCarros[i].Peso > PesoMax)
            PesoMax = ListaCarros[i].Peso;
        if(ListaCarros[i].Volume > VolMax)
            VolMax = ListaCarros[i].Volume;
    }

    entrada >> TotalItens;

    Pacote ListaItens[TotalItens];

    for(int i = 0; i < TotalItens; i++){
        entrada >> ListaItens[i].Codigo >> ListaItens[i].Valor >> ListaItens[i].Peso >> ListaItens[i].Volume;
    }
    
    float* bloco = new float[(TotalItens + 1) * (PesoMax + 1) * (VolMax + 1)];

    float*** Matriz = new float**[TotalItens + 1];

    for (int i = 0; i <= TotalItens; i++) {
        Matriz[i] = new float*[PesoMax + 1];
        for (int j = 0; j <= PesoMax; j++) {
            Matriz[i][j] = bloco + (i * (PesoMax + 1) * (VolMax + 1)) + (j * (VolMax + 1));
        }
    }
    
    int x, y, z;

    float SomaValor = 0;
    int SomaPeso = 0;
    int SomaVolume = 0;
    string SomaNome = "";

    //int sla1 = 0, sla2 = 0, sla3 = 0;

    for(int i = 0; i < TotalCarros; i++){//otimizar isso
        saida << "[" << ListaCarros[i].Placa << "]R$";

        x = TotalItens;
        y = ListaCarros[i].Peso;
        z = ListaCarros[i].Volume;

        //ordena a matriz com todos os que ainda nao foram enviados
        for(int i = 0; i <= x; i++){
            for(int j = 0; j <= y; j++){
                for(int k = 0; k <= z; k++){
                    if(i == 0 || j == 0 || k == 0)
                        Matriz[i][j][k] = 0;
                    else if(ListaItens[i-1].Peso > j || ListaItens[i-1].Volume > k || ListaItens[i - 1].Enviado){
                        Matriz[i][j][k] = Matriz[i-1][j][k];
                    }else{
                        float var1 = ListaItens[i-1].Valor + Matriz[i-1][j-ListaItens[i-1].Peso][k-ListaItens[i-1].Volume];
                        float var2 = Matriz[i-1][j][k];
                        if(var1 > var2){
                            Matriz[i][j][k] = var1;
                        }else{
                            Matriz[i][j][k] = var2;
                        }
                    }
                }
            }
        }
        
        //lê a matriz
        while (x > 0){
            if((Matriz[x][y][z] != Matriz[x-1][y][z])){   
                ListaItens[x-1].Enviado = true;
                y -= ListaItens[x-1].Peso;
                z -= ListaItens[x-1].Volume;
                SomaPeso += ListaItens[x-1].Peso;
                SomaValor += ListaItens[x-1].Valor;
                SomaVolume += ListaItens[x-1].Volume;
                SomaNome =  "," + ListaItens[x-1].Codigo + SomaNome; 
            }
            x--;
        }
        
        //formata a linha de saida
        SomaNome.erase(0,1);
        saida << fixed << setprecision(2) << SomaValor << "," << SomaPeso << "KG(" << (int)round(SomaPeso*100.0/ListaCarros[i].Peso) << "%)," << SomaVolume << "L(" << (int)round(SomaVolume*100.0/ListaCarros[i].Volume) << "%)->" << SomaNome;
        saida << endl;

        SomaNome = "";
        SomaPeso = 0;
        SomaValor = 0;
        SomaVolume = 0;
    }

    for(int i = 0; i < TotalItens; i++){
        if(!ListaItens[i].Enviado){
            SomaValor += ListaItens[i].Valor;
            SomaPeso += ListaItens[i].Peso;
            SomaVolume += ListaItens[i].Volume;
            SomaNome += ListaItens[i].Codigo;
            SomaNome += ",";
        }
    }

    SomaNome.pop_back();
    saida << "PENDENTE:R$" <<  fixed << setprecision(2) << SomaValor << "," << SomaPeso << "KG," << SomaVolume << "L->" << SomaNome;

    delete[] bloco; 

    for (int i = 0; i <= TotalItens; i++) {
        delete[] Matriz[i]; 
    }

    delete[] Matriz; 
    
    auto fim = std::chrono::high_resolution_clock::now();

    // Calcula a duração
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);

    std::cout << "Tempo de execução: " << duracao.count() << " ms" << std::endl;

    return 0;
}