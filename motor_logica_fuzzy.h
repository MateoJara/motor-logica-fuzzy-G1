#ifndef MOTOR_LOGICA_FUZZY_H
#define MOTOR_LOGICA_FUZZY_H

#include <string>
#include <vector>
#include "variables.h"
#include "base_de_conocimiento.h"

class MotorLogicaFuzzy {
private:
    base_de_conocimiento base; // Para almacenar las reglas del motor de lógica difusa
public:
    MotorLogicaFuzzy(); // Constructor sin parámetros
    MotorLogicaFuzzy(base_de_conocimiento base); // Constructor con parámetros para inicializar el motor con una variable
    ~MotorLogicaFuzzy(); // Destructor
    void agregarBase(base_de_conocimiento base); // Método para agregar una base de conocimiento al motor de lógica difusa
    base_de_conocimiento& getBase(); // Método para obtener la base de conocimiento almacenada en el motor de lógica difusa
    float pertenenciaTriangular(float x, float a, float b, float c); // Método para calcular la función de pertenencia de un valor x a un conjunto difuso con forma triangular definida por los puntos a, b y c
    float pertenenciaTrapecial(float x, float a, float b, float c, float d); // Método para calcular la función de pertenencia de un valor x a un conjunto difuso con forma trapecial definida por los puntos a, b, c y d
    void imprimirVariables() const; // Método para imprimir las variables almacenadas en la base de conocimiento del motor de lógica difusa
    float inferir(const vector<float>& valoresEntrada); // Método para realizar la inferencia difusa, se le pasa un vector con los valores de entrada correspondientes a las variables de entrada almacenadas en la base de conocimiento, retorna el valor inferido para la variable de salida
};

#endif // MOTOR_LOGICA_FUZZY_H