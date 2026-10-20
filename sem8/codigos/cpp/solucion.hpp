#pragma once
#include "hotel_integrado.hpp"
inline void demo() {
    SistemaHotel s;
    auto habitaciones = s.habitaciones.ordenar(
        [](const auto &a, const auto &b) { return a->numero() < b->numero(); });
    std::cout << "PRECIOS=" << std::fixed << std::setprecision(2);
    for (std::size_t i = 0; i < habitaciones.size(); ++i) {
        std::cout << (i ? "," : "") << habitaciones[i]->precioNoche();
    }
    auto r = s.servicio.crearReserva("R1", "H1", 102, "2026-11-01", "2026-11-03", true).valor();
    auto conflicto = s.servicio.crearReserva("R2", "H1", 102, "2026-11-02", "2026-11-04");
    std::string error;
    try {
        s.servicio.crearReserva("R3", "H1", 101, "2026-11-03", "2026-11-01");
    } catch (const ErrorDatosHotel &) {
        error = "ErrorDatosHotel";
    }
    auto libres = s.servicio.disponibles("2026-11-01", "2026-11-03");
    auto ordenadas = s.habitaciones.ordenar(
        [](const auto &a, const auto &b) { return a->precioNoche() < b->precioNoche(); });
    std::cout << "\nRESERVA=" << r->codigo() << "\nTOTAL=" << r->total()
              << "\nCONFLICTO=" << conflicto.error()
              << "\nBUSQUEDA=" << s.reservas.buscarResultado("NO").error()
              << "\nERROR_DOMINIO=" << error << "\nDISPONIBLES=";
    for (std::size_t i = 0; i < libres.size(); ++i) {
        std::cout << (i ? "," : "") << libres[i]->numero();
    }
    std::cout << "\nORDENADAS=";
    for (std::size_t i = 0; i < ordenadas.size(); ++i) {
        std::cout << (i ? "," : "") << ordenadas[i]->numero();
    }
    s.servicio.anularReserva("R1", true);
    std::cout << "\nANULADA=" << nombreEstado(r->estado()) << "\nDISPONIBLES_TRAS_ANULAR="
              << s.servicio.disponibles("2026-11-01", "2026-11-03").size() << '\n';
}
