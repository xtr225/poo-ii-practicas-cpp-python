#include "servicio.hpp"
#include <iostream>

int main() {
    for (const auto& notas : std::vector<std::vector<double>>{
             {}, {0, 20}, {12, 14, 16}, {10, 11}, {16, 14}, {10}}) {
        RepositorioEstudiantes repo;
        AuditoriaMemoria auditoria;
        ServicioAcademico servicio(repo, auditoria);
        servicio.registrar("E001", "Ana");
        for (double nota : notas) servicio.agregarNota("E001", nota);
        std::cout << servicio.reporte(GeneradorCSV()) << '\n';
    }
}
