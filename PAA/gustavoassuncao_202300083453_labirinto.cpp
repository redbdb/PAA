#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//.\gustavoassuncao_202300083453_labirinto.exe entradaf.txt saida.txt

bool BackTrack(int x, int y, int Largura, int Altura, char** Matriz, ofstream &saida)//ajustar saida de BT e verificar lógica
{   
    if(x == 0 || x == Largura - 1 || y == 0 || y == Altura - 1){
        saida << "|FIM@" << y << "," << x;
        return true;
    }
        
    
    Matriz[x][y] = '1';

    if(Matriz[x + 1][y] == '0'){
        saida << "|D->" << y << "," << x + 1;
        if(BackTrack(x + 1, y, Largura, Altura, Matriz, saida)){
            return true;
        }else{
            saida << "|BT@" << y << "," << x + 1 << "->" << y << "," << x; 
        }
            
    }
    
    if(Matriz[x][y - 1] == '0'){
        saida << "|F->" << y - 1 << "," << x;
        if(BackTrack(x, y - 1, Largura, Altura, Matriz, saida)){
            return true;
        }else{
            saida << "|BT@" << y - 1 << "," << x << "->" << y << "," << x;
        }  
    }
    
    if(Matriz[x - 1][y] == '0'){
        saida << "|E->" << y << "," << x - 1;
        if(BackTrack(x - 1 , y, Largura, Altura, Matriz, saida)){
            return true;
        }else{
            saida << "|BT@" << y << "," << x - 1 << "->" << y << "," << x;
        }
    }

    if(Matriz[x][y + 1] == '0'){
        saida << "|T->" << y + 1 << "," << x;
        if(BackTrack(x, y + 1, Largura, Altura, Matriz, saida)){
            return true;
        }else{
            saida << "|BT@" << y + 1 << "," << x << "->" << y << "," << x;
        }
    }
    
    return false;
}   

int main(int argc, char const *argv[])
{
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);

    int nLab;
    int Largura, Altura;
    int Inicio[2] = {0,0};//coordenadas de inicio

    entrada >> nLab;

    char** Labirinto = new char*[100];
    for (int i = 0; i < 100; i++) {
        Labirinto[i] = new char[100];
    }
    
    for(int i = 0; i < nLab; i++)
    {

        entrada >> Largura >> Altura;

        //lê entrada e acha x
        for (int j = 0; j < Altura; j++)
        {
            for(int k = 0; k < Largura; k++)
            {
                entrada >> Labirinto[k][j];
                if (Labirinto[k][j] == 'X')
                {
                    Inicio[0] = k;
                    Inicio[1] = j;
                }
                
            }
        }
        
        saida << "L" << i << ":INI@" << Inicio[1] << "," << Inicio[0];

        //percorre a matriz função
        if(!BackTrack(Inicio[0],Inicio[1],Largura, Altura, Labirinto, saida))
            saida << "|" << "FIM@-,-";

        saida << endl;
    }

    for (int i = 0; i < 100; i++) {
        delete[] Labirinto[i]; 
    }
    delete[] Labirinto;

    return 0;
}