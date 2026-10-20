#pragma once
#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct Nota {
    std::string curso;
    double calificacion;
};

inline bool vacio(const std::string& valor) {
    return valor.find_first_not_of(" \t\r\n") == std::string::npos;
}

class Estudiante {
    std::string nombre_;
    std::string codigo_;
    std::string escuela_;
    std::vector<Nota> notas_;

public:
    Estudiante(std::string nombre, std::string codigo,
               std::string escuela = "Ing. Sistemas")
        : nombre_(std::move(nombre)), codigo_(std::move(codigo)),
          escuela_(std::move(escuela)) {
        if (vacio(nombre_)) throw std::invalid_argument("El nombre no puede estar vacio");
        if (vacio(codigo_)) throw std::invalid_argument("El codigo no puede estar vacio");
        if (vacio(escuela_)) throw std::invalid_argument("La escuela no puede estar vacia");
    }

    Estudiante(std::string nombre, int codigo, std::string escuela = "Ing. Sistemas")
        : Estudiante(std::move(nombre), std::to_string(codigo), std::move(escuela)) {
        if (codigo <= 0) throw std::invalid_argument("El codigo debe ser positivo");
    }

    const std::string& getNombre() const { return nombre_; }
    const std::string& getCodigo() const { return codigo_; }
    const std::string& getEscuela() const { return escuela_; }
    const std::vector<Nota>& getNotas() const { return notas_; }

    void agregarNota(const std::string& curso, double calificacion) {
        if (vacio(curso)) throw std::invalid_argument("El curso no puede estar vacio");
        if (!std::isfinite(calificacion) || calificacion < 0 || calificacion > 20)
            throw std::invalid_argument("La nota debe estar entre 0 y 20");
        notas_.push_back({curso, calificacion});
    }

    double calcularPPA() const {
        if (notas_.empty()) return 0.0;
        double suma = 0.0;
        for (const auto& nota : notas_) suma += nota.calificacion;
        return suma / static_cast<double>(notas_.size());
    }

    bool estaAprobado() const { return calcularPPA() >= 10.5; }

    std::string descripcion() const {
        return "[" + codigo_ + "] " + nombre_ + " (" + escuela_ + ")";
    }
};
