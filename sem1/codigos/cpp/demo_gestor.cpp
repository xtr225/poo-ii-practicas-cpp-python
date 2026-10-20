#include "servicio.hpp"
#include "reportes_json.hpp"
#include <iostream>

int main() {
    RepositorioEstudiantes repositorio;
    AuditoriaMemoria auditoria;
    ServicioAcademico servicio(repositorio, auditoria);
    servicio.registrar("E001", "Ana");
    servicio.agregarNota("E001", 10);
    servicio.agregarNota("E001", 11);
    std::cout << servicio.reporte(GeneradorCSV()) << '\n';
    std::cout << servicio.reporte(GeneradorJSON()) << '\n';
}
