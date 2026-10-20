"""Entidades: conservan datos válidos y calculan resultados, sin archivos ni consola."""
from dataclasses import dataclass
from math import isfinite


@dataclass(frozen=True)
class Nota:
    curso: str
    calificacion: float

    def __post_init__(self):
        if not isinstance(self.curso, str) or not self.curso.strip():
            raise ValueError("El curso no puede estar vacio")
        if (isinstance(self.calificacion, bool)
                or not isinstance(self.calificacion, (int, float))
                or not isfinite(self.calificacion)
                or not 0 <= self.calificacion <= 20):
            raise ValueError("La nota debe estar entre 0 y 20")


class Estudiante:
    def __init__(self, nombre: str, codigo: str | int, escuela: str = "Ing. Sistemas"):
        if not isinstance(nombre, str) or not nombre.strip():
            raise ValueError("El nombre no puede estar vacio")
        if isinstance(codigo, bool) or not isinstance(codigo, (str, int)):
            raise ValueError("El codigo debe ser texto o un entero positivo")
        if (isinstance(codigo, int) and codigo <= 0) or not str(codigo).strip():
            raise ValueError("El codigo no puede estar vacio ni ser no positivo")
        if not isinstance(escuela, str) or not escuela.strip():
            raise ValueError("La escuela no puede estar vacia")
        self._nombre = nombre
        self._codigo = str(codigo)
        self._escuela = escuela
        self._notas: list[Nota] = []

    @property
    def nombre(self) -> str:
        return self._nombre

    @property
    def codigo(self) -> str:
        return self._codigo

    @property
    def escuela(self) -> str:
        return self._escuela

    @property
    def notas(self) -> tuple[Nota, ...]:
        # Una tupla de notas inmutables evita modificar la colección por esta interfaz.
        return tuple(self._notas)

    def agregar_nota(self, curso: str, calificacion: float) -> None:
        nota = Nota(curso, calificacion)  # Validar antes de modificar el estado.
        self._notas.append(nota)

    def calcular_ppa(self) -> float:
        if not self._notas:
            return 0.0
        return sum(nota.calificacion for nota in self._notas) / len(self._notas)

    def esta_aprobado(self) -> bool:
        return self.calcular_ppa() >= 10.5

    def __str__(self) -> str:
        # Representar no produce efectos de salida; la consola pertenece al programa principal.
        return f"[{self.codigo}] {self.nombre} ({self.escuela})"
