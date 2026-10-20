# Semana 4 Interfaces composición e inyección de dependencias

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


UNAP SIS206 POO II 2026 II. Identificador S4-001. Fecha 19 de octubre de 2026.

## Objetivo y alcance

Definir contratos intercambiables para notificaciones y reportes, y separar la cotización del hotel mediante una dependencia recibida desde el exterior.

Los esquemas mezclan contratos con métodos pendientes y exportaciones simuladas. Se completaron las implementaciones, se aislaron errores por canal y se añadió una prueba con exportador de memoria. Las notificaciones continúan siendo simulaciones y no contactan servicios externos.

## Desarrollo por actividades

| Actividad | Fuente resuelta | Comprobación |
| --- | --- | --- |
| 1 Notificaciones | python/solucion.py | ABC, Protocol, Email, SMS y SistemaAlertas. |
| 2 Reportes | python/solucion.py | Cambio de exportador, firma simulada y mock verificable. |
| 3 C++ | cpp/solucion.hpp | Interfaz abstracta y propiedad mediante unique_ptr. |
| 4 Comparaciones | ACTIVIDADES_RESUELTAS.md | Tablas de abstracción y estrategias de diseño completas. |
| 5 Hotel | CalculadoraTarifa y ServicioCotizacion | Interfaz de dos métodos y cálculo recibido en el constructor. |

## Decisiones de implementación

Email y SMS implementan INotificador y comparten un CanalSimulado por composición. Un SMS no hereda de Email. SistemaAlertas cuenta solo envíos exitosos y registra el error de un canal sin impedir que los demás se procesen.

GeneradorReporte recibe exportador y firmador opcional. En Python transforma el diccionario a una representación intermedia y sus exportadores escriben CSV o JSON válido. En C++ recibe contenido ya serializado y delega su destino; esta diferencia del contrato interno se declara, y no se afirma una comparación byte a byte de los archivos exportados.

FirmadorDigital es una simulación explícita que crea una copia marcada. No ofrece autenticidad, certificados ni una firma criptográfica. Los mensajes de notificación se guardan en memoria; no se utiliza correo ni telefonía reales.

## Contratos y estrategias de diseño

| Aspecto | Python o decisión | C++ o justificación |
| --- | --- | --- |
| Instanciación | ABC abstracta y Protocol no se instancian | Clase con virtual puro no se instancia |
| Estado e implementación | ABC puede tener ambos; Protocol describe estructura | Una clase abstracta puede tener ambos; una interfaz pura los evita por diseño |
| Método abstracto | @abstractmethod | virtual metodo(...) = 0 |
| Herencia explícita | ABC nominal; Protocol puede ser estructural | Implementación nominal mediante herencia |
| Cambio de comportamiento | Composición permite sustituir dependencias | Herencia fija la estructura del subtipo |
| Pruebas | Mock o doble inyectado sin archivos | Implementación de interfaz recibida por el cliente |

## Composición frente a herencia

| Criterio | Composición | Herencia de implementación |
| --- | --- | --- |
| Flexibilidad | Sustitución de objetos en ejecución | Estructura de la jerarquía fijada por tipos |
| Pruebas | Dobles inyectables | Puede exigir preparar la base y sus dependencias |
| Reutilización | Colaboración por contratos | Reutiliza implementación de la base |
| Acoplamiento | Interfaz pública del colaborador | Contrato y posibles detalles protegidos de la base |
| Cuándo usar | Comportamiento intercambiable y HAS A | IS A que preserva el contrato de sustitución |

## Análisis y reflexión

### Agregar XML

En composición basta un nuevo ExportadorXML; las combinaciones con y sin firma reutilizan el generador. En el diseño concreto de herencia pueden requerirse variantes para las combinaciones. El número depende de esa jerarquía, no hay una cifra universal.

### Mock para GeneradorReporte

Debe implementar extension y exportar, registrar contenido y ruta y no tocar el sistema de archivos. La prueba compara el número de llamadas y los datos recibidos.

### CSV con firma y JSON sin firma

Se construyen dos generadores con dependencias diferentes, o se cambia el exportador de uno. Las configuraciones conviven sin introducir una subclase por combinación.

### Protocol frente a ABC

Protocol admite conformidad estructural sin modificar clases ajenas. ABC conviene cuando se necesita impedir instanciaciones incompletas o compartir un contrato nominal. runtime_checkable comprueba presencia de miembros, no la corrección completa de sus tipos o de su conducta.

### Dependencias creadas internamente

Un cliente que construye su base de datos fuerza conexiones o recursos durante una prueba. Recibir la abstracción permite sustituirla por memoria y verificar la coordinación sin infraestructura real.

### Herencia múltiple

Puede introducir ambigüedad, duplicación de bases y conflicto de inicialización. Python dispone de MRO y super cooperativo; C++ ofrece reglas propias y herencia virtual. Java no permite herencia múltiple de clases concretas. Ninguno de estos hechos obliga a usar jerarquías profundas.

## Proyecto integrador

CalculadoraTarifa declara calcular y descripcion. TarifaEstandar implementa el contrato; ServicioCotizacion recibe esa dependencia. Se utiliza herencia para representar el contrato y composición para elegir el comportamiento de cotización. El diagrama distingue propiedad exclusiva, referencias y realización de interfaz.

[Modelo de clases](diagramas/diagrama_clases.png) y [modelo editable](diagramas/diagrama_clases.drawio).

## Pruebas ejecutadas

Se aprobaron 11 pruebas con unittest, 11 con pytest y 10 con C++17. Pytest recoge las mismas pruebas Python; no son dos conjuntos distintos para sumar.

Las compilaciones usan -Wall -Wextra -Wpedantic -Werror. La demostración Python y C++ produce la misma salida:

```text
CANALES=2
SMS_LARGO=1
COTIZACION=300.00
```

Las cifras proceden de evidencias/verificacion.json. Las salidas completas y los comandos ejecutados permanecen en evidencias/.

## Conclusiones

La comprobación relevante es cambiar o sustituir una dependencia sin editar el cliente que la consume. Las pruebas muestran esa sustitución y evitan presentar simulaciones como servicios reales.

## Declaración de IA y pendientes externos

Codex analizó las guías, generó las soluciones y pruebas, ejecutó las comprobaciones y preparó los documentos y diagramas. Estas evidencias corresponden a una sesión asistida. No se atribuyen al estudiante comprensión, horas de programación, revisión de un compañero ni una defensa realizada.

Antes de entregar se deben completar los datos personales, renombrar el notebook con ApellidoNombre y publicar o vincular el repositorio GitHub. No se inventa una URL ni una nota de evaluación. Los commits locales registran la preparación actual y no se presentan como actividad histórica durante siete semanas.

## Referencias

Zanabria Gálvez, A. H. (2026). SIS2062026IIPracticaSemana4.pdf. Material del curso SIS206 proporcionado por el usuario.

Python Software Foundation. (s. f.). abc Abstract Base Classes y typing en Python 3.12. https://docs.python.org/3.12/library/abc.html y https://docs.python.org/3.12/library/typing.html

Fowler, M. (2018). Refactoring Improving the Design of Existing Code (2.ª ed.). Addison Wesley. Información de la obra y propósito de refactorización: https://martinfowler.com/books/refactoring.html

Object Management Group. (2017). Unified Modeling Language 2.5.1. https://www.omg.org/spec/UML/2.5.1/
