#include "gestor_academico.cpp"
#include <iostream>

int main() {
    GestorAcademico gestor;
    gestor.estudiantes.push_back({"E001", "Ana", {}});
    gestor.agregarNota("E001", 10);
    gestor.agregarNota("E001", 11);
    const auto& notas = gestor.estudiantes.front().notas;
    std::cout << "Promedio inicial: " << (notas[0] + notas[1]) / 2.0 << '\n';
    gestor.agregarNota("E001", 25);
    std::cout << "Defecto observado: acepta " << gestor.estudiantes.front().notas.back() << '\n';
}
