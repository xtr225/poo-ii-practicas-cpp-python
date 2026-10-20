"""Errores del dominio hotelero y verificadores de contratos."""


class ErrorHotel(Exception):
    pass


class ErrorDatosHotel(ErrorHotel):
    pass


class ErrorEstadoReserva(ErrorHotel):
    pass


class ErrorDisponibilidad(ErrorHotel):
    pass
