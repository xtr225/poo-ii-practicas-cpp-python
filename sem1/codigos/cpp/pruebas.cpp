#include "servicio.hpp"
#include "reportes_json.hpp"
#include <cstdio>
#include <functional>
#include <iostream>
#include <limits>

void exigir(bool condicion) {
    if (!condicion) throw std::runtime_error("Resultado distinto al esperado");
}

template <typename Accion>
void rechaza(Accion accion) {
    bool rechazado = false;
    try { accion(); } catch (const std::invalid_argument&) { rechazado = true; }
    exigir(rechazado);
}

int main() {
    using Caso = std::pair<std::string, std::function<void()>>;
    std::vector<Caso> casos = {
        {"promedio sin notas", [] {
            Estudiante e("Ana", "E001"); exigir(e.calcularPPA() == 0);
        }},
        {"promedio 16 y 14", [] {
            Estudiante e("Ana", "E001"); e.agregarNota("A", 16); e.agregarNota("B", 14);
            exigir(e.calcularPPA() == 15);
        }},
        {"tres notas", [] {
            Estudiante e("Ana", "E001");
            for (double n : {12, 14, 16}) e.agregarNota("Curso", n);
            exigir(e.calcularPPA() == 14);
        }},
        {"extremos 0 y 20", [] {
            Estudiante e("Ana", "E001"); e.agregarNota("A", 0); e.agregarNota("B", 20);
            exigir(e.calcularPPA() == 10);
        }},
        {"limite 10.5", [] {
            Estudiante e("Ana", "E001"); e.agregarNota("A", 10);
            exigir(!e.estaAprobado()); e.agregarNota("B", 11); exigir(e.estaAprobado());
        }},
        {"nota negativa", [] {
            Estudiante e("Ana", "E001"); rechaza([&] { e.agregarNota("A", -0.1); });
            exigir(e.getNotas().empty());
        }},
        {"nota superior", [] {
            Estudiante e("Ana", "E001"); rechaza([&] { e.agregarNota("A", 20.1); });
            exigir(e.getNotas().empty());
        }},
        {"nota no finita", [] {
            Estudiante e("Ana", "E001");
            rechaza([&] { e.agregarNota("A", std::numeric_limits<double>::quiet_NaN()); });
            rechaza([&] { e.agregarNota("A", std::numeric_limits<double>::infinity()); });
        }},
        {"nombre vacio", [] { rechaza([] { Estudiante e(" ", "E001"); }); }},
        {"codigo vacio", [] { rechaza([] { Estudiante e("Ana", ""); }); }},
        {"codigo entero invalido", [] { rechaza([] { Estudiante e("Ana", 0); }); }},
        {"escuela vacia", [] { rechaza([] { Estudiante e("Ana", "E001", ""); }); }},
        {"curso vacio", [] {
            Estudiante e("Ana", "E001"); rechaza([&] { e.agregarNota(" ", 16); });
        }},
        {"registrar y recuperar", [] {
            RepositorioEstudiantes repo; repo.guardar(Estudiante("Ana", "E001"));
            exigir(repo.buscar("E001").getNombre() == "Ana");
        }},
        {"codigo duplicado", [] {
            RepositorioEstudiantes repo; repo.guardar(Estudiante("Ana", "E001"));
            rechaza([&] { repo.guardar(Estudiante("Otra", "E001")); });
            exigir(repo.todos().size() == 1);
        }},
        {"codigo inexistente", [] {
            RepositorioEstudiantes repo; rechaza([&] { repo.buscar("NO"); });
        }},
        {"auditoria valida", [] {
            RepositorioEstudiantes repo; AuditoriaMemoria auditoria;
            ServicioAcademico servicio(repo, auditoria); servicio.registrar("E001", "Ana");
            servicio.agregarNota("E001", 16); exigir(auditoria.eventos().at(0) == "nota:E001:16");
        }},
        {"invalida sin auditoria", [] {
            RepositorioEstudiantes repo; AuditoriaMemoria auditoria;
            ServicioAcademico servicio(repo, auditoria); servicio.registrar("E001", "Ana");
            rechaza([&] { servicio.agregarNota("E001", 25); });
            exigir(auditoria.eventos().empty());
        }},
        {"reporte sin cambiar notas", [] {
            RepositorioEstudiantes repo; AuditoriaMemoria auditoria;
            ServicioAcademico servicio(repo, auditoria); servicio.registrar("E001", "Ana");
            servicio.agregarNota("E001", 10); servicio.agregarNota("E001", 11);
            exigir(servicio.reporte(GeneradorCSV()) == "E001;Ana;10.50;APROBADO");
            exigir(repo.buscar("E001").getNotas().size() == 2);
        }},
        {"CSV escapa separadores", [] {
            exigir(campoCSV("Ana; \"Luz\"") == "\"Ana; \"\"Luz\"\"\"");
        }},
        {"JSON vacio", [] {
            exigir(GeneradorJSON().generar({}) == "[]");
        }},
        {"JSON escapa caracteres", [] {
            exigir(cadenaJSON("Ana\n\"Luz\"") == "\"Ana\\u000a\\\"Luz\\\"\"");
        }},
        {"JSON no cambia estado", [] {
            RepositorioEstudiantes repo; AuditoriaMemoria auditoria;
            ServicioAcademico servicio(repo, auditoria); servicio.registrar("E001", "Ana");
            servicio.agregarNota("E001", 10); servicio.agregarNota("E001", 11);
            const auto csv = servicio.reporte(GeneradorCSV());
            const auto json = servicio.reporte(GeneradorJSON());
            exigir(json.find("\"promedio\":10.5") != std::string::npos);
            exigir(servicio.reporte(GeneradorCSV()) == csv);
            exigir(repo.buscar("E001").getNotas().size() == 2);
        }},
        {"auditoria a archivo", [] {
            const std::string ruta = "auditoria_prueba.txt";
            { std::ofstream limpiar(ruta, std::ios::trunc); }
            AuditoriaArchivo auditoria(ruta); auditoria.registrar("evento");
            std::ifstream archivo(ruta); std::string linea; std::getline(archivo, linea);
            archivo.close();
            std::remove(ruta.c_str());
            exigir(linea == "evento");
        }},
    };
    std::size_t aprobados = 0;
    for (const auto& caso : casos) {
        try { caso.second(); ++aprobados; std::cout << "OK " << caso.first << '\n'; }
        catch (const std::exception& error) {
            std::cerr << "FALLO " << caso.first << ": " << error.what() << '\n';
        }
    }
    std::cout << aprobados << '/' << casos.size() << " pruebas aprobadas\n";
    return aprobados == casos.size() ? 0 : 1;
}
