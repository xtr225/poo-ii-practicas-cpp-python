#include "solucion.hpp"
int main() {
    Pruebas t;
    t.caso("estado inicial", [] { exigir(Matricula().estado() == EstadoMatricula::REGISTRADA); });
    t.caso("secuencia valida", [] {
        Matricula m;
        m.validarPago();
        m.suspender("deuda");
        m.regularizar();
        m.anular();
        exigir(m.estado() == EstadoMatricula::ANULADA && m.historial().size() == 5);
    });
    t.caso("final irreversible", [] {
        Matricula m;
        m.anular();
        debeFallar([&] { m.validarPago(); });
    });
    t.caso("motivo atomico", [] {
        Matricula m;
        debeFallar([&] { m.suspender("deuda"); });
        exigir(m.motivo().empty() && m.historial().size() == 1);
    });
    t.caso("regularizar precondicion", [] {
        Matricula m;
        debeFallar([&] { m.regularizar(); });
    });
    t.caso("guardia", [] {
        MaquinaEstados<int> m(0, {{0, 1}});
        debeFallar([&] { m.transicionar(1, [] { return false; }); });
        exigir(m.estado() == 0);
        m.transicionar(1, [] { return true; });
        exigir(m.estado() == 1);
    });
    t.caso("reserva final", [] {
        SistemaHotel s;
        auto r = s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03").valor();
        r->ingresar();
        r->finalizar();
        exigir(r->estado() == EstadoReserva::FINALIZADA);
        debeFallar<ErrorEstadoReserva>([&] { r->anular(); });
    });
    t.caso("anular libera", [] {
        SistemaHotel s;
        s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03");
        s.servicio.anularReserva("R1");
        exigir(s.servicio.disponibles("2026-11-01", "2026-11-03").size() == 3);
    });
    return t.terminar();
}
