#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>

using namespace std;

int acc = 0;

//.\gustavoassuncao_202300083453_quicksort.exe entradaquick.txt saidaquick.txt

struct op{
    string tipo = "N";
    int nOp;
};

void troca(int* V1, int* V2){
    acc++;
    int var = *V1;
    *V1 = *V2;
    *V2 = var;
}

int lomuto(int* V, int inicio, int fim) {
    int pivo = V[fim], cont1 = inicio - 1, cont2 = inicio;

    for(cont2 = inicio; cont2 < fim; cont2++){
        if(V[cont2] <= pivo) {
            troca(&V[++cont1], &V[cont2]);
        }
    }
    troca(&V[++cont1], &V[fim]);

    return cont1;
}

int hoare(int* V, int inicio, int fim) {
    int pivo = V[inicio], cont1 = inicio - 1, cont2 = fim + 1;

    while(1) {
        while(V[--cont2] > pivo);
        while(V[++cont1] < pivo);
        if(cont1 < cont2) {
            troca(&V[cont1], &V[cont2]);
        }
        else return cont2;
    }
}

int lomuto_rand(int* V, int inicio, int fim){
    troca(&V[fim], &V[inicio + (abs(V[inicio]) % (fim - inicio + 1))]);

    return lomuto(V, inicio, fim);
}

int hoare_rand(int* V, int inicio, int fim){
    troca(&V[inicio], &V[inicio + (abs(V[inicio]) % (fim - inicio + 1))]);
    
    return hoare(V, inicio, fim);
}

int hoare_med(int* V, int inicio, int fim){
    int n1 = inicio + (fim - inicio + 1)/4;
    int n2 = inicio + (fim - inicio + 1)/2;
    int n3 = inicio + 3*(fim - inicio + 1)/4;

    if((V[n1] >= V[n2] && V[n1] <= V[n3])||(V[n1] >= V[n3] && V[n1] <= V[n2]))
        troca(&V[n1], &V[inicio]);
    else if((V[n2] >= V[n1] && V[n2] <= V[n3])||(V[n2] >= V[n3] && V[n2] <= V[n1]))
        troca(&V[n2], &V[inicio]);
    else
        troca(&V[n3], &V[inicio]);

    return hoare(V, inicio, fim);
}

int lomuto_med(int* V, int inicio, int fim){
    int n1 = inicio + (fim - inicio + 1)/4;
    int n2 = inicio + (fim - inicio + 1)/2;
    int n3 = inicio + 3*(fim - inicio + 1)/4;

    if((V[n1] >= V[n2] && V[n1] <= V[n3])||(V[n1] >= V[n3] && V[n1] <= V[n2]))
        troca(&V[n1], &V[fim]);
    else if((V[n2] >= V[n1] && V[n2] <= V[n3])||(V[n2] >= V[n3] && V[n2] <= V[n1]))
        troca(&V[n2], &V[fim]);
    else
        troca(&V[n3], &V[fim]);

    return lomuto(V, inicio, fim);
}

void quicksort(int* V, int inicio, int fim, int modo){
    acc++;
    if(fim > inicio){
        int pivo;
        if(modo == 1)
            pivo = hoare(V, inicio, fim);
        else if(modo == 2)
            pivo = lomuto(V, inicio, fim);
        else if(modo == 3)
            pivo = hoare_rand(V, inicio, fim);
        else if(modo == 4)
            pivo = lomuto_rand(V, inicio, fim);
        else if(modo == 5)
            pivo = hoare_med(V, inicio, fim);
        else
            pivo = lomuto_med(V, inicio, fim);

        if(modo%2 == 0)
            quicksort(V, inicio, pivo - 1, modo);
        else
            quicksort(V, inicio, pivo, modo);
        quicksort(V, pivo + 1, fim, modo);
    }
}

void merge(op* lista, op* aux, int inicio, int meio, int fim){
    
    for(int i = inicio; i <= meio; i++)
        aux[i] = lista[i];

    for(int i = meio + 1; i <= fim; i++)
        aux[i] = lista[i];

    int cont1 = inicio;
    int cont2 = meio + 1;
    int cont = inicio;
    
    while(cont1 <= meio && cont2 <= fim){
        if(aux[cont1].nOp <= aux[cont2].nOp)
            lista[cont++] = aux[cont1++];
        else 
            lista[cont++] = aux[cont2++];
    }

    while(cont1 <= meio)
        lista[cont++] = aux[cont1++];
    while(cont2 <= fim)
        lista[cont++] = aux[cont2++];
}

void mergeSort(op* lista, op* aux, int inicio, int fim){
    if(inicio < fim){
        int meio = inicio + (fim - inicio)/2;
        
        mergeSort(lista,aux, inicio, meio);
        mergeSort(lista,aux, meio + 1, fim);
        
        merge(lista, aux, inicio, meio, fim);
    }
}

int main(int argc, char *argv[]){

    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);
    
    string str;
    int totalListas, tamanho;

    getline(entrada, str);
    totalListas = stoi(str);
    op lista[totalListas][7];
    op aux2[7];

    for(int i = 0; i < totalListas; i++){
        getline(entrada, str);
        tamanho = stoi(str);
        
        int listaAtual[tamanho], aux[tamanho];
        
        lista[i][0].nOp = tamanho;
        
        getline(entrada, str);
        istringstream marcador(str);

        for(int j = 0; j < tamanho; j++){
            getline(marcador, str, ' ');
            listaAtual[j] = stoi(str);
        }

        memcpy(aux, listaAtual, tamanho * sizeof(int));
        quicksort(aux, 0, tamanho - 1, 1);
        lista[i][1].nOp = acc;
        lista[i][1].tipo = "HP";
        acc = 0;

        memcpy(aux, listaAtual, tamanho * sizeof(int));
        quicksort(aux, 0, tamanho - 1, 2);
        lista[i][2].nOp = acc;
        lista[i][2].tipo = "LP";
        acc = 0;

        memcpy(aux, listaAtual, tamanho * sizeof(int));
        quicksort(aux, 0, tamanho - 1, 3);
        lista[i][3].nOp = acc;
        lista[i][3].tipo = "HA";
        acc = 0;
        
        memcpy(aux, listaAtual, tamanho * sizeof(int));
        quicksort(aux, 0, tamanho - 1, 4);
        lista[i][4].nOp = acc;
        lista[i][4].tipo = "LA";
        acc = 0;
        
        memcpy(aux, listaAtual, tamanho * sizeof(int));
        quicksort(aux, 0, tamanho - 1, 5);
        lista[i][5].nOp = acc;
        lista[i][5].tipo = "HM";
        acc = 0;
        
        quicksort(listaAtual, 0, tamanho - 1, 6);
        lista[i][6].nOp = acc;
        lista[i][6].tipo = "LM";
        acc = 0;
        
        mergeSort(lista[i], aux2, 1, 6);

        
    }
    
    for(int i = 0; i < totalListas; i++){
        saida << i << ":";
        for(int j = 0; j < 7; j++){
            saida << lista[i][j].tipo << "(" << lista[i][j].nOp << ")";
            if(j != 6)
                saida << ",";
        }
        saida << endl;
    }
        
    entrada.close();
    saida.close();

    
    
    return 0;
}