"""Herencia justificada, despacho dinámico y contraejemplo de Liskov."""

from abc import ABC, abstractmethod
from math import isfinite
import re
from typing import override


class PersonalUniversitario(ABC):
    TARIFA_HORA_BASE = 75.0

    def __init__(self, dni, nombres, apellidos):
        if not re.fullmatch(r"[0-9]{8}", dni):
            raise ValueError("DNI invalido")
        if not nombres.strip() or not apellidos.strip():
            raise ValueError("Nombre vacio")
        self.__dni, self._nombres, self._apellidos = dni, nombres, apellidos

    @property
    def dni(self):
        return self.__dni

    @property
    def nombre_completo(self):
        return f"{self._apellidos}, {self._nombres}"

    @abstractmethod
    def calcular_remuneracion(self):
        raise NotImplementedError

    def __str__(self):
        return f"{self.nombre_completo}: S/ {self.calcular_remuneracion():.2f}"


class Docente(PersonalUniversitario):
    FACTORES = {"Auxiliar": 1.0, "Asociado": 1.4, "Principal": 1.8}

    def __init__(self, dni, nombres, apellidos, horas, categoria):
        super().__init__(dni, nombres, apellidos)
        if type(horas) is not int or not 1 <= horas <= 40:
            raise ValueError("Horas fuera de 1 a 40")
        if categoria not in self.FACTORES:
            raise ValueError("Categoria invalida")
        self._horas, self._categoria = horas, categoria

    @override
    def calcular_remuneracion(self):
        return self._horas * self.TARIFA_HORA_BASE * self.FACTORES[self._categoria]


class Administrativo(PersonalUniversitario):
    def __init__(self, dni, nombres, apellidos, salario_base, bono=0):
        super().__init__(dni, nombres, apellidos)
        if not isfinite(salario_base) or salario_base <= 0:
            raise ValueError("Salario positivo requerido")
        if not isfinite(bono) or bono < 0:
            raise ValueError("Bono no negativo requerido")
        self._salario_base, self._bono = salario_base, bono

    @override
    def calcular_remuneracion(self):
        return self._salario_base + self._bono


class Investigador(PersonalUniversitario):
    def __init__(self, dni, nombres, apellidos, proyectos):
        super().__init__(dni, nombres, apellidos)
        if type(proyectos) is not int or proyectos < 1:
            raise ValueError("Proyectos positivos requeridos")
        self._proyectos = proyectos

    @override
    def calcular_remuneracion(self):
        return self._proyectos * 1500.0


def planilla(personal):
    """El cliente imprime el resultado; el cálculo no pregunta el subtipo."""
    filas = [str(persona) for persona in personal]
    return filas + [f"TOTAL={sum(p.calcular_remuneracion() for p in personal):.2f}"]


def verificar_contrato_personal(persona):
    valor = persona.calcular_remuneracion()
    return (
        bool(persona.nombre_completo)
        and isfinite(valor)
        and valor > 0
        and persona.nombre_completo in str(persona)
    )


class Figura(ABC):
    @abstractmethod
    def area(self):
        raise NotImplementedError


class Rectangulo(Figura):
    def __init__(self, ancho, alto):
        self.set_ancho(ancho)
        self.set_alto(alto)

    def set_ancho(self, v):
        if not isfinite(v) or v <= 0:
            raise ValueError("Dimension invalida")
        self._ancho = v

    def set_alto(self, v):
        if not isfinite(v) or v <= 0:
            raise ValueError("Dimension invalida")
        self._alto = v

    def area(self):
        return self._ancho * self._alto


class CuadradoIncompatible(Rectangulo):
    """Contraejemplo intencional: rompe la independencia de ancho y alto."""

    def __init__(self, lado):
        super().__init__(lado, lado)

    def set_ancho(self, v):
        super().set_ancho(v)
        self._alto = v

    def set_alto(self, v):
        super().set_alto(v)
        self._ancho = v


class Cuadrado(Figura):
    """Alternativa correcta: comparte el contrato área, no setters independientes."""

    def __init__(self, lado):
        if not isfinite(lado) or lado <= 0:
            raise ValueError("Lado invalido")
        self._lado = lado

    def area(self):
        return self._lado**2


def comprobar_rectangulo(figura):
    figura.set_ancho(4)
    figura.set_alto(5)
    assert figura.area() == 20, "Se rompio el contrato de dimensiones independientes"


def datos():
    return [
        Docente("12345678", "Juan Carlos", "Mamani Quispe", 16, "Asociado"),
        Docente("23456789", "Maria Elena", "Quispe Torres", 12, "Principal"),
        Administrativo("34567890", "Pedro", "Huanca Lazo", 2800, 350),
        Investigador("45678901", "Ana Lucia", "Flores Ramos", 2),
    ]


def demo():
    personal = datos()
    return planilla(personal) + [
        f"CONTRATOS={sum(verificar_contrato_personal(p) for p in personal)}"
    ]
