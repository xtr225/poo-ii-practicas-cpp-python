"""Notificaciones simuladas y reportes intercambiables mediante contratos."""

from abc import ABC, abstractmethod
from typing import Protocol, runtime_checkable
from pathlib import Path
import csv
import io
import json


class INotificador(ABC):
    @abstractmethod
    def enviar(self, destinatario, mensaje):
        raise NotImplementedError

    @abstractmethod
    def es_disponible(self):
        raise NotImplementedError

    @property
    @abstractmethod
    def nombre(self):
        raise NotImplementedError


@runtime_checkable
class ProtocolNotificador(Protocol):
    def enviar(self, destinatario: str, mensaje: str) -> bool: ...
    def es_disponible(self) -> bool: ...


class CanalSimulado:
    """Comportamiento compartido por composición, sin envío real."""

    def __init__(self, servidor):
        if not servidor.strip():
            raise ValueError("Servidor vacio")
        self._servidor, self.activo, self.enviados = servidor, True, []

    def es_disponible(self):
        return self.activo

    def enviar(self, destinatario, mensaje):
        if not self.es_disponible():
            return False
        if not destinatario.strip() or not mensaje:
            raise ValueError("Destino o mensaje vacio")
        self.enviados.append((destinatario, mensaje))
        return True


class NotificadorEmail(INotificador):
    def __init__(self, servidor):
        self._canal = CanalSimulado(servidor)

    @property
    def nombre(self):
        return "Email"

    @property
    def enviados(self):
        return self._canal.enviados

    @property
    def activo(self):
        return self._canal.activo

    @activo.setter
    def activo(self, valor):
        self._canal.activo = valor

    def es_disponible(self):
        return self._canal.es_disponible()

    def enviar(self, destinatario, mensaje):
        return self._canal.enviar(destinatario, mensaje)


class NotificadorSMS(INotificador):
    MAX_CHARS = 160

    def __init__(self, proveedor):
        self._canal = CanalSimulado(proveedor)

    @property
    def nombre(self):
        return "SMS"

    def es_disponible(self):
        return self._canal.es_disponible()

    def enviar(self, destinatario, mensaje):
        if len(mensaje) > self.MAX_CHARS:
            raise ValueError("SMS excede 160 caracteres")
        return self._canal.enviar(destinatario, mensaje)


class SistemaAlertas:
    def __init__(self, notificadores):
        if not notificadores:
            raise ValueError("Se requiere un canal")
        self._notificadores = list(notificadores)
        self.errores = []

    def agregar_notificador(self, notificador):
        self._notificadores.append(notificador)

    def alertar(self, dest, mensaje):
        enviados = 0
        for notificador in self._notificadores:
            try:
                if notificador.es_disponible() and notificador.enviar(dest, mensaje):
                    enviados += 1
            except Exception as error:
                self.errores.append(str(error))
        return enviados


class Exportador(ABC):
    @property
    @abstractmethod
    def extension(self):
        raise NotImplementedError

    @abstractmethod
    def exportar(self, contenido, ruta):
        raise NotImplementedError


class ExportadorCSV(Exportador):
    @property
    def extension(self):
        return ".csv"

    def exportar(self, contenido, ruta):
        datos = json.loads(contenido)
        salida = io.StringIO(newline="")
        escritor = csv.writer(salida, lineterminator="\n")
        escritor.writerow(datos.keys())
        escritor.writerow(datos.values())
        Path(str(ruta) + self.extension).write_text(salida.getvalue(), encoding="utf-8")


class ExportadorJSON(Exportador):
    @property
    def extension(self):
        return ".json"

    def exportar(self, contenido, ruta):
        json.loads(contenido)
        Path(str(ruta) + self.extension).write_text(contenido, encoding="utf-8")


class Firmador(ABC):
    @abstractmethod
    def firmar(self, ruta):
        raise NotImplementedError


class FirmadorDigital(Firmador):
    """Simulación didáctica; no representa una firma criptográfica válida."""

    def firmar(self, ruta):
        destino = str(ruta) + ".signed"
        Path(destino).write_text(
            "FIRMA SIMULADA\n" + Path(ruta).read_text(encoding="utf-8"), encoding="utf-8"
        )
        return destino


class GeneradorReporte:
    def __init__(self, exportador, firmador=None):
        self._exportador, self._firmador = exportador, firmador

    def cambiar_exportador(self, exportador):
        self._exportador = exportador

    def generar(self, datos, ruta):
        contenido = json.dumps(datos, ensure_ascii=False, allow_nan=False)
        self._exportador.exportar(contenido, ruta)
        destino = str(ruta) + self._exportador.extension
        return self._firmador.firmar(destino) if self._firmador else destino


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
            raise ValueError("Noches positivas requeridas")
        return habitacion.precio_noche() * noches

    def descripcion(self):
        return "Estandar"


class ServicioCotizacion:
    def __init__(self, calculadora):
        self._calculadora = calculadora

    def cotizar(self, habitacion, noches):
        return self._calculadora.calcular(habitacion, noches)


def demo():
    from .hotel_herencia import HabitacionDoble

    email, sms = NotificadorEmail("smtp.local"), NotificadorSMS("Simulado")
    alertas = SistemaAlertas([email, sms])
    return [
        f"CANALES={alertas.alertar('destino_prueba','Aviso de prueba')}",
        f"SMS_LARGO={alertas.alertar('destino_prueba','X'*161)}",
        f"COTIZACION={ServicioCotizacion(TarifaEstandar()).cotizar(HabitacionDoble(102),2):.2f}",
    ]
