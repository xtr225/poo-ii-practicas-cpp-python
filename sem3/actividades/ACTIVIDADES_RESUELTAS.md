# Semana 3 Herencia polimorfismo y sustitución

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


UNAP SIS206 POO II 2026 II. Identificador S3-001. Fecha 19 de octubre de 2026.

## Objetivo y alcance

Implementar una planilla heterogénea de personal, comprobar un contrato común y justificar las relaciones de herencia del proyecto hotelero.

Docente y Administrativo estaban incompletos. La jerarquía necesita preservar remuneración positiva y nombre válido en todos los subtipos. El ejemplo de Cuadrado se conserva como contraejemplo intencional y se ofrece una alternativa con Figura independiente.

## Desarrollo por actividades

| Actividad | Fuente resuelta | Comprobación |
| --- | --- | --- |
| 1 IS A y HAS A | ACTIVIDADES_RESUELTAS.md | Se justifican las seis relaciones solicitadas. |
| 2 Planilla | python/solucion.py | Docente, Administrativo e Investigador con validación y despacho polimórfico. |
| 3 Liskov | python/solucion.py | Contrato común comprobado; fallo esperado del cuadrado incompatible. |
| 4 C++ | cpp/solucion.hpp | Base abstracta, destructor virtual, override y vector de unique_ptr. |
| 5 Hotel | python/hotel_herencia.py y cpp/hotel_herencia.hpp | Simple, Doble y Suite sustituyen a Habitacion al cotizar. |

## Decisiones de implementación

La planilla llama calcular_remuneracion o calcularRemuneracion a través del contrato base. No elige fórmulas mediante condicionales sobre el tipo del objeto. Los cuatro ejemplos producen 1680, 1620, 3150 y 3000; el total es 9450.

Se valida que el bono administrativo sea finito y no negativo, además de un salario positivo. Esa regla adicional evita construir una subclase que incumpla el contrato de remuneración positiva. Las horas admiten 1 a 40; las categorías usan los factores 1.0, 1.4 y 1.8.

La regla correcta de Liskov es no fortalecer precondiciones ni debilitar poscondiciones. La formulación inversa de un párrafo de la guía no se usa como regla de implementación. Cambiar las dos dimensiones de CuadradoIncompatible rompe la independencia prometida por Rectangulo.

## Análisis de las seis relaciones

| Aspecto | Python o decisión | C++ o justificación |
| --- | --- | --- |
| Posgrado y Estudiante | IS A | El subtipo conserva el contrato de estudiante y añade especialización. |
| Sensor y Dispositivo | IS A | Un sensor es un dispositivo con una función específica. |
| Informe e Impresora | HAS A o dependencia de uso | El informe utiliza una impresora; no es una impresora. |
| LibroDigital y Libro | IS A con contrato común | Metadatos y lectura compartidos; no exigir soporte físico en la base. |
| Motocicleta y Vehiculo | IS A | Comparte el contrato de vehículo y lo especializa. |
| Departamento y Empleado | HAS A por agregación | Agrupa empleados cuya existencia no depende del departamento. |

## Análisis y reflexión

### Por qué falla el cuadrado

La matemática clasifica figuras por sus propiedades; la interfaz mutable de Rectangulo promete cambios independientes de ancho y alto. El cuadrado no puede mantener ambas promesas. La alternativa comparte solo Figura.area y mantiene ambas clases separadas.

### Decisión en el hotel

Simple, Doble y Suite son habitaciones y preservan una cotización positiva. Una reserva tiene un huésped y una habitación; no debe heredar de ellos. La relación se modela mediante referencias o composición según la vida de cada objeto.

### Destructor virtual

Al destruir un objeto derivado a través de un puntero a una base sin destructor virtual se incurre en comportamiento indefinido. La base polimórfica declara un destructor virtual y unique_ptr gestiona la liberación.

### Duck typing y tipos estrictos

Python admite objetos que cumplen operaciones sin pertenecer a una jerarquía nominal; facilita pruebas y adaptadores, con errores potencialmente tardíos. C++ verifica el contrato declarado durante compilación, a costa de mayor declaración de tipos. ABC y las pruebas hacen explícitas las expectativas en Python.

## Proyecto integrador

La jerarquía hotelera mantiene una base Habitacion y tres variantes con precio_noche o precioNoche sobrescrito. Una colección heterogénea cotiza 100, 150 y 200 para una tarifa base de 100. La capacidad y los invariantes siguen perteneciendo a la base. El diagrama separa esta herencia de las asociaciones de Reserva.

[Modelo de clases](diagramas/diagrama_clases.png) y [modelo editable](diagramas/diagrama_clases.drawio).

## Pruebas ejecutadas

Se aprobaron 13 pruebas con unittest, 13 con pytest y 13 con C++17. Pytest recoge las mismas pruebas Python; no son dos conjuntos distintos para sumar.

Las compilaciones usan -Wall -Wextra -Wpedantic -Werror. La demostración Python y C++ produce la misma salida:

```text
Mamani Quispe, Juan Carlos: S/ 1680.00
Quispe Torres, Maria Elena: S/ 1620.00
Huanca Lazo, Pedro: S/ 3150.00
Flores Ramos, Ana Lucia: S/ 3000.00
TOTAL=9450.00
CONTRATOS=4
```

Las cifras proceden de evidencias/verificacion.json. Las salidas completas y los comandos ejecutados permanecen en evidencias/.

## Conclusiones

La herencia se justifica por un contrato que puede sustituirse. El contraejemplo falla de manera controlada y las alternativas mantienen contratos coherentes. La guía mezcla los rótulos PC1 y PC3; esta entrega se identifica como Semana 3 y no atribuye una calificación.

## Declaración de IA y pendientes externos

Codex analizó las guías, generó las soluciones y pruebas, ejecutó las comprobaciones y preparó los documentos y diagramas. Estas evidencias corresponden a una sesión asistida. No se atribuyen al estudiante comprensión, horas de programación, revisión de un compañero ni una defensa realizada.

Antes de entregar se deben completar los datos personales, renombrar el notebook con ApellidoNombre y publicar o vincular el repositorio GitHub. No se inventa una URL ni una nota de evaluación. Los commits locales registran la preparación actual y no se presentan como actividad histórica durante siete semanas.

## Referencias

Zanabria Gálvez, A. H. (2026). SIS2062026IIPracticaSemana3.pdf. Material del curso SIS206 proporcionado por el usuario.

Python Software Foundation. (s. f.). abc Abstract Base Classes y typing en Python 3.12. https://docs.python.org/3.12/library/abc.html y https://docs.python.org/3.12/library/typing.html

Fowler, M. (2018). Refactoring Improving the Design of Existing Code (2.ª ed.). Addison Wesley. Información de la obra y propósito de refactorización: https://martinfowler.com/books/refactoring.html

Object Management Group. (2017). Unified Modeling Language 2.5.1. https://www.omg.org/spec/UML/2.5.1/
