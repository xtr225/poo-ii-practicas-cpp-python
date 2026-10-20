#pragma once
#include "comun.hpp"

class PersonalUniversitario {
    std::string dni_, nombres_, apellidos_;

  public:
    PersonalUniversitario(std::string dni, std::string nombres, std::string apellidos)
        : dni_(std::move(dni)), nombres_(std::move(nombres)), apellidos_(std::move(apellidos)) {
        if (!std::regex_match(dni_, std::regex("[0-9]{8}"))) {
            throw std::invalid_argument("DNI invalido");
        }
        textoValido(nombres_);
        textoValido(apellidos_);
    }
    virtual ~PersonalUniversitario() = default;
    virtual double calcularRemuneracion() const = 0;
    std::string nombreCompleto() const {
        return apellidos_ + ", " + nombres_;
    }
    std::string descripcion() const {
        std::ostringstream out;
        out << nombreCompleto() << ": S/ " << std::fixed << std::setprecision(2)
            << calcularRemuneracion();
        return out.str();
    }
};
class Docente : public PersonalUniversitario {
    int horas_;
    double factor_;
    static double factor(const std::string &categoria) {
        if (categoria == "Auxiliar") {
            return 1;
        }
        if (categoria == "Asociado") {
            return 1.4;
        }
        if (categoria == "Principal") {
            return 1.8;
        }
        throw std::invalid_argument("Categoria invalida");
    }

  public:
    Docente(std::string dni, std::string nombres, std::string apellidos, int horas,
            const std::string &categoria)
        : PersonalUniversitario(std::move(dni), std::move(nombres), std::move(apellidos)),
          horas_(horas), factor_(factor(categoria)) {
        if (horas < 1 || horas > 40) {
            throw std::invalid_argument("Horas invalidas");
        }
    }
    double calcularRemuneracion() const override {
        return horas_ * 75.0 * factor_;
    }
};
class Administrativo : public PersonalUniversitario {
    double salario_, bono_;

  public:
    Administrativo(std::string dni, std::string nombres, std::string apellidos, double salario,
                   double bono = 0)
        : PersonalUniversitario(std::move(dni), std::move(nombres), std::move(apellidos)),
          salario_(salario), bono_(bono) {
        positivo(salario_);
        if (!std::isfinite(bono_) || bono_ < 0) {
            throw std::invalid_argument("Bono invalido");
        }
    }
    double calcularRemuneracion() const override {
        return salario_ + bono_;
    }
};
class Investigador : public PersonalUniversitario {
    int proyectos_;

  public:
    Investigador(std::string dni, std::string nombres, std::string apellidos, int proyectos)
        : PersonalUniversitario(std::move(dni), std::move(nombres), std::move(apellidos)),
          proyectos_(proyectos) {
        if (proyectos < 1) {
            throw std::invalid_argument("Proyectos invalidos");
        }
    }
    double calcularRemuneracion() const override {
        return proyectos_ * 1500.0;
    }
};
inline bool verificarContrato(const PersonalUniversitario &p) {
    return !p.nombreCompleto().empty() && std::isfinite(p.calcularRemuneracion()) &&
           p.calcularRemuneracion() > 0 &&
           p.descripcion().find(p.nombreCompleto()) != std::string::npos;
}
class Figura {
  public:
    virtual ~Figura() = default;
    virtual double area() const = 0;
};
class Rectangulo : public Figura {
  protected:
    double ancho_, alto_;

  public:
    Rectangulo(double a, double b) : ancho_(a), alto_(b) {
        positivo(a);
        positivo(b);
    }
    virtual void setAncho(double v) {
        positivo(v);
        ancho_ = v;
    }
    virtual void setAlto(double v) {
        positivo(v);
        alto_ = v;
    }
    double area() const override {
        return ancho_ * alto_;
    }
};
class CuadradoIncompatible : public Rectangulo {
  public:
    explicit CuadradoIncompatible(double v) : Rectangulo(v, v) {}
    void setAncho(double v) override {
        positivo(v);
        ancho_ = alto_ = v;
    }
    void setAlto(double v) override {
        positivo(v);
        ancho_ = alto_ = v;
    }
};
class Cuadrado : public Figura {
    double lado_;

  public:
    explicit Cuadrado(double v) : lado_(v) {
        positivo(v);
    }
    double area() const override {
        return lado_ * lado_;
    }
};
inline bool comprobarRectangulo(Rectangulo &r) {
    r.setAncho(4);
    r.setAlto(5);
    return r.area() == 20;
}
inline std::vector<std::unique_ptr<PersonalUniversitario>> datos() {
    std::vector<std::unique_ptr<PersonalUniversitario>> p;
    p.push_back(
        std::make_unique<Docente>("12345678", "Juan Carlos", "Mamani Quispe", 16, "Asociado"));
    p.push_back(
        std::make_unique<Docente>("23456789", "Maria Elena", "Quispe Torres", 12, "Principal"));
    p.push_back(std::make_unique<Administrativo>("34567890", "Pedro", "Huanca Lazo", 2800, 350));
    p.push_back(std::make_unique<Investigador>("45678901", "Ana Lucia", "Flores Ramos", 2));
    return p;
}
inline double planilla(const std::vector<std::unique_ptr<PersonalUniversitario>> &personal) {
    double total = 0;
    for (const auto &p : personal) {
        std::cout << p->descripcion() << '\n';
        total += p->calcularRemuneracion();
    }
    std::cout << "TOTAL=" << std::fixed << std::setprecision(2) << total << '\n';
    return total;
}
inline void demo() {
    auto personal = datos();
    planilla(personal);
    int correctos = 0;
    for (const auto &p : personal) {
        if (verificarContrato(*p)) {
            ++correctos;
        }
    }
    std::cout << "CONTRATOS=" << correctos << '\n';
}
