#pragma once
#include "errores_hotel.hpp"
#include "estados.hpp"
#include "hotel_herencia.hpp"
#include "repositorio.hpp"
class CalculadoraTarifa {
  public:
    virtual ~CalculadoraTarifa() = default;
    virtual double calcular(const Habitacion &, int noches) const = 0;
    virtual std::string descripcion() const = 0;
};
class TarifaEstandar : public CalculadoraTarifa {
  public:
    double calcular(const Habitacion &h, int noches) const override {
        if (noches < 1) {
            throw ErrorDatosHotel("Noches invalidas");
        }
        return h.precioNoche() * noches;
    }
    std::string descripcion() const override {
        return "Estandar";
    }
};
class INotificadorHotel {
  public:
    virtual ~INotificadorHotel() = default;
    virtual bool enviar(const std::string &, const std::string &) = 0;
    virtual bool esDisponible() const = 0;
};
class NotificadorSimulado : public INotificadorHotel {
  public:
    std::vector<std::pair<std::string, std::string>> eventos;
    bool esDisponible() const override {
        return true;
    }
    bool enviar(const std::string &destino, const std::string &mensaje) override {
        eventos.push_back({destino, mensaje});
        return true;
    }
};
class ReservaHotel {
    std::string codigo_, entrada_, salida_;
    std::shared_ptr<Huesped> huesped_;
    std::shared_ptr<Habitacion> habitacion_;
    double total_;
    MaquinaEstados<EstadoReserva> maquina_ = maquinaReserva();
    void transicionar(EstadoReserva destino) {
        try {
            maquina_.transicionar(destino);
        } catch (const std::invalid_argument &e) {
            throw ErrorEstadoReserva(e.what());
        }
        verificarInvariante();
    }

  public:
    ReservaHotel(std::string codigo, std::shared_ptr<Huesped> huesped,
                 std::shared_ptr<Habitacion> habitacion, std::string entrada, std::string salida,
                 double total)
        : codigo_(std::move(codigo)), entrada_(std::move(entrada)), salida_(std::move(salida)),
          huesped_(std::move(huesped)), habitacion_(std::move(habitacion)), total_(total) {
        if (!huesped_ || !habitacion_ || !std::isfinite(total_) || total_ <= 0) {
            throw ErrorDatosHotel("Reserva invalida");
        }
        try {
            textoValido(codigo_);
            if (ordinal(salida_) <= ordinal(entrada_)) {
                throw std::invalid_argument("Fechas invalidas");
            }
        } catch (const std::invalid_argument &e) {
            throw ErrorDatosHotel(e.what());
        }
        verificarInvariante();
    }
    void verificarInvariante() const {
        assert(!codigo_.empty() && total_ > 0);
        assert(ordinal(salida_) > ordinal(entrada_));
    }
    const std::string &codigo() const noexcept {
        return codigo_;
    }
    const std::string &entrada() const noexcept {
        return entrada_;
    }
    const std::string &salida() const noexcept {
        return salida_;
    }
    const std::shared_ptr<Huesped> &huesped() const noexcept {
        return huesped_;
    }
    const std::shared_ptr<Habitacion> &habitacion() const noexcept {
        return habitacion_;
    }
    double total() const noexcept {
        return total_;
    }
    int noches() const {
        return ordinal(salida_) - ordinal(entrada_);
    }
    EstadoReserva estado() const noexcept {
        return maquina_.estado();
    }
    const auto &historial() const noexcept {
        return maquina_.historial();
    }
    void confirmar() {
        transicionar(EstadoReserva::CONFIRMADA);
    }
    void ingresar() {
        transicionar(EstadoReserva::HOSPEDADA);
    }
    void finalizar() {
        transicionar(EstadoReserva::FINALIZADA);
    }
    void anular() {
        transicionar(EstadoReserva::ANULADA);
    }
};
using RepoHuespedes = Repositorio<std::shared_ptr<Huesped>, std::string>;
using RepoHabitaciones = Repositorio<std::shared_ptr<Habitacion>, int>;
using RepoReservas = Repositorio<std::shared_ptr<ReservaHotel>, std::string>;
class ServicioReservas {
    RepoHuespedes &huespedes_;
    RepoHabitaciones &habitaciones_;
    RepoReservas &reservas_;
    std::unique_ptr<CalculadoraTarifa> tarifa_;
    std::unique_ptr<INotificadorHotel> notificador_;
    bool solapadas(int numero, const std::string &entrada, const std::string &salida) const {
        return reservas_.contar([&](const auto &r) {
            return r->habitacion()->numero() == numero && r->estado() != EstadoReserva::ANULADA &&
                   entrada < r->salida() && r->entrada() < salida;
        }) > 0;
    }
    void notificar(const ReservaHotel &r, const std::string &mensaje) {
        try {
            if (notificador_->esDisponible()) {
                notificador_->enviar(r.huesped()->id(), mensaje);
            }
        } catch (const std::exception &e) {
            avisos.push_back(e.what());
        }
    }

  public:
    std::vector<std::string> avisos;
    ServicioReservas(RepoHuespedes &huespedes, RepoHabitaciones &habitaciones,
                     RepoReservas &reservas, std::unique_ptr<CalculadoraTarifa> tarifa,
                     std::unique_ptr<INotificadorHotel> notificador)
        : huespedes_(huespedes), habitaciones_(habitaciones), reservas_(reservas),
          tarifa_(std::move(tarifa)), notificador_(std::move(notificador)) {
        if (!tarifa_ || !notificador_) {
            throw ErrorDatosHotel("Dependencia nula");
        }
    }
    Result<std::shared_ptr<ReservaHotel>>
    crearReserva(const std::string &codigo, const std::string &huesped, int numero,
                 const std::string &entrada, const std::string &salida, bool aviso = false) {
        int noches = 0;
        try {
            textoValido(codigo);
            noches = ordinal(salida) - ordinal(entrada);
        } catch (const std::invalid_argument &e) {
            throw ErrorDatosHotel(e.what());
        }
        if (noches <= 0) {
            throw ErrorDatosHotel("Reserva invalida");
        }
        if (reservas_.existe(codigo)) {
            return Result<std::shared_ptr<ReservaHotel>>::fallo("Codigo de reserva duplicado");
        }
        auto h = huespedes_.buscar(huesped);
        auto hab = habitaciones_.buscar(numero);
        if (!h || !hab) {
            return Result<std::shared_ptr<ReservaHotel>>::fallo(
                "Huesped o habitacion no encontrado");
        }
        if (solapadas(numero, entrada, salida)) {
            return Result<std::shared_ptr<ReservaHotel>>::fallo("Habitacion sin disponibilidad");
        }
        auto r = std::make_shared<ReservaHotel>(codigo, *h, *hab, entrada, salida,
                                                tarifa_->calcular(**hab, noches));
        r->confirmar();
        reservas_.insertar(r);
        if (aviso) {
            notificar(*r, "Reserva confirmada");
        }
        return Result<std::shared_ptr<ReservaHotel>>::ok(r);
    }
    Result<std::shared_ptr<ReservaHotel>> anularReserva(const std::string &codigo,
                                                        bool aviso = false) {
        auto r = reservas_.buscarResultado(codigo);
        if (!r.esExito()) {
            return r;
        }
        r.valor()->anular();
        if (aviso) {
            notificar(*r.valor(), "Reserva anulada");
        }
        return r;
    }
    std::vector<std::shared_ptr<Habitacion>> disponibles(const std::string &entrada,
                                                         const std::string &salida) const {
        if (ordinal(salida) <= ordinal(entrada)) {
            throw ErrorDatosHotel("Fechas invalidas");
        }
        auto r = habitaciones_.filtrar(
            [&](const auto &h) { return !solapadas(h->numero(), entrada, salida); });
        std::sort(r.begin(), r.end(),
                  [](const auto &a, const auto &b) { return a->numero() < b->numero(); });
        return r;
    }
};
struct SistemaHotel {
    RepoHuespedes huespedes;
    RepoHabitaciones habitaciones;
    RepoReservas reservas;
    ServicioReservas servicio;
    SistemaHotel()
        : huespedes([](const auto &h) { return h->id(); }),
          habitaciones([](const auto &h) { return h->numero(); }),
          reservas([](const auto &r) { return r->codigo(); }),
          servicio(huespedes, habitaciones, reservas, std::make_unique<TarifaEstandar>(),
                   std::make_unique<NotificadorSimulado>()) {
        huespedes.guardar(std::make_shared<Huesped>("H1", "Ana", "12345678"));
        habitaciones.guardar(std::make_shared<HabitacionSimple>(101));
        habitaciones.guardar(std::make_shared<HabitacionDoble>(102));
        habitaciones.guardar(std::make_shared<Suite>(103));
    }
    SistemaHotel(const SistemaHotel &) = delete;
    SistemaHotel &operator=(const SistemaHotel &) = delete;
    SistemaHotel(SistemaHotel &&) = delete;
    SistemaHotel &operator=(SistemaHotel &&) = delete;
};
