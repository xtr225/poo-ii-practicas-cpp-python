#pragma once
#include "infraestructura.hpp"

inline std::string cadenaJSON(const std::string& valor) {
    std::ostringstream salida;
    salida << '"';
    for (unsigned char caracter : valor) {
        if (caracter == '"') salida << "\\\"";
        else if (caracter == '\\') salida << "\\\\";
        else if (caracter < 32)
            salida << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                   << static_cast<unsigned int>(caracter) << std::dec;
        else salida << static_cast<char>(caracter);
    }
    salida << '"';
    return salida.str();
}

class GeneradorJSON : public GeneradorReporte {
public:
    std::string generar(const std::vector<Estudiante>& estudiantes) const override {
        std::ostringstream salida;
        salida << '[';
        bool primera = true;
        for (const auto& estudiante : estudiantes) {
            if (!primera) salida << ',';
            primera = false;
            salida << "{\"codigo\":" << cadenaJSON(estudiante.getCodigo())
                   << ",\"nombre\":" << cadenaJSON(estudiante.getNombre())
                   << ",\"promedio\":" << std::setprecision(17) << estudiante.calcularPPA()
                   << ",\"estado\":"
                   << cadenaJSON(estudiante.estaAprobado() ? "APROBADO" : "DESAPROBADO") << '}';
        }
        salida << ']';
        return salida.str();
    }
};
