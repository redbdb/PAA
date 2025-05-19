#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//.\gustavoassuncao_202300083453_criptografia.exe entradaf.txt saida.txt

/*
s=E8613C49876806B074535ACF62DD673D
m=C1953BF75E2AA86EA5C94B7C4345FCE7
c=29C9E347235ACEDCC86B6F7A5791438960FC24F37C4A262492C6F79D12719231
*/

// Estrutura de número
typedef struct num_t {
    uint32_t* d;
    uint32_t n;
    uint32_t t;
} num_t;
// Definição da base numérica
const uint64_t b = ((uint64_t)(1) << (sizeof(uint32_t) << 3));

int main(int argc, char *argv[]){
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);
    
    int operacoes;

    entrada >> operacoes;

    string a, b, g, p, c, m;
    string comp;
    
    for(int i = 0; i < operacoes; i++){

        entrada >> comp;

        if(comp == "dh"){//troca chave diffie 
            entrada >> a >> b >> g >> p;
        }

        if(comp == "d"){//decriptação aes
            entrada >> c;
            
        }else{//encriptação aes
            entrada >> m;
            
        }
    }
}