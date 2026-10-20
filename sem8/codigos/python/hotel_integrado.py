"""Proyecto integrador de Unidad I con reservas, repositorios, errores y estados."""

from abc import ABC, abstractmethod
from datetime import date
from math import isfinite
from .hotel_base import Huesped, Habitacion
from .hotel_herencia import HabitacionSimple, HabitacionDoble, Suite
from .repositorio import Repositorio
from .result import Result
from .errores_hotel import ErrorDatosHotel, ErrorEstadoReserva
from .estados import EstadoReserva, maquina_reserva


class CalculadoraTarifa(ABC):
    @abstractmethod
    def calcular(self, habitacion, noches):
        raise NotImplementedError

    @abstractmethod
    def descripcion(self):
        raise NotImplementedError


class TarifaEstandar(CalculadoraTarifa):
    def calcular(self, habitacion, noches):
        if type(noches) is not int or noches < 1:
            raise ErrorDatosHotel("Noches invalidas")
        return habitacion.precio_noche() * noches

    def descripcion(self):
        return "Estandar"


class INotificadorHotel(ABC):
    @abstractmethod
    def enviar(self, destino, mensaje):
        raise NotImplementedError

    @abstractmethod
    def es_disponible(self):
        raise NotImplementedError


class NotificadorSimulado(INotificadorHotel):
    def __init__(self):
        self.eventos = []

    def es_disponible(self):
        return True

    def enviar(self, destino, mensaje):
        self.eventos.append((destino, mensaje))
        return True


class ReservaHotel:
    def __init__(self, codigo, huesped, habitacion, entrada, salida, total):
        try:
            inicio, fin = date.fromisoformat(entrada), date.fromisoformat(salida)
        except (ValueError, TypeError) as causa:
            raise ErrorDatosHotel("Fecha invalida") from causa
        if not codigo.strip() or fin <= inicio or not isfinite(total) or total <= 0:
            raise ErrorDatosHotel("Reserva invalida")
        self.__codigo, self.__huesped, self.__habitacion = codigo, huesped, habitacion
        self.__entrada, self.__salida, self.__total = entrada, salida, float(total)
        self._maquina = maquina_reserva()
        self._verificar_invariante()

    @property
    def codigo(self):
        return self.__codigo

    @property
    def huesped(self):
        return self.__huesped

    @property
    def habitacion(self):
        return self.__habitacion

    @property
    def entrada(self):
        return self.__entrada

    @property
    def salida(self):
        return self.__salida

    @property
    def total(self):
        return self.__total

    @property
    def noches(self):
        return (date.fromisoformat(self.salida) - date.fromisoformat(self.entrada)).days

    @property
    def estado(self):
        return self._maquina.estado

    @property
    def historial(self):
        return self._maquina.historial

    def _verificar_invariante(self):
        assert self.codigo.strip() and self.noches > 0 and self.total > 0

    def _transicionar(self, destino):
        try:
            self._maquina.transicionar(destino)
        except ValueError as causa:
            raise ErrorEstadoReserva(str(causa)) from causa
        self._verificar_invariante()

    def confirmar(self):
        self._transicionar(EstadoReserva.CONFIRMADA)

    def ingresar(self):
        self._transicionar(EstadoReserva.HOSPEDADA)

    def finalizar(self):
        self._transicionar(EstadoReserva.FINALIZADA)

    def anular(self):
        self._transicionar(EstadoReserva.ANULADA)


class ServicioReservas:
    def __init__(self, huespedes, habitaciones, reservas, tarifa, notificador):
        self.__huespedes, self.__habitaciones, self.__reservas = huespedes, habitaciones, reservas
        self._tarifa, self._notificador = tarifa, notificador
        self.avisos = []

    @property
    def huespedes(self):
        return self.__huespedes

    @property
    def habitaciones(self):
        return self.__habitaciones

    @property
    def reservas(self):
        return self.__reservas

    def _notificar(self, reserva, mensaje):
        try:
            if self._notificador.es_disponible():
                self._notificador.enviar(reserva.huesped.id, mensaje)
        except Exception as error:
            self.avisos.append(str(error))

    def crear_reserva(self, codigo, huesped_id, numero, entrada, salida, notificar=False):
        try:
            inicio, fin = date.fromisoformat(entrada), date.fromisoformat(salida)
        except (ValueError, TypeError) as causa:
            raise ErrorDatosHotel("Fecha invalida") from causa
        if not codigo.strip() or fin <= inicio:
            raise ErrorDatosHotel("Reserva invalida")
        if self.reservas.existe(codigo):
            return Result.fallo("Codigo de reserva duplicado")
        huesped = self.huespedes.buscar_resultado(huesped_id)
        habitacion = self.habitaciones.buscar_resultado(numero)
        if not huesped.es_exito or not habitacion.es_exito:
            return Result.fallo("Huesped o habitacion no encontrado")
        if self._solapadas(numero, entrada, salida):
            return Result.fallo("Habitacion sin disponibilidad")
        reserva = ReservaHotel(
            codigo,
            huesped.valor,
            habitacion.valor,
            entrada,
            salida,
            self._tarifa.calcular(habitacion.valor, (fin - inicio).days),
        )
        reserva.confirmar()
        self.reservas.insertar(reserva)
        if notificar:
            self._notificar(reserva, "Reserva confirmada")
        return Result.ok(reserva)

    def _solapadas(self, numero, entrada, salida):
        return self.reservas.filtrar(
            lambda r: r.habitacion.numero == numero
            and r.estado != EstadoReserva.ANULADA
            and entrada < r.salida
            and r.entrada < salida
        )

    def anular_reserva(self, codigo, notificar=False):
        resultado = self.reservas.buscar_resultado(codigo)
        if not resultado.es_exito:
            return resultado
        resultado.valor.anular()
        if notificar:
            self._notificar(resultado.valor, "Reserva anulada")
        return resultado

    def disponibles(self, entrada, salida):
        if date.fromisoformat(salida) <= date.fromisoformat(entrada):
            raise ErrorDatosHotel("Fechas invalidas")
        return sorted(
            self.habitaciones.filtrar(lambda h: not self._solapadas(h.numero, entrada, salida)),
            key=lambda h: h.numero,
        )


def crear_sistema():
    huespedes = Repositorio(lambda h: h.id)
    habitaciones = Repositorio(lambda h: h.numero)
    reservas = Repositorio(lambda r: r.codigo)
    huespedes.guardar(Huesped("H1", "Ana", "12345678"))
    for h in [HabitacionSimple(101), HabitacionDoble(102), Suite(103)]:
        habitaciones.guardar(h)
    return ServicioReservas(
        huespedes, habitaciones, reservas, TarifaEstandar(), NotificadorSimulado()
    )
