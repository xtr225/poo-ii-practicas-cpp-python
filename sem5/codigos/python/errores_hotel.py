"""Errores del dominio hotelero y verificadores de contratos."""

from .hotel_base import Habitacion, Reserva
from .result import Result


class ErrorHotel(Exception):
    pass


class ErrorDatosHotel(ErrorHotel):
    pass


class ErrorEstadoReserva(ErrorHotel):
    pass


class ErrorDisponibilidad(ErrorHotel):
    pass


class HabitacionRobusta(Habitacion):
    def __init__(self, *args):
        try:
            super().__init__(*args)
        except (ValueError, RuntimeError) as causa:
            raise ErrorDatosHotel(str(causa)) from causa
        self._verificar_invariante()

    def _verificar_invariante(self):
        assert self.numero > 0 and self.tarifa > 0
        assert 0 <= self.plazas_libres <= self.capacidad

    def alojar(self, cantidad):
        try:
            super().alojar(cantidad)
        except (ValueError, RuntimeError) as causa:
            raise ErrorDisponibilidad(str(causa)) from causa
        self._verificar_invariante()


class ReservaRobusta(Reserva):
    def __init__(self, *args):
        try:
            super().__init__(*args)
        except (ValueError, RuntimeError) as causa:
            raise ErrorDatosHotel(str(causa)) from causa
        self._verificar_invariante()

    def _verificar_invariante(self):
        assert self.codigo.strip() and self.noches > 0
        assert self.estado in {"REGISTRADA", "CONFIRMADA", "ANULADA"}

    def confirmar(self):
        try:
            super().confirmar()
        except RuntimeError as causa:
            raise ErrorEstadoReserva(str(causa)) from causa
        self._verificar_invariante()

    def anular(self):
        try:
            super().anular()
        except RuntimeError as causa:
            raise ErrorEstadoReserva(str(causa)) from causa
        self._verificar_invariante()


def buscar_habitacion(habitaciones, numero):
    return next(
        (Result.ok(h) for h in habitaciones if h.numero == numero),
        Result.fallo("Habitacion no encontrada"),
    )
