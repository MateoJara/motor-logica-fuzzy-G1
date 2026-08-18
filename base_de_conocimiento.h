#ifndef BASE_DE_CONOCIMIENTO_H
#define BASE_DE_CONOCIMIENTO_H

#include "regla.h"
#include "variables.h"
#include <vector>

class base_de_conocimiento {
private:
  vector<Variable> variables; // Vector para almacenar las variables de entrada y salida
  vector<Regla> reglas; // Vector para almacenar las reglas del motor de lógica difusa
public:
  base_de_conocimiento();  // Constructor sin parámetros
  ~base_de_conocimiento(); // Destructor
  bool cargarVariables(string ruta); // Método para cargar las variables desde un archivo de texto, se le pasa la ruta del archivo como parámetro
  bool cargarReglas(string ruta); // Método para cargar las reglas desde un archivo de texto, se le pasa la ruta del archivo como parámetro
  void imprimirVariables() const;   // Método para imprimir las variables almacenadas
  void imprimirReglas() const; // Método para imprimir las reglas almacenadas
  void agregarVariable(const Variable &var); // Método para agregar una variable al motor de lógica difusa
  void agregarRegla(const Regla &regla); // Método para agregar una regla al motor de lógica difusa
  vector<Variable> &getVariables(); // Método para obtener el vector de variables almacenadas
  vector<Regla> &getReglas(); // Método para obtener el vector de reglas almacenadas
  bool hayVariablesEntrada() const; // Método para verificar si hay variables de entrada almacenadas
  Variable &buscarVariable(string &nombreVariable); // Método para buscar una variable por su nombre, retorna una referencia a la variable encontrada
  Regla &buscarRegla(const vector<pair<string, string>> &antecedentesBusqueda, string &varSalida, string &etiquetaSalida); // Método para buscar una regla
  bool verificarOperabilidad(vector<Variable> &vars); // Método para verificar la operabilidad del motor de lógica difusa, retorna true si el motor es operable, false en caso contrario
};

#endif // BASE_DE_CONOCIMIENTO_H