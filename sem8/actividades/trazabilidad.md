# Trazabilidad de los casos de uso

| Mensaje | Receptor | Archivo |
| --- | --- | --- |
| crear_reserva | ServicioReservas | hotel_integrado.py / hotel_integrado.hpp |
| existe | Repositorio | repositorio.py / repositorio.hpp |
| buscar_resultado / buscarResultado | Repositorio | repositorio.py / repositorio.hpp |
| _solapadas / solapadas | ServicioReservas | hotel_integrado.py / hotel_integrado.hpp |
| filtrar | Repositorio | repositorio.py / repositorio.hpp |
| calcular | CalculadoraTarifa | hotel_integrado.py / hotel_integrado.hpp |
| confirmar | ReservaHotel | hotel_integrado.py / hotel_integrado.hpp |
| insertar | Repositorio | repositorio.py / repositorio.hpp |
| anular_reserva / anularReserva | ServicioReservas | hotel_integrado.py / hotel_integrado.hpp |
| anular | ReservaHotel | hotel_integrado.py / hotel_integrado.hpp |
| transicionar | MaquinaEstados | estados.py / estados.hpp |
| es_disponible / esDisponible y enviar | INotificadorHotel | hotel_integrado.py / hotel_integrado.hpp |
| disponibles | ServicioReservas | hotel_integrado.py / hotel_integrado.hpp |

CU01 corresponde a crear y confirmar una reserva. CU02 corresponde a anular y consultar disponibilidad. Los mensajes de error vuelven por Result o por una excepción de dominio según el contrato. Las notificaciones son simuladas.
