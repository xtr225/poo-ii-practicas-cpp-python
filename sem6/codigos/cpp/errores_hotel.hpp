#pragma once
#include "hotel_base.hpp"
#include "result.hpp"
class ErrorHotel : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
    virtual ~ErrorHotel() = default;
};
class ErrorDatosHotel : public ErrorHotel {
  public:
    using ErrorHotel::ErrorHotel;
};
class ErrorEstadoReserva : public ErrorHotel {
  public:
    using ErrorHotel::ErrorHotel;
};
class ErrorDisponibilidad : public ErrorHotel {
  public:
    using ErrorHotel::ErrorHotel;
};
class HabitacionRobusta : public Habitacion {
  public:
    HabitacionRobusta(int numero, const std::string &tipo, int capacidad, double tarifa) try
        : Habitacion(numero, tipo, capacidad, tarifa) {
        verificarInvariante();
    } catch (const std::invalid_argument &e) {
        throw ErrorDatosHotel(e.what());
    }
    void verificarInvariante() const {
        assert(numero() > 0 && tarifa() > 0);
        assert(plazasLibres() >= 0 && plazasLibres() <= capacidad());
    }
    void alojar(int n) {
        try {
            Habitacion::alojar(n);
        } catch (const std::runtime_error &e) {
            throw ErrorDisponibilidad(e.what());
        }
        verificarInvariante();
    }
};
class ReservaRobusta : public Reserva {
  public:
    ReservaRobusta(const std::string &codigo, const std::string &entrada,
                   const std::string &salida) try
        : Reserva(codigo, entrada, salida) {
        verificarInvariante();
    } catch (const std::invalid_argument &e) {
        throw ErrorDatosHotel(e.what());
    }
    void verificarInvariante() const {
        assert(!codigo().empty() && noches() > 0);
    }
    void confirmar() {
        try {
            Reserva::confirmar();
        } catch (const std::runtime_error &e) {
            throw ErrorEstadoReserva(e.what());
        }
        verificarInvariante();
    }
    void anular() {
        try {
            Reserva::anular();
        } catch (const std::runtime_error &e) {
            throw ErrorEstadoReserva(e.what());
        }
        verificarInvariante();
    }
};
inline Result<std::shared_ptr<Habitacion>>
buscarHabitacion(const std::vector<std::shared_ptr<Habitacion>> &hs, int numero) {
    for (const auto &h : hs) {
        if (h && h->numero() == numero) {
            return Result<std::shared_ptr<Habitacion>>::ok(h);
        }
    }
    return Result<std::shared_ptr<Habitacion>>::fallo("Habitacion no encontrada");
}
