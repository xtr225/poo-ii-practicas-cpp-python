import unittest
import tempfile
import json
from pathlib import Path
from python.solucion import *
from python.hotel_herencia import HabitacionDoble


class ExportadorMock(Exportador):
    def __init__(self):
        self.llamadas = []

    @property
    def extension(self):
        return ".mock"

    def exportar(self, contenido, ruta):
        self.llamadas.append((contenido, ruta))


class ContratosSemana4(unittest.TestCase):
    def test_abc_no_instanciable(self):
        with self.assertRaises(TypeError):
            INotificador()

    def test_protocol_estructural(self):
        class Canal:
            def enviar(self, d, m):
                return True

            def es_disponible(self):
                return True

        self.assertIsInstance(Canal(), ProtocolNotificador)

    def test_dos_canales(self):
        self.assertEqual(
            SistemaAlertas([NotificadorEmail("local"), NotificadorSMS("local")]).alertar(
                "d", "mensaje"
            ),
            2,
        )

    def test_canal_no_disponible(self):
        n = NotificadorEmail("local")
        n.activo = False
        self.assertEqual(SistemaAlertas([n, NotificadorSMS("local")]).alertar("d", "mensaje"), 1)

    def test_sms_limites(self):
        n = NotificadorSMS("local")
        self.assertTrue(n.enviar("d", "á" * 160))
        with self.assertRaises(ValueError):
            n.enviar("d", "x" * 161)

    def test_error_aislado_no_detiene_otros(self):
        s = SistemaAlertas([NotificadorSMS("local"), NotificadorEmail("local")])
        self.assertEqual(s.alertar("d", "X" * 161), 1)
        self.assertEqual(len(s.errores), 1)

    def test_lista_vacia(self):
        with self.assertRaises(ValueError):
            SistemaAlertas([])

    def test_mock_sin_archivos(self):
        mock = ExportadorMock()
        GeneradorReporte(mock).generar({"n": 45}, "prueba")
        self.assertEqual(len(mock.llamadas), 1)
        self.assertEqual(json.loads(mock.llamadas[0][0]), {"n": 45})

    def test_cambio_exportador_y_firma(self):
        with tempfile.TemporaryDirectory() as d:
            base = Path(d) / "reporte"
            g = GeneradorReporte(ExportadorCSV())
            self.assertTrue(Path(g.generar({"n": 45}, base)).is_file())
            g.cambiar_exportador(ExportadorJSON())
            self.assertEqual(
                json.loads(Path(g.generar({"n": 45}, base)).read_text(encoding="utf-8")), {"n": 45}
            )
            firmado = GeneradorReporte(ExportadorCSV(), FirmadorDigital()).generar({"n": 45}, base)
            self.assertIn("FIRMA SIMULADA", Path(firmado).read_text(encoding="utf-8"))

    def test_inyeccion_hotel(self):
        self.assertEqual(ServicioCotizacion(TarifaEstandar()).cotizar(HabitacionDoble(102), 2), 300)

    def test_noches_invalidas(self):
        with self.assertRaises(ValueError):
            ServicioCotizacion(TarifaEstandar()).cotizar(HabitacionDoble(102), 0)


if __name__ == "__main__":
    unittest.main()
