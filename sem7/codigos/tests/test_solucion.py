import unittest
from python.estados import EstadoMatricula, EstadoReserva, MaquinaEstados, Matricula
from python.hotel_integrado import crear_sistema
from python.errores_hotel import ErrorEstadoReserva


class ContratosSemana7(unittest.TestCase):
    def test_estado_inicial(self):
        self.assertEqual(Matricula().estado, EstadoMatricula.REGISTRADA)

    def test_secuencia_valida(self):
        m = Matricula()
        m.validar_pago()
        m.suspender("deuda")
        m.regularizar()
        m.anular()
        self.assertEqual((m.estado, len(m.historial)), (EstadoMatricula.ANULADA, 5))

    def test_final_irreversible(self):
        m = Matricula()
        m.anular()
        with self.assertRaises(ValueError):
            m.validar_pago()

    def test_suspension_invalida_no_cambia_motivo(self):
        m = Matricula()
        with self.assertRaises(ValueError):
            m.suspender("deuda")
        self.assertEqual(
            (m.estado, m.motivo, len(m.historial)), (EstadoMatricula.REGISTRADA, "", 1)
        )

    def test_regularizar_requiere_suspension(self):
        with self.assertRaises(ValueError):
            Matricula().regularizar()

    def test_historial_solo_consulta(self):
        self.assertIsInstance(Matricula().historial, tuple)

    def test_guardia(self):
        m = MaquinaEstados("A", {("A", "B"): lambda c: c.get("pago", False)})
        with self.assertRaises(ValueError):
            m.transicionar("B", {})
        self.assertEqual(m.estado, "A")
        m.transicionar("B", {"pago": True})
        self.assertEqual(m.estado, "B")

    def test_reserva_secuencia_y_final(self):
        s = crear_sistema()
        r = s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03").valor
        r.ingresar()
        r.finalizar()
        self.assertEqual(r.estado, EstadoReserva.FINALIZADA)
        with self.assertRaises(ErrorEstadoReserva):
            r.anular()

    def test_anulacion_libera_disponibilidad(self):
        s = crear_sistema()
        s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03")
        s.anular_reserva("R1")
        self.assertEqual(len(s.disponibles("2026-11-01", "2026-11-03")), 3)


if __name__ == "__main__":
    unittest.main()
