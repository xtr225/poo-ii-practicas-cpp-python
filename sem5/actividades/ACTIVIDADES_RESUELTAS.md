# Semana 5 Excepciones contratos y Result

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


UNAP SIS206 POO II 2026 II. Identificador S5-001. Fecha 19 de octubre de 2026.

## Objetivo y alcance

Completar jerarquías de errores, una matrícula con contratos y un pipeline de Result que propague fallos esperados sin anidar resultados.

La plantilla C++ Result<T> usa T y string como alternativas y constructores. Al instanciar Result<string> ambas alternativas se confunden. También hay que implementar eliminar_curso, conservar al menos un curso y comprobar la fecha en ambos lenguajes. Los límites no deben depender exclusivamente de assert.

## Desarrollo por actividades

| Actividad | Fuente resuelta | Comprobación |
| --- | --- | --- |
| 1 Errores | excepciones.py y errores_academicos.hpp | Familias, datos de contexto, duplicado y captura específica. |
| 2 Matricula | python/solucion.py y cpp/matricula.hpp | Agregar, eliminar, anular y verificar precondiciones e invariantes. |
| 3 Result | python/result.py y cpp/result.hpp | Éxito o fallo discriminado y pipeline 1001 o 9999. |
| 4 Pruebas | tests/test_solucion.py y cpp/pruebas.cpp | Casos válidos, fallos esperados, causa original y cortocircuito. |
| 5 Hotel | errores_hotel.py y errores_hotel.hpp | Cuatro errores de dominio y contratos de habitación y reserva. |

## Decisiones de implementación

Se usa una alternativa FalloResult con etiqueta distinta del valor T. Result<string> puede contener un texto exitoso o un error sin ambigüedad. En Python el discriminante explícito permite incluso un éxito cuyo valor sea None.

map sigue el uso encadenado solicitado en la guía: recibe funciones que devuelven Result y propaga su resultado directamente. Su semántica corresponde a bind o and_then, no a una transformación que produzca Result<Result<U>>. La siguiente función no se ejecuta cuando ya existe un fallo.

Matricula exige código, uno a seis cursos distintos y fecha no futura. Agregar o eliminar después de anular falla antes de modificar. Eliminar el último curso está prohibido. Las entradas inválidas se rechazan con excepciones; assert comprueba errores internos de poscondición e invariante.

## Errores en ambos lenguajes

| Aspecto | Python o decisión | C++ o justificación |
| --- | --- | --- |
| Base | Exception y subclases del dominio | std::runtime_error y subclases del dominio |
| Contexto | Diccionario contexto y atributos | Datos privados con getters |
| No lanzar | No existe un calificador equivalente | noexcept cuando la operación lo garantiza |
| Causa | raise Error from causa | std::throw_with_nested si se implementa |
| Familia | except desde lo específico a lo general | catch por referencia const desde lo específico |
| Destrucción | Gestión de objetos de Python | La virtualidad se hereda de std::exception |

## Análisis y reflexión

### Por qué map omite la siguiente función

Un fallo carece de un valor válido para el siguiente paso. Se devuelve el mismo error y no se ejecuta la función, lo que también evita sus efectos secundarios.

### Result frente a una excepción

Un fallo es un valor que vuelve por la ruta normal. Una excepción transfiere el control hasta una captura compatible. Result hace visible el fallo en la interfaz, pero el lenguaje no obliga por sí solo a inspeccionarlo.

### Cuándo usar Result en el hotel

Una habitación o reserva inexistente y una búsqueda sin coincidencia son casos esperados. Un contrato de construcción roto o un fallo de archivo requieren un tratamiento diferente y explícito.

### Destructor de excepciones

En esta implementación la base ya hereda un destructor virtual de std::runtime_error. No escribir virtual de nuevo no elimina esa propiedad. La advertencia sobre comportamiento indefinido se refiere a destruir mediante otra base que realmente carezca de destructor virtual.

### assert y precondiciones

assert puede desaparecer con -DNDEBUG o Python -O; por eso no protege entradas del usuario. Es útil como comprobación interna durante desarrollo. Las excepciones de validación siguen activas en la compilación de producción.

### Estrategia más segura

No existe una elección universal. Result puede ignorarse, y una excepción sin captura normalmente termina la operación o el programa en vez de ser ignorada. La seguridad depende del contrato, los tipos, la política de recuperación y las pruebas.

## Proyecto integrador

El hotel define ErrorHotel, ErrorDatosHotel, ErrorEstadoReserva y ErrorDisponibilidad. HabitacionRobusta y ReservaRobusta comprueban invariantes después de operaciones relevantes. Los constructores conservan las causas al traducir errores en Python. La búsqueda de habitación devuelve Result en los dos lenguajes y se comprueban tres casos válidos y tres rechazos del dominio.

[Modelo de clases](diagramas/diagrama_clases.png) y [modelo editable](diagramas/diagrama_clases.drawio).

## Pruebas ejecutadas

Se aprobaron 22 pruebas con unittest, 22 con pytest y 22 con C++17. Pytest recoge las mismas pruebas Python; no son dos conjuntos distintos para sumar.

Las compilaciones usan -Wall -Wextra -Wpedantic -Werror. La demostración Python y C++ produce la misma salida:

```text
CURSOS=1
EXITO=Inscrito: Juan Mamani en SIS206
FALLO=Estudiante no encontrado
```

Las cifras proceden de evidencias/verificacion.json. Las salidas completas y los comandos ejecutados permanecen en evidencias/.

## Conclusiones

Se comprobó el caso problemático Result<string>, la propagación corta y la conservación de estado ante errores. Se utilizó g++ 10.3.0 con C++17; la guía recomienda g++ 12 o superior, por lo que la versión exacta empleada queda declarada y no se presenta como la recomendada.

## Declaración de IA y pendientes externos

Codex analizó las guías, generó las soluciones y pruebas, ejecutó las comprobaciones y preparó los documentos y diagramas. Estas evidencias corresponden a una sesión asistida. No se atribuyen al estudiante comprensión, horas de programación, revisión de un compañero ni una defensa realizada.

Antes de entregar se deben completar los datos personales, renombrar el notebook con ApellidoNombre y publicar o vincular el repositorio GitHub. No se inventa una URL ni una nota de evaluación. Los commits locales registran la preparación actual y no se presentan como actividad histórica durante siete semanas.

## Referencias

Zanabria Gálvez, A. H. (2026). SIS2062026IIPracticaSemana5.docx. Material del curso SIS206 proporcionado por el usuario.

Python Software Foundation. (s. f.). abc Abstract Base Classes y typing en Python 3.12. https://docs.python.org/3.12/library/abc.html y https://docs.python.org/3.12/library/typing.html

Fowler, M. (2018). Refactoring Improving the Design of Existing Code (2.ª ed.). Addison Wesley. Información de la obra y propósito de refactorización: https://martinfowler.com/books/refactoring.html

Object Management Group. (2017). Unified Modeling Language 2.5.1. https://www.omg.org/spec/UML/2.5.1/
