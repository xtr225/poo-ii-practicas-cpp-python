#pragma once
#include "infraestructura.hpp"

class ServicioAcademico {
    RepositorioEstudiantes& repositorio_;
    Auditoria& auditoria_;

public:
    ServicioAcademico(RepositorioEstudiantes& repositorio, Auditoria& auditoria)
        : repositorio_(repositorio), auditoria_(auditoria) {}

    void registrar(const std::string& codigo, const std::string& nombre) {
        repositorio_.guardar(Estudiante(nombre, codigo));
    }

    void agregarNota(const std::string& codigo, double nota,
                     const std::string& curso = "General") {
        repositorio_.buscar(codigo).agregarNota(curso, nota);
        std::ostringstream evento;
        evento << "nota:" << codigo << ':' << nota;
        auditoria_.registrar(evento.str());
    }

    std::string reporte(const GeneradorReporte& generador) const {
        return generador.generar(repositorio_.todos());
    }
};
