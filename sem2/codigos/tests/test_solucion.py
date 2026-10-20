import unittest
from datetime import date
from python.solucion import Curso, Docente
from python.hotel_base import Huesped, Habitacion, Reserva


class ContratosSemana2(unittest.TestCase):
    def curso(self):
        return Curso("SIS206", "POO", 4, 3)

    def docente(self):
        return Docente("12345678", "Ana", "Mamani", "ana@unap.edu.pe", date(1980, 5, 14))

    def test_constructor_valido(self):
        self.assertEqual(self.curso().creditos, 4)

    def test_codigos_invalidos(self):
        for codigo in ["X9", "sis206", "SIS206x", "SIS１２３"]:
            with self.subTest(codigo=codigo), self.assertRaises(ValueError):
                Curso(codigo, "POO", 4, 30)

    def test_creditos_fronteras(self):
        for credito in [1, 6]:
            self.assertEqual(Curso("AB123", "X", credito, 30).creditos, credito)
        for credito in [0, 7, True, 1.5]:
            with self.subTest(c=credito), self.assertRaises(ValueError):
                Curso("SIS206", "X", credito, 30)

    def test_cupos_fronteras(self):
        for cupos in [0, 51]:
            with self.subTest(c=cupos), self.assertRaises(ValueError):
                Curso("SIS206", "X", 4, cupos)

    def test_curso_lleno(self):
        c = self.curso()
        for _ in range(3):
            c.inscribir_estudiante()
        with self.assertRaises(RuntimeError):
            c.inscribir_estudiante()
        self.assertTrue(c.esta_lleno)

    def test_no_reducir_cupos_bajo_inscritos(self):
        c = self.curso()
        c.inscribir_estudiante()
        c.inscribir_estudiante()
        with self.assertRaises(ValueError):
            c.cupos_maximo = 1
        self.assertEqual(c.cupos_maximo, 3)

    def test_propiedades_solo_lectura(self):
        c = self.curso()
        for attr in ["inscritos", "cupos_disponibles", "esta_lleno"]:
            with self.subTest(attr=attr), self.assertRaises(AttributeError):
                setattr(c, attr, 99)

    def test_name_mangling(self):
        with self.assertRaises(AttributeError):
            getattr(self.curso(), "__codigo")

    def test_dni_correo(self):
        for dni, correo in [("123", "ana@unap.edu.pe"), ("12345678", "ana@unap.edu.pe.evil")]:
            with self.subTest(dni=dni), self.assertRaises(ValueError):
                Docente(dni, "Ana", "Mamani", correo, date(1980, 5, 14))

    def test_edad_antes_y_en_cumpleanos(self):
        d = self.docente()
        self.assertEqual(d.edad_en(date(2026, 5, 13)), 45)
        self.assertEqual(d.edad_en(date(2026, 5, 14)), 46)

    def test_propiedades_derivadas_cambian_con_curso(self):
        d, c = self.docente(), self.curso()
        d.asignar_curso(c)
        c.creditos = 6
        c.inscribir_estudiante()
        self.assertEqual((d.total_creditos, d.total_inscritos), (6, 1))

    def test_asignacion_limite_tres_y_duplicado(self):
        d = self.docente()
        for n in [206, 207, 208]:
            d.asignar_curso(Curso(f"SIS{n}", "X", 4, 30))
        with self.assertRaises(RuntimeError):
            d.asignar_curso(Curso("SIS209", "X", 4, 30))
        with self.assertRaises(ValueError):
            d.asignar_curso(d.cursos[0])

    def test_huesped_documento(self):
        h = Huesped("H1", "Ana", "12345678")
        with self.assertRaises(ValueError):
            h.actualizar_documento("12")
        self.assertEqual(h.documento, "12345678")

    def test_habitacion_capacidad(self):
        h = Habitacion(101, "Doble", 2, 100)
        h.alojar(2)
        with self.assertRaises(ValueError):
            h.capacidad = 1
        self.assertEqual(h.plazas_libres, 0)

    def test_habitacion_tarifa(self):
        with self.assertRaises(ValueError):
            Habitacion(101, "Doble", 2, float("nan"))

    def test_reserva_fechas_y_negocio(self):
        r = Reserva("R1", "2026-10-18", "2026-10-20")
        self.assertEqual(r.noches, 2)
        r.confirmar()
        r.anular()
        with self.assertRaises(RuntimeError):
            r.confirmar()

    def test_reserva_fecha_invalida(self):
        for fin in ["2026-10-18", "2026-02-30"]:
            with self.subTest(fin=fin), self.assertRaises(ValueError):
                Reserva("R1", "2026-10-18", fin)


if __name__ == "__main__":
    unittest.main()
