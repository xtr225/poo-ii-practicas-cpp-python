# Semana 2 Encapsulamiento e invariantes

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


UNAP SIS206 POO II 2026 II. Identificador S2-001. Fecha 19 de octubre de 2026.

## Objetivo y alcance

Proteger el estado de Curso y Docente mediante propiedades y operaciones que preservan invariantes, y trasladar el mismo criterio a las tres clases iniciales del hotel.

Las plantillas dejan pendientes los setters de código, créditos y cupos. Además, limitar los cupos a 1 a 50 no basta: una reducción nunca debe situar el máximo por debajo de los inscritos. La edad y los totales del docente deben calcularse desde sus datos actuales, sin duplicar valores derivados.

## Desarrollo por actividades

| Actividad | Fuente resuelta | Comprobación |
| --- | --- | --- |
| 1 Curso | python/solucion.py | Código validado; créditos 1 a 6; cupos 1 a 50; inscritos protegidos. |
| 2 Docente | python/solucion.py | DNI de ocho dígitos, correo UNAP, edad y totales calculados; máximo tres cursos. |
| 3 C++ | cpp/solucion.hpp | Setters validados y consultas const; mismo caso de demostración. |
| 4 Equivalencias | ACTIVIDADES_RESUELTAS.md | Tabla completa y explicación del uso de const. |
| 5 Hotel | python/hotel_base.py y cpp/hotel_base.hpp | Huesped, Habitacion y Reserva con invariantes y operaciones de negocio. |

## Decisiones de implementación

Se siguió el patrón explícito de las dos plantillas: entre dos y cuatro letras mayúsculas y tres dígitos ASCII. El enunciado breve menciona SIS seguido de dígitos; esta diferencia se declara para que el contrato elegido pueda revisarse. SIS206 se acepta en ambas interpretaciones.

Los constructores usan las validaciones de sus setters. Los cambios se comprueban antes de asignar, de modo que un error conserva el valor anterior. La colección de cursos del docente se consulta como tupla en Python y se comparte de manera controlada en C++ para reflejar las modificaciones de cada curso.

La edad se calcula con fecha actual; edad_en permite comprobar cumpleaños con una referencia fija, 18 de octubre de 2026, sin depender del día de ejecución. nombre_completo, cupos_disponibles y los totales no ofrecen setter.

## Equivalencias de encapsulamiento

| Aspecto | Python o decisión | C++ o justificación |
| --- | --- | --- |
| Privado | __atributo modifica el nombre | private restringe el acceso |
| Protegido | _atributo es una convención | protected es una restricción del lenguaje |
| Getter | @property sin setter | Método de consulta const |
| Setter | @atributo.setter con validación | setAtributo con comprobación y excepción |
| Propiedad calculada | @property que calcula al consultar | Getter const que calcula al consultar |
| No modificar | No existe un const equivalente | Calificador const para la operación |
| Validación | raise ValueError | throw std::invalid_argument |

## Análisis y reflexión

### Por qué un const Curso no puede inscribirse

La operación modifica inscritos y no es const. El compilador impide llamarla a través de una referencia u objeto const. Así, una función que recibe const Curso& puede consultar sin autorizar una modificación accidental.

### Privacidad e invariante

Ocultar un atributo limita las rutas de acceso; un invariante especifica qué valores y relaciones deben mantenerse. Puede evitarse una validación repetida dentro de una operación privada ya comprobada, pero la interfaz pública no debe dejar visible un estado inválido.

### Ventaja de property

El cliente conserva la sintaxis de atributo al pasar de dato simple a propiedad validada. C++ hace explícita la llamada al setter. Ninguna sintaxis sustituye el diseño de las reglas.

### Cuándo almacenar una propiedad derivada

Solo si existe una medición que justifica el costo de recalcular y una estrategia correcta de invalidación. Guardar cupos_disponibles introduce el riesgo de que se desactualice al inscribir o cambiar la capacidad.

### Ejemplo de const correctness

Una función mostrar(const Curso& c) debe poder llamar getCodigo() y cuposDisponibles(). Si estos getters no fueran const, esa llamada no compilaría aunque su implementación solo leyera datos.

## Proyecto integrador

Huesped protege identificación, nombre y documento, calcula una etiqueta y valida la actualización de documento. Habitacion protege número, tipo, capacidad, tarifa y ocupación; calcula plazas libres y comprueba alojar. Reserva protege código y fechas, calcula noches y controla confirmar o anular. Los identificadores y las fechas se mantienen de solo lectura cuando no existe un cambio de negocio justificado.

[Modelo de clases](diagramas/diagrama_clases.png) y [modelo editable](diagramas/diagrama_clases.drawio).

## Pruebas ejecutadas

Se aprobaron 17 pruebas con unittest, 17 con pytest y 16 con C++17. Pytest recoge las mismas pruebas Python; no son dos conjuntos distintos para sumar.

Las compilaciones usan -Wall -Wextra -Wpedantic -Werror. La demostración Python y C++ produce la misma salida:

```text
CURSO=SIS206
LIBRES=27
CREDITOS=4
INSCRITOS=3
EDAD_REFERENCIA=46
```

Las cifras proceden de evidencias/verificacion.json. Las salidas completas y los comandos ejecutados permanecen en evidencias/.

## Conclusiones

La protección útil depende de mantener reglas entre atributos, no de añadir getters a todos los campos. Se verificaron tanto las fronteras individuales como la relación entre cupos e inscritos.

## Declaración de IA y pendientes externos

Codex analizó las guías, generó las soluciones y pruebas, ejecutó las comprobaciones y preparó los documentos y diagramas. Estas evidencias corresponden a una sesión asistida. No se atribuyen al estudiante comprensión, horas de programación, revisión de un compañero ni una defensa realizada.

Antes de entregar se deben completar los datos personales, renombrar el notebook con ApellidoNombre y publicar o vincular el repositorio GitHub. No se inventa una URL ni una nota de evaluación. Los commits locales registran la preparación actual y no se presentan como actividad histórica durante siete semanas.

## Referencias

Zanabria Gálvez, A. H. (2026). SIS2062026IIPracticaSemana2.pdf. Material del curso SIS206 proporcionado por el usuario.

Python Software Foundation. (s. f.). abc Abstract Base Classes y typing en Python 3.12. https://docs.python.org/3.12/library/abc.html y https://docs.python.org/3.12/library/typing.html

Fowler, M. (2018). Refactoring Improving the Design of Existing Code (2.ª ed.). Addison Wesley. Información de la obra y propósito de refactorización: https://martinfowler.com/books/refactoring.html

Object Management Group. (2017). Unified Modeling Language 2.5.1. https://www.omg.org/spec/UML/2.5.1/
