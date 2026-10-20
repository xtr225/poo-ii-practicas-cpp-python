// Parte II, página 11: se agregan los encabezados necesarios al fragmento.
#include <fstream>
#include <string>
#include <vector>

struct Estudiante {
    std::string codigo;
    std::string nombre;
    std::vector<double> notas;
};

class GestorAcademico {
public:
    std::vector<Estudiante> estudiantes;

    void agregarNota(const std::string& codigo, double nota) {
        for (auto& e : estudiantes) {
            if (e.codigo == codigo) {
                e.notas.push_back(nota);
                std::ofstream log("auditoria.txt", std::ios::app);
                log << codigo << ";" << nota << "\n";
            }
        }
    }
};
