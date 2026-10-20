"""Actividades 1 y 2: invariantes, composición y propiedades calculadas."""

import re
from datetime import date


def entero(valor, minimo, maximo):
    if type(valor) is not int or not minimo <= valor <= maximo:
        raise ValueError("Entero fuera de rango")
    return valor


class Curso:
    # Se sigue el patrón explícito de las plantillas Python y C++ de la guía.
    _PATRON_CODIGO = re.compile(r"[A-Z]{2,4}[0-9]{3}")

    def __init__(self, codigo, nombre, creditos, cupos_maximo):
        self.__inscritos = 0
        if not isinstance(nombre, str) or not nombre.strip():
            raise ValueError("Nombre vacio")
        self.__nombre = nombre
        self.codigo = codigo
        self.creditos = creditos
        self.cupos_maximo = cupos_maximo

    @property
    def nombre(self):
        return self.__nombre

    @property
    def codigo(self):
        return self.__codigo

    @codigo.setter
    def codigo(self, valor):
        if not isinstance(valor, str) or not self._PATRON_CODIGO.fullmatch(valor):
            raise ValueError("Codigo debe tener 2 a 4 letras mayusculas y 3 digitos")
        self.__codigo = valor

    @property
    def creditos(self):
        return self.__creditos

    @creditos.setter
    def creditos(self, valor):
        self.__creditos = entero(valor, 1, 6)

    @property
    def cupos_maximo(self):
        return self.__cupos_maximo

    @cupos_maximo.setter
    def cupos_maximo(self, valor):
        entero(valor, 1, 50)
        if valor < self.__inscritos:
            raise ValueError("Cupos menores que inscritos")
        self.__cupos_maximo = valor

    @property
    def inscritos(self):
        return self.__inscritos

    @property
    def cupos_disponibles(self):
        return self.cupos_maximo - self.inscritos

    @property
    def esta_lleno(self):
        return self.cupos_disponibles == 0

    def inscribir_estudiante(self):
        if self.esta_lleno:
            raise RuntimeError("Curso sin cupos")
        self.__inscritos += 1

    def __str__(self):
        return f"{self.codigo} - {self.nombre} ({self.creditos} cr) [{self.inscritos}/{self.cupos_maximo} inscritos]"


class Docente:
    def __init__(self, dni, nombres, apellidos, correo, fecha_nac):
        if not re.fullmatch(r"[0-9]{8}", dni):
            raise ValueError("DNI invalido")
        if not re.fullmatch(r"[^@\s]+@unap\.edu\.pe", correo):
            raise ValueError("Correo institucional invalido")
        if not nombres.strip() or not apellidos.strip():
            raise ValueError("Nombre vacio")
        if type(fecha_nac) is not date or fecha_nac > date.today():
            raise ValueError("Nacimiento invalido")
        self.__dni, self.__nombres, self.__apellidos = dni, nombres, apellidos
        self.__correo, self.__fecha_nac = correo, fecha_nac
        self.__cursos = []

    @property
    def dni(self):
        return self.__dni

    @property
    def nombre_completo(self):
        return f"{self.__apellidos}, {self.__nombres}"

    def edad_en(self, hoy):
        if hoy < self.__fecha_nac:
            raise ValueError("Referencia anterior al nacimiento")
        return (
            hoy.year
            - self.__fecha_nac.year
            - ((hoy.month, hoy.day) < (self.__fecha_nac.month, self.__fecha_nac.day))
        )

    @property
    def edad(self):
        return self.edad_en(date.today())

    @property
    def cursos(self):
        return tuple(self.__cursos)

    @property
    def total_creditos(self):
        return sum(c.creditos for c in self.__cursos)

    @property
    def total_inscritos(self):
        return sum(c.inscritos for c in self.__cursos)

    def asignar_curso(self, curso):
        if any(c.codigo == curso.codigo for c in self.__cursos):
            raise ValueError("Curso duplicado")
        if len(self.__cursos) >= 3:
            raise RuntimeError("Maximo tres cursos")
        self.__cursos.append(curso)


def demo():
    c = Curso("SIS206", "POO II", 4, 30)
    for _ in range(3):
        c.inscribir_estudiante()
    d = Docente("12345678", "Ana", "Mamani", "ana@unap.edu.pe", date(1980, 5, 14))
    d.asignar_curso(c)
    return [
        f"CURSO={c.codigo}",
        f"LIBRES={c.cupos_disponibles}",
        f"CREDITOS={d.total_creditos}",
        f"INSCRITOS={d.total_inscritos}",
        f"EDAD_REFERENCIA={d.edad_en(date(2026, 10, 18))}",
    ]
