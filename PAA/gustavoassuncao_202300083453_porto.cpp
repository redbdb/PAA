#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//.\gustavoassuncao_202300083453_porto.exe entradaporto.txt saidaporto.txt

struct Container{
    string cod;
    string cnpj;
    float peso;

    bool erroCnpj = false;
    float erroPeso = 0;

    int pos;
};

Container buscaBin(Container* lista, Container alvo, int inicio, int fim){//função que faz busca binaria
    while (fim >= inicio) {
        int meio = inicio + (fim - inicio) / 2;

        if (alvo.cod == lista[meio].cod)
            return lista[meio];
        else if (alvo.cod < lista[meio].cod) 
            return buscaBin(lista, alvo, inicio, meio - 1); 
        else
            return buscaBin(lista, alvo, meio + 1, fim);  
    }
    Container r = alvo;
    r.pos = -1;
    return r;
}

float erroP(float p1, float p2){//calculo simples do erro de peso, se for mais de 10% retorna, se nao desconsidera, 0
    float dif = p1 - p2;
    dif = abs(dif);
    if(round(dif*100/p1) > 10)
        return dif*100/p1;
    else
        return 0;
}

void mergeString(Container* lista, Container* aux, int inicio, int meio, int fim){//ordena por ordem de codigo alfabetico
    
    for(int i = inicio; i <= meio; i++)
        aux[i] = lista[i];

    for(int i = meio + 1; i <= fim; i++)
        aux[i] = lista[i];

    int cont1 = inicio;
    int cont2 = meio + 1;
    int cont = inicio;
    
    while(cont1 <= meio && cont2 <= fim){
        if(aux[cont1].cod <= aux[cont2].cod)
            lista[cont++] = aux[cont1++];
        else if(aux[cont1].cod >= aux[cont2].cod)
            lista[cont++] = aux[cont2++];
    }

    while(cont1 <= meio)
        lista[cont++] = aux[cont1++];
    while(cont2 <= fim)
        lista[cont++] = aux[cont2++];
}

void merge(Container* lista, Container* aux, int inicio, int meio, int fim){//ordena por: erro cnpj > erro peso maior > posiçõa na entrada

    for(int i = inicio; i <= meio; i++)
        aux[i] = lista[i];

    for(int i = meio+1; i <= fim; i++)
        aux[i] = lista[i];

    int cont1 = inicio;
    int cont2 = meio + 1;
    int cont = inicio;
    
    while(cont1 <= meio && cont2 <= fim){
        //se um dos dois tiver cnpj e o outro nao
        if(aux[cont1].erroCnpj && !aux[cont2].erroCnpj)
            lista[cont++] = aux[cont1++];
        else if(!aux[cont1].erroCnpj && aux[cont2].erroCnpj)
            lista[cont++] = aux[cont2++];
        //o com maior erro de peso
        else if(round(aux[cont1].erroPeso) > round(aux[cont2].erroPeso))
            lista[cont++] = aux[cont1++];
        else if(round(aux[cont2].erroPeso) > round(aux[cont1].erroPeso))
            lista[cont++] = aux[cont2++];
        //o com menor indice
        else if(aux[cont1].pos < aux[cont2].pos)
            lista[cont++] = aux[cont1++];
        else if(aux[cont1].pos > aux[cont2].pos)
            lista[cont++] = aux[cont2++];
             
    }

    while(cont1 <= meio )
        lista[cont++] = aux[cont1++];
    while(cont2 <= fim )
        lista[cont++] = aux[cont2++];
}

void mergeSort(Container* lista, Container* aux, int inicio, int fim, int tipo){//merge sort que pode ordenar por ordem alfabetica de codigo ou por prioridade na saida
    if(inicio < fim){
        int meio = inicio + (fim - inicio)/2;
        
        mergeSort(lista,aux, inicio, meio, tipo);
        mergeSort(lista,aux, meio + 1, fim, tipo);
        if(tipo == 2)
            merge(lista,aux, inicio, meio, fim);
        else if(tipo == 1)
            mergeString(lista, aux, inicio, meio, fim);
    }
}

int main(int argc, char *argv[]){

    ifstream entrada(argv[1]);
    ofstream saida(argv[2]);
    
    string str;

    getline(entrada, str);
    
    int n = stoi(str);
    
    Container cadastrados[n];
    
    for(int i = 0; i < n; i++){//percorre as linhas e pega informações dos cadastrados
        getline(entrada, str);
        
        istringstream marcador(str);
        getline(marcador, cadastrados[i].cod, ' ');

        getline(marcador, cadastrados[i].cnpj, ' ');

        getline(marcador, str, ' ');
        cadastrados[i].peso = stoi(str);

        cadastrados[i].pos = i;
    }
    
    getline(entrada, str);
    
    n = stoi(str);
    
    Container aux[n];
    
    Container selecionados[n];
    
    for(int i = 0; i < n; i++){//percorre linha e pega informações dos selecionados
        getline(entrada, str);
        
        istringstream marcador(str);
        getline(marcador, selecionados[i].cod, ' ');

        getline(marcador, selecionados[i].cnpj, ' ');

        getline(marcador, str, ' ');
        selecionados[i].peso = stoi(str);
    }
    
    mergeSort(selecionados, aux, 0, n - 1, 1);//ordena os selecionados
    
    Container errados[n];

    int c = 0;//usado pra alocar progressivamente os errados e servir de parametro de ate onde o for q ira percorre ele pode ir
    
    for(int i = 0; i < (int)(sizeof(cadastrados)/sizeof(cadastrados[0])); i++){//percorre o cadastrados
        Container comp = buscaBin(selecionados, cadastrados[i], 0, n - 1);//tenta achar nos selecionados
        if(comp.pos == -1)//se posição for -1 quer dizer que nao achou
            continue;
        if(comp.cnpj != cadastrados[i].cnpj){//pra caso tenha erro de cnpj
            errados[c] = comp;
            errados[c].erroCnpj = true;
            errados[c].pos = i;
            c++;
        } 
        else if(erroP(comp.peso, cadastrados[i].peso) != 0){//para caso tenha erro de peso
            errados[c] = comp;
            errados[c].erroPeso = erroP(cadastrados[i].peso, comp.peso);
            errados[c].pos = i;
            c++;   
        }
    }
    
    mergeSort(errados,aux, 0, c - 1, 2);//ordena o vetor final de saida
    
    for(int i = 0; i < c; i++){//formatação da saida
            if(errados[i].erroCnpj == true){
                saida << errados[i].cod << ":" << cadastrados[errados[i].pos].cnpj << "<->" << errados[i].cnpj << endl;
            }
            else if(errados[i].erroPeso != 0){
                saida << errados[i].cod << ":" << abs(cadastrados[errados[i].pos].peso - errados[i].peso) << "kg" << "(" << round(errados[i].erroPeso) << "%)" << endl;
            }
    }
    
    entrada.close();
    saida.close();

    return 0;
}