# Semana 7 Modelo UML estados y preparación EVI1

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


UNAP SIS206 POO II 2026 II. Identificador S7-001. Fecha 19 de octubre de 2026.

## Objetivo y alcance

Documentar el proyecto del hotel mediante clases, dos secuencias y estados, comprobar su correspondencia con el código y preparar el paquete técnico EVI1.

El ejemplo de máquina de estados modifica el motivo de suspensión antes de comprobar si la transición es válida. Se corrigió el orden: validar y transicionar primero, actualizar el motivo después. El UML debe representar la vida y cardinalidad reales; un vector por sí solo no garantiza una multiplicidad mínima de uno.

## Desarrollo por actividades

| Actividad | Fuente resuelta | Comprobación |
| --- | --- | --- |
| 1 Clases | docs/uml/ | Modelo elaborado, generación desde Python y C++ y comparación documentada. |
| 2 Secuencias | secuencia_CU01 y secuencia_CU02 | Crear y anular reserva con participantes, mensajes, alt y opt. |
| 3 Estados | python/estados.py y cpp/estados.hpp | Máquinas genéricas y transición integrada en ReservaHotel. |
| 4 Revisión | docs/revision_tecnica.md | Autorrevisión asistida y ficha separada para el compañero. |
| 5 EVI1 | README y paquete de integración | Fuentes, pruebas, UML e informe de decisiones enlazados. |

## Decisiones de implementación

El modelo de clases distingue entidades, repositorios y servicios. La reserva mantiene una máquina de estados propia; huésped y habitación tienen existencia independiente. ServicioReservas recibe repositorios y contratos de cotización y notificación.

Crear una reserva consulta huésped y habitación, comprueba cruces de fechas, cotiza, confirma e inserta. Anular consulta la reserva y solicita una transición; la disponibilidad se deriva de las reservas no anuladas y no se guarda como una bandera duplicada.

Los estados del hotel son REGISTRADA, CONFIRMADA, HOSPEDADA, FINALIZADA y ANULADA. FINALIZADA y ANULADA no admiten salidas. Las transiciones inválidas dejan intactos estado e historial. El ejemplo académico mantiene REGISTRADA, VIGENTE, SUSPENDIDA y ANULADA.

## Notación y comprobaciones importantes

| Aspecto | Python o decisión | C++ o justificación |
| --- | --- | --- |
| Herencia | Habitacion a sus variantes | Triángulo vacío hacia la base |
| Realización | TarifaEstandar implementa contrato | Línea discontinua hacia la interfaz |
| Composición | ReservaHotel posee su máquina | Rombo lleno en el propietario |
| Asociación | Reserva usa huésped y habitación | Existencia independiente y cardinalidades explícitas |
| Multiplicidad | Colección puede estar vacía | 0..* salvo invariante más restrictivo |
| Const y noexcept | Son propiedades distintas | Una consulta const todavía puede lanzar |

## Análisis y reflexión

### Qué tiene prioridad si UML y código difieren

El código ejecutado describe la conducta actual y el modelo expresa intención. La diferencia debe convertirse en una tarea de revisión: corregir el diseño o la implementación y actualizar ambos en el mismo cambio. Ninguno debe modificarse a ciegas para ocultar una discrepancia.

### Implicancia de los tipos en pyreverse

Las anotaciones ayudan a reconstruir relaciones. Un diagrama generado no demuestra por sí solo vida, propiedad exclusiva o cardinalidades del dominio; esas decisiones se verifican con el código y sus invariantes.

### Enum y diccionario frente a switch

La tabla hace visibles los pares permitidos y facilita añadir transiciones. enum class y switch ofrecen comprobación estática de nombres y cobertura revisable en C++. En ambos casos se necesitan pruebas de pares prohibidos y estados finales.

### Clases faltantes en el manual

La comparación usa los inventarios generados y las listas de clases de las fuentes. Las diferencias de auxiliares del lenguaje se explican en el documento de consistencia en vez de ocultarse.

### Relaciones no detectadas automáticamente

La propiedad y la multiplicidad se revisan por separado. Una referencia compartida no se transforma en composición por aparecer como atributo y un tipo de colección no impone por sí solo un mínimo de elementos.

### Revisión cruzada

Se entrega una revisión técnica asistida con comentarios y una ficha para un revisor humano. No se inventa nombre, nota ni intercambio de repositorios con un compañero.

## Proyecto integrador

Los diagramas de crear y anular reserva siguen llamadas existentes en ServicioReservas, Repositorio, ReservaHotel, MaquinaEstados y el notificador simulado. La tabla de trazabilidad vincula cada mensaje con el método y archivo. El diagrama de estados coincide con los cinco estados del enum y sus cinco transiciones hoteleras.

[Modelo de clases](diagramas/diagrama_clases.png) y [modelo editable](diagramas/diagrama_clases.drawio).

## Pruebas ejecutadas

Se aprobaron 9 pruebas con unittest, 9 con pytest y 8 con C++17. Pytest recoge las mismas pruebas Python; no son dos conjuntos distintos para sumar.

Las compilaciones usan -Wall -Wextra -Wpedantic -Werror. La demostración Python y C++ produce la misma salida:

```text
MATRICULA=ANULADA
HISTORIAL_MATRICULA=5
RESERVA=FINALIZADA
HISTORIAL_RESERVA=4
```

Las cifras proceden de evidencias/verificacion.json. Las salidas completas y los comandos ejecutados permanecen en evidencias/.

## Conclusiones

Las pruebas de estados y la comparación entre modelo y fuentes aportan evidencia técnica para EVI1. La revisión humana de pares, la publicación GitHub y la explicación individual siguen siendo actividades del estudiante.

## Paquete UML y evaluación humana

[Crear reserva](diagramas/secuencia_CU01.png), [anular reserva](diagramas/secuencia_CU02.png) y [estados](diagramas/estados_entidad.png). Las fuentes PlantUML están en el mismo directorio.

La generación con pyreverse y Doxygen está registrada en evidencias/uml_verificacion.json. La interpretación de propiedad y multiplicidades se comprueba aparte; no se deduce solo del número de atributos.

[Trazabilidad](trazabilidad.md), [consistencia](consistencia_uml.md), [revisión técnica](revision_tecnica.md), [ficha de pares](ficha_revision_pares.md) y [guion de sustentación](guion_sustentacion.md).

## Declaración de IA y pendientes externos

Codex analizó las guías, generó las soluciones y pruebas, ejecutó las comprobaciones y preparó los documentos y diagramas. Estas evidencias corresponden a una sesión asistida. No se atribuyen al estudiante comprensión, horas de programación, revisión de un compañero ni una defensa realizada.

Antes de entregar se deben completar los datos personales, renombrar el notebook con ApellidoNombre y publicar o vincular el repositorio GitHub. No se inventa una URL ni una nota de evaluación. Los commits locales registran la preparación actual y no se presentan como actividad histórica durante siete semanas.

## Referencias

Zanabria Gálvez, A. H. (2026). SIS2062026IIPracticaSemana7.docx. Material del curso SIS206 proporcionado por el usuario.

Python Software Foundation. (s. f.). abc Abstract Base Classes y typing en Python 3.12. https://docs.python.org/3.12/library/abc.html y https://docs.python.org/3.12/library/typing.html

Fowler, M. (2018). Refactoring Improving the Design of Existing Code (2.ª ed.). Addison Wesley. Información de la obra y propósito de refactorización: https://martinfowler.com/books/refactoring.html

Object Management Group. (2017). Unified Modeling Language 2.5.1. https://www.omg.org/spec/UML/2.5.1/
