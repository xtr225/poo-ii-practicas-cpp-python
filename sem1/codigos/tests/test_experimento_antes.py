import contextlib
import io
import json
import unittest

from baseline.ejecutar import directorio_temporal
from experimento.gestor_con_json import GestorAcademico


class PruebasExtensionMonolitica(unittest.TestCase):
    def test_json_y_csv_conservados(self):
        with directorio_temporal() as carpeta:
            gestor = GestorAcademico()
            gestor.registrar("E001", "Ana")
            gestor.agregar_nota("E001", 10)
            gestor.agregar_nota("E001", 11)
            self.assertEqual(json.loads(gestor.reporte_json()), [{
                "codigo": "E001", "nombre": "Ana", "promedio": 10.5, "estado": "APROBADO"}])
            with contextlib.redirect_stdout(io.StringIO()):
                gestor.reporte()
            self.assertEqual((carpeta / "reporte.csv").read_text(encoding="utf-8"),
                             "E001;Ana;10.50;APROBADO")


if __name__ == "__main__":
    unittest.main()
