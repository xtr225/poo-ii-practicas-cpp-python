"""Máquinas de estados con validación previa a cualquier mutación."""

from enum import Enum, auto


class EstadoMatricula(Enum):
    REGISTRADA = auto()
    VIGENTE = auto()
    SUSPENDIDA = auto()
    ANULADA = auto()


class MaquinaEstados:
    def __init__(self, inicial, transiciones):
        self._estado = inicial
        self._TRANSICIONES = dict(transiciones)
        self._historial = [(None, inicial)]

    @property
    def estado(self):
        return self._estado

    @property
    def historial(self):
        return tuple(self._historial)

    def transicionar(self, destino, contexto=None):
        clave = (self._estado, destino)
        if clave not in self._TRANSICIONES:
            raise ValueError("Transicion no permitida")
        guardia = self._TRANSICIONES[clave]
        if guardia and not guardia(contexto or {}):
            raise ValueError("Guardia no satisfecha")
        self._historial.append((self._estado, destino))
        self._estado = destino


class Matricula:
    def __init__(self, codigo="E001", cursos=("SIS206",)):
        if not codigo.strip() or not 1 <= len(cursos) <= 6 or len(set(cursos)) != len(cursos):
            raise ValueError("Matricula invalida")
        self._codigo, self._cursos = codigo, tuple(cursos)
        E = EstadoMatricula
        self._maquina = MaquinaEstados(
            E.REGISTRADA,
            {
                (E.REGISTRADA, E.VIGENTE): None,
                (E.REGISTRADA, E.ANULADA): None,
                (E.VIGENTE, E.SUSPENDIDA): None,
                (E.VIGENTE, E.ANULADA): None,
                (E.SUSPENDIDA, E.VIGENTE): None,
                (E.SUSPENDIDA, E.ANULADA): None,
            },
        )
        self._motivo = ""

    @property
    def estado(self):
        return self._maquina.estado

    @property
    def historial(self):
        return self._maquina.historial

    @property
    def motivo(self):
        return self._motivo

    def validar_pago(self):
        if self.estado != EstadoMatricula.REGISTRADA:
            raise ValueError("Pago solo desde registrada")
        self._maquina.transicionar(EstadoMatricula.VIGENTE)

    def suspender(self, motivo):
        if not motivo.strip():
            raise ValueError("Motivo requerido")
        self._maquina.transicionar(EstadoMatricula.SUSPENDIDA)
        self._motivo = motivo

    def regularizar(self):
        if self.estado != EstadoMatricula.SUSPENDIDA:
            raise ValueError("Regularizar solo suspendida")
        self._maquina.transicionar(EstadoMatricula.VIGENTE)
        self._motivo = ""

    def anular(self):
        self._maquina.transicionar(EstadoMatricula.ANULADA)


class EstadoReserva(Enum):
    REGISTRADA = auto()
    CONFIRMADA = auto()
    HOSPEDADA = auto()
    FINALIZADA = auto()
    ANULADA = auto()


def maquina_reserva():
    E = EstadoReserva
    return MaquinaEstados(
        E.REGISTRADA,
        {
            (E.REGISTRADA, E.CONFIRMADA): None,
            (E.REGISTRADA, E.ANULADA): None,
            (E.CONFIRMADA, E.HOSPEDADA): None,
            (E.CONFIRMADA, E.ANULADA): None,
            (E.HOSPEDADA, E.FINALIZADA): None,
        },
    )
