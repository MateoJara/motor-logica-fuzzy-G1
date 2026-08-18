#include "base_de_conocimiento.h"

#include "regla.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// Constructor sin parametros
base_de_conocimiento::base_de_conocimiento() {}

// Destructor
base_de_conocimiento::~base_de_conocimiento() {}

// Método para cargar las variables desde un archivo de texto, se le pasa la ruta del archivo como parámetro
bool base_de_conocimiento::cargarVariables(string ruta) {
  string linea; // Variable para almacenar cada línea del archivo mientras se lee
  ifstream archivo(ruta);  // Se define un objeto ifstream para manejar la lectura del archivo de reglas
  if (archivo.is_open()) { // Si el archivo se pudo abrir correctamente, se procede a leerlo
    cout << "Leyendo archivo de reglas..." << endl;
    while (getline(archivo, linea)) { // Mientras que el archivo tenga líneas para leer, se lee cada línea y se almacena en la variable linea
      if (linea.empty()){
        continue; // Saltar líneas vacías
      }
      stringstream ss(linea); // Aquí almacenamos toda la linea leida
      string primeraPalabra;  // En esta variable almacenamos la primera palabra de la línea para determinar si es una variable o una etiqueta
      ss >> primeraPalabra; // Capturamos la primera palabra de la línea y la guardamos
      if (primeraPalabra == "VARIABLE_IN" || primeraPalabra == "VARIABLE_OUT") { // Si la primera palabra es VARIABLE_IN o VARIABLE_OUT, entonces se trata de una variable, por lo que se leen el nombre y el rango de la variable
        string nombre; // Para almacenar el nombre de la variable
        float min, max; // Para almacenar el rango de la variable
        ss >> nombre >> min >> max; // Leemos el nombre y el rango de la variable
        Variable temp(nombre, primeraPalabra == "VARIABLE_IN", min, max);    // Creamos una instancia temporal de la clase
        agregarVariable(temp); // Agrega la variable a la base de conocimiento
      } else {
        // Si no es una variable, asumimos que es una etiqueta
        string etiqueta = primeraPalabra; // La primera palabra es el nombre
        string stringTipoForma; // Para almacenar el tipo de forma (triangular o trapecial) como string
        ss >> stringTipoForma;  // Se lee el tipo de forma (triangular o trapecial) como string
        bool tipoForma =  (stringTipoForma == "TRIANGULAR"); // Para almacenar el tipo de forma (triangular o trapecial) como booleano, true para triangular y false para trapecial
        float minA, maxA, minB, maxB; // Para almacenar los puntos del etiqueta
        if (tipoForma) {
          ss >> minA >> maxA >> minB; // Se leen el tipo de forma (triangular) y los puntos del etiqueta
        } else {
          ss >> minA >> maxA >> minB >> maxB; // Se leen el tipo de forma (triangular o trapecial) y los puntos del etiqueta
        }
        // Acá almacenamos temporalmente los puntos de la etiqueta leída para luego agregarlos a la última variable leída
        float punto;
        while (ss >> punto) {
          cout << punto << " ";
        }
        if (!variables.empty()) { // Validamos si la base ya tiene variables
          Variable &ultimaVar =
              variables.back(); // En caso que sí, obtenemos una referencia a la última variable agregada a la base para agregarle la etiqueta leída
          if (tipoForma) { // Si la forma es triangular, se llama al método agregarEtiquetaRango con los puntos 3 correspondientes
            ultimaVar.agregarEtiquetaRango(etiqueta, minA, maxA, minB);
          } else { // En caso que no sea triangular, se llama al método agregarEtiquetaRango con los puntos 4 correspondientes
            ultimaVar.agregarEtiquetaRango(etiqueta, minA, maxA, minB, maxB);
          }
        }
      }
    }
    archivo.close(); // Cerramos el archivo después de terminar de leerlo
    cout << "Lectura finalizada con exito" << endl; // Informamos al usuario que la lectura del archivo se ha completado exitosamente
  } else {
    return false; // Se retorna un código de error para indicar error en la carga
  }
  return true; // Retorna true si la carga fue exitosa, false en caso contrario
}

bool base_de_conocimiento::cargarReglas(string ruta) {
  ifstream archivo(ruta); // Se define un objeto ifstream para manejar la lectura del archivo de reglas
  string linea; // Variable para almacenar cada línea del archivo mientras se lee

  if (!archivo.is_open()){
    return false; // Si el archivo no se pudo abrir correctamente retornamos false para indicar que la carga de reglas ha fallado
  }
  cout << "Cargando reglas fuzzy..." << endl; // Informamos al usuario que se ha iniciado el proceso de carga de reglas
  while (getline(archivo, linea)) { // Mientras que el archivo tenga líneas para leer, se lee cada línea y se almacena en la variable linea
    if (linea.empty()) {
      continue; // Si hay líneas vacías, se saltan
    }
    stringstream ss(linea); // Aquí almacenamos toda la linea leida
    string token, varSalida, etiquetaSalida;

    ss >> token; // Salta "SI"
    vector<pair<Variable *, string>> antecedentes;
    vector<string> operadores;
    bool error = false;

    while (ss >> token) { // Mientras que haya tokens para leer en la línea, se procesan los antecedentes y operadores lógicos
      if (token == "ENTONCES")
        break;
      if (token == "Y" || token == "O") { // Si el token es un operador lógico, se agrega al vector de operadores
        operadores.push_back(token);
        continue;
      }
      string varName = token;
      ss >> token; // "ES"
      string etiquetaName; // Para almacenar el nombre de la etiqueta del antecedente
      ss >> etiquetaName;
      try { // Se busca la variable por su nombre utilizando el método buscarVariable, si se encuentra se agrega un par con un puntero a la variable y el nombre de la etiqueta al vector de antecedentes, si no se encuentra se muestra un mensaje de error al usuario y se marca un error para saltar la carga de esta regla
        Variable &v = buscarVariable(varName);
        antecedentes.push_back({&v, etiquetaName});
      } catch (...) {
        cout << "Error: Una de las variables en la regla no existe en la base."
             << endl;
        error = true;
        break;
      }
    }
    if (error) {
      continue;
    }
    ss >> varSalida; // Lee nombre Var Salida
    ss >> token; // Salta "ES"
    ss >> etiquetaSalida; // Lee Etiqueta Salida

    try { // Se busca la variable de salida por su nombre utilizando el método buscarVariable, si se encuentra se crea un objeto Regla con los antecedentes, operadores, variable de salida y etiqueta de salida leídos, y se agrega al vector de reglas, si no se encuentra se muestra un mensaje de error al usuario y se marca un error para saltar la carga de esta regla
      Variable &vs = buscarVariable(varSalida);
      Regla nuevaRegla(
          antecedentes, operadores, vs,
          etiquetaSalida); // Creamos el objeto regla con los valores leídos
      reglas.push_back(nuevaRegla); // Y la añadimos al vector de reglas de la
    } catch (...) {
      cout << "Error: Una de las variables en la regla no existe en la base."
           << endl; // Si ocurre un error al buscar las variables se muestra un mensaje de error al usuario, pero se continúa intentando cargar las siguientes reglas
    }
  }

  archivo.close(); // Cerramos el archivo después de terminar de leerlo
  return true; // Retorna true si la carga fue exitosa, false en caso contrario
}

// Método para imprimir las variables almacenadas en el motor de lógica difusa, llamando al método imprimir() de cada variable
void base_de_conocimiento::imprimirVariables() const {
  for (const Variable &var : variables) {
    var.imprimir();
  }
}

// Método para imprimir las reglas almacenadas en el motor de lógica difusa, llamando al método imprimirEnLenguajeNatural() de cada regla
void base_de_conocimiento::imprimirReglas() const {
  for (const Regla &regla : reglas) {
    regla.imprimirEnLenguajeNatural();
  }
}

// Método para agregar una variable al motor de lógica difusa, simplemente agrega la variable al vector de variables
void base_de_conocimiento::agregarVariable(const Variable &var) {
  variables.push_back(var);
}

// Método para agregar una regla al motor de lógica difusa, simplemente agrega la regla al vector de reglas
void base_de_conocimiento::agregarRegla(const Regla &regla) {
  reglas.push_back(regla);
}

// Método para obtener el vector de variables almacenadas en el motor de lógica difusa, retorna una referencia al vector para permitir modificaciones directas
vector<Variable> &base_de_conocimiento::getVariables() { return variables; }

// Método para obtener el vector de reglas almacenadas en el motor de lógica difusa, retorna una referencia al vector para permitir modificaciones directas
vector<Regla> &base_de_conocimiento::getReglas() { return reglas; }

// Método para verificar si hay variables de entrada almacenadas en el motor de lógica difusa, retorna true si hay al menos una variable de entrada, false en caso contrario
bool base_de_conocimiento::hayVariablesEntrada() const {
  for (const Variable &var : variables) { // Recorremos el vector de variables para verificar si hay al menos una variable de entrada
    if (var.esVariableEntrada()) {
      return true; // Si encontramos una variable de entrada, retornamos true
    }
  }
  return false;
}

// Método para buscar una variable por su nombre, retorna una referencia a la variable encontrada para permitir modificaciones directas
Variable &base_de_conocimiento::buscarVariable(string &nombreVariable) {
  for (Variable &var : variables) { // Recorremos el vector de variables
    if (var.getNombre() == nombreVariable) { // Si encontramos una variable con el nombre buscado
      return var; // Retorna una referencia a la variable encontrada para permitir modificaciones directas
    }
  }
  if (nombreVariable == "salir") { // Evaluamos que no desee salir del programa
    cout << "Saliendo del programa..." << endl;
    exit(0); // Salimos del programa con un código de éxito
  }
  // En caso de no encontrar la variable le mostramos un mensaje de error al usuario
  cout << "No se encontró una variable de entrada con el nombre '" << nombreVariable << "'. Por favor, intente nuevamente: ";
  cin >> nombreVariable; // Y le permitimos al usuario ingresar un nuevo nombre de variable para buscar
  return buscarVariable(nombreVariable); // Llama recursivamente a la función para buscar la
}

// Método para buscar una regla por sus variables y etiquetas, retorna una referencia a la regla encontrada para permitir modificaciones directas
Regla &base_de_conocimiento::buscarRegla(const vector<pair<string, string>> &antecedentesBusqueda, string &varSalida, string &etiquetaSalida) {
  for (Regla &regla : reglas) { // Recorremos el vector de reglas
    bool antecedenteMatch = true;
    const vector<pair<Variable *, string>> &reglaAnts = regla.getAntecedentes();
    if (reglaAnts.size() != antecedentesBusqueda.size()) { // Si la cantidad de antecedentes de la regla no coincide con la cantidad de antecedentes buscados, no es la regla que estamos buscando, por lo que se continúa con la siguiente regla
      continue;
    }
    for (int i = 0; i < (int)reglaAnts.size(); ++i) { // Recorremos los antecedentes de la regla y comparamos el nombre de la variable y el nombre de la etiqueta con los antecedentes buscados, si alguno no coincide, no es la regla que estamos buscando, por lo que se marca como no coincidente y se sale del ciclo para continuar con la siguiente regla
      if (reglaAnts[i].first->getNombre() != antecedentesBusqueda[i].first || reglaAnts[i].second != antecedentesBusqueda[i].second) {
        antecedenteMatch = false;
        break;
      }
    }
    if (antecedenteMatch && regla.getSalida()->getNombre() == varSalida && regla.getEtiquetaSalida() == etiquetaSalida) {
      return regla; // Retorna una referencia a la regla encontrada para permitir modificaciones directas
    }
  }
  cout << "No se encontró una regla con las variables y etiquetas especificadas. Por favor, intente nuevamente." << endl;
  exit(1); // Lanza una excepción para indicar que no se encontró la regla
}

// Método para verificar la operabilidad de los vectores pasados como parámetros
bool base_de_conocimiento::verificarOperabilidad(vector<Variable> &vars) {
  int counterIn = 0;
  int counterOut = 0;
  if (vars.size() < 2) {
    cout << "Error: Se esperaba al menos 2 variables (1 de entrada y 1 de salida). Se encontraron " << vars.size() << " variables.\nModifique el archivo e intente de nuevo." << endl;
    return false;
  }
  for (int i = 0; i < vars.size(); i++) { // Recorremos el vector de variables para contar cuántas son de entrada y cuántas son de salida
    if (vars[i].esVariableEntrada()) {
      counterIn++;
    } else {
      counterOut++;
    }
  }
  if (counterIn < 1 || counterOut != 1) { // Validamos que haya al menos una variable de entrada y exactamente una variable de salida, si no se cumple esta condición se muestra un mensaje de error al usuario indicando el problema encontrado
    cout << "Error: Se esperaba al menos 1 variable de entrada y exactamente 1 de salida. Se encontraron " << counterIn << " variables de entrada y " << counterOut << " variables de salida.\nModifique el archivo e intente de nuevo." << endl;
    return false;
  }
  return true;
}