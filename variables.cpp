#include "variables.h"
#include <iostream>

using namespace std;

// Metodo constructor para inicializar los atributos de la clase Variables, asignando los valores que recibe como parámetros a los atributos
Variable::Variable(const string& n, bool entrada, float m, float M) {
    nombre = n;
    esEntrada = entrada;
    min = m;
    max = M;
}

// Método para obtener el nombre de la variable
const string& Variable::getNombre() const {
    return nombre;
}

// Retorna true si la variable es de entrada, false si es de salida
bool Variable::esVariableEntrada() const {
    return esEntrada;
}

// Método para obtener el valor mínimo del rango de la variable
float Variable::getMin() const {
    return min;
}

// Método para obtener el valor máximo del rango de la variable
float Variable::getMax() const {
    return max;
}

// Este método agrega una etiqueta de rango a la variable, con su tipo (triangular o trapecial) y sus puntos correspondientes
void Variable::agregarEtiquetaRango(const string& etiqueta, float punto1, float punto2, float punto3) {
    etiquetas_rangos.push_back(etiqueta); // Agrega el nombre de la etiqueta al vector de etiquetas
    tipo_etiquetas.push_back(true); // true para triangular
    puntos_etiquetas.push_back({punto1, punto2, punto3, 0.0f}); // Rellenamos los puntos restantes con 0 para mantener la consistencia
}

void Variable::agregarEtiquetaRango(const string& etiqueta, float punto1, float punto2, float punto3, float punto4) {
    etiquetas_rangos.push_back(etiqueta); // Agrega el nombre de la etiqueta al vector de etiquetas
    tipo_etiquetas.push_back(false); // false para trapecial
    puntos_etiquetas.push_back({punto1, punto2, punto3, punto4}); // Agrega los puntos del trapecio al vector de puntos de etiquetas
}

// Método para obtener el vector de etiquetas de rango
const vector<string>& Variable::getEtiquetasRangos() const {
    return etiquetas_rangos;
}

// Método para obtener el vector de puntos de etiquetas
const vector<array<float, 4>>& Variable::getPuntosEtiquetas() const {
    return puntos_etiquetas;
}

// Retorna los valores de pertenencia calculados
const vector<float>& Variable::getGradosPertenencia() const {
    return grados_pertenencia;
}

// Método para fuzzificar un valor real y almacenar los grados internamente
void Variable::fuzzificar(float valor) {
    grados_pertenencia.clear();
    for (int i = 0; i < (int)etiquetas_rangos.size(); ++i) {
        float f_Ent = 0.0f;
        array<float, 4> p = puntos_etiquetas[i];
        if (tipo_etiquetas[i]) { // Triangular
            if (valor < p[0] || valor > p[2]) f_Ent = 0.0f;
            else if (valor == p[1]) f_Ent = 1.0f;
            else if (valor >= p[0] && valor < p[1]) f_Ent = (valor - p[0]) / (p[1] - p[0]);
            else f_Ent = (p[2] - valor) / (p[2] - p[1]);
        } else { // Trapecial
            if (valor < p[0] || valor > p[3]) f_Ent = 0.0f;
            else if (valor >= p[1] && valor <= p[2]) f_Ent = 1.0f;
            else if (valor >= p[0] && valor < p[1]) f_Ent = (valor - p[0]) / (p[1] - p[0]);
            else f_Ent = (p[3] - valor) / (p[3] - p[2]);
        }
        grados_pertenencia.push_back(f_Ent);
    }
}

// Método para imprimir la información de la variable, incluyendo su nombre, tipo, rango y etiquetas de rango con sus puntos correspondientes
void Variable::imprimir() const {
    string tipoVariable;
    if (esEntrada) {
        tipoVariable = "Entrada";
    } else {
        tipoVariable = "Salida";
    }
    cout << "Variable: " << nombre << " | Tipo: " << tipoVariable << " | Rango: [" << min << ", " << max << "]" << endl;
    for (int i = 0; i < (int)etiquetas_rangos.size(); ++i) {
        string tipoEtiqueta;
        if (tipo_etiquetas[i]) {
            tipoEtiqueta = "Triangular";
        } else {
            tipoEtiqueta = "Trapecial";
        }
        cout << "   - Etiqueta: " << etiquetas_rangos[i] << " | Tipo: " << tipoEtiqueta << " | Puntos: ";
        if(tipo_etiquetas[i]) { // Si es triangular, imprimimos solo los primeros 3 puntos
            for (int j = 0; j < 3; ++j) {
                cout << puntos_etiquetas[i][j] << " ";
            }
        } else { // Si es trapecial, imprimimos los 4 puntos
            for (int j = 0; j < 4; ++j) {
                cout << puntos_etiquetas[i][j] << " ";
            }
        }
        cout << endl;
    }
}