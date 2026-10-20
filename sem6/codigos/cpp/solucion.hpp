#pragma once
#include "hotel_herencia.hpp"
#include "repositorio.hpp"
struct Estudiante {
    int codigo;
    std::string nombre;
    double ppa;
    std::string escuela;
};
inline Repositorio<Estudiante, int> crearRepositorio() {
    Repositorio<Estudiante, int> r([](const Estudiante &e) { return e.codigo; });
    for (const auto &e : std::vector<Estudiante>{{1001, "Juan", 15.2, "Ing. Sistemas"},
                                                 {1002, "Maria", 18.5, "Ing. Sistemas"},
                                                 {1003, "Pedro", 9.8, "Ciencias Comp."},
                                                 {1004, "Ana", 13, "Ing. Sistemas"}}) {
        r.guardar(e);
    }
    return r;
}
inline Repositorio<std::shared_ptr<Habitacion>, int> crearHotel() {
    Repositorio<std::shared_ptr<Habitacion>, int> r([](const auto &h) { return h->numero(); });
    r.guardar(std::make_shared<HabitacionSimple>(101));
    r.guardar(std::make_shared<HabitacionDoble>(102));
    r.guardar(std::make_shared<Suite>(103));
    r.guardar(std::make_shared<HabitacionSimple>(104, 120));
    return r;
}
inline void demo() {
    auto r = crearRepositorio();
    auto h = crearHotel();
    auto ranking = r.ordenar([](const auto &a, const auto &b) { return a.ppa > b.ppa; });
    std::cout << "DESTACADOS=" << r.contar([](const auto &e) {
        return e.ppa >= 14 && e.escuela == "Ing. Sistemas";
    }) << "\nRANKING=";
    for (std::size_t i = 0; i < ranking.size(); ++i) {
        std::cout << (i ? "," : "") << ranking[i].codigo;
    }
    std::cout << "\nPROMEDIO=" << std::fixed << std::setprecision(3)
              << r.acumular(0.0, [](double s, const auto &e) { return s + e.ppa; }) / r.tamano();
    std::cout << "\nHABITACIONES_DOS_PLAZAS=" << h.contar([](const auto &e) {
        return e->plazasLibres() >= 2;
    }) << "\nRANKING_HOTEL=";
    auto hs =
        h.ordenar([](const auto &a, const auto &b) { return a->precioNoche() > b->precioNoche(); });
    for (std::size_t i = 0; i < hs.size(); ++i) {
        std::cout << (i ? "," : "") << hs[i]->numero();
    }
    std::cout << "\nPROMEDIO_HOTEL=" << std::setprecision(2)
              << h.acumular(0.0, [](double s, const auto &e) { return s + e->precioNoche(); }) /
                     h.tamano();
    ColaAtencion<std::string> c;
    c.encolar("Ana");
    c.encolar("Juan");
    std::cout << "\nFIFO=" << *c.atender()
              << "\nVENTANAS=" << ventanas(std::vector<int>{12, 15, 18, 14, 16}, 3).size() << '\n';
}
