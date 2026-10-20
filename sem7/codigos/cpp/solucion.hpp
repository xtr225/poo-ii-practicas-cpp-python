#pragma once
#include "hotel_integrado.hpp"
inline void demo() {
    Matricula m;
    m.validarPago();
    m.suspender("deuda");
    m.regularizar();
    m.anular();
    SistemaHotel s;
    auto r = s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03").valor();
    r->ingresar();
    r->finalizar();
    std::cout << "MATRICULA=" << m.nombreEstado()
              << "\nHISTORIAL_MATRICULA=" << m.historial().size()
              << "\nRESERVA=" << nombreEstado(r->estado())
              << "\nHISTORIAL_RESERVA=" << r->historial().size() << '\n';
}
