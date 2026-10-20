"""Result inmutable y discriminado; map encadena funciones que retornan Result."""

from dataclasses import dataclass
from typing import Generic, TypeVar, Callable

T = TypeVar("T")
U = TypeVar("U")


@dataclass(frozen=True)
class Result(Generic[T]):
    _exito: bool
    _dato: object

    def __post_init__(self):
        if type(self._exito) is not bool:
            raise TypeError("Discriminante booleano requerido")
        if not self._exito and (not isinstance(self._dato, str) or not self._dato):
            raise ValueError("Mensaje de error requerido")

    @staticmethod
    def ok(valor: T) -> "Result[T]":
        return Result(True, valor)

    @staticmethod
    def fallo(error: str) -> "Result[T]":
        return Result(False, error)

    @property
    def es_exito(self):
        return self._exito

    @property
    def valor(self) -> T:
        if not self.es_exito:
            raise RuntimeError("Result fallido: " + str(self._dato))
        return self._dato

    @property
    def error(self):
        if self.es_exito:
            raise RuntimeError("Un exito no tiene error")
        return self._dato

    def map(self, f: Callable[[T], "Result[U]"]) -> "Result[U]":
        if not self.es_exito:
            return Result.fallo(self.error)
        resultado = f(self.valor)
        if not isinstance(resultado, Result):
            raise TypeError("map espera una funcion que retorne Result")
        return resultado
