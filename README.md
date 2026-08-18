# Motor de Lógica Difusa (Fuzzy Logic)

Sistema de inferencia difusa (Mamdani) en C++ aplicado al control de dirección de un robot con dos sensores de distancia (izquierdo y derecho) y un sensor de velocidad. A partir de las mediciones de entrada, el motor calcula mediante fuzzificación, evaluación de reglas y desfuzzificación por centroide una velocidad de giro (`Velocidad_Giro`) para evitar obstáculos.

## Estructura del proyecto

```
main.cpp                    Punto de entrada: carga variables/reglas y maneja la interacción por consola
variables.h / variables.cpp Clase Variable: rangos, etiquetas (triangulares/trapeciales) y fuzzificación
regla.h / regla.cpp         Clase Regla: antecedentes, operadores lógicos (Y/O) y consecuente
base_de_conocimiento.h/.cpp Carga de variables y reglas desde archivo, búsqueda y validaciones
motor_logica_fuzzy.h/.cpp   Motor de inferencia: pertenencia, evaluación de reglas, agregación y centroide
variables_fuzzy.txt         Definición de variables de entrada/salida y sus etiquetas difusas
reglas.txt                  Base de reglas (SI ... ENTONCES ...)
Pruebas/                    Capturas de pantalla de ejecuciones de prueba
```

## Requisitos

- Compilador C++ compatible con C++17 (probado con `g++` de MSYS2/MinGW-w64 13.2.0).

## Compilación

Desde la raíz del proyecto:

```bash
g++ -std=c++17 -o fuzzy_engine main.cpp base_de_conocimiento.cpp motor_logica_fuzzy.cpp regla.cpp variables.cpp
```

## Ejecución

```bash
./fuzzy_engine
```

El programa (todo por consola, en español):

1. Carga las variables difusas desde `variables_fuzzy.txt` (ruta fija en `main.cpp`) y las imprime.
2. Permite evaluar el grado de pertenencia de un valor a las etiquetas de una variable ingresando su nombre (o `salir` para continuar).
3. Carga las reglas desde `reglas.txt` y las imprime en lenguaje natural.
4. Pide la distancia del sensor derecho, la distancia del sensor izquierdo y la velocidad actual; calcula la distancia mínima efectiva y ejecuta la inferencia difusa completa (fuzzificación → evaluación de reglas → agregación → centroide), mostrando el detalle de cada paso y el resultado final de `Velocidad_Giro`.
5. Pregunta si se desea repetir la inferencia (`s`/`n`).

## Formato de los archivos de datos

**`variables_fuzzy.txt`** — una variable por bloque:

```
VARIABLE_IN <nombre> <min> <max>
<etiqueta> TRIANGULAR <a> <b> <c>
<etiqueta> TRAPEZOIDAL <a> <b> <c> <d>
...
VARIABLE_OUT <nombre> <min> <max>
...
```

**`reglas.txt`** — una regla por línea:

```
SI <var1> ES <etiqueta1> [Y|O <var2> ES <etiqueta2> ...] ENTONCES <varSalida> ES <etiquetaSalida>
```

## Estado de la prueba

Se compiló el proyecto con `g++ -std=c++17` sin errores (solo warnings menores de variables no usadas y comparaciones de signo) y se ejecutó de extremo a extremo con entradas simuladas:

- Carga de variables y reglas: correcta.
- Evaluación de pertenencia individual: correcta.
- Inferencia completa con activación real de una regla (`Distancia` cercana a la izquierda + `Velocidad` alta) produjo `Velocidad_Giro ≈ 24.59` (giro fuerte a la derecha), consistente con la regla `SI Distancia ES IzquierdaCercana Y Velocidad ES Alta ENTONCES Velocidad_Giro ES GiroFuerteDerecha`.

### Limitación conocida

`reglas.txt` incluye 3 reglas (líneas 11, 12 y 15) que usan la etiqueta `Centrado` para la variable `Distancia`, pero esa etiqueta **no está definida** en `variables_fuzzy.txt` (las etiquetas existentes son `IzquierdaLejana`, `IzquierdaMedia`, `IzquierdaCercana`, `DerechaCercana`, `DerechaMedia`, `DerechaLejana`). Como resultado, esas reglas nunca se activan (su grado de pertenencia siempre es 0). Además, existe una zona de valores medios (p. ej. `Distancia` en `IzquierdaMedia` con `Velocidad` en `Baja`) para la que no hay ninguna regla definida, por lo que la inferencia puede no activar ninguna regla y devolver `0` por defecto. Si se desea cobertura completa, se debe agregar la etiqueta `Centrado` a la variable `Distancia` en `variables_fuzzy.txt` y/o completar las combinaciones faltantes en `reglas.txt`.
