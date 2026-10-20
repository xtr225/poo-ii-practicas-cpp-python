# Semana 1 Diagnóstico y refactorización de un sistema académico

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

La Parte II se demuestra además con `python -m python.demo_gestor` y con `cpp/demo_gestor.cpp`.


Curso SIS206 POO II, UNAP, semestre 2026 II. Identificador de trabajo S1-001.

## Alcance y forma de estudiar

Se desarrollaron las cinco actividades de la primera parte y el laboratorio de la Parte II de la guía. El código académico se ofrece como solución explicada para estudiar y contrastar. El proyecto integrador conserva el tema de reservas de hotel que ya figura en el plan del curso; aquí se presenta solamente su propuesta inicial y tres clases candidatas.

Orden de lectura: `baseline/estudiante_original.py`, diagnóstico siguiente, `python/dominio.py`, `python/main.py`, `cpp/dominio.hpp`, `cpp/main.cpp` y finalmente los componentes de la Parte II. Las versiones previas de `../codigo_ordenado/` no fueron reemplazadas.

## Actividad 1 Diagnóstico antes de modificar

Las líneas corresponden a `baseline/estudiante_original.py`, copia del ejemplo ya indentado. Se analizó primero la fuente y después se ejecutó el caso inicial. Las notas 16 y 14 producen promedio 15.0. El caso sin notas ya estaba contemplado en el original; no se lo presenta como un defecto.

| Hallazgo | Líneas | Riesgo y corrección |
|---|---|---|
| Clase `est` poco expresiva | 2 | No comunica el concepto del dominio. Renombrar `Estudiante` y usar PascalCase. |
| Atributos, parámetros y métodos abreviados | 3 a 23 | `a`, `p`, `h`, `add`, `ppa` y `pr` exigen deducir su intención. Usar nombres como `codigo`, `escuela`, `notas` y `agregar_nota`. |
| Estado público sin acceso controlado | 4 a 7 | Un cliente puede reemplazar la colección o el código. Ofrecer propiedades de lectura y operaciones que preserven reglas. |
| Notas sin validar | 9 y 10 | Se admite 25 o un valor negativo. Rechazar valores fuera de 0 a 20 antes de agregarlos. |
| Registros posicionales | 10 y 17 | `i[1]` depende del orden de una lista. Reemplazarla por `Nota.curso` y `Nota.calificacion`. |
| Presentación acoplada a la entidad | 20 a 23 | Cambiar el formato de salida requiere editar `Estudiante`. Mover impresión al cliente y generación de reportes a otra clase. |
| Constructor sin contrato mínimo | 3 a 7 | Se aceptan nombre vacío o código sin identificar. Declarar y comprobar reglas mínimas de construcción. |
| Contratos y significado sin documentar | 2 a 23 | No explica el rango de notas ni si el promedio es ponderado. Documentar las reglas y comprobarlas con pruebas. |

Los dos primeros hallazgos separan el nombre del tipo de la nomenclatura de sus miembros. El diagnóstico también debe explicar los problemas de estado, validación, representación y responsabilidad; contar abreviaturas aisladas no sustituye ese análisis.

## Actividad 2 Refactorización en Python

`Nota` es una dataclass inmutable: relaciona un curso con su calificación y valida ambos al construirse. `Estudiante` almacena `_nombre`, `_codigo`, `_escuela` y `_notas`; expone propiedades de lectura. El código se normaliza a texto para admitir tanto el ejemplo numérico de la primera parte como `E001` de la Parte II. No se inventó un formato universitario de longitud fija.

`agregar_nota` construye primero una nota válida y después modifica la colección. `calcular_ppa` devuelve 0.0 sin notas o la suma dividida entre la cantidad. Es promedio aritmético, porque la guía no entrega créditos para ponderarlo. `__str__` devuelve una descripción; no imprime. El cliente `python/main.py` decide la presentación y captura `ValueError`.

La propiedad `notas` devuelve una tupla de notas inmutables. `_nombre` expresa una convención de uso interno, no una prohibición del lenguaje. Python permite acceder deliberadamente a esos miembros; `@property` controla la interfaz pública normal.

Las reglas adicionales de robustez son explícitas: nombre, curso y escuela no vacíos; código entero positivo o texto no vacío; notas numéricas finitas y entre 0 y 20. Se rechazan booleanos como calificación en Python. Los casos válidos conservan sus promedios; rechazar datos inválidos es una corrección funcional exigida por la práctica, no un comportamiento que el original ya tuviera.

Salida de las actividades 2 y 3, idéntica entre lenguajes:

```text
[20210500] Juan Carlos Mamani Quispe (Ing. Sistemas)
PPA: 15.00
Error capturado: La nota debe estar entre 0 y 20
```

## Actividad 3 Implementación equivalente en C++

`cpp/dominio.hpp` define `Nota` y `Estudiante`. Los atributos de la clase son privados por defecto; los métodos de lectura devuelven referencias constantes. `agregarNota` verifica el rango y los valores no finitos antes de insertar. Una nota inválida provoca `std::invalid_argument`. `calcularPPA` tiene el mismo contrato que Python y los métodos que consultan datos son `const`.

`cpp/main.cpp` contiene la impresión y la captura de la excepción. Las pruebas y demostraciones se compilaron con C++17 y C++20 para cubrir las dos versiones que aparecen en el documento. Se usan `-Wall -Wextra -Wpedantic -Werror`: cualquier advertencia detiene la compilación.

| Python | C++ | Propósito |
|---|---|---|
| `@dataclass Nota` | `struct Nota` | Representar una nota mediante campos con nombre. |
| `__init__` | Constructor | Crear un objeto que cumple sus reglas mínimas. |
| `self` | Objeto receptor implícito `this` | Acceder al estado del objeto. |
| `_nombre` y `@property` | Miembro privado y getter `const` | Definir una interfaz de lectura. |
| Lista interna y tupla de consulta | Vector privado y referencia constante | Proteger la colección durante el uso normal de la interfaz. |
| `ValueError` y `try/except` | `invalid_argument` y `try/catch` | Comunicar y atender entradas inválidas. |

No se supone que la privacidad por convención de Python sea tan estricta como `private` en C++.

## Actividad 4 Cohesión y acoplamiento

**Cuál diseño tiene mayor cohesión y por qué.** El diseño B reúne las operaciones de notas y promedio en `Estudiante`, el almacenamiento en el repositorio y la exportación en su generador. Cada componente tiene una finalidad identificable. Una clase puede tener pocos métodos y seguir siendo poco cohesiva si esos métodos atienden tareas independientes.

**Qué sucede con el diseño A al cambiar el formato de exportación.** Hay que editar una clase que también registra estudiantes y calcula promedios. Eso obliga a revisar operaciones ajenas al reporte y amplía el riesgo de regresión. En B se cambia o agrega el generador, manteniendo estable la entidad.

**Qué principio SOLID se incumple.** El principio de responsabilidad única: la clase A cambia por reglas académicas, persistencia, presentación y notificaciones. Son motivos de cambio distintos, aunque todos aparezcan dentro del mismo sistema.

**Por qué B facilita las pruebas.** Es posible probar promedios sin crear archivos ni enviar correos. El servicio recibe un repositorio y una auditoría que pueden funcionar en memoria. Un generador se prueba comparando su texto con el resultado esperado. Tener más clases ayuda cuando sus límites expresan responsabilidades reales; no es por sí solo una medida de calidad.

## Actividad 5 Propuesta inicial del proyecto integrador

**Nombre:** Sistema de reservas de hotel en Puno. **Problema:** organizar los datos de huéspedes, habitaciones y reservas para reducir registros dispersos y permitir el control de disponibilidad en etapas posteriores. **Usuarios:** recepción, administración y personal encargado de reservas. El tema coincide con `plan_trabajo_proyecto_hotel_POO2.md` y con el README del proyecto que ya existe en la carpeta del curso.

| Clase candidata | Atributos privados iniciales | Responsabilidad |
|---|---|---|
| `Huesped` | `id`, `nombre`, `documento` | Identificar a la persona que solicita alojamiento. |
| `Habitacion` | `numero`, `tipo`, `capacidad` | Describir una unidad de alojamiento y su capacidad. |
| `Reserva` | `codigo`, `fechaEntrada`, `fechaSalida`, `estado` | Representar una solicitud de alojamiento con sus fechas y estado. |

El diagrama `proyecto_integrador/clases_iniciales.png` contiene estas tres clases con visibilidad privada y sin relaciones. Se acompaña de `clases_iniciales.drawio` editable. Las relaciones, herencia, tarifas y persistencia quedan fuera de este modelo inicial.

Se creó un repositorio Git local de la práctica con etapas baseline, tests, refactor y final. No hay una URL de GitHub verificada ni se publicó el proyecto; esa dirección debe completarse cuando el estudiante cree o vincule su repositorio. El nombre propuesto por la guía es `POO2-2026II-Apellido`, sustituyendo Apellido por el dato real.

## Parte II Responsabilidades y laboratorio

En `baseline/gestor_academico.py`, `GestorAcademico` registra, guarda, calcula, escribe auditoría, exporta CSV e imprime. Su lista pública permite cambios sin validación. El fragmento C++ presenta el mismo problema con `estudiantes` público y `ofstream` dentro de `agregarNota`. Se añadieron encabezados y un programa de demostración al fragmento C++ para ejecutarlo sin atribuir al original funciones que no contiene.

| Hallazgo obligatorio | Ubicación del original Python | Riesgo | Refactorización |
|---|---|---|---|
| Responsabilidades mezcladas | `agregar_nota` y `reporte` | Una clase cambia por reglas y por formatos o destinos. | Extraer dominio, auditoría y generadores. |
| Estado expuesto | `__init__`, lista `estudiantes` | Se pueden insertar duplicados o alterar notas directamente. | Colección interna y operaciones de registro o consulta. |
| Validación ausente | `registrar` y `agregar_nota` | Datos inválidos y código repetido. | Constructor, objeto Nota y repositorio con comprobaciones. |
| Dependencia concreta | `open` en notas y reportes | Para probar el dominio se necesitan archivos. | Recibir auditoría y devolver el contenido del reporte. |
| Duplicación potencial | Promedio y estado dentro de `reporte` | Un segundo formato repetiría reglas académicas. | Centralizar promedio y aprobación en Estudiante. |

La implementación distribuye el trabajo así: `Estudiante` conoce notas y aprobación; `RepositorioEstudiantes` guarda y busca; los generadores devuelven CSV o JSON; auditoría conserva eventos; `ServicioAcademico` coordina. La entidad no conoce consola ni archivos. La aplicación decide si muestra o guarda el texto devuelto. El límite de aprobación es 10.5, como establece la Parte II.

Primero se ejecutaron seis pruebas de caracterización sobre entradas válidas del original. Después se implementó el diseño nuevo y se compararon sus seis resultados CSV con el original. La aceptación de notas fuera de rango y códigos repetidos se registró como defecto previo; las pruebas finales exigen su rechazo. Así se evita presentar como verdes antes de corregir unas reglas que el original no cumplía.

Las pruebas cubren registro y búsqueda, 0 y 20, promedio de tres notas, colección vacía, aprobación en 10.5, rechazo de -0.1 y 20.1, duplicados, ausencia de cambios al generar reportes, auditoría e intercambio de generador. El resumen ejecutado está en `evidencias/verificacion.json` y los detalles en los archivos de texto contiguos.

### Mini experimento EXP POO 001

Se agregó JSON manteniendo CSV en dos variantes Python. `experimento/gestor_con_json.py` incorpora el método dentro de una copia del monolito; `python/reportes_json.py` agrega un generador al diseño separado. El núcleo nuevo no se editó para habilitar el formato. Las métricas se obtienen con el mismo criterio: se cuentan archivos productivos, clases existentes editadas, clases nuevas y métodos públicos funcionales; se excluyen pruebas y documentación de los recuentos de impacto.

Los tiempos de `evidencias/experimento_inicio.json` abarcan la elaboración asistida desde el registro de la solicitud hasta las pruebas en verde. Son dos observaciones consecutivas realizadas con IA, con distinto volumen de pruebas al final. No miden tiempo de programación del estudiante ni demuestran que una arquitectura sea más rápida en general. El resultado defendible es el aislamiento del cambio, incluso si crece la cantidad total de clases.

La separación no garantiza transacciones: si falla la auditoría después de agregar una nota válida, la nota ya fue incorporada. Para esta práctica se declara ese límite; la atomicidad entre datos y bitácora requeriría una decisión de persistencia adicional.

## Preguntas de reflexión

**Defecto más perjudicial para el mantenimiento.** Mezclar reglas, archivos y presentación hace que cambios frecuentes en reportes obliguen a tocar la misma clase que administra notas. El estado público agrava el problema al permitir datos que incumplen esas reglas. Separar responsabilidades y proteger las operaciones ataca ambos riesgos.

**Atributos privados en ambos lenguajes.** `_nombre` es una convención de Python. `__nombre` activa modificación de nombre para evitar colisiones y acceso accidental, pero tampoco es una barrera absoluta. En C++, `nombre_` no significa privado por su sufijo; la restricción surge de `private` o del acceso privado por defecto de `class`. Las propiedades y getters permiten consultar sin ofrecer una asignación libre.

**Más clases y archivos en el diseño B.** Aumentan la navegación y el ensamblaje inicial, pero permiten modificar y probar responsabilidades por separado. El diseño A puede ser razonable en una demostración breve y descartable con requisitos estables; deja de ser cómodo cuando aparecen nuevos formatos o destinos.

**Git desde el comienzo.** Conserva la versión inicial, permite revisar qué cambió y relaciona cada mejora con sus pruebas. Un único commit final pierde la evidencia del proceso. El historial local de esta entrega conserva las etapas reales y utiliza una identidad de asistente, sin atribuirlas al estudiante.

## Preguntas de análisis e investigación

**Pocos métodos y baja cohesión.** Sí: una clase con solo `promedio` y `enviar_correo` reúne reglas académicas y comunicación externa. La cantidad de métodos no describe su unidad de propósito.

**Cuándo extraer clases aumenta innecesariamente el acoplamiento.** Cuando se crean envoltorios sin responsabilidad estable y cada operación exige atravesar muchos objetos que conocen sus detalles. Deben extraerse conceptos con un motivo de cambio propio y una interfaz pequeña.

**Refactorizar y reescribir.** Refactorizar cambia la organización manteniendo una conducta observada y comprobada. Reescribir construye otra solución, con mayor riesgo de omitir casos existentes. Incorporar validaciones nuevas se documentó separadamente del mantenimiento de los resultados válidos.

**Repositorio en base de datos.** El dominio puede conservarse si el repositorio mantiene su contrato. Harían falta pruebas de integración, un criterio para duplicados, manejo de conexiones y transacciones; la latencia y los fallos externos cambiarían las mediciones.

**Hipótesis para comparar estudiante, IA y estudiante con IA.** Bajo la misma tarea y pruebas de aceptación, la colaboración podría reducir el tiempo frente al estudiante solo manteniendo una comprensión que debe comprobarse mediante explicación individual. Es una hipótesis, no un resultado de esta entrega. Para contrastarla se requieren varios participantes, asignación de condiciones, tareas equivalentes, tiempo medido, tasa de pruebas y evaluación ciega de las explicaciones.

## Declaración de uso de IA

Se utilizó Codex para analizar la guía, generar las soluciones académicas Python y C++, escribir pruebas, ejecutar verificaciones, construir los diagramas y redactar este informe y sus anexos. La ejecución y los resultados conservados pertenecen a esta sesión asistida. No se atribuyen al estudiante horas de trabajo, comprensión individual, sustentación ni resultados experimentales humanos. El estudiante debe estudiar el material, verificarlo en su entorno y declarar el apoyo según las indicaciones del curso.

## Referencias y evidencia

- Zanabria Gálvez, A. H. (2026). Guía de práctica de laboratorio Semana 1, SIS206 POO II, UNAP. Material proporcionado, páginas 1 a 13.
- Plan de trabajo del sistema de reservas de hotel POO II y README del proyecto local, consultados para mantener el tema del proyecto integrador.
- Fuentes de esta entrega: `baseline/`, `python/`, `cpp/` y `tests/`.
- Resultados: `evidencias/verificacion.json`, salidas de pruebas y `metricas.csv`.

Las obras de Fowler y Martin aparecen en la bibliografía de la guía. No se afirma haber consultado esos libros para medir ni ejecutar esta entrega.
