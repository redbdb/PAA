#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cctype>
#include <iomanip>

using namespace std;

//.\gustavoassuncao_202300083453_compressao.exe entradacomp.txt saidacomp.txt

/*
0->HUF(20.00%)=00
1->HUF(42.86%)=9C6B50
2->HUF(22.22%)=0000
2->RLE(22.22%)=09FF
3->HUF(25.00%)=C0
*/

//SO FALTA CORRGIR CONVERSÃO DE BINARIO PRA HEXADECIMAL NO FIM DO HUF

struct Seq{
    string* hexa;
    int nHexa;
    float pHUF;
    float pRLE;
    string HUF;
    string RLE;
};

string RLE(string* lista, int tamanho){
    string var, saida = "";
    int cont;
    stringstream ss;

    for(int i = 0; i < tamanho; i++){
        var = lista[i];
        cont = 1;
        ss.str("");
        ss.clear();

        while(lista[i] == lista[i+1] && i + 1 < tamanho){
            i++;
            cont++;
        }
        if(cont < 16){
            ss << uppercase << hex << cont;
            saida = saida + "0" + ss.str() + var;
        }
        else{
            ss << uppercase << hex << cont;
            saida = saida + ss.str() + var;
        }
           
    }
    return saida;
}

//HUF abaixo

struct no {
    int F;
    string S;
    no* D;
    no* E;
    no* prox;
};

struct fila_p_min{
    no* inicio;
};

int tamanho(fila_p_min* fpm){
    int cont = 0;
    no* atual = fpm->inicio;
    while (atual){
        cont++;
        atual = atual->prox;
    }
    return cont;
}

void inserir(fila_p_min* fpm, int freq, string s, no* D, no* E){
    no* novo = new no;
    novo->F = freq;
    novo->S = s;
    novo->D = D;
    novo->E = E;
    if(!fpm->inicio || freq < fpm->inicio->F){
        novo->prox = fpm->inicio;
        fpm->inicio = novo;
        return;
    }
    no* atual = fpm->inicio;
    while (atual->prox && atual->prox->F <= freq){
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
}

no* extrair_min(fila_p_min* fpm){
    no* saida = fpm->inicio;
    fpm->inicio = fpm->inicio->prox;
    return saida;
}

no* construir_arvore(int H[], int n) {
    // Criação de fila de prioridade mínima
    fila_p_min* fpm = new fila_p_min;
    fpm->inicio = NULL;
    // Inserindo símbolos não nulos na fila
    for(int i = 0; i < n; i++){
        if(H[i]) 
            inserir(fpm, H[i], to_string(i), NULL, NULL);//i é a posição em que se chou repetição, EM DECIMAL
    }
    // Combinação dos nós com menor frequência
    if(tamanho(fpm) == 1){
        no* x = extrair_min(fpm);
        inserir(fpm, x->F, "", NULL, x);
    }
    while(tamanho(fpm) > 1) {
        no* x = extrair_min(fpm);
        no* y = extrair_min(fpm);
        inserir(fpm, x->F + y->F, "", y, x);
    }
    // Retornando a raiz da árvore
    return extrair_min(fpm);
}

void construir_tabela(no* raiz, string cod,string* tabela){//problema aqui com certeza
    
    if(!raiz) return;
    if (raiz->S.length() > 0) {
        tabela[stoi(raiz->S)] = cod;
    }
    construir_tabela(raiz->D, cod + "1", tabela);
    construir_tabela(raiz->E, cod + "0", tabela);
}

string compactar(string* lista, string* tabela, int tamanho){
    string saida = "";
    string conversor = "";
    stringstream ss;

    for(int i = 0; i < tamanho; i++){
        conversor += tabela[stoi(lista[i], nullptr, 16)];
        
        
        while(conversor.length()%4 != 0){//a cada 4 ou qundo chegar no fim converte de binario 
            conversor = "0" + conversor;
            
        }

        if(conversor.length() == 4){//a cada 4 digitos converte pra hexadecimal em maisculo
            ss << uppercase << hex << stoi(conversor, nullptr, 2);
            saida += ss.str();
            conversor = "";
            ss.str("");
            ss.clear();
        }
    }

    if(saida.length()%2!=0)//coloca um 0 no fim pra ficar com numero par
        saida = saida + "0";
    
    return saida;
}  

string HUF(string* lista, int tamanho){
    //frequência de cada um dos itens de 00 até FF
    cout << "tamanho: " << tamanho << endl;
    int frequencias[256] = {0};
    for (int i = 0; i < tamanho; i++){
        frequencias[stoi(lista[i], nullptr, 16)]++;
    }
    
    //constroi a tabela***
    no* raiz = construir_arvore(frequencias, 256);
    string tabela[256];
    construir_tabela(raiz, "", tabela);
    
    //compactar vai retornar a string compressa
    return compactar(lista, tabela, tamanho);
}

int main(int argc, char const *argv[]){
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);
    
    int NumSeq;

    entrada >> NumSeq;

    Seq Sequencias[NumSeq];

    for(int i = 0; i < NumSeq; i++){
        entrada >> Sequencias[i].nHexa;
        Sequencias[i].hexa = new string[Sequencias[i].nHexa];
        for(int j = 0; j < Sequencias[i].nHexa; j++){
            entrada >> Sequencias[i].hexa[j];
        }
    }

    for(int i = 0; i < NumSeq; i++){
        Sequencias[i].RLE = RLE(Sequencias[i].hexa, Sequencias[i].nHexa);
        Sequencias[i].pRLE = 100*(((float)Sequencias[i].RLE.length())/(float)(2*Sequencias[i].nHexa));
        Sequencias[i].HUF = HUF(Sequencias[i].hexa, Sequencias[i].nHexa);
        Sequencias[i].pHUF = 100*(((float)Sequencias[i].HUF.length())/(float)(2*Sequencias[i].nHexa));
    }

    for(int i = 0; i < NumSeq; i++){
        if(Sequencias[i].pHUF < Sequencias[i].pRLE){
            saida << i << "->HUF(" << fixed << setprecision(2) << Sequencias[i].pHUF << "%)=" << Sequencias[i].HUF << endl;
        }else if(Sequencias[i].pHUF > Sequencias[i].pRLE){
            saida << i << "->RLE(" << fixed << setprecision(2) << Sequencias[i].pRLE << "%)=" << Sequencias[i].RLE << endl;
        } else {
            saida << i << "->HUF(" << fixed << setprecision(2) << Sequencias[i].pHUF << "%)=" << Sequencias[i].HUF << endl;
            saida << i << "->RLE(" << fixed << setprecision(2) << Sequencias[i].pRLE << "%)=" << Sequencias[i].RLE << endl;
        }
        
    }

    return 0;
}
