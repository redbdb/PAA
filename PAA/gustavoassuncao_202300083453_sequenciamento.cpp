#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//.\gustavoassuncao_202300083453_sequenciamento.exe entradaseq.txt saidaseq.txt

float acc = 0;

struct doenca{
    int pos;
    string id;
    int nGenes;
    string* Genes;
    float porcentagem = 0;
};

void calcular_tabela(int* k, string P) {
    k[0] = -1;
    for(int i = 1, j = -1; i < (int)P.length(); i++) {
        while(j >= 0 && P[j + 1] != P[i])
            j = k[j];
        if(P[j + 1] == P[i])
            j++;
        k[i] = j;   
    }
}

void KMP(int* k, string T, string P, int sub, int marcador, int m){
    int n = T.length();
    int contL = 0;
    calcular_tabela(k, P);
    for(int i = 0, j = -1; i < n; i++) {
        while(j >= 0 && P[j + 1] != T[i]){
            if(contL < sub){
                contL = k[j] + 1;
                j = k[j];
            }else{
                KMP(k, T, P.substr(j + 1), sub, marcador + contL, m);
                return;
            }
        }   
        if(P[j + 1] == T[i]){ 
            j++;
            contL++;
        }
        if(marcador + contL >= 9*m/10.0 && contL >= sub){
            acc++;
            return;
        }
    }
}
    

void merge(doenca* lista, doenca* aux, int inicio, int meio, int fim){
    
    for(int i = inicio; i <= meio; i++)
        aux[i] = lista[i];

    for(int i = meio + 1; i <= fim; i++)
        aux[i] = lista[i];

    int cont1 = inicio;
    int cont2 = meio + 1;
    int cont = inicio;
    
    while(cont1 <= meio && cont2 <= fim){
        if(aux[cont1].porcentagem > aux[cont2].porcentagem)
            lista[cont++] = aux[cont1++];
        else if(aux[cont1].porcentagem < aux[cont2].porcentagem)
            lista[cont++] = aux[cont2++];
        else{
            if(aux[cont1].pos < aux[cont2].pos)
                lista[cont++] = aux[cont1++];
            else
                lista[cont++] = aux[cont2++];
        }
    }

    while(cont1 <= meio)
        lista[cont++] = aux[cont1++];
    while(cont2 <= fim)
        lista[cont++] = aux[cont2++];
}

void mergeSort(doenca* lista, doenca* aux, int inicio, int fim){
    if(inicio < fim){
        int meio = inicio + (fim - inicio)/2;
        
        mergeSort(lista,aux, inicio, meio);
        mergeSort(lista,aux, meio + 1, fim);
        
        merge(lista, aux, inicio, meio, fim);
    }
}

int main(int argc, char const *argv[]){
    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);

    int TamanhoSubcadeia, NumDoencas;
    string Codigo;

    entrada >> TamanhoSubcadeia;
    entrada >> Codigo;
    entrada >> NumDoencas;

    doenca ListaDoencas[NumDoencas];

    for (int i = 0; i < NumDoencas; i++){
        ListaDoencas[i].pos = i;
        entrada >> ListaDoencas[i].id;
        entrada >> ListaDoencas[i].nGenes;
        ListaDoencas[i].Genes = new string[ListaDoencas[i].nGenes];
        for(int j = 0; j < ListaDoencas[i].nGenes; j++){
            entrada >> ListaDoencas[i].Genes[j]; 
        } 
    }

    for(int i = 0; i < NumDoencas; i++){
        for(int j = 0; j < ListaDoencas[i].nGenes; j++){
            int K[ListaDoencas[i].Genes[j].length()];
            int m = ListaDoencas[i].Genes[j].length();
            KMP(K ,Codigo ,ListaDoencas[i].Genes[j], TamanhoSubcadeia, 0, m);
        }
        ListaDoencas[i].porcentagem = round(acc*100/ListaDoencas[i].nGenes);
        acc = 0;
    }

    doenca aux[NumDoencas];
    mergeSort(ListaDoencas, aux, 0, NumDoencas - 1);

    for(int i = 0; i < NumDoencas; i++){
        saida << ListaDoencas[i].id << "->" << ListaDoencas[i].porcentagem << "%" << endl;
    }


    return 0;
}