#include <iostream>

#include "base_de_conocimiento.h"
#include "motor_logica_fuzzy.h"
#include "variables.h"
using namespace std;

void evaluarPertenencia(Variable var); // Función para evaluar la pertenencia de un valor a las etiquetas de una variable, se le pasa la variable como parámetro
MotorLogicaFuzzy motor; // Se crea una instancia del motor de lógica difusa para almacenar las variables y reglas que se leerán del archivo

int main() {
  string ruta, linea; // Se define la variable ruta para almacenar la ruta del archivo de reglas y la variable linea para almacenar cada línea del archivo mientras se lee
  // Se le da la bienvenida al usuario y se le pide que ingrese la ruta del archivo de reglas
  cout << "Bienvenido al sistema de resolución por lógica difusa.\n ";
  // A continuación escriba la ruta a su archivo de reglas:
  ruta = "variables_fuzzy.txt"; // Capturamos la ruta del archivo en la variable ruta
  base_de_conocimiento base; // Se crea una instancia de la clase base_de_conocimiento para almacenar las variables leídas del archivo
  base.cargarVariables(ruta); // Se llama al método cargarVariables para leer las variables del archivo y almacenarlas en la base de conocimiento
  motor.agregarBase(base); // Se agrega la base de conocimiento al motor de lógica difusa
  // Le mostramos al usuario las variables cargadas al motor de lógica tras leer el archivo
  cout << "\t\tVariables cargadas:" << endl;
  base.imprimirVariables();

  bool hayVariableBuscada = true;
  while (hayVariableBuscada) { // Mientras que el usuario desee seguir evaluando la pertenencia en variables
    string nombreVariable;// Leeremos que variable desea evaluar
    cout << "\nIngrese el nombre de una variable de entrada para evaluar (o 'salir' para terminar): ";
    cin >> nombreVariable;
    if (nombreVariable == "salir") { // Evaluamos que no desee salir del programa
      hayVariableBuscada = false;
      cout << "Saliendo del evaluador de pertenencia..." << endl;
    } else {
      try {
        Variable &variable = base.buscarVariable(nombreVariable); // Se busca la variable ingresada
        cout << "Variable encontrada." << endl;
        evaluarPertenencia(variable); // Si se encuentra la variable, se llama a la función evaluarPertenencia
      } catch (...) {
        cout << "No se encontró una variable con el nombre '" << nombreVariable << "'. Por favor, intente nuevamente." << endl;
      }
    }
  }

  base.cargarReglas("reglas.txt"); // Se llama al método cargarReglas para leer las reglas del archivo y almacenarlas en la base de conocimiento
  base.imprimirReglas(); // Le mostramosal usuario las reglas cargadas al motor de lógica tras leer el archivo
  motor.agregarBase(base); // Aquí agregamos para que el motor reciba la base CON las reglas cargadas
  bool inferir = true;
  while (inferir) {
    if (base.hayVariablesEntrada()) { // Verificamos que haya variables de entrada configuradas en la base de conocimiento para poder realizar la inferencia
      vector<float> valoresEntrada(base.getVariables().size(),0.0f); // Creamos un vector de floats para almacenar los valores de entrada que se le pedirán al usuario, inicializados en 0.0f
      vector<Variable> &vars = base.getVariables(); // Obtenemos el vector de variables de la base de conocimiento
      cout << "\nIniciando proceso de inferencia..." << endl;

      float valorDistanciaDerecho, valorDistanciaIzquierdo, distanciaMinima;
      cout << "\nIngrese el valor para la medición de distancia del sensor derecho (En CM): ";
      cin >> valorDistanciaDerecho;
      cout << "\nIngrese el valor para la medición de distancia del sensor izquierdo (En CM): ";
      cin >> valorDistanciaIzquierdo;

      // Se toma la menor de las dos distancias.
      if (valorDistanciaIzquierdo < valorDistanciaDerecho) {
        distanciaMinima = valorDistanciaIzquierdo;
      } else { // En caso de estar más cerca del lado derecho, se le suma 300 cm a la distancia del sensor derecho
        distanciaMinima = 300 + valorDistanciaDerecho;
      }

      cout << "\nDistancia mínima calculada: " << distanciaMinima << " CM" << endl;

      float velocidad;
      cout << "\nIngrese el valor de la velocidad actual del robot (En CM/s): ";
      cin >> velocidad;

      valoresEntrada[0] = distanciaMinima; // Variable de entrada: Distancia (índice 0)
      valoresEntrada[1] = velocidad; // Variable de entrada: Velocidad (índice 1)

      // Ejecutar el motor de inferencia
      float resultado = motor.inferir(valoresEntrada);
      cout << "\nEl proceso de inferencia ha finalizado con éxito." << endl;
    } else {
      cout << "\nError: No hay variables de entrada configuradas para realizar la inferencia." << endl; // Si no hay variables de entrada, se muestra un mensaje de error indicando que no se puede realizar la inferencia
    }
    cout << "\n¿Desea realizar otra inferencia? (s/n): ";
    char respuesta;
    cin >> respuesta;
    if (respuesta != 's' && respuesta != 'S') {
      inferir = false;
      cout << "Saliendo del programa..." << endl;
    }
  }
  return 0;
}

void evaluarPertenencia(Variable var) {
  // Le pedimos al usuario que ingrese un valor para evaluar la pertenencia a las etiquetas de la variable
  cout << "Evaluando pertenencia para la variable " << var.getNombre() << ", ingrese el valor del que desea evaluar la pertenencia: ";
  float valor;
  cin >> valor;
  
  // Llamamos al nuevo método interno de la variable que fuzzifica y guarda los valores
  var.fuzzificar(valor);
  
  const vector<float>& grados = var.getGradosPertenencia();
  for (int i = 0; i < (int)var.getEtiquetasRangos().size(); ++i) { 
    string etiqueta = var.getEtiquetasRangos()[i];
    cout << "Grado de pertenencia a la etiqueta '" << etiqueta << "': " << grados[i] << endl;
  }
}