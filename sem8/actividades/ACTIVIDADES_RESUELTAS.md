# Semana 8 Integración y evaluación de la Unidad I

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


UNAP SIS206 POO II 2026 II. Identificador S8-001. Fecha 19 de octubre de 2026.

## Objetivo y alcance

Integrar el hotel de la Unidad I en una demostración que reúna encapsulamiento, polimorfismo, interfaces, errores, repositorios y UML, con una entrega preparada para la defensa.

La guía es una evaluación y no una nueva colección de TODO. La entrega técnica debe verificarse por ejecución y por trazabilidad, mientras que la revisión de pares y la defensa oral no pueden sustituirse con resultados generados por un asistente.

## Desarrollo por actividades

| Actividad | Fuente resuelta | Comprobación |
| --- | --- | --- |
| Repositorio | cpp, python, docs y tests | Estructura y comandos reproducibles; historial local de preparación. |
| Modelo UML | docs/uml/ | Clases completas, dos secuencias y un diagrama de estados. |
| Demostración | python/main.py y cpp/main.cpp | Reserva válida, conflicto, búsqueda fallida, error, orden y cancelación. |
| Informe | informe/ | Tres páginas con las cinco secciones exactas de EVI1. |
| Defensa | docs/guion_sustentacion.md | Guion de diez minutos y ficha de revisión para completar. |

## Decisiones de implementación

HabitacionSimple, HabitacionDoble y Suite son habitaciones y mantienen el contrato de precio positivo. La demostración utiliza una colección heterogénea, con precios de 100, 150 y 200. La reserva usa un huésped y una habitación; no hereda de ellos.

ServicioReservas recibe CalculadoraTarifa e INotificadorHotel. Se puede sustituir la cotización o el notificador sin cambiar la entidad. Las notificaciones se simulan en memoria. La máquina de estados pertenece a la reserva y valida los cambios antes de registrarlos.

ErrorHotel agrupa errores de datos, estados y disponibilidad. Las búsquedas y conflictos esperados retornan Result. Se rechazan fechas inválidas y se mantiene intacta la colección si una creación falla.

Los repositorios usan dict o unordered_map para búsquedas por ID; las listas o vectores ordenados son vistas de consulta. Los intervalos de alojamiento son semiabiertos: una reserva que sale el día en que otra entra no se solapa.

## Evidencia de integración

| Aspecto | Python o decisión | C++ o justificación |
| --- | --- | --- |
| Encapsulamiento | Propiedades y getters | Reglas de construcción y consulta controlada |
| Polimorfismo | Tres tipos de habitación | Precios diferenciados mediante el contrato base |
| Composición | Máquina, tarifa y notificador | Dependencias y propiedad representadas en UML |
| Errores | Excepciones y Result | Datos inválidos separados de búsquedas sin resultado |
| Colecciones | Filtrar, ordenar y recorrer | Misma implementación genérica para varias entidades |
| Documentación | UML y trazabilidad | Diagramas ligados a métodos implementados |

## Análisis y reflexión

### Punto técnico más delicado

La coordinación entre disponibilidad y ciclo de vida. Se resolvió calculando la disponibilidad desde intervalos y estados, evitando banderas duplicadas. Esto describe el trabajo técnico de la sesión y no una experiencia de aprendizaje atribuida al estudiante.

### Qué cambiar en otra iteración

Añadir persistencia transaccional y una política explícita para fallos de notificación, junto con datos y calendarios de negocio acordados. En Unidad II podrían formalizarse estrategias de tarifa y una fachada de casos de uso.

### Cómo defender la herencia

Mostrar la colección de habitaciones y el mismo llamado precio_noche o precioNoche en cada subtipo. Explicar por qué Reserva tiene una habitación y por qué no es una habitación.

### Cómo demostrar errores

Ejecutar el caso de fechas invertidas, mostrar ErrorDatosHotel capturado y después el Result fallido de una búsqueda o solapamiento. El programa debe continuar tras los fallos controlados.

### Qué falta de la evaluación humana

El estudiante debe publicar o vincular GitHub, completar sus datos, explicar el código, responder preguntas del docente y realizar la revisión con su compañero. El historial local documenta esta preparación asistida, no siete semanas de trabajo humano previo.

## Proyecto integrador

Sistema de reservas de hotel en Puno para recepción y administración. Registra huéspedes y habitaciones de prueba, cotiza, confirma reservas, impide solapamientos, consulta disponibilidad, ordena por precio y anula reservas. No incluye cobros reales, correo real, facturación ni persistencia de producción en esta Unidad I.

[Modelo de clases](diagramas/diagrama_clases.png) y [modelo editable](diagramas/diagrama_clases.drawio).

## Pruebas ejecutadas

Se aprobaron 13 pruebas con unittest, 13 con pytest y 12 con C++17. Pytest recoge las mismas pruebas Python; no son dos conjuntos distintos para sumar.

Las compilaciones usan -Wall -Wextra -Wpedantic -Werror. La demostración Python y C++ produce la misma salida:

```text
PRECIOS=100.00,150.00,200.00
RESERVA=R1
TOTAL=300.00
CONFLICTO=Habitacion sin disponibilidad
BUSQUEDA=Recurso no encontrado
ERROR_DOMINIO=ErrorDatosHotel
DISPONIBLES=101,103
ORDENADAS=101,102,103
ANULADA=ANULADA
DISPONIBLES_TRAS_ANULAR=3
```

Las cifras proceden de evidencias/verificacion.json. Las salidas completas y los comandos ejecutados permanecen en evidencias/.

## Conclusiones

La demostración técnica es reproducible en ambos lenguajes y conserva salidas equivalentes. Su preparación no equivale a aprobar EVI1: la valoración del docente, la defensa y la revisión de pares permanecen fuera de esta sesión.

## Paquete UML y evaluación humana

[Crear reserva](diagramas/secuencia_CU01.png), [anular reserva](diagramas/secuencia_CU02.png) y [estados](diagramas/estados_entidad.png). Las fuentes PlantUML están en el mismo directorio.

La generación con pyreverse y Doxygen está registrada en evidencias/uml_verificacion.json. La interpretación de propiedad y multiplicidades se comprueba aparte; no se deduce solo del número de atributos.

[Trazabilidad](trazabilidad.md), [consistencia](consistencia_uml.md), [revisión técnica](revision_tecnica.md), [ficha de pares](ficha_revision_pares.md) y [guion de sustentación](guion_sustentacion.md).

## Declaración de IA y pendientes externos

Codex analizó las guías, generó las soluciones y pruebas, ejecutó las comprobaciones y preparó los documentos y diagramas. Estas evidencias corresponden a una sesión asistida. No se atribuyen al estudiante comprensión, horas de programación, revisión de un compañero ni una defensa realizada.

Antes de entregar se deben completar los datos personales, renombrar el notebook con ApellidoNombre y publicar o vincular el repositorio GitHub. No se inventa una URL ni una nota de evaluación. Los commits locales registran la preparación actual y no se presentan como actividad histórica durante siete semanas.

## Referencias

Zanabria Gálvez, A. H. (2026). SIS2062026IIPracticaSemana8EVI1.docx. Material del curso SIS206 proporcionado por el usuario.

Python Software Foundation. (s. f.). abc Abstract Base Classes y typing en Python 3.12. https://docs.python.org/3.12/library/abc.html y https://docs.python.org/3.12/library/typing.html

Fowler, M. (2018). Refactoring Improving the Design of Existing Code (2.ª ed.). Addison Wesley. Información de la obra y propósito de refactorización: https://martinfowler.com/books/refactoring.html

Object Management Group. (2017). Unified Modeling Language 2.5.1. https://www.omg.org/spec/UML/2.5.1/
