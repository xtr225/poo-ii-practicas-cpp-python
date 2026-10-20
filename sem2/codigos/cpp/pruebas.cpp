#include "hotel_base.hpp"
#include "solucion.hpp"
int main() {
    Pruebas t;
    t.caso("curso valido", [] {
        const Curso c("SIS206", "POO", 4, 30);
        exigir(c.getCreditos() == 4);
    });
    t.caso("codigo", [] { debeFallar<std::invalid_argument>([] { Curso c("X9", "X", 4, 30); }); });
    t.caso("creditos",
           [] { debeFallar<std::invalid_argument>([] { Curso c("SIS206", "X", 0, 30); }); });
    t.caso("cupos",
           [] { debeFallar<std::invalid_argument>([] { Curso c("SIS206", "X", 4, 51); }); });
    t.caso("lleno", [] {
        Curso c("SIS206", "X", 4, 1);
        c.inscribirEstudiante();
        debeFallar([&] { c.inscribirEstudiante(); });
        exigir(c.estaLleno());
    });
    t.caso("reducir sin romper", [] {
        Curso c("SIS206", "X", 4, 3);
        c.inscribirEstudiante();
        c.inscribirEstudiante();
        debeFallar([&] { c.setCuposMaximo(1); });
        exigir(c.getCuposMaximo() == 3);
    });
    t.caso("dni",
           [] { debeFallar([] { Docente d("123", "A", "B", "a@unap.edu.pe", "1980-05-14"); }); });
    t.caso("correo", [] {
        debeFallar([] { Docente d("12345678", "A", "B", "a@unap.edu.pe.evil", "1980-05-14"); });
    });
    t.caso("edad", [] {
        Docente d("12345678", "A", "B", "a@unap.edu.pe", "1980-05-14");
        exigir(d.edadEn("2026-05-13") == 45 && d.edadEn("2026-05-14") == 46);
    });
    t.caso("datos derivados", [] {
        Docente d("12345678", "A", "B", "a@unap.edu.pe", "1980-05-14");
        auto c = std::make_shared<Curso>("SIS206", "X", 4, 30);
        d.asignarCurso(c);
        c->setCreditos(6);
        c->inscribirEstudiante();
        exigir(d.totalCreditos() == 6 && d.totalInscritos() == 1);
    });
    t.caso("limite docente", [] {
        Docente d("12345678", "A", "B", "a@unap.edu.pe", "1980-05-14");
        for (auto s : {"SIS206", "SIS207", "SIS208"}) {
            d.asignarCurso(std::make_shared<Curso>(s, "X", 4, 30));
        }
        debeFallar([&] { d.asignarCurso(std::make_shared<Curso>("SIS209", "X", 4, 30)); });
    });
    t.caso("huesped", [] {
        Huesped h("H1", "Ana", "12345678");
        debeFallar([&] { h.setDocumento("12"); });
        exigir(h.documento() == "12345678");
    });
    t.caso("habitacion", [] {
        Habitacion h(101, "Doble", 2, 100);
        h.alojar(2);
        debeFallar([&] { h.setCapacidad(1); });
        exigir(h.plazasLibres() == 0);
    });
    t.caso("tarifa no finita",
           [] { debeFallar([] { Habitacion h(101, "Doble", 2, std::nan("")); }); });
    t.caso("reserva", [] {
        Reserva r("R1", "2026-10-18", "2026-10-20");
        exigir(r.noches() == 2);
        r.confirmar();
        r.anular();
        debeFallar([&] { r.confirmar(); });
    });
    t.caso("fecha inexistente",
           [] { debeFallar([] { Reserva r("R1", "2026-02-28", "2026-02-30"); }); });
    return t.terminar();
}
