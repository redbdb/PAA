#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//.\gustavoassuncao_202300083453_sequenciamento.exe entradaseq.txt saidaseq.txt

/*XY1WZ2AB->100%
HUEBR->100%
ABCDE->67%
H1N1->25%*/

//reconhece espalhados de n em n

float acc = 0;

struct doenca{
    string id;
    int nGenes;
    string* Genes;
    float porcentagem = 0;
};

void CalcularTabela(int* K, string P, int sub){//criar "checkpoint" a cada sub letras 
    K[0] = -1;
    for(int i = 1, j = -1; i < P.length(); i++){
        while(j >= 0 && P[j + 1] != P[i])
            j = K[j];//aui
        if(P[j + 1] == P[i])
            j++;
        K[i] = j;
    }
}

void KMP(int* K, string T, string P, int sub){//a cada sub letras retornar para sub inves do indice
    int n = T.length(), m = P.length();
    CalcularTabela(K, P, sub);
    for(int i = 0, j = -1; i < n; i++){
        while(j >= 0 && P[j + 1] != T[i])
            j = K[j];//aqui
        if(P[j + 1] == T[i])
            j++;
        if(j == m - 1 || j + 1 >= 9*m/10.00){//talvez trocar para j + 2, ajustar?
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
        if(aux[cont1].porcentagem >= aux[cont2].porcentagem)
            lista[cont++] = aux[cont1++];
        else if(aux[cont1].porcentagem <= aux[cont2].porcentagem)
            lista[cont++] = aux[cont2++];
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

    //trecho para pegar entrdas: codigo genetico, tamanho da subcadeia, quantidade de doenças

    int TamanhoSubcadeia, NumDoencas;
    string Codigo;

    entrada >> TamanhoSubcadeia;
    entrada >> Codigo;
    entrada >> NumDoencas;

    //lista de structs do tipo doença com todas as doenças, struct tem nome da doença, quantidade de genes causadores, lista de genes causadores e 
    //porcentagem de chance de ter a doença no codigo, essa ultima parte é usada possteriormente, inicializada com 0

    doenca ListaDoencas[NumDoencas];

    for (int i = 0; i < NumDoencas; i++){
        entrada >> ListaDoencas[i].id;
        entrada >> ListaDoencas[i].nGenes;
        ListaDoencas[i].Genes = new string[ListaDoencas[i].nGenes];
        for(int j = 0; j < ListaDoencas[i].nGenes; j++){
            entrada >> ListaDoencas[i].Genes[j]; 
        } 
    }

    //percorre a lista de doenças, para cada doença verifica os codigos geneticos de cada um para ver se bate
    //se tiver o codigo lá, ele adiciona o contador, faz regra de 3 pra ver quantos genes causadores existem e achar porcentagem, o acc é em float para a porcentagem ser em float

    for(int i = 0; i < NumDoencas; i++){
        for(int j = 0; j < ListaDoencas[i].nGenes; j++){
            int K[ListaDoencas[i].Genes[j].length()];
            KMP(K ,Codigo ,ListaDoencas[i].Genes[j], TamanhoSubcadeia);
        }
        ListaDoencas[i].porcentagem = (acc*100/ListaDoencas[i].nGenes);
        acc = 0;
    }

    //organiza em ordem decrescente de porcentagem a lista

    doenca aux[NumDoencas];
    mergeSort(ListaDoencas, aux, 0, NumDoencas - 1);

    //escreve saida, percorrendo a lista e arredondando a porcentagem

    for(int i = 0; i < NumDoencas; i++){
        saida << ListaDoencas[i].id << "->" << round(ListaDoencas[i].porcentagem) << "%" << endl;
    }


    return 0;
}
