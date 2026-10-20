#pragma once
#include "comun.hpp"

class Curso {
    std::string codigo_, nombre_;
    int creditos_ = 1, cuposMaximo_ = 1, inscritos_ = 0;

  public:
    Curso(const std::string &codigo, std::string nombre, int creditos, int cupos)
        : nombre_(std::move(nombre)) {
        textoValido(nombre_);
        setCodigo(codigo);
        setCreditos(creditos);
        setCuposMaximo(cupos);
    }
    const std::string &getCodigo() const noexcept {
        return codigo_;
    }
    const std::string &getNombre() const noexcept {
        return nombre_;
    }
    int getCreditos() const noexcept {
        return creditos_;
    }
    int getInscritos() const noexcept {
        return inscritos_;
    }
    int getCuposMaximo() const noexcept {
        return cuposMaximo_;
    }
    int cuposDisponibles() const noexcept {
        return cuposMaximo_ - inscritos_;
    }
    bool estaLleno() const noexcept {
        return cuposDisponibles() == 0;
    }
    void setCodigo(const std::string &codigo) {
        if (!std::regex_match(codigo, std::regex("[A-Z]{2,4}[0-9]{3}"))) {
            throw std::invalid_argument("Codigo invalido");
        }
        codigo_ = codigo;
    }
    void setCreditos(int valor) {
        if (valor < 1 || valor > 6) {
            throw std::invalid_argument("Creditos fuera de rango");
        }
        creditos_ = valor;
    }
    void setCuposMaximo(int valor) {
        if (valor < 1 || valor > 50 || valor < inscritos_) {
            throw std::invalid_argument("Cupos invalidos");
        }
        cuposMaximo_ = valor;
    }
    void inscribirEstudiante() {
        if (estaLleno()) {
            throw std::runtime_error("Curso sin cupos");
        }
        ++inscritos_;
    }
};

class Docente {
    std::string dni_, nombres_, apellidos_, correo_, nacimiento_;
    std::vector<std::shared_ptr<Curso>> cursos_;

  public:
    Docente(std::string dni, std::string nombres, std::string apellidos, std::string correo,
            std::string nacimiento)
        : dni_(std::move(dni)), nombres_(std::move(nombres)), apellidos_(std::move(apellidos)),
          correo_(std::move(correo)), nacimiento_(std::move(nacimiento)) {
        if (!std::regex_match(dni_, std::regex("[0-9]{8}"))) {
            throw std::invalid_argument("DNI invalido");
        }
        if (!std::regex_match(correo_, std::regex("[^@\\s]+@unap\\.edu\\.pe"))) {
            throw std::invalid_argument("Correo invalido");
        }
        textoValido(nombres_);
        textoValido(apellidos_);
        if (ordinal(nacimiento_) > ordinal(hoyISO())) {
            throw std::invalid_argument("Nacimiento futuro");
        }
    }
    std::string nombreCompleto() const {
        return apellidos_ + ", " + nombres_;
    }
    int edadEn(const std::string &hoy) const {
        if (ordinal(hoy) < ordinal(nacimiento_)) {
            throw std::invalid_argument("Referencia invalida");
        }
        return std::stoi(hoy.substr(0, 4)) - std::stoi(nacimiento_.substr(0, 4)) -
               (hoy.substr(5) < nacimiento_.substr(5));
    }
    int edad() const {
        return edadEn(hoyISO());
    }
    int totalCreditos() const {
        int total = 0;
        for (const auto &curso : cursos_) {
            total += curso->getCreditos();
        }
        return total;
    }
    int totalInscritos() const {
        int total = 0;
        for (const auto &curso : cursos_) {
            total += curso->getInscritos();
        }
        return total;
    }
    void asignarCurso(std::shared_ptr<Curso> curso) {
        if (!curso) {
            throw std::invalid_argument("Curso nulo");
        }
        for (const auto &actual : cursos_) {
            if (actual->getCodigo() == curso->getCodigo()) {
                throw std::invalid_argument("Curso duplicado");
            }
        }
        if (cursos_.size() >= 3) {
            throw std::runtime_error("Maximo tres cursos");
        }
        cursos_.push_back(std::move(curso));
    }
};

inline void demo() {
    auto curso = std::make_shared<Curso>("SIS206", "POO II", 4, 30);
    for (int i = 0; i < 3; ++i) {
        curso->inscribirEstudiante();
    }
    Docente docente("12345678", "Ana", "Mamani", "ana@unap.edu.pe", "1980-05-14");
    docente.asignarCurso(curso);
    std::cout << "CURSO=" << curso->getCodigo() << "\nLIBRES=" << curso->cuposDisponibles()
              << "\nCREDITOS=" << docente.totalCreditos()
              << "\nINSCRITOS=" << docente.totalInscritos()
              << "\nEDAD_REFERENCIA=" << docente.edadEn("2026-10-18") << '\n';
}
