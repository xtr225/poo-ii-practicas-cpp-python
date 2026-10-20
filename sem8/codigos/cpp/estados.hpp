#pragma once
#include "comun.hpp"
#include <optional>
template <class E> class MaquinaEstados {
    E estado_;
    std::vector<std::pair<E, E>> transiciones_;
    std::vector<std::pair<std::optional<E>, E>> historial_;

  public:
    MaquinaEstados(E inicial, std::vector<std::pair<E, E>> tabla)
        : estado_(inicial), transiciones_(std::move(tabla)), historial_{{std::nullopt, inicial}} {}
    E estado() const noexcept {
        return estado_;
    }
    const auto &historial() const noexcept {
        return historial_;
    }
    void transicionar(E destino, const std::function<bool()> &guardia = {}) {
        if (std::find(transiciones_.begin(), transiciones_.end(),
                      std::make_pair(estado_, destino)) == transiciones_.end()) {
            throw std::invalid_argument("Transicion no permitida");
        }
        if (guardia && !guardia()) {
            throw std::invalid_argument("Guardia no satisfecha");
        }
        historial_.push_back({estado_, destino});
        estado_ = destino;
    }
};
enum class EstadoMatricula { REGISTRADA, VIGENTE, SUSPENDIDA, ANULADA };
class Matricula {
    MaquinaEstados<EstadoMatricula> maquina_;
    std::string motivo_;

  public:
    Matricula()
        : maquina_(EstadoMatricula::REGISTRADA,
                   {{EstadoMatricula::REGISTRADA, EstadoMatricula::VIGENTE},
                    {EstadoMatricula::REGISTRADA, EstadoMatricula::ANULADA},
                    {EstadoMatricula::VIGENTE, EstadoMatricula::SUSPENDIDA},
                    {EstadoMatricula::VIGENTE, EstadoMatricula::ANULADA},
                    {EstadoMatricula::SUSPENDIDA, EstadoMatricula::VIGENTE},
                    {EstadoMatricula::SUSPENDIDA, EstadoMatricula::ANULADA}}) {}
    EstadoMatricula estado() const noexcept {
        return maquina_.estado();
    }
    const auto &historial() const noexcept {
        return maquina_.historial();
    }
    const std::string &motivo() const noexcept {
        return motivo_;
    }
    std::string nombreEstado() const {
        switch (estado()) {
        case EstadoMatricula::REGISTRADA:
            return "REGISTRADA";
        case EstadoMatricula::VIGENTE:
            return "VIGENTE";
        case EstadoMatricula::SUSPENDIDA:
            return "SUSPENDIDA";
        case EstadoMatricula::ANULADA:
            return "ANULADA";
        }
        return "";
    }
    void validarPago() {
        if (estado() != EstadoMatricula::REGISTRADA) {
            throw std::invalid_argument("Pago solo registrada");
        }
        maquina_.transicionar(EstadoMatricula::VIGENTE);
    }
    void suspender(const std::string &motivo) {
        textoValido(motivo);
        maquina_.transicionar(EstadoMatricula::SUSPENDIDA);
        motivo_ = motivo;
    }
    void regularizar() {
        if (estado() != EstadoMatricula::SUSPENDIDA) {
            throw std::invalid_argument("Regularizar solo suspendida");
        }
        maquina_.transicionar(EstadoMatricula::VIGENTE);
        motivo_.clear();
    }
    void anular() {
        maquina_.transicionar(EstadoMatricula::ANULADA);
    }
};
enum class EstadoReserva { REGISTRADA, CONFIRMADA, HOSPEDADA, FINALIZADA, ANULADA };
inline std::string nombreEstado(EstadoReserva estado) {
    switch (estado) {
    case EstadoReserva::REGISTRADA:
        return "REGISTRADA";
    case EstadoReserva::CONFIRMADA:
        return "CONFIRMADA";
    case EstadoReserva::HOSPEDADA:
        return "HOSPEDADA";
    case EstadoReserva::FINALIZADA:
        return "FINALIZADA";
    case EstadoReserva::ANULADA:
        return "ANULADA";
    }
    return "";
}
inline MaquinaEstados<EstadoReserva> maquinaReserva() {
    using E = EstadoReserva;
    return MaquinaEstados<E>(E::REGISTRADA, {{E::REGISTRADA, E::CONFIRMADA},
                                             {E::REGISTRADA, E::ANULADA},
                                             {E::CONFIRMADA, E::HOSPEDADA},
                                             {E::CONFIRMADA, E::ANULADA},
                                             {E::HOSPEDADA, E::FINALIZADA}});
}
