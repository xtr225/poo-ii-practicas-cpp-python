"""Demostración aislada: los archivos generados viven en un directorio temporal."""
import contextlib
import os
import tempfile
from pathlib import Path

from baseline.gestor_academico import GestorAcademico


@contextlib.contextmanager
def directorio_temporal():
    anterior = Path.cwd()
    with tempfile.TemporaryDirectory() as carpeta:
        os.chdir(carpeta)
        try:
            yield Path(carpeta)
        finally:
            os.chdir(anterior)


if __name__ == "__main__":
    with directorio_temporal() as carpeta:
        gestor = GestorAcademico()
        gestor.registrar("E001", "Ana")
        gestor.agregar_nota("E001", 10)
        gestor.agregar_nota("E001", 11)
        gestor.reporte()
        print((carpeta / "reporte.csv").read_text(encoding="utf-8"))
        gestor.agregar_nota("E001", 25)
        print("Defecto observado: acepta 25 ->", gestor.estudiantes[0]["notas"])
