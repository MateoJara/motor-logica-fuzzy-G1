#ifndef REGLA_H
#define REGLA_H

#include <string>
#include <vector>
#include "variables.h"

using namespace std;

class Regla {
private:
    vector<pair<Variable*, string>> antecedentes; // Punteros a variables de entrada y sus etiquetas
    vector<string> operadores; // Operadores lógicos ('Y' o 'O')
    Variable* salida; // Puntero a la variable de salida
    string etiquetaSalida; // Etiqueta de la variable de salida
public:
    Regla(const vector<pair<Variable*, string>>& ants, const vector<string>& ops, Variable& s, const string& etS); // Constructor
    ~Regla(); // Destructor
    void imprimirEnLenguajeNatural() const; // Método para imprimir la regla en lenguaje natural
    void imprimirParametros() const; // Método para imprimir los parámetros de la regla
    const vector<pair<Variable*, string>>& getAntecedentes() const; // Obtener los antecedentes
    const vector<string>& getOperadores() const; // Obtener los operadores lógicos
    Variable* getSalida() const; // Método para obtener el puntero a la variable de salida
    const string& getEtiquetaSalida() const; // Método para obtener la etiqueta de la variable de salida
    void agregarAntecedente(Variable& var, const string& etiqueta, const string& operador = "Y"); // Método para agregar o modificar antecedentes
    void modificarSalida(Variable& nuevaSalida, const string& nuevaEtiqueta); // Método para modificar salida
};

#endif // REGLA_H