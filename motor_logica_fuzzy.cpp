#include "motor_logica_fuzzy.h"
#include <iostream>
using namespace std;

// Constructor sin parametros
MotorLogicaFuzzy::MotorLogicaFuzzy() {
  // Inicializa el vector de variables como un vector vacío
}

// Constructor con parametros
MotorLogicaFuzzy::MotorLogicaFuzzy(base_de_conocimiento base) {
  this->base = base;
}

// Destructor
MotorLogicaFuzzy::~MotorLogicaFuzzy() {}

// Método para agregar una base de conocimiento al motor de lógica difusa,simplemente asigna la base de conocimiento al atributo base del motor
void MotorLogicaFuzzy::agregarBase(base_de_conocimiento base) {
  this->base = base;
}

// Método para obtener la base de conocimiento almacenada en el motor de lógica difusa, retorna una referencia a la base de conocimiento para permitir modificaciones directas
base_de_conocimiento &MotorLogicaFuzzy::getBase() { return base; }

// Método para calcular la función de pertenencia de un valor x a un conjunto difuso con forma triangular definida por los puntos a, b y c
float MotorLogicaFuzzy::pertenenciaTriangular(float x, float a, float b, float c) {
  if (x < a || x > c) { // Si el valor pertenece a los extremos del triángulo la pertenencia es 0
    return 0.0f;
  } else if (x == b) { // Si el valor es el vértice del triángulo la pertenencia es 1
    return 1.0f;
  } else if (x >= a && x < b) { // Si el valor está entre a y b, la pertenencia se calcula de forma lineal
    return (x - a) / (b - a);
  } else { // En caso que x > b && x <= c
    return (c - x) / (c - b);
  }
}

// Método para calcular la función de pertenencia de un valor x a un conjunto difuso con forma trapecial definida por los puntos a, b, c y d
float MotorLogicaFuzzy::pertenenciaTrapecial(float x, float a, float b, float c, float d) {
  if (x < a || x > d) { // Si el valor pertenece a los extremos del trapecio la pertenencia es 0
    return 0.0f;
  } else if (x >= b && x <= c) { // Si el valor está entre b y c, la pertenencia es 1
    return 1.0f;
  } else if (x >= a && x < b) { // Si el valor está entre a y b, la pertenencia se calcula de forma lineal
    return (x - a) / (b - a);
  } else { // En caso que x > c && x <= d
    return (d - x) / (d - c);
  }
}

// Método para imprimir las variables almacenadas en la base de conocimiento del motor de lógica difusa
void MotorLogicaFuzzy::imprimirVariables() const { base.imprimirVariables(); }

// Método para realizar la inferencia difusa, se le pasa un vector con los valores de entrada correspondientes a las variables de entrada almacenadas en la base de conocimiento, retorna el valor inferido para la variable de salida
float MotorLogicaFuzzy::inferir(const vector<float> &valoresEntrada) {
  cout << "Iniciando la inferencia\n";

  vector<Regla> &reglas = base.getReglas();
  vector<Variable> &vars = base.getVariables();

  cout << "Fuzzificación de variables de entrada\n";
  // Fuzzificamos cada variable de entrada y mostramos los resultados
  for (int i = 0; i < (int)vars.size(); ++i) {
    if (vars[i].esVariableEntrada()) {
      float valorReal = valoresEntrada[i];
      vars[i].fuzzificar(valorReal); // Calculamos y almacenamos los grados de pertenencia internamente
      cout << "  Variable: " << vars[i].getNombre() << " | Valor real: " << valorReal << " | Rango: [" << vars[i].getMin() << ", " << vars[i].getMax() << "]" << endl;
      // Mostramos el grado de pertenencia para cada etiqueta de la variable
      const vector<string> &etiquetas = vars[i].getEtiquetasRangos();
      const vector<float> &grados = vars[i].getGradosPertenencia();
      for (int j = 0; j < (int)etiquetas.size(); ++j) {
        cout << "    -> " << etiquetas[j] << "(" << valorReal << ") = ";
        if (grados[j] > 0.0f) {
          cout << "Pertenece con un grado de "; // Indicamos con que grado pertenece
        }
        cout << grados[j] << endl;
      }
      cout << endl;
    }
  }

  cout << "Evaluación de reglas, implicación y agregación\n";
  int resolucion = 100; // Granularidad de muestreo para el área de salida (Para calcular el centroide y a partir de este poder obtener el centro de gravedad del area acumulada)
  vector<vector<float>> areasAgregadas; // Áreas resultantes de la agregación para cada variable de salida
  vector<int> indicesSalida; // Índices de las variables de salida en el vector de variables
  // Preparamos los vectores de áreas para cada variable de salida
  for (int i = 0; i < (int)vars.size(); ++i) { // Recorremos todas las variables para preparar los vectores de áreas para cada variable de salida
    if (!vars[i].esVariableEntrada()) { // Si la variable no es de entrada
      areasAgregadas.push_back(vector<float>(resolucion + 1, 0.0f)); // Agregamos un vector de ceros para cada variable de salida
      indicesSalida.push_back(i); // Agregamos el índice de la variable de salida
    }
  }
  int reglasActivas = 0; // Contador de reglas con fuerza mayor a 0
  // Evaluamos cada regla
  for (int i = 0; i < (int)reglas.size(); ++i) {
    Regla &r = reglas[i];
    float fuerzaRegla = -1.0f; // Este es un valor inicial para indicar que aún no se ha calculado la fuerza de la regla
    cout << "\n  [Regla " << i + 1 << "] ";
    r.imprimirEnLenguajeNatural(); // Mostramos la regla en lenguaje natural controlar cómo se combina el primer antecedente con los siguientes
    // Obtenemos los antecedentes y operadores de la regla para evaluar su activación
    const vector<pair<Variable *, string>> &antecedentes = r.getAntecedentes();
    const vector<string> &operadores = r.getOperadores();
    if (antecedentes.empty()) {
      fuerzaRegla = 0.0f; // Prevenir activación accidental si la regla no tiene antecedentes
      cout << "    (!) Regla sin antecedentes, fuerza = 0" << endl;
      continue;
    }
    // Evaluamos cada antecedente de la regla (Para saber que tanto pesa cada
    // variable en cada regla)
    for (int ind = 0; ind < (int)antecedentes.size(); ++ind) {
      const pair<Variable *, string> &antecedente = antecedentes[ind];
      Variable *v = antecedente.first; // Almacenamos el puntero a la variable del antecedente para facilitar su uso
      string etiqueta = antecedente.second; // Almacenamos la etiqueta del antecedente para facilitar su uso
      float gradoPertenencia = 0.0f; // Este es el grado de pertenencia que se obtendrá para este antecedente, se inicializa en 0 y se actualizará con el valor fuzzificado correspondiente
      // Buscamos el valor correspondiente en el vector de entrada y obtenemos el grado de pertenencia desde los valores fuzzificados
      for (int k = 0; k < (int)vars.size(); ++k) {
        if (vars[k].getNombre() == v->getNombre()) {
          // Buscamos el índice de la etiqueta en la variable para obtener el grado fuzzificado
          const vector<string> &etiquetasVar = vars[k].getEtiquetasRangos();
          const vector<float> &gradosVar = vars[k].getGradosPertenencia();
          for (int j = 0; j < (int)etiquetasVar.size(); ++j) {
            if (etiquetasVar[j] == etiqueta) {
              gradoPertenencia = gradosVar[j]; // Usamos el grado ya calculado en la fuzzificación
              break;
            }
          }
          break;
        }
      }
      cout << "    Antecedente " << ind + 1 << ": " << v->getNombre() << " ES " << etiqueta << " -> mu = " << gradoPertenencia;
      // Combinamos con el operador difuso correspondiente
      if (ind == 0) {
        fuerzaRegla = gradoPertenencia;
      } else {
        string op;
        if (ind - 1 < (int)operadores.size()) {
          op = operadores[ind - 1];
        } else {
          op = "Y";
        }
        if (op == "O") {
          cout << "  [OR: max(" << fuerzaRegla << ", " << gradoPertenencia
               << ")]";
          fuerzaRegla = max(fuerzaRegla, gradoPertenencia);
        } else {
          cout << "  [AND: min(" << fuerzaRegla << ", " << gradoPertenencia
               << ")]";
          fuerzaRegla = min(fuerzaRegla, gradoPertenencia);
        }
      }
      cout << endl;
    }
    cout << "    - Fuerza de activacion de la regla: " << fuerzaRegla << " -> Consecuente: " << r.getSalida()->getNombre() << " ES " << r.getEtiquetaSalida() << endl;
    if (fuerzaRegla > 0.0f) {
      reglasActivas++;
    } else {
      cout << "    (Regla no activada, fuerza = 0)" << endl;
    }

    Variable *vS = r.getSalida(); // Creamos un puntero a la variable de salida
    int indArea = -1;
    for (int k = 0; k < (int)indicesSalida.size(); ++k) {
      if (vars[indicesSalida[k]].getNombre() == vS->getNombre()) {
        indArea = k;
        break;
      }
    }

    if (indArea != -1 && fuerzaRegla > 0.0f) {
      for (int step = 0; step <= resolucion; ++step) {
        float x = vS->getMin() + step * (vS->getMax() - vS->getMin()) / resolucion; // Esta variable representa el punto x en el universo de la variable de salida que estamos evaluando, se recorre desde el mínimo hasta el máximo con una granularidad definida por la resolución
        float muSalida = 0.0f;

        // Calculamos la pertenencia del punto x al conjunto de salida de esta regla
        const vector<string> &etiquetasSalida = vS->getEtiquetasRangos();
        const vector<array<float, 4>> &puntosSalida = vS->getPuntosEtiquetas();
        for (int j = 0; j < (int)etiquetasSalida.size(); ++j) { // Vamos etiqueta por etiqueta
          if (etiquetasSalida[j] == r.getEtiquetaSalida()) { // Si encontramos la etiqueta de salida
            array<float, 4> p = puntosSalida[j];
            if (p[3] ==
                0.0f) { // Si el cuarto punto es 0, es una etiqueta triangular
              muSalida = pertenenciaTriangular(x, p[0], p[1], p[2]);
            } else { // Si el cuarto punto no es 0, es una etiqueta trapecial
              muSalida = pertenenciaTrapecial(x, p[0], p[1], p[2], p[3]);
            }
            break;
          }
        }
        // Implicación Mamdani: cortamos el conjunto de salida al nivel de la fuerza
        float corte = min(fuerzaRegla, muSalida);
        // Agregación: combinamos con el máximo sobre todas las reglas
        areasAgregadas[indArea][step] = max(areasAgregadas[indArea][step], corte);
      }
    }
  }

  cout << "\n  Total de reglas evaluadas: " << reglas.size() << " | Reglas activadas (fuerza > 0): " << reglasActivas << endl;

  cout << "Desfuzzificación con base en el centroide\n";
  float ultimoResultado = 0.0f;
  for (int i = 0; i < (int)indicesSalida.size(); ++i) {
    Variable &vS = vars[indicesSalida[i]]; // Variable de salida para la que se calculará el resultado difuso, se obtiene a partir del índice almacenado en indicesSalida que corresponde a esta variable de salida
    float numerador = 0.0f; // El numerador del centroide se calcula como la suma de x * mu(x) para cada punto x en el universo de la variable de salida, donde mu(x) es el valor de pertenencia del punto x al conjunto difuso resultante de la agregación
    float denominador = 0.0f; // El denominador del centroide se calcula como la suma de mu(x) para cada punto x en el universo de la variable de salida, es decir, la suma de los valores de pertenencia sin multiplicar por x

    cout << "  Variable de salida: " << vS.getNombre() << " | Rango: [" << vS.getMin() << ", " << vS.getMax() << "]" << endl;

    // Calculamos el centroide del área agregada
    for (int step = 0; step <= resolucion; ++step) {
      float x = vS.getMin() + step * (vS.getMax() - vS.getMin()) / resolucion;
      float y = areasAgregadas[i][step];
      numerador += x * y;
      denominador += y;
    }
    if (denominador == 0.0f) { // Para evitar división por cero, si el área es cero, se puede asignar un valor predeterminado o indicar que no se pudo defuzzificar
      ultimoResultado = 0.0f;
      cout << "  (!) Area agregada es cero, no se pudo defuzzificar.\nNinguna regla fue activada para esta variable de salida." << endl;
    } else {
      ultimoResultado = numerador / denominador;
      cout << "  Centroide: Sum(x * mu(x)) / Sum(mu(x)) = " << numerador << " / " << denominador << endl;
    }
    // Imprimimos el resultado final para esta variable de salida
    cout << "  - Resultado final: " << vS.getNombre() << " = " << ultimoResultado << endl;
  }
  return ultimoResultado;
}