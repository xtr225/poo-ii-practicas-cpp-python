#include "dominio.hpp"
#include <iomanip>
#include <iostream>

int main() {
    Estudiante estudiante("Juan Carlos Mamani Quispe", 20210500, "Ing. Sistemas");
    estudiante.agregarNota("POO II", 16);
    estudiante.agregarNota("Base de Datos I", 14);
    std::cout << estudiante.descripcion() << '\n';
    std::cout << "PPA: " << std::fixed << std::setprecision(2)
              << estudiante.calcularPPA() << '\n';
    try {
        estudiante.agregarNota("Redes", 25);
    } catch (const std::invalid_argument& error) {
        std::cout << "Error capturado: " << error.what() << '\n';
    }
}
