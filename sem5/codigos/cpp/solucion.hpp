#pragma once
#include "matricula.hpp"
#include "result.hpp"
struct EstudiantePipeline {
    std::string nombre;
    double ppa;
};
inline Result<EstudiantePipeline> buscarEstudiante(int codigo) {
    return codigo == 1001 ? Result<EstudiantePipeline>::ok({"Juan Mamani", 15})
                          : Result<EstudiantePipeline>::fallo("Estudiante no encontrado");
}
inline Result<EstudiantePipeline> validarPpaMinimo(const EstudiantePipeline &est) {
    return est.ppa >= 14 ? Result<EstudiantePipeline>::ok(est)
                         : Result<EstudiantePipeline>::fallo("PPA insuficiente");
}
inline Result<std::string> inscribir(const EstudiantePipeline &est, const std::string &curso) {
    return Result<std::string>::ok("Inscrito: " + est.nombre + " en " + curso);
}
inline Result<std::string> pipeline(int codigo) {
    return buscarEstudiante(codigo).map(validarPpaMinimo).map([](const EstudiantePipeline &e) {
        return inscribir(e, "SIS206");
    });
}
inline void demo() {
    Matricula m("20210500", {"SIS206"});
    m.agregarCurso("SIS207");
    m.eliminarCurso("SIS207");
    std::cout << "CURSOS=" << m.numCursos() << "\nEXITO=" << pipeline(1001).valor()
              << "\nFALLO=" << pipeline(9999).error() << '\n';
}
