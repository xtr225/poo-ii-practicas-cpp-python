#include "solucion.hpp"
#include <type_traits>
int main() {
    Pruebas t;
    t.caso("polimorfismo", [] {
        SistemaHotel s;
        auto hs = s.habitaciones.ordenar(
            [](const auto &a, const auto &b) { return a->numero() < b->numero(); });
        exigir(hs[0]->precioNoche() == 100 && hs[1]->precioNoche() == 150 &&
               hs[2]->precioNoche() == 200);
    });
    t.caso("interfaz", [] { exigir(std::is_abstract<CalculadoraTarifa>::value); });
    t.caso("reserva valida", [] {
        SistemaHotel s;
        auto r = s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03").valor();
        exigir(r->total() == 300 && r->estado() == EstadoReserva::CONFIRMADA);
    });
    t.caso("huesped ausente", [] {
        SistemaHotel s;
        exigir(!s.servicio.crearReserva("R1", "NO", 102, "2026-11-01", "2026-11-03").esExito());
    });
    t.caso("fecha invalida atomica", [] {
        SistemaHotel s;
        debeFallar<ErrorDatosHotel>(
            [&] { s.servicio.crearReserva("R1", "H1", 102, "2026-11-03", "2026-11-01"); });
        exigir(s.reservas.tamano() == 0);
    });
    t.caso("codigo duplicado", [] {
        SistemaHotel s;
        s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03");
        exigir(!s.servicio.crearReserva("R1", "H1", 101, "2026-11-01", "2026-11-03").esExito());
    });
    t.caso("solapamiento", [] {
        SistemaHotel s;
        s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03");
        exigir(!s.servicio.crearReserva("R2", "H1", 102, "2026-11-02", "2026-11-04").esExito());
    });
    t.caso("adyacencia", [] {
        SistemaHotel s;
        s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03");
        exigir(s.servicio.crearReserva("R2", "H1", 102, "2026-11-03", "2026-11-05").esExito());
    });
    t.caso("anulada no bloquea", [] {
        SistemaHotel s;
        s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03");
        s.servicio.anularReserva("R1");
        exigir(s.servicio.crearReserva("R2", "H1", 102, "2026-11-01", "2026-11-03").esExito());
    });
    t.caso("estado invalido atomico", [] {
        SistemaHotel s;
        auto r = s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03").valor();
        r->ingresar();
        debeFallar<ErrorEstadoReserva>([&] { r->anular(); });
        exigir(r->estado() == EstadoReserva::HOSPEDADA);
    });
    t.caso("result y filtros", [] {
        SistemaHotel s;
        exigir(!s.reservas.buscarResultado("NO").esExito());
        exigir(s.habitaciones.filtrar([](const auto &h) { return h->capacidad() >= 2; }).size() ==
               2);
    });
    t.caso("sistema no copiable", [] {
        exigir(!std::is_copy_constructible<SistemaHotel>::value &&
               !std::is_move_constructible<SistemaHotel>::value);
    });
    return t.terminar();
}
