#ifndef VARIABLES_H
#define VARIABLES_H

#include <array>
#include <string>
#include <vector>

using namespace std;

class Variable {
private:
  string nombre;  // Nombre de la variable
  bool esEntrada; // true si es variable de entrada, false si es variable de salida
  float min, max; // Rango de la variable, se asume que el rango es un intervalo [min, max]
  vector<string> etiquetas_rangos; // Para almacenar el nombre de cada etiqueta de rango asociada a la variable
  vector<bool> tipo_etiquetas; // Para almacenar el tipo de cada etiqueta (triangular// sería true o trapecial seria false)
  vector<array<float, 4>> puntos_etiquetas; // Para almacenar los puntos de cada etiqueta, si es triangular se usan los primeros 3 puntos y el cuarto se deja en 0, si es trapecial se usan los 4 puntos
  vector<float> grados_pertenencia; // Almacena los resultados de fuzzificar()
public:
  Variable(const string &n, bool entrada, float m, float M); // Constructor para inicializar los atributos de la clase Variable
  const string &getNombre() const; // Método que retorna el nombre de la variable
  bool esVariableEntrada() const; // Retorna true si la variable es de entrada, false si es de salida
  float getMin() const; // Método que retorna el valor mínimo de la variable
  float getMax() const; // Método que retorna el valor máximo de la variable
  void agregarEtiquetaRango(const string &etiqueta, float punto1, float punto2, float punto3); // Método para agregar una etiqueta de rango triangular a la variable, se le pasa el nombre de la etiqueta y los 3 puntos que definen el triángulo
  void agregarEtiquetaRango(const string &etiqueta, float punto1, float punto2, float punto3, float punto4); // Método para agregar una etiqueta de rango trapecial a la variable, se le pasa el nombre de la etiqueta y los 4 puntos que definen el trapecio
  const vector<string> &getEtiquetasRangos() const; // Método que retorna las etiquetas de rango de la variable
  const vector<array<float, 4>> &getPuntosEtiquetas() const; // Método que retorna los puntos de las etiquetas de la variable
  const vector<float> &getGradosPertenencia() const; // Retorna los valores de pertenencia calculados
  void fuzzificar(float valor); // Método para fuzzificar un valor real y almacenar los grados internamente
  void imprimir() const; // Método para imprimir los datos de la variable
};

#endif // VARIABLES_H