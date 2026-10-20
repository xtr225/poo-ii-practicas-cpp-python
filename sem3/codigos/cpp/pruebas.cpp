#include "hotel_herencia.hpp"
#include "solucion.hpp"
#include <type_traits>
int main() {
    Pruebas t;
    t.caso("base abstracta y destructor virtual", [] {
        exigir(std::is_abstract<PersonalUniversitario>::value &&
               std::has_virtual_destructor<PersonalUniversitario>::value);
    });
    t.caso("remuneraciones", [] {
        auto p = datos();
        double esperado[] = {1680, 1620, 3150, 3000};
        for (std::size_t i = 0; i < p.size(); ++i) {
            exigir(std::abs(p[i]->calcularRemuneracion() - esperado[i]) < 0.001);
        }
    });
    t.caso("contratos todos", [] {
        for (const auto &p : datos()) {
            exigir(verificarContrato(*p));
        }
    });
    t.caso("dni", [] { debeFallar([] { Docente d("12", "A", "B", 16, "Asociado"); }); });
    t.caso("horas", [] {
        for (int h : {0, 41}) {
            debeFallar([h] { Docente d("12345678", "A", "B", h, "Asociado"); });
        }
    });
    t.caso("categoria", [] { debeFallar([] { Docente d("12345678", "A", "B", 16, "Otro"); }); });
    t.caso("salario", [] { debeFallar([] { Administrativo a("12345678", "A", "B", 0); }); });
    t.caso("bono no debilita contrato",
           [] { debeFallar([] { Administrativo a("12345678", "A", "B", 2800, -5000); }); });
    t.caso("proyectos", [] { debeFallar([] { Investigador a("12345678", "A", "B", 0); }); });
    t.caso("rectangulo", [] {
        Rectangulo r(2, 2);
        exigir(comprobarRectangulo(r));
    });
    t.caso("violacion intencional detectada", [] {
        CuadradoIncompatible c(2);
        exigir(!comprobarRectangulo(c));
    });
    t.caso("alternativa figura", [] {
        Cuadrado c(5);
        exigir(c.area() == 25);
    });
    t.caso("hotel heterogeneo", [] {
        std::vector<std::unique_ptr<Habitacion>> hs;
        hs.push_back(std::make_unique<HabitacionSimple>(101));
        hs.push_back(std::make_unique<HabitacionDoble>(102));
        hs.push_back(std::make_unique<Suite>(103));
        double esperado[] = {100, 150, 200};
        for (std::size_t i = 0; i < hs.size(); ++i) {
            exigir(hs[i]->precioNoche() == esperado[i]);
        }
    });
    return t.terminar();
}
