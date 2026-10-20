#include "solucion.hpp"
int main() {
    Pruebas t;
    t.caso("guardar buscar", [] {
        auto r = crearRepositorio();
        exigir(r.buscar(1001)->nombre == "Juan");
    });
    t.caso("guardar actualiza", [] {
        auto r = crearRepositorio();
        r.guardar({1001, "Otro", 16, "Sistemas"});
        exigir(r.tamano() == 4 && r.buscar(1001)->nombre == "Otro");
    });
    t.caso("insertar duplicado", [] {
        auto r = crearRepositorio();
        debeFallar([&] { r.insertar({1001, "Otro", 16, "Sistemas"}); });
    });
    t.caso("eliminar cero", [] {
        Repositorio<int, int> r([](int n) { return n; });
        r.guardar(0);
        exigir(r.eliminar(0) && !r.eliminar(0));
    });
    t.caso("busqueda ausente", [] { exigir(!crearRepositorio().buscarResultado(9999).esExito()); });
    t.caso("filtrar contar", [] {
        auto r = crearRepositorio();
        exigir(r.contar(
                   [](const auto &e) { return e.ppa >= 14 && e.escuela == "Ing. Sistemas"; }) == 2);
    });
    t.caso("ordenar", [] {
        auto v =
            crearRepositorio().ordenar([](const auto &a, const auto &b) { return a.ppa > b.ppa; });
        exigir(v[0].codigo == 1002 && v[3].codigo == 1003);
    });
    t.caso("acumular", [] {
        auto r = crearRepositorio();
        exigir(std::abs(r.acumular(0.0, [](double a, const auto &e) { return a + e.ppa; }) /
                            r.tamano() -
                        14.125) < 0.0001);
    });
    t.caso("lista copia", [] {
        auto r = crearRepositorio();
        auto v = r.todos();
        v.clear();
        exigir(r.tamano() == 4);
    });
    t.caso("cola fifo", [] {
        ColaAtencion<int> c;
        c.encolar(0);
        c.encolar(1);
        exigir(*c.atender() == 0 && *c.atender() == 1 && !c.atender());
    });
    t.caso("iterador copia independiente", [] {
        PlanillaIterador<int> p({1, 3, 2}, std::greater<int>());
        auto copia = p;
        p = PlanillaIterador<int>({9}, std::greater<int>());
        exigir(std::vector<int>(copia.begin(), copia.end()) == std::vector<int>({3, 2, 1}));
    });
    t.caso("ventanas", [] {
        exigir(ventanas(std::vector<int>{12, 15, 18, 14, 16}, 3) ==
               std::vector<std::vector<int>>({{12, 15, 18}, {15, 18, 14}, {18, 14, 16}}));
    });
    t.caso("ventanas invalidas", [] {
        for (int k : {-1, 0, 4}) {
            debeFallar([k] { ventanas(std::vector<int>{1, 2, 3}, k); });
        }
    });
    t.caso("hotel", [] {
        auto r = crearHotel();
        exigir(r.contar([](const auto &h) { return h->plazasLibres() >= 2; }) == 2);
    });
    return t.terminar();
}
