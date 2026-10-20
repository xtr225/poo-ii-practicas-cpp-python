"""Matrícula por contrato y pipeline de fallos esperados."""

from datetime import date
from .excepciones import *
from .result import Result


class Matricula:
    MAX_CURSOS = 6

    def __init__(self, codigo, cursos, fecha=None):
        fecha = date.today() if fecha is None else fecha
        if not codigo.strip():
            raise ErrorValidacion("Codigo vacio")
        if not 1 <= len(cursos) <= 6:
            raise ErrorCapacidad(len(cursos), 6, "matricula")
        if len(set(cursos)) != len(cursos) or any(not c.strip() for c in cursos):
            raise ErrorValidacion("Cursos vacios o repetidos")
        if type(fecha) is not date or fecha > date.today():
            raise ErrorValidacion("Fecha invalida o futura")
        self._codigo_estudiante, self._cursos, self._fecha, self._anulada = (
            codigo,
            list(cursos),
            fecha,
            False,
        )
        self._verificar_invariante()

    @property
    def cursos(self):
        return tuple(self._cursos)

    @property
    def anulada(self):
        return self._anulada

    def _verificar_invariante(self):
        assert self._codigo_estudiante.strip()
        assert 1 <= len(self._cursos) <= 6 and len(set(self._cursos)) == len(self._cursos)
        assert all(c.strip() for c in self._cursos)
        assert self._fecha <= date.today()

    def _puede_modificar(self):
        if self._anulada:
            raise ErrorAcademico("Matricula anulada")

    def agregar_curso(self, curso):
        self._puede_modificar()
        if not curso.strip() or curso in self._cursos:
            raise ErrorValidacion("Curso vacio o repetido")
        if len(self._cursos) >= 6:
            raise ErrorCapacidad(len(self._cursos), 6, "matricula")
        self._cursos.append(curso)
        assert self._cursos[-1] == curso
        self._verificar_invariante()

    def eliminar_curso(self, curso):
        self._puede_modificar()
        if curso not in self._cursos:
            raise ErrorRecursoNoEncontrado("Curso", curso)
        if len(self._cursos) == 1:
            raise ErrorValidacion("Debe quedar al menos un curso")
        self._cursos.remove(curso)
        assert curso not in self._cursos
        self._verificar_invariante()

    def anular(self):
        self._puede_modificar()
        self._anulada = True
        self._verificar_invariante()

    def diagnostico(self):
        try:
            self._verificar_invariante()
            return f"Matricula OK {len(self._cursos)}/6"
        except AssertionError:
            return "Matricula INVALIDA"


def buscar_estudiante(codigo):
    return (
        Result.ok({"codigo": 1001, "nombre": "Juan Mamani", "ppa": 15.0})
        if codigo == 1001
        else Result.fallo("Estudiante no encontrado")
    )


def validar_ppa_minimo(estudiante):
    return Result.ok(estudiante) if estudiante["ppa"] >= 14 else Result.fallo("PPA insuficiente")


def inscribir(estudiante, curso):
    return Result.ok(f"Inscrito: {estudiante['nombre']} en {curso}")


def pipeline(codigo):
    return buscar_estudiante(codigo).map(validar_ppa_minimo).map(lambda e: inscribir(e, "SIS206"))


def demo():
    m = Matricula("20210500", ["SIS206"])
    m.agregar_curso("SIS207")
    m.eliminar_curso("SIS207")
    return [
        f"CURSOS={len(m.cursos)}",
        f"EXITO={pipeline(1001).valor}",
        f"FALLO={pipeline(9999).error}",
    ]
