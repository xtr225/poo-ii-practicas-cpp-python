"""Generación pura: devuelve texto y deja al cliente decidir dónde guardarlo."""
import csv
import io
from collections.abc import Iterable

from .dominio import Estudiante


class GeneradorCSV:
    def generar(self, estudiantes: Iterable[Estudiante]) -> str:
        salida = io.StringIO(newline="")
        escritor = csv.writer(salida, delimiter=";", lineterminator="\n")
        for estudiante in estudiantes:
            escritor.writerow([
                estudiante.codigo,
                estudiante.nombre,
                f"{estudiante.calcular_ppa():.2f}",
                "APROBADO" if estudiante.esta_aprobado() else "DESAPROBADO",
            ])
        return salida.getvalue().removesuffix("\n")
