"""Segundo formato incorporado sin editar dominio, servicio ni CSV."""
import json
from collections.abc import Iterable

from .dominio import Estudiante


class GeneradorJSON:
    def generar(self, estudiantes: Iterable[Estudiante]) -> str:
        filas = [{
            "codigo": estudiante.codigo,
            "nombre": estudiante.nombre,
            "promedio": estudiante.calcular_ppa(),
            "estado": "APROBADO" if estudiante.esta_aprobado() else "DESAPROBADO",
        } for estudiante in estudiantes]
        return json.dumps(filas, ensure_ascii=False, allow_nan=False)
