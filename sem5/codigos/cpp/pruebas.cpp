#include "errores_hotel.hpp"
#include "solucion.hpp"
int main() {
    Pruebas t;
    t.caso("constructor valido", [] {
        Matricula m("E1", {"SIS206"});
        exigir(m.numCursos() == 1);
    });
    t.caso("sin cursos", [] { debeFallar<ErrorCapacidad>([] { Matricula m("E1", {}); }); });
    t.caso("maximo seis", [] {
        debeFallar<ErrorCapacidad>([] { Matricula m("E1", {"1", "2", "3", "4", "5", "6", "7"}); });
    });
    t.caso("duplicados",
           [] { debeFallar<ErrorValidacion>([] { Matricula m("E1", {"SIS206", "SIS206"}); }); });
    t.caso("fecha futura", [] {
        debeFallar<ErrorValidacion>([] { Matricula m("E1", {"SIS206"}, "9999-12-31"); });
    });
    t.caso("agregar y eliminar", [] {
        Matricula m("E1", {"SIS206"});
        m.agregarCurso("SIS207");
        m.eliminarCurso("SIS207");
        exigir(m.numCursos() == 1);
    });
    t.caso("ultimo curso", [] {
        Matricula m("E1", {"SIS206"});
        debeFallar<ErrorValidacion>([&] { m.eliminarCurso("SIS206"); });
        exigir(m.numCursos() == 1);
    });
    t.caso("anulada", [] {
        Matricula m("E1", {"SIS206"});
        m.anular();
        debeFallar<ErrorAcademico>([&] { m.agregarCurso("SIS207"); });
    });
    t.caso("diagnostico", [] {
        Matricula m("E1", {"SIS206"});
        exigir(m.diagnostico().find("OK") != std::string::npos);
    });
    t.caso("duplicada con contexto", [] {
        std::vector<int> ids = {1001};
        try {
            validarMatricula(1001, ids);
            exigir(false);
        } catch (const ErrorMatriculaDuplicada &e) {
            exigir(e.codigoEstudiante() == "1001");
        }
    });
    t.caso("captura especifica", [] {
        bool capturado = false;
        try {
            throw ErrorCalificacion(25);
        } catch (const ErrorCalificacion &e) {
            capturado = e.valor() == 25;
        } catch (const ErrorAcademico &) {
            exigir(false);
        }
        exigir(capturado);
    });
    t.caso("result string", [] {
        exigir(Result<std::string>::ok("valor").valor() == "valor");
        exigir(Result<std::string>::fallo("error").error() == "error");
    });
    t.caso("acceso invalido",
           [] { debeFallar<std::logic_error>([] { Result<int>::fallo("error").valor(); }); });
    t.caso("pipeline", [] { exigir(pipeline(1001).esExito() && !pipeline(9999).esExito()); });
    t.caso("cortocircuito", [] {
        bool llamado = false;
        auto r = Result<int>::fallo("error").map([&](int n) {
            llamado = true;
            return Result<int>::ok(n);
        });
        exigir(!r.esExito() && !llamado);
    });
    t.caso("ppa insuficiente", [] { exigir(!validarPpaMinimo({"Ana", 10}).esExito()); });
    t.caso("hotel valido habitacion", [] {
        HabitacionRobusta h(101, "Doble", 2, 100);
        h.alojar(1);
        h.verificarInvariante();
        exigir(h.plazasLibres() == 1);
    });
    t.caso("hotel valida reserva", [] {
        ReservaRobusta r("R1", "2026-10-18", "2026-10-20");
        r.confirmar();
        r.verificarInvariante();
        exigir(r.noches() == 2);
    });
    t.caso("hotel result", [] { exigir(!buscarHabitacion({}, 101).esExito()); });
    t.caso("hotel constructor invalido",
           [] { debeFallar<ErrorDatosHotel>([] { HabitacionRobusta h(101, "Doble", 0, 100); }); });
    t.caso("hotel sin plazas", [] {
        HabitacionRobusta h(101, "Doble", 2, 100);
        debeFallar<ErrorDisponibilidad>([&] { h.alojar(3); });
    });
    t.caso("hotel estado", [] {
        ReservaRobusta r("R1", "2026-10-18", "2026-10-20");
        r.anular();
        debeFallar<ErrorEstadoReserva>([&] { r.confirmar(); });
    });
    return t.terminar();
}
