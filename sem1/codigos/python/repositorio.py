"""Almacenamiento en memoria: no conoce CSV, JSON ni archivos."""
from .dominio import Estudiante


class RepositorioEstudiantes:
    def __init__(self):
        self._estudiantes: dict[str, Estudiante] = {}

    def guardar(self, estudiante: Estudiante) -> None:
        if estudiante.codigo in self._estudiantes:
            raise ValueError("El codigo ya esta registrado")
        self._estudiantes[estudiante.codigo] = estudiante

    def buscar(self, codigo: str | int) -> Estudiante:
        try:
            return self._estudiantes[str(codigo)]
        except KeyError:
            raise ValueError("Estudiante no encontrado") from None

    def todos(self) -> tuple[Estudiante, ...]:
        return tuple(self._estudiantes.values())
