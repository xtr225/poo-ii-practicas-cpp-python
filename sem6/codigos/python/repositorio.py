"""Repositorio genérico: guardar actualiza; insertar rechaza un ID repetido."""

from collections import deque
from functools import reduce
from typing import Generic, TypeVar, Callable, Iterator
from .result import Result

T = TypeVar("T")
ID = TypeVar("ID")


class Repositorio(Generic[T, ID]):
    def __init__(self, fn_id: Callable[[T], ID]):
        if not callable(fn_id):
            raise ValueError("Extractor de ID requerido")
        self._fn_id, self._datos = fn_id, {}

    def guardar(self, entidad: T):
        self._datos[self._fn_id(entidad)] = entidad

    def insertar(self, entidad: T):
        if self.existe(self._fn_id(entidad)):
            raise ValueError("ID duplicado")
        self.guardar(entidad)

    def buscar(self, identificador: ID):
        return self._datos.get(identificador)

    def buscar_resultado(self, identificador: ID):
        if identificador in self._datos:
            return Result.ok(self._datos[identificador])
        return Result.fallo("Recurso no encontrado")

    def eliminar(self, identificador: ID):
        if identificador not in self._datos:
            return False
        del self._datos[identificador]
        return True

    def todos(self):
        return list(self._datos.values())

    def filtrar(self, predicado):
        return [e for e in self if predicado(e)]

    def ordenar(self, key, reverse=False):
        return sorted(self, key=key, reverse=reverse)

    def acumular(self, inicial, funcion):
        return reduce(funcion, self, inicial)

    def contar(self, predicado):
        return sum(1 for e in self if predicado(e))

    def existe(self, identificador):
        return identificador in self._datos

    def primero(self, predicado):
        return next((e for e in self if predicado(e)), None)

    def estadisticas(self, atributo):
        valores = [getattr(e, atributo) for e in self]
        return (
            {
                "min": min(valores),
                "max": max(valores),
                "promedio": sum(valores) / len(valores),
                "n": len(valores),
            }
            if valores
            else {"min": None, "max": None, "promedio": None, "n": 0}
        )

    def __len__(self):
        return len(self._datos)

    def __contains__(self, identificador):
        return self.existe(identificador)

    def __iter__(self) -> Iterator[T]:
        return iter(self._datos.values())


class ColaAtencion(Generic[T]):
    def __init__(self):
        self._cola = deque()

    def encolar(self, elemento: T):
        self._cola.append(elemento)

    def atender(self):
        return self._cola.popleft() if self._cola else None

    def __len__(self):
        return len(self._cola)

    def __iter__(self):
        return iter(self._cola)


class PlanillaIterador(Generic[T]):
    def __init__(self, personal, key=lambda p: p.calcular_remuneracion()):
        self._ordenados = sorted(personal, key=key, reverse=True)

    def __iter__(self):
        return iter(self._ordenados)


def ventana_deslizante(datos, k):
    if type(k) is not int or not 1 <= k <= len(datos):
        raise ValueError("Ventana fuera de 1 a longitud")
    for i in range(len(datos) - k + 1):
        yield datos[i : i + k]
