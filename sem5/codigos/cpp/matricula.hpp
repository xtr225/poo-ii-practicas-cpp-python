#pragma once
#include "errores_academicos.hpp"
#include <set>
class Matricula {
    std::string codigo_, fecha_;
    std::vector<std::string> cursos_;
    bool anulada_ = false;
    void puedeModificar() const {
        if (anulada_) {
            throw ErrorAcademico("Matricula anulada");
        }
    }
    void verificarInvariante() const {
        assert(!codigo_.empty());
        assert(cursos_.size() >= 1 && cursos_.size() <= 6);
        assert(std::set<std::string>(cursos_.begin(), cursos_.end()).size() == cursos_.size());
        assert(ordinal(fecha_) <= ordinal(hoyISO()));
    }

  public:
    Matricula(std::string codigo, std::vector<std::string> cursos, std::string fecha = hoyISO())
        : codigo_(std::move(codigo)), fecha_(std::move(fecha)), cursos_(std::move(cursos)) {
        if (codigo_.find_first_not_of(" \t\r\n") == std::string::npos) {
            throw ErrorValidacion("Codigo vacio");
        }
        if (cursos_.empty() || cursos_.size() > 6) {
            throw ErrorCapacidad(static_cast<int>(cursos_.size()), 6);
        }
        if (std::set<std::string>(cursos_.begin(), cursos_.end()).size() != cursos_.size()) {
            throw ErrorValidacion("Duplicados");
        }
        for (const auto &c : cursos_) {
            if (c.find_first_not_of(" \t\r\n") == std::string::npos) {
                throw ErrorValidacion("Curso vacio");
            }
        }
        if (ordinal(fecha_) > ordinal(hoyISO())) {
            throw ErrorValidacion("Fecha futura");
        }
        verificarInvariante();
    }
    void agregarCurso(const std::string &curso) {
        puedeModificar();
        if (curso.find_first_not_of(" \t\r\n") == std::string::npos ||
            std::find(cursos_.begin(), cursos_.end(), curso) != cursos_.end()) {
            throw ErrorValidacion("Curso vacio o repetido");
        }
        if (cursos_.size() >= 6) {
            throw ErrorCapacidad(static_cast<int>(cursos_.size()), 6);
        }
        cursos_.push_back(curso);
        assert(cursos_.back() == curso);
        verificarInvariante();
    }
    void eliminarCurso(const std::string &curso) {
        puedeModificar();
        auto it = std::find(cursos_.begin(), cursos_.end(), curso);
        if (it == cursos_.end()) {
            throw ErrorRecursoNoEncontrado("Curso inexistente");
        }
        if (cursos_.size() == 1) {
            throw ErrorValidacion("Debe quedar al menos un curso");
        }
        cursos_.erase(it);
        assert(std::find(cursos_.begin(), cursos_.end(), curso) == cursos_.end());
        verificarInvariante();
    }
    void anular() {
        puedeModificar();
        anulada_ = true;
        verificarInvariante();
    }
    int numCursos() const noexcept {
        return static_cast<int>(cursos_.size());
    }
    bool anulada() const noexcept {
        return anulada_;
    }
    const auto &cursos() const noexcept {
        return cursos_;
    }
    std::string diagnostico() const {
        return "Matricula OK " + std::to_string(cursos_.size()) + "/6";
    }
};
