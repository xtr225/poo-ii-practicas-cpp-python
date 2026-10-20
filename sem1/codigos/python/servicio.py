"""Coordinación: las dependencias se reciben, no se crean internamente."""
from .dominio import Estudiante
from .repositorio import RepositorioEstudiantes


class ServicioAcademico:
    def __init__(self, repositorio: RepositorioEstudiantes, auditoria):
        self._repositorio = repositorio
        self._auditoria = auditoria

    def registrar(self, codigo: str | int, nombre: str, escuela="Ing. Sistemas") -> Estudiante:
        estudiante = Estudiante(nombre, codigo, escuela)
        self._repositorio.guardar(estudiante)
        return estudiante

    def agregar_nota(self, codigo: str | int, nota: float, curso="General") -> None:
        estudiante = self._repositorio.buscar(codigo)
        estudiante.agregar_nota(curso, nota)
        self._auditoria.registrar(f"nota:{codigo}:{nota:g}")

    def reporte(self, generador) -> str:
        return generador.generar(self._repositorio.todos())
