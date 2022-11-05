#include <bitset>
#include <iostream>
#include <math.h>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

int datos = 500; //Modificable
int dimensiones = 10; //Modificable
int intLimit = 8;
//Nota: Se pierde precision de Vecinos mas cercanos de acuerdo a la cantidad de datos y de dimensiones
//Los porcentajes de precision se disparan cuando tenemos pocos datos
const int inicio = 3, fin = 32;
const int bucles = 4;

int vecinos[bucles] = {5, 10, 15, 20};
//Cada elemento es la cantidad de vecinos que vamos a buscar

struct Nodo {
  int value, indice;
  Nodo(int v, int i) {
    value = v;
    indice = i;
  }
};

bool comparar(const Nodo &a, const Nodo &b) { return a.value < b.value; }

vector<vector<int>> fill(int data, int dims) {
  vector<vector<int>> dataFinal;

  srand(time(NULL));

  for (int i = 0; i < data; i++) {
    vector<int> data1Dim;
    for (int j = 0; j < dims; j++) {
      int temp = rand() % intLimit;
      data1Dim.push_back(temp);
    }
    dataFinal.push_back(data1Dim);
  }
  return dataFinal;
}

vector<bitset<inicio>> aBits(vector<int> dato) {
  vector<bitset<inicio>> aux;

  for (int i = 0; i < dato.size(); i++) {
    aux.push_back(bitset<inicio>(dato[i]));
  }

  return aux;
}

int aEntero(bitset<fin> dato) {
  int expo = 0, ultimo = 0;

  while ((dato >>= 1) != 0) {
    ultimo += dato[0] * pow(2, expo++);
  }
  return ultimo;
}

int combine(vector<bitset<inicio>> d) {
  bitset<fin> bFinal;

  for (int i = 0; i < dimensiones * inicio; i++) {
    bFinal[i] = d[i % dimensiones][i / dimensiones];
  }
  return aEntero(bFinal);
}

vector<int> ZOrder(vector<vector<int>> MD) {
  vector<int> dFinal;
  for (int i = 0; i < datos; i++) {
    vector<bitset<inicio>> aux;
    aux = aBits(MD[i]);
    dFinal.push_back(combine(aux));
  }
  return dFinal;
}

int distEuclideana(vector<int> MD, vector<int> d) {
  int resultado = 0;
  for (int i = 0; i < dimensiones; i++) {
    resultado += pow((MD[i] - d[i]), 2);
  }
  resultado = sqrt(resultado);
  return resultado;
}

vector<int> knnMulti(vector<vector<int>> MD, vector<int> d, int v) {
  v++;
  priority_queue<Nodo, vector<Nodo>, decltype(comparar) *> cola1(comparar);
  for (int i = 0; i < v; i++) {
    cola1.push(Nodo(INT32_MAX, -1));
  }

  for (int i = 0; i < datos; i++) {
    Nodo dist(distEuclideana(MD[i], d), i);
    cola1.push(dist);
    cola1.pop();
  }

  vector<int> indices;
  for (int i = 1; i < v; i++) {
    indices.push_back(cola1.top().indice);
    cola1.pop();
  }

  return indices;
}

vector<int> knnUni(vector<int> dZOrder, int d, int v) {
  v++;
  priority_queue<Nodo, vector<Nodo>, decltype(comparar) *> cola1(comparar);
  for (int i = 0; i < v; i++) {
    cola1.push(Nodo(INT32_MAX, -1));
  }

  for (int i = 0; i < datos; i++) {
    Nodo dist(abs(dZOrder[i] - d), i);
    cola1.push(dist);
    cola1.pop();
  }

  vector<int> indices;
  for (int i = 1; i < v; i++) {
    indices.push_back(cola1.top().indice);
    cola1.pop();
  }

  return indices;
}

int main() {

  vector<vector<int>> datosMulti = fill(datos, dimensiones);
  vector<int> datosZOrder = ZOrder(datosMulti);
  vector<vector<int>> vecinosM, vecinosU;

  for (int i = 0; i < bucles; i++) {
    int aleatorio = rand() % datos;
    vecinosM.push_back(knnMulti(datosMulti, datosMulti[aleatorio], vecinos[i]));
    vecinosU.push_back(knnUni(datosZOrder, datosZOrder[aleatorio], vecinos[i]));
  }

  int init = 0;
  float resultado;
  
  /*for (int i = 0; i < datosMulti.size(); i++) {
    cout << "Dato " << i + 1 << " en " << dimensiones << " dimensiones: ";
    for (int j = 0; j < datosMulti[i].size(); j++) {
      cout << datosMulti[i][j] << " ";
    }
    cout << endl;
  }
  
  cout << endl;

  for (int i = 0; i < datosZOrder.size(); i++) {
    cout << "Dato " << i + 1 << " despues de la indexacion en ZOrder: ";
    cout << datosZOrder[i] << endl;
  }
  cout << endl;*/
  
  for (int i = 0; i < bucles; i++) {
    resultado = 0;
    for (int j = 0; j < vecinos[i]; j++) {
      for (int k = 0; k < vecinos[i]; k++) {
        if (vecinosU[i][j] == vecinosM[i][k]) {
          resultado++;
        }
      }
    }
    init = vecinos[i] - 1;

    cout << "% de acierto para " << vecinos[i]
         << " vecinos mas proximos es: " << float((resultado / vecinos[i]) * 100)
         << "%. " << endl;
  }
}
