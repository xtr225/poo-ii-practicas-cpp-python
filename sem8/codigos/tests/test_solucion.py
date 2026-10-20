import unittest
from python.hotel_integrado import crear_sistema, ReservaHotel, CalculadoraTarifa
from python.estados import EstadoReserva
from python.errores_hotel import ErrorDatosHotel, ErrorEstadoReserva
from python.solucion import demo


class ContratosSemana8(unittest.TestCase):
    def test_demo_completa(self):
        self.assertIn("TOTAL=300.00", demo())

    def test_herencia_y_polimorfismo(self):
        s = crear_sistema()
        self.assertEqual(
            [h.precio_noche() for h in s.habitaciones.ordenar(lambda h: h.numero)], [100, 150, 200]
        )

    def test_interfaz_abstracta(self):
        with self.assertRaises(TypeError):
            CalculadoraTarifa()

    def test_crear_valida(self):
        s = crear_sistema()
        r = s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03").valor
        self.assertEqual((r.total, r.estado), (300, EstadoReserva.CONFIRMADA))

    def test_sin_huesped(self):
        self.assertFalse(
            crear_sistema().crear_reserva("R1", "NO", 102, "2026-11-01", "2026-11-03").es_exito
        )

    def test_fechas_invalidas(self):
        s = crear_sistema()
        with self.assertRaises(ErrorDatosHotel):
            s.crear_reserva("R1", "H1", 102, "2026-11-03", "2026-11-01")
        self.assertEqual(len(s.reservas), 0)

    def test_duplicado(self):
        s = crear_sistema()
        s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03")
        self.assertFalse(s.crear_reserva("R1", "H1", 101, "2026-11-01", "2026-11-03").es_exito)

    def test_solapamiento_rechazado(self):
        s = crear_sistema()
        s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03")
        self.assertFalse(s.crear_reserva("R2", "H1", 102, "2026-11-02", "2026-11-04").es_exito)

    def test_reservas_adyacentes_validas(self):
        s = crear_sistema()
        s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03")
        self.assertTrue(s.crear_reserva("R2", "H1", 102, "2026-11-03", "2026-11-05").es_exito)

    def test_anulada_no_bloquea(self):
        s = crear_sistema()
        s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03")
        s.anular_reserva("R1")
        self.assertTrue(s.crear_reserva("R2", "H1", 102, "2026-11-01", "2026-11-03").es_exito)

    def test_estado_invalido_atomico(self):
        s = crear_sistema()
        r = s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03").valor
        r.ingresar()
        with self.assertRaises(ErrorEstadoReserva):
            r.anular()
        self.assertEqual(r.estado, EstadoReserva.HOSPEDADA)

    def test_filtro_orden_y_result(self):
        s = crear_sistema()
        self.assertEqual(
            [h.numero for h in s.habitaciones.ordenar(lambda h: h.precio_noche(), True)],
            [103, 102, 101],
        )
        self.assertFalse(s.reservas.buscar_resultado("NO").es_exito)

    def test_notificaciones_solo_simuladas(self):
        s = crear_sistema()
        s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03", True)
        self.assertEqual(len(s._notificador.eventos), 1)


if __name__ == "__main__":
    unittest.main()
