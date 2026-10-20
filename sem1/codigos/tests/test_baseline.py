"""Caracteriza los casos válidos antes de modificar el diseño."""
import contextlib
import io
import unittest

from baseline.ejecutar import directorio_temporal
from baseline.gestor_academico import GestorAcademico


class PruebasCaracterizacion(unittest.TestCase):
    def setUp(self):
        self.contexto = directorio_temporal()
        self.carpeta = self.contexto.__enter__()
        self.addCleanup(self.contexto.__exit__, None, None, None)
        self.gestor = GestorAcademico()
        self.gestor.registrar("E001", "Ana")

    def fila(self, notas):
        for nota in notas:
            self.gestor.agregar_nota("E001", nota)
        with contextlib.redirect_stdout(io.StringIO()):
            self.gestor.reporte()
        return (self.carpeta / "reporte.csv").read_text(encoding="utf-8")

    def test_registro(self):
        self.assertEqual(self.gestor.estudiantes[0]["codigo"], "E001")

    def test_sin_notas(self):
        self.assertEqual(self.fila([]), "E001;Ana;0.00;DESAPROBADO")

    def test_extremos_validos(self):
        self.assertEqual(self.fila([0, 20]), "E001;Ana;10.00;DESAPROBADO")

    def test_promedio_tres_notas(self):
        self.assertEqual(self.fila([12, 14, 16]), "E001;Ana;14.00;APROBADO")

    def test_limite_aprobacion(self):
        self.assertEqual(self.fila([10, 11]), "E001;Ana;10.50;APROBADO")

    def test_reporte_no_cambia_notas(self):
        self.fila([16, 14])
        self.assertEqual(self.gestor.estudiantes[0]["notas"], [16, 14])


if __name__ == "__main__":
    unittest.main()
