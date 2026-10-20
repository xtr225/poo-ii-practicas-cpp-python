#pragma once
#include "comun.hpp"
class ErrorAcademico : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
    virtual ~ErrorAcademico() = default;
};
class ErrorValidacion : public ErrorAcademico {
  public:
    using ErrorAcademico::ErrorAcademico;
};
class ErrorCalificacion : public ErrorValidacion {
    double valor_;

  public:
    explicit ErrorCalificacion(double valor)
        : ErrorValidacion("Calificacion fuera de rango"), valor_(valor) {}
    double valor() const noexcept {
        return valor_;
    }
};
class ErrorCapacidad : public ErrorAcademico {
    int actual_, maximo_;

  public:
    ErrorCapacidad(int actual, int maximo)
        : ErrorAcademico("Capacidad invalida"), actual_(actual), maximo_(maximo) {}
    int actual() const noexcept {
        return actual_;
    }
    int maximo() const noexcept {
        return maximo_;
    }
};
class ErrorRecursoNoEncontrado : public ErrorAcademico {
  public:
    using ErrorAcademico::ErrorAcademico;
};
class ErrorMatriculaDuplicada : public ErrorValidacion {
    std::string codigo_;

  public:
    explicit ErrorMatriculaDuplicada(std::string codigo)
        : ErrorValidacion("Matricula duplicada"), codigo_(std::move(codigo)) {}
    const std::string &codigoEstudiante() const noexcept {
        return codigo_;
    }
};
inline void validarMatricula(int codigo, std::vector<int> &matriculados) {
    if (std::find(matriculados.begin(), matriculados.end(), codigo) != matriculados.end()) {
        throw ErrorMatriculaDuplicada(std::to_string(codigo));
    }
}
