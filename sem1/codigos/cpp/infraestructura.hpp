#pragma once
#include "dominio.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>

class RepositorioEstudiantes {
    std::vector<Estudiante> estudiantes_;

public:
    void guardar(Estudiante estudiante) {
        for (const auto& actual : estudiantes_)
            if (actual.getCodigo() == estudiante.getCodigo())
                throw std::invalid_argument("El codigo ya esta registrado");
        estudiantes_.push_back(std::move(estudiante));
    }

    Estudiante& buscar(const std::string& codigo) {
        for (auto& estudiante : estudiantes_)
            if (estudiante.getCodigo() == codigo) return estudiante;
        throw std::invalid_argument("Estudiante no encontrado");
    }

    const std::vector<Estudiante>& todos() const { return estudiantes_; }
};

class Auditoria {
public:
    virtual ~Auditoria() = default;
    virtual void registrar(const std::string& evento) = 0;
};

class AuditoriaMemoria : public Auditoria {
    std::vector<std::string> eventos_;

public:
    void registrar(const std::string& evento) override { eventos_.push_back(evento); }
    const std::vector<std::string>& eventos() const { return eventos_; }
};

class AuditoriaArchivo : public Auditoria {
    std::string ruta_;

public:
    explicit AuditoriaArchivo(std::string ruta) : ruta_(std::move(ruta)) {}
    void registrar(const std::string& evento) override {
        std::ofstream archivo(ruta_, std::ios::app);
        if (!archivo) throw std::runtime_error("No se pudo abrir la auditoria");
        archivo << evento << '\n';
        if (!archivo) throw std::runtime_error("No se pudo escribir la auditoria");
    }
};

class GeneradorReporte {
public:
    virtual ~GeneradorReporte() = default;
    virtual std::string generar(const std::vector<Estudiante>& estudiantes) const = 0;
};

inline std::string campoCSV(const std::string& valor) {
    if (valor.find_first_of(";\"\n\r") == std::string::npos) return valor;
    std::string salida = "\"";
    for (char letra : valor) {
        if (letra == '"') salida += '"';
        salida += letra;
    }
    return salida + '"';
}

class GeneradorCSV : public GeneradorReporte {
public:
    std::string generar(const std::vector<Estudiante>& estudiantes) const override {
        std::ostringstream salida;
        bool primera = true;
        for (const auto& estudiante : estudiantes) {
            if (!primera) salida << '\n';
            primera = false;
            salida << campoCSV(estudiante.getCodigo()) << ';'
                   << campoCSV(estudiante.getNombre()) << ';'
                   << std::fixed << std::setprecision(2) << estudiante.calcularPPA() << ';'
                   << (estudiante.estaAprobado() ? "APROBADO" : "DESAPROBADO");
        }
        return salida.str();
    }
};
