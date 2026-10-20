import unittest
from datetime import date, timedelta
from pathlib import Path
from tempfile import TemporaryDirectory
from python.solucion import Matricula, pipeline, validar_ppa_minimo
from python.excepciones import *
from python.result import Result
from python.errores_hotel import *


class ContratosSemana5(unittest.TestCase):
    def test_constructor_valido(self):
        self.assertEqual(Matricula("E1", ["SIS206"]).cursos, ("SIS206",))

    def test_sin_cursos(self):
        with self.assertRaises(ErrorCapacidad):
            Matricula("E1", [])

    def test_maximo_seis(self):
        with self.assertRaises(ErrorCapacidad):
            Matricula("E1", [str(n) for n in range(7)])

    def test_duplicados(self):
        with self.assertRaises(ErrorValidacion):
            Matricula("E1", ["SIS206", "SIS206"])

    def test_fecha_futura(self):
        with self.assertRaises(ErrorValidacion):
            Matricula("E1", ["SIS206"], date.today() + timedelta(days=1))

    def test_agregar_y_eliminar(self):
        m = Matricula("E1", ["SIS206"])
        m.agregar_curso("SIS207")
        m.eliminar_curso("SIS207")
        self.assertEqual(m.cursos, ("SIS206",))

    def test_no_eliminar_ultimo(self):
        m = Matricula("E1", ["SIS206"])
        with self.assertRaises(ErrorValidacion):
            m.eliminar_curso("SIS206")
        self.assertEqual(len(m.cursos), 1)

    def test_anular_impide_modificar(self):
        m = Matricula("E1", ["SIS206"])
        m.anular()
        with self.assertRaises(ErrorAcademico):
            m.agregar_curso("SIS207")
        with self.assertRaises(ErrorAcademico):
            m.eliminar_curso("SIS206")

    def test_diagnostico_e_invariante(self):
        self.assertIn("OK", Matricula("E1", ["SIS206"]).diagnostico())

    def test_matricula_duplicada_contexto(self):
        with self.assertRaises(ErrorMatriculaDuplicada) as c:
            validar_matricula(1001, [1001])
        self.assertEqual(c.exception.codigo_estudiante, "1001")
        self.assertIsInstance(c.exception, ErrorValidacion)

    def test_causa_preservada(self):
        with TemporaryDirectory() as d:
            with self.assertRaises(ErrorRecursoNoEncontrado) as c:
                cargar_nota_desde_archivo(Path(d) / "ausente", 1001)
            self.assertIsInstance(c.exception.__cause__, OSError)

    def test_result_string_exito_y_fallo(self):
        self.assertEqual(Result.ok("valor").valor, "valor")
        self.assertEqual(Result.fallo("error").error, "error")

    def test_result_impide_acceso_incorrecto(self):
        with self.assertRaises(RuntimeError):
            _ = Result.fallo("error").valor
        with self.assertRaises(RuntimeError):
            _ = Result.ok("ok").error

    def test_pipeline_exito_y_fallo(self):
        self.assertTrue(pipeline(1001).es_exito)
        self.assertFalse(pipeline(9999).es_exito)

    def test_cortocircuito(self):
        llamadas = []
        r = Result.fallo("error").map(lambda x: llamadas.append(x))
        self.assertFalse(r.es_exito)
        self.assertEqual(llamadas, [])

    def test_ppa_insuficiente(self):
        self.assertFalse(validar_ppa_minimo({"ppa": 10}).es_exito)

    def test_hotel_valido_habitacion(self):
        h = HabitacionRobusta(101, "Doble", 2, 100)
        h.alojar(1)
        h._verificar_invariante()
        self.assertEqual(h.plazas_libres, 1)

    def test_hotel_valida_reserva(self):
        r = ReservaRobusta("R1", "2026-10-18", "2026-10-20")
        r.confirmar()
        r._verificar_invariante()
        self.assertEqual(r.noches, 2)

    def test_hotel_busqueda_result(self):
        self.assertFalse(buscar_habitacion([], 101).es_exito)

    def test_hotel_constructor_invalido(self):
        with self.assertRaises(ErrorDatosHotel):
            HabitacionRobusta(101, "Doble", 0, 100)

    def test_hotel_capacidad(self):
        with self.assertRaises(ErrorDisponibilidad):
            HabitacionRobusta(101, "Doble", 2, 100).alojar(3)

    def test_hotel_estado(self):
        r = ReservaRobusta("R1", "2026-10-18", "2026-10-20")
        r.anular()
        with self.assertRaises(ErrorEstadoReserva):
            r.confirmar()


if __name__ == "__main__":
    unittest.main()
