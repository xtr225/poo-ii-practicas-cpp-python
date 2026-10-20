# Consistencia entre modelo y código

| Comprobación | Resultado | Evidencia |
| --- | --- | --- |
| Clases Python modeladas | 23 | inventario_fuentes.json |
| Generación pyreverse | Ejecutada | automatico_python y logs |
| Generación Doxygen | Ejecutada | automatico_../codigos/cpp/xml e imágenes SVG |
| Composición de máquina | Revisada | ReservaHotel construye su máquina |
| Multiplicidades | Revisadas por contrato | Repositorios admiten cero entidades; una reserva tiene un titular y una habitación |
| Interfaces | Revisadas | CalculadoraTarifa e INotificadorHotel tienen dos operaciones |
| Estado | Verificado por pruebas | Cinco estados y cinco transiciones permitidas |

Los diagramas se elaboraron con asistencia de IA. El modelo de fuentes Python usa Any cuando no existe un tipo declarado; los diagramas C++ generados conservan los tipos estáticos. Las relaciones y cardinalidades se interpretan a partir de los contratos y no se aceptan automáticamente sin revisión.

Const no implica noexcept, private en C++ no depende del sufijo y un vector no implica por sí solo 1..*. Estas precisiones corrigen simplificaciones del material.
