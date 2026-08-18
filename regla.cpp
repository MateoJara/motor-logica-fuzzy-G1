#include "regla.h"

#include <iostream>
using namespace std;

// Constructor
Regla::Regla(const vector<pair<Variable*, string>>& ants, const vector<string>& ops, Variable& s, const string& etS) {
    antecedentes = ants;
    operadores = ops;
    salida = &s;
    etiquetaSalida = etS;
}

// Destructor
Regla::~Regla() {}

// Método para imprimir la regla en lenguaje natural
void Regla::imprimirEnLenguajeNatural() const {
    cout << "Si ";
    for (int i = 0; i < (int)antecedentes.size(); ++i) {
        cout << antecedentes[i].first->getNombre() << " es " << antecedentes[i].second;
        if (i < antecedentes.size() - 1) {
            string op;
            if (i < operadores.size()) {
                op = operadores[i];
            } else {
                op = "y";
            }
            if (op == "Y") op = "y";
            if (op == "O") op = "o";
            cout << " " << op << " ";
        }
    }
    cout << ", entonces " << salida->getNombre() << " es " << etiquetaSalida << "." << endl;
}

// Método para imprimir los parámetros de la regla
void Regla::imprimirParametros() const {
    for (int i = 0; i < (int)antecedentes.size(); ++i) {
        cout << "Entrada " << i + 1 << ": " << antecedentes[i].first->getNombre() << " - Etiqueta: " << antecedentes[i].second;
        if (i < antecedentes.size() - 1 && i < operadores.size()) {
            cout << " (Operador siguiente: " << operadores[i] << ")";
        }
        cout << "\n";
    }
    cout << "Salida: " << salida->getNombre() << " - Etiqueta: " << etiquetaSalida << endl;
}

// Método para obtener los antecedentes
const vector<pair<Variable*, string>>& Regla::getAntecedentes() const {
    return antecedentes;
}

// Método para obtener los operadores
const vector<string>& Regla::getOperadores() const {
    return operadores;
}

// Método para obtener el puntero a la variable de salida
Variable* Regla::getSalida() const {
    return salida;
}

// Método para obtener la etiqueta de la variable de salida
const string& Regla::getEtiquetaSalida() const {
    return etiquetaSalida;
}

// Método para agregar un antecedente nuevo a la regla
void Regla::agregarAntecedente(Variable& var, const string& etiqueta, const string& operador) {
    if (!antecedentes.empty()) {
        operadores.push_back(operador);
    }
    antecedentes.push_back({&var, etiqueta});
}

// Método para modificar la variable de salida y su etiqueta asociada
void Regla::modificarSalida(Variable& nuevaSalida, const string& nuevaEtiqueta) {
    salida = &nuevaSalida;
    etiquetaSalida = nuevaEtiqueta;
}