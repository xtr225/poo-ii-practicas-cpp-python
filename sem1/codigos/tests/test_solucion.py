import contextlib
import csv
import io
import tempfile
import unittest
from dataclasses import FrozenInstanceError
from pathlib import Path

from baseline.ejecutar import directorio_temporal
from baseline.gestor_academico import GestorAcademico
from python.auditoria import AuditoriaArchivo, AuditoriaMemoria
from python.dominio import Estudiante, Nota
from python.reportes import GeneradorCSV
from python.repositorio import RepositorioEstudiantes
from python.servicio import ServicioAcademico


class PruebasDominio(unittest.TestCase):
    def setUp(self):
        self.estudiante = Estudiante("Ana", "E001")

    def test_promedio_vacio(self):
        self.assertEqual(self.estudiante.calcular_ppa(), 0.0)

    def test_promedio_16_14(self):
        self.estudiante.agregar_nota("POO", 16)
        self.estudiante.agregar_nota("BD", 14)
        self.assertEqual(self.estudiante.calcular_ppa(), 15)

    def test_tres_notas(self):
        for nota in [12, 14, 16]:
            self.estudiante.agregar_nota("Curso", nota)
        self.assertEqual(self.estudiante.calcular_ppa(), 14)

    def test_extremos(self):
        for nota in [0, 20]:
            self.estudiante.agregar_nota("Curso", nota)
        self.assertEqual(self.estudiante.calcular_ppa(), 10)

    def test_rechaza_invalidas_sin_modificar_estado(self):
        for nota in [-0.1, 20.1, 25, float("nan"), float("inf"), True, "15"]:
            with self.subTest(nota=nota), self.assertRaises(ValueError):
                self.estudiante.agregar_nota("Curso", nota)
        self.assertEqual(self.estudiante.notas, ())

    def test_umbral_aprobacion(self):
        self.estudiante.agregar_nota("A", 10)
        self.assertFalse(self.estudiante.esta_aprobado())
        self.estudiante.agregar_nota("B", 11)
        self.assertTrue(self.estudiante.esta_aprobado())

    def test_constructor(self):
        for nombre, codigo, escuela in [("", "E1", "Sistemas"), ("Ana", "", "Sistemas"),
                                        ("Ana", 0, "Sistemas"), ("Ana", "E1", "")]:
            with self.subTest(codigo=codigo), self.assertRaises(ValueError):
                Estudiante(nombre, codigo, escuela)

    def test_nota_tiene_campos_y_es_inmutable(self):
        nota = Nota("POO", 16)
        self.assertEqual(nota.curso, "POO")
        with self.assertRaises(FrozenInstanceError):
            nota.calificacion = 25

    def test_propiedades_sin_escritura(self):
        with self.assertRaises(AttributeError):
            self.estudiante.nombre = "Otro"
        self.assertIsInstance(self.estudiante.notas, tuple)

    def test_curso_vacio(self):
        with self.assertRaises(ValueError):
            self.estudiante.agregar_nota(" ", 16)


class PruebasServicio(unittest.TestCase):
    def setUp(self):
        self.repo = RepositorioEstudiantes()
        self.auditoria = AuditoriaMemoria()
        self.servicio = ServicioAcademico(self.repo, self.auditoria)
        self.estudiante = self.servicio.registrar("E001", "Ana")

    def test_registrar_y_recuperar(self):
        self.assertIs(self.repo.buscar("E001"), self.estudiante)

    def test_rechaza_duplicado(self):
        with self.assertRaises(ValueError):
            self.servicio.registrar("E001", "Otra")
        self.assertEqual(len(self.repo.todos()), 1)

    def test_estudiante_inexistente(self):
        with self.assertRaises(ValueError):
            self.servicio.agregar_nota("NO", 16)

    def test_invalida_no_genera_auditoria(self):
        with self.assertRaises(ValueError):
            self.servicio.agregar_nota("E001", 25)
        self.assertEqual(self.auditoria.eventos, ())

    def test_auditoria_en_memoria(self):
        self.servicio.agregar_nota("E001", 16)
        self.assertEqual(self.auditoria.eventos, ("nota:E001:16",))

    def test_auditoria_en_archivo(self):
        with tempfile.TemporaryDirectory() as carpeta:
            archivo = Path(carpeta) / "auditoria.txt"
            servicio = ServicioAcademico(self.repo, AuditoriaArchivo(archivo))
            servicio.agregar_nota("E001", 14)
            self.assertEqual(archivo.read_text(encoding="utf-8"), "nota:E001:14\n")

    def test_csv_no_modifica_entidad(self):
        self.servicio.agregar_nota("E001", 10)
        self.servicio.agregar_nota("E001", 11)
        antes = self.estudiante.notas
        self.assertEqual(self.servicio.reporte(GeneradorCSV()), "E001;Ana;10.50;APROBADO")
        self.assertEqual(antes, self.estudiante.notas)

    def test_csv_escapa_delimitadores(self):
        self.servicio.registrar("E002", 'Ana; "Luz"')
        filas = list(csv.reader(io.StringIO(self.servicio.reporte(GeneradorCSV())), delimiter=";"))
        self.assertEqual(filas[1][1], 'Ana; "Luz"')

    def test_seis_casos_conservados(self):
        # Mismos datos y contrato de CSV antes y después para los casos válidos.
        for notas in [[], [0, 20], [12, 14, 16], [10, 11], [16, 14], [10]]:
            with self.subTest(notas=notas), directorio_temporal() as carpeta:
                anterior = GestorAcademico()
                anterior.registrar("E001", "Ana")
                nuevo = ServicioAcademico(RepositorioEstudiantes(), AuditoriaMemoria())
                nuevo.registrar("E001", "Ana")
                for nota in notas:
                    anterior.agregar_nota("E001", nota)
                    nuevo.agregar_nota("E001", nota)
                with contextlib.redirect_stdout(io.StringIO()):
                    anterior.reporte()
                self.assertEqual((carpeta / "reporte.csv").read_text(encoding="utf-8"),
                                 nuevo.reporte(GeneradorCSV()))


if __name__ == "__main__":
    unittest.main()
