import json
import unittest

from python.auditoria import AuditoriaMemoria
from python.reportes import GeneradorCSV
from python.reportes_json import GeneradorJSON
from python.repositorio import RepositorioEstudiantes
from python.servicio import ServicioAcademico


class PruebasExtensionSeparada(unittest.TestCase):
    def setUp(self):
        self.repo = RepositorioEstudiantes()
        self.servicio = ServicioAcademico(self.repo, AuditoriaMemoria())

    def test_json_y_csv_conservados(self):
        estudiante = self.servicio.registrar("E001", "Ana")
        for nota in [10, 11]:
            self.servicio.agregar_nota("E001", nota)
        antes = estudiante.notas
        self.assertEqual(json.loads(self.servicio.reporte(GeneradorJSON())), [{
            "codigo": "E001", "nombre": "Ana", "promedio": 10.5, "estado": "APROBADO"}])
        self.assertEqual(self.servicio.reporte(GeneradorCSV()), "E001;Ana;10.50;APROBADO")
        self.assertEqual(antes, estudiante.notas)

    def test_vacio(self):
        self.assertEqual(json.loads(self.servicio.reporte(GeneradorJSON())), [])

    def test_sin_notas(self):
        self.servicio.registrar("E001", "Ana")
        fila = json.loads(self.servicio.reporte(GeneradorJSON()))[0]
        self.assertEqual(fila["promedio"], 0)
        self.assertEqual(fila["estado"], "DESAPROBADO")

    def test_escapes_y_unicode(self):
        nombre = 'María; "Luz"\nPuno'
        self.servicio.registrar("E001", nombre)
        self.assertEqual(json.loads(self.servicio.reporte(GeneradorJSON()))[0]["nombre"], nombre)


if __name__ == "__main__":
    unittest.main()
