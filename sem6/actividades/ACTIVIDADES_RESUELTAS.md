# Semana 6 Repositorios iteradores y rendimiento

## Organización de la entrega

- `../codigos/`: soluciones Python y C++ y sus pruebas. En la semana 1, baseline y experimento son parte de la comparación solicitada.
- `../html/index.html`: tutorial interactivo de los ejemplos iniciales.
- `../html/codigos_resueltos.html`: lector de las soluciones finales.
- `../informes/`: informe editable y PDF de esta semana.
- `diagramas/` y `resultados/`: entregables de las actividades que los requieren.

Las rutas Python, C++, tests, baseline y experimento citadas en el informe se leen desde `codigos`. Las referencias anteriores a `docs/uml` corresponden a `actividades/diagramas`; los registros citados como `evidencias` se encuentran en `actividades/resultados`. La carpeta de trabajo antes llamada desarrollo se reorganizó en estos apartados para presentar la entrega.

## Ejecutar el código

Abrir una terminal dentro de la carpeta `codigos` de esta semana. Requiere Python 3.12 o superior y g++ con C++17.

```powershell
python -m python.main
python -m unittest discover -s tests -v
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror cpp/main.cpp -o demo.exe
./demo.exe
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror cpp/pruebas.cpp -o pruebas.exe
./pruebas.exe
```

Los ejecutables y las salidas se crean solo al ejecutar los comandos. El notebook adjunto prepara sus propias fuentes para ejecutarlas en Colab.

El benchmark se ejecuta con `python -m python.benchmark` y compilando `cpp/benchmark.cpp` con `-O2`. Genera la carpeta evidencias al ejecutarse; las mediciones originales usadas en el informe están en resultados.


UNAP SIS206 POO II 2026 II. Identificador S6-001. Fecha 19 de octubre de 2026.

## Objetivo y alcance

Completar y comprobar los repositorios, consultas, recorridos y mediciones presentes en la carpeta de la semana 6, adaptándolos también al hotel.

No se dispone de una guía oficial de esta semana. El alcance se deriva de actividad_02, repositorios, iteradores y benchmark existentes. Se identificó que eliminar devolvía False al borrar una entidad con valor falso; que la ventana admitía cero o negativos; y que el benchmark reconstruía dict y set durante cada búsqueda.

## Desarrollo por actividades

| Actividad | Fuente resuelta | Comprobación |
| --- | --- | --- |
| Repositorio | python/repositorio.py y cpp/repositorio.hpp | Guardar o actualizar, insertar sin duplicados, buscar, eliminar y recorrer. |
| Consultas | python/solucion.py y cpp/solucion.hpp | Filtrar destacados, ordenar el ranking y obtener estadísticas. |
| Iteradores | Repositorio, ColaAtencion y PlanillaIterador | FIFO, recorrido independiente y copia segura del contenedor C++. |
| Ventanas | ventana_deslizante y ventanas | Tres ventanas válidas; rechazo de k fuera de 1 a longitud. |
| Benchmark y hotel | python/benchmark.py y cpp/benchmark.cpp | Construcción separada y tres consultas hoteleras. |

## Decisiones de implementación

guardar conserva la semántica de actualizar del material original. insertar ofrece rechazo de duplicados de manera explícita. buscar_resultado distingue una búsqueda fallida incluso si el dato almacenado es None. eliminar comprueba la existencia del ID y no la verdad del valor borrado.

PlanillaIterador conserva una instantánea de los datos ordenados. C++ crea sus iteradores al comenzar el recorrido y no guarda iteradores vinculados a otra copia del vector. ColaAtencion usa deque para atender por orden de llegada.

El generador Python produce una ventana al consumirla. La versión C++17 devuelve todas las ventanas en un vector; su mayor consumo de memoria se declara. Las dos versiones validan k positivo y menor o igual al tamaño. El benchmark usa cuatro tamaños, 200 consultas iguales por lote y mediana de tres repeticiones, separando construcción y búsqueda.

## Colecciones y contratos

| Aspecto | Python o decisión | C++ o justificación |
| --- | --- | --- |
| Mapa por ID | dict de entidades | unordered_map con extractor de ID |
| Ausencia | None o Result fallido | optional sin valor o Result fallido |
| Filtrar | Predicado sobre cada entidad | Predicado sobre cada entidad |
| Ordenar | sorted con key y reverse | sort con comparador |
| FIFO | deque append y popleft | deque push_back y pop_front |
| Ventana | Generación incremental | Vector de ventanas materializadas |

## Análisis y reflexión

### Por qué la tabla hash parece constante

La búsqueda esperada o promedio es O(1) bajo condiciones habituales de dispersión. No es una garantía absoluta: colisiones y otros factores pueden producir peores casos. Las mediciones son observaciones del equipo, no una prueba de complejidad.

### Cuándo usar lista ordenada y bisect

Cuando se necesita orden y hay muchas consultas con pocos cambios. La búsqueda es O(log n), aunque insertar en una lista puede desplazar elementos O(n). Debe separarse el costo de mantener el orden del costo de consultar.

### Por qué corregir el benchmark

El original medía la creación de dict y set más la búsqueda, frente a una lista ya construida. Ese contraste no aisla la operación que se pretendía comparar. Los tiempos de construcción siguen siendo relevantes, pero se registran en otra columna.

### Qué conserva un iterador de planilla

La posición pertenece al recorrido y la instantánea pertenece al contenedor. Copiar una colección C++ que almacena iteradores a otra colección deja referencias ajenas o inválidas; la versión nueva solo almacena datos y obtiene begin y end del propietario actual.

### Consultas del hotel

Se filtran habitaciones con al menos dos plazas; se ordenan por precio nocturno; se calcula el promedio y pueden agruparse por tipo. En los datos de demostración hay dos habitaciones de al menos dos plazas y el precio promedio es 142.50.

## Proyecto integrador

Repositorio<T,ID> o Repositorio[Entidad,ID] administra habitaciones por número, preservando la misma implementación para otras entidades. Los identificadores de las entidades del hotel son de solo lectura. Las colecciones devueltas son copias del contenedor; eso no implica clonar profundamente cada objeto compartido.

[Modelo de clases](diagramas/diagrama_clases.png) y [modelo editable](diagramas/diagrama_clases.drawio).

## Pruebas ejecutadas

Se aprobaron 15 pruebas con unittest, 15 con pytest y 14 con C++17. Pytest recoge las mismas pruebas Python; no son dos conjuntos distintos para sumar.

Las compilaciones usan -Wall -Wextra -Wpedantic -Werror. La demostración Python y C++ produce la misma salida:

```text
DESTACADOS=2
RANKING=1002,1001,1004,1003
PROMEDIO=14.125
HABITACIONES_DOS_PLAZAS=2
RANKING_HOTEL=103,102,104,101
PROMEDIO_HOTEL=142.50
FIFO=Ana
VENTANAS=3
```

Las cifras proceden de evidencias/verificacion.json. Las salidas completas y los comandos ejecutados permanecen en evidencias/.

## Conclusiones

La nueva medición compara consultas equivalentes y conserva un checksum de aciertos. Los resultados no se extrapolan a todo hardware ni representan una actividad oficial desconocida. El informe diferencia el código disponible de los requisitos derivados de él.

## Resultados medidos del benchmark

| Lenguaje | N | Estructura | Construcción ms | Búsqueda lote ms | Aciertos |
| --- | --- | --- | --- | --- | --- |
| Python | 1000 | list | 0.0229 | 1.2579 | 102 |
| Python | 1000 | dict | 0.1326 | 0.0271 | 102 |
| Python | 1000 | set | 0.0499 | 0.0253 | 102 |
| Python | 1000 | bisect | 0.0258 | 0.0865 | 102 |
| Python | 10000 | list | 0.3829 | 13.7240 | 101 |
| Python | 10000 | dict | 1.0593 | 0.0175 | 101 |
| Python | 10000 | set | 0.5212 | 0.0154 | 101 |
| Python | 10000 | bisect | 0.1622 | 0.0929 | 101 |
| Python | 100000 | list | 3.0410 | 143.9689 | 102 |
| Python | 100000 | dict | 10.6256 | 0.0163 | 102 |
| Python | 100000 | set | 4.9872 | 0.0272 | 102 |
| Python | 100000 | bisect | 1.7511 | 0.0766 | 102 |
| Python | 1000000 | list | 33.6440 | 1389.2981 | 127 |
| Python | 1000000 | dict | 82.6500 | 0.0178 | 127 |
| Python | 1000000 | set | 57.0706 | 0.0155 | 127 |
| Python | 1000000 | bisect | 24.3856 | 0.0913 | 127 |
| C++ | 1000 | vector | 0.0012 | 0.0617 | 102 |
| C++ | 1000 | unordered_map | 0.0791 | 0.0017 | 102 |
| C++ | 1000 | unordered_set | 0.0513 | 0.0015 | 102 |
| C++ | 1000 | binary_search | 0.0007 | 0.0079 | 102 |
| C++ | 10000 | vector | 0.0367 | 0.6394 | 101 |
| C++ | 10000 | unordered_map | 0.6410 | 0.0015 | 101 |
| C++ | 10000 | unordered_set | 0.6447 | 0.0021 | 101 |
| C++ | 10000 | binary_search | 0.0309 | 0.0099 | 101 |
| C++ | 100000 | vector | 0.1205 | 4.9762 | 102 |
| C++ | 100000 | unordered_map | 7.7163 | 0.0027 | 102 |
| C++ | 100000 | unordered_set | 8.7971 | 0.0026 | 102 |
| C++ | 100000 | binary_search | 0.1857 | 0.0084 | 102 |
| C++ | 1000000 | vector | 2.2593 | 42.0091 | 127 |
| C++ | 1000000 | unordered_map | 124.1530 | 0.0157 | 127 |
| C++ | 1000000 | unordered_set | 104.8310 | 0.0101 | 127 |
| C++ | 1000000 | binary_search | 1.1283 | 0.0222 | 127 |

Cada lote contiene 200 consultas y se informa la mediana de tres repeticiones. C++ se compiló con -O2; Python conserva su intérprete. No se atribuyen diferencias exclusivamente al lenguaje: contenedores, compilación, memoria y entorno también influyen.

## Declaración de IA y pendientes externos

Codex analizó las guías, generó las soluciones y pruebas, ejecutó las comprobaciones y preparó los documentos y diagramas. Estas evidencias corresponden a una sesión asistida. No se atribuyen al estudiante comprensión, horas de programación, revisión de un compañero ni una defensa realizada.

Antes de entregar se deben completar los datos personales, renombrar el notebook con ApellidoNombre y publicar o vincular el repositorio GitHub. No se inventa una URL ni una nota de evaluación. Los commits locales registran la preparación actual y no se presentan como actividad histórica durante siete semanas.

## Referencias

Zanabria Gálvez, A. H. (2026). Código existente de sem6 sin guía fuente disponible. Material del curso SIS206 proporcionado por el usuario.

Python Software Foundation. (s. f.). abc Abstract Base Classes y typing en Python 3.12. https://docs.python.org/3.12/library/abc.html y https://docs.python.org/3.12/library/typing.html

Fowler, M. (2018). Refactoring Improving the Design of Existing Code (2.ª ed.). Addison Wesley. Información de la obra y propósito de refactorización: https://martinfowler.com/books/refactoring.html

Object Management Group. (2017). Unified Modeling Language 2.5.1. https://www.omg.org/spec/UML/2.5.1/
