#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cctype>

using namespace std;

//.\gustavoassuncao_202300083453_compressao.exe entradacomp.txt saidacomp.txt

/*
0->HUF(20.00%)=00
1->HUF(42.86%)=9C6B50
2->HUF(22.22%)=0000
2->RLE(22.22%)=09FF
3->HUF(25.00%)=C0
*/

//implementar huf

struct Seq{
    string* hexa;
    int nHexa;
    float pHUF = 1000;
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

/*
string entrada = "abracadabra";
    int frequencias[256] = {0};
    for (char c : entrada) frequencias[(unsigned char)c]++;

    no* raiz = construir_arvore(frequencias, 256);
    string tabela[256];
    construir_tabela(raiz, "", tabela);

    string compactado;
    compactar(entrada, compactado, tabela);

    cout << "Texto Compactado: " << compactado << endl;
    */

struct no {
    int F;
    string S;
    no* D;
    no* E;
    no* prox;
};

struct fila_p_min {
    no* inicio;
};

void inserir(fila_p_min* fpm, int freq, char c, no* D, no* E) {
    no* novo = new no{freq, string(1, c), D, E, NULL};
    if (!fpm->inicio || freq < fpm->inicio->F) {
        novo->prox = fpm->inicio;
        fpm->inicio = novo;
        return;
    }
    no* atual = fpm->inicio;
    while (atual->prox && atual->prox->F <= freq) {
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
}

no* extrair_min(fila_p_min* fpm) {
    if (!fpm->inicio) return NULL;
    no* menor = fpm->inicio;
    fpm->inicio = menor->prox;
    return menor;
}

int tamanho(fila_p_min* fpm) {
    int tam = 0;
    no* atual = fpm->inicio;
    while (atual) {
        tam++;
        atual = atual->prox;
    }
    return tam;
}

void construir_tabela(no* raiz, string codigo, string tabela[256]) {
    if (!raiz) return;
    if (raiz->S.length() == 1) {
        tabela[(unsigned char)raiz->S[0]] = codigo;
    }
    construir_tabela(raiz->D, codigo + "0", tabela);
    construir_tabela(raiz->E, codigo + "1", tabela);
}

no* construir_arvore(int H[], int n) {
    fila_p_min* fpm = new fila_p_min();
    fpm->inicio = NULL;
    for (int i = 0; i < n; i++) {
        if (H[i]) inserir(fpm, H[i], i, NULL, NULL);
    }
    while (tamanho(fpm) > 1) {
        no* x = extrair_min(fpm);
        no* y = extrair_min(fpm);
        inserir(fpm, x->F + y->F, '\0', x, y);
    }
    return extrair_min(fpm);
}

void anexar(string& C, const string& codificacao) {
    C += codificacao;
}

void compactar(const string& E, string& C, string T[256]) {
    for (char c : E) {
        anexar(C, T[(unsigned char)c]);
    }
}
    
string HUF(string* lista, int tamanho){
    int frequencias[256] = {0};
    for (int i = 0; i < tamanho; i++){
        //frequencias[(unsigned char)lista[i]]++; converter de string para unsigned char
    }
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
        //Sequencias[i].HUF = HUF();
        //Sequencias[i].pRLE = 100*(((float)Sequencias[i].RLE.length())/(float)(2*Sequencias[i].nHexa));
    }

    for(int i = 0; i < NumSeq; i++){
        if(Sequencias[i].pHUF < Sequencias[i].pRLE){
            saida << i << "->HUF(" << Sequencias[i].pHUF << "%)=" << Sequencias[i].HUF << endl;
        }else if(Sequencias[i].pHUF > Sequencias[i].pRLE){
            saida << i << "->RLE(" << Sequencias[i].pRLE << "%)=" << Sequencias[i].RLE << endl;
        } else {
            saida << i << "->HUF(" << Sequencias[i].pHUF << "%)=" << Sequencias[i].HUF << endl;
            saida << i << "->RLE(" << Sequencias[i].pRLE << "%)=" << Sequencias[i].RLE << endl;
        }
        
    }

    return 0;
}
