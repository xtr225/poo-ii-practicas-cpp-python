#pragma once
#include "comun.hpp"
#include "hotel_herencia.hpp"
#include <fstream>

class INotificador {
  public:
    virtual ~INotificador() = default;
    virtual bool enviar(const std::string &, const std::string &) = 0;
    virtual bool esDisponible() const = 0;
    virtual std::string nombre() const = 0;
};
class CanalSimulado {
    std::string servidor_;
    bool activo_ = true;

  public:
    std::vector<std::pair<std::string, std::string>> enviados;
    explicit CanalSimulado(std::string servidor) : servidor_(std::move(servidor)) {
        textoValido(servidor_);
    }
    bool esDisponible() const {
        return activo_;
    }
    void setActivo(bool v) {
        activo_ = v;
    }
    bool enviar(const std::string &dest, const std::string &mensaje) {
        if (!esDisponible()) {
            return false;
        }
        textoValido(dest);
        textoValido(mensaje);
        enviados.push_back({dest, mensaje});
        return true;
    }
};
class NotificadorEmail : public INotificador {
    CanalSimulado canal_;

  public:
    explicit NotificadorEmail(std::string origen) : canal_(std::move(origen)) {}
    std::string nombre() const override {
        return "Email";
    }
    bool esDisponible() const override {
        return canal_.esDisponible();
    }
    void setActivo(bool v) {
        canal_.setActivo(v);
    }
    bool enviar(const std::string &d, const std::string &m) override {
        return canal_.enviar(d, m);
    }
};
inline std::size_t caracteresUTF8(const std::string &texto) {
    std::size_t total = 0;
    for (unsigned char c : texto) {
        if ((c & 0xc0) != 0x80) {
            ++total;
        }
    }
    return total;
}
class NotificadorSMS : public INotificador {
    CanalSimulado canal_;

  public:
    explicit NotificadorSMS(std::string origen) : canal_(std::move(origen)) {}
    bool esDisponible() const override {
        return canal_.esDisponible();
    }
    std::string nombre() const override {
        return "SMS";
    }
    bool enviar(const std::string &destino, const std::string &mensaje) override {
        if (caracteresUTF8(mensaje) > 160) {
            throw std::invalid_argument("SMS largo");
        }
        return canal_.enviar(destino, mensaje);
    }
};
class SistemaAlertas {
    std::vector<std::unique_ptr<INotificador>> notificadores_;

  public:
    std::vector<std::string> errores;
    explicit SistemaAlertas(std::vector<std::unique_ptr<INotificador>> ns)
        : notificadores_(std::move(ns)) {
        if (notificadores_.empty()) {
            throw std::invalid_argument("Se requiere un canal");
        }
        for (const auto &n : notificadores_) {
            if (!n) {
                throw std::invalid_argument("Canal nulo");
            }
        }
    }
    int alertar(const std::string &dest, const std::string &mensaje) {
        int total = 0;
        for (auto &n : notificadores_) {
            try {
                if (n->esDisponible() && n->enviar(dest, mensaje)) {
                    ++total;
                }
            } catch (const std::exception &e) {
                errores.push_back(e.what());
            }
        }
        return total;
    }
};
inline void escribir(const std::string &ruta, const std::string &contenido) {
    std::ofstream f(ruta);
    if (!f) {
        throw std::runtime_error("No se pudo abrir reporte");
    }
    f << contenido;
    if (!f) {
        throw std::runtime_error("No se pudo escribir reporte");
    }
}
class Exportador {
  public:
    virtual ~Exportador() = default;
    virtual std::string extension() const = 0;
    virtual void exportar(const std::string &contenido, const std::string &ruta) const = 0;
};
class ExportadorCSV : public Exportador {
  public:
    std::string extension() const override {
        return ".csv";
    }
    void exportar(const std::string &contenido, const std::string &ruta) const override {
        escribir(ruta + extension(), contenido);
    }
};
class ExportadorJSON : public Exportador {
  public:
    std::string extension() const override {
        return ".json";
    }
    void exportar(const std::string &contenido, const std::string &ruta) const override {
        escribir(ruta + extension(), contenido);
    }
};
class Firmador {
  public:
    virtual ~Firmador() = default;
    virtual std::string firmar(const std::string &ruta) const = 0;
};
class FirmadorDigital : public Firmador {
  public:
    std::string firmar(const std::string &ruta) const override {
        std::ifstream f(ruta);
        if (!f) {
            throw std::runtime_error("Reporte inexistente");
        }
        std::ostringstream contenido;
        contenido << f.rdbuf();
        escribir(ruta + ".signed", "FIRMA SIMULADA\n" + contenido.str());
        return ruta + ".signed";
    }
};
class GeneradorReporte {
    std::unique_ptr<Exportador> exportador_;
    std::unique_ptr<Firmador> firmador_;

  public:
    explicit GeneradorReporte(std::unique_ptr<Exportador> exp,
                              std::unique_ptr<Firmador> firma = nullptr)
        : exportador_(std::move(exp)), firmador_(std::move(firma)) {
        if (!exportador_) {
            throw std::invalid_argument("Exportador nulo");
        }
    }
    void cambiarExportador(std::unique_ptr<Exportador> exp) {
        if (!exp) {
            throw std::invalid_argument("Exportador nulo");
        }
        exportador_ = std::move(exp);
    }
    std::string generar(const std::string &contenido, const std::string &ruta) const {
        exportador_->exportar(contenido, ruta);
        auto salida = ruta + exportador_->extension();
        return firmador_ ? firmador_->firmar(salida) : salida;
    }
};
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
            throw std::invalid_argument("Noches invalidas");
        }
        return h.precioNoche() * noches;
    }
    std::string descripcion() const override {
        return "Estandar";
    }
};
class ServicioCotizacion {
    std::unique_ptr<CalculadoraTarifa> calculadora_;

  public:
    explicit ServicioCotizacion(std::unique_ptr<CalculadoraTarifa> c) : calculadora_(std::move(c)) {
        if (!calculadora_) {
            throw std::invalid_argument("Calculadora nula");
        }
    }
    double cotizar(const Habitacion &h, int noches) const {
        return calculadora_->calcular(h, noches);
    }
};
inline void demo() {
    std::vector<std::unique_ptr<INotificador>> ns;
    ns.push_back(std::make_unique<NotificadorEmail>("smtp.local"));
    ns.push_back(std::make_unique<NotificadorSMS>("Simulado"));
    SistemaAlertas alertas(std::move(ns));
    ServicioCotizacion servicio(std::make_unique<TarifaEstandar>());
    std::cout << "CANALES=" << alertas.alertar("destino_prueba", "Aviso de prueba")
              << "\nSMS_LARGO=" << alertas.alertar("destino_prueba", std::string(161, 'X'))
              << "\nCOTIZACION=" << std::fixed << std::setprecision(2)
              << servicio.cotizar(HabitacionDoble(102), 2) << '\n';
}
