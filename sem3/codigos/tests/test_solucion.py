import unittest
from python.solucion import (
    PersonalUniversitario,
    Docente,
    Administrativo,
    Investigador,
    datos,
    planilla,
    verificar_contrato_personal,
    Rectangulo,
    CuadradoIncompatible,
    Cuadrado,
    comprobar_rectangulo,
)
from python.hotel_herencia import HabitacionSimple, HabitacionDoble, Suite, cotizar_habitaciones


class ContratosSemana3(unittest.TestCase):
    def test_base_abstracta(self):
        with self.assertRaises(TypeError):
            PersonalUniversitario("12345678", "A", "B")

    def test_remuneraciones(self):
        self.assertEqual(
            [round(p.calcular_remuneracion(), 2) for p in datos()], [1680, 1620, 3150, 3000]
        )

    def test_planilla_polimorfica(self):
        self.assertEqual(planilla(datos())[-1], "TOTAL=9450.00")

    def test_contrato_todas_subclases(self):
        self.assertTrue(all(verificar_contrato_personal(p) for p in datos()))

    def test_dni_invalido(self):
        with self.assertRaises(ValueError):
            Docente("12", "A", "B", 16, "Asociado")

    def test_horas_limites(self):
        for n in [0, 41]:
            with self.subTest(n=n), self.assertRaises(ValueError):
                Docente("12345678", "A", "B", n, "Asociado")

    def test_categoria(self):
        with self.assertRaises(ValueError):
            Docente("12345678", "A", "B", 16, "Otro")

    def test_salario_y_bono(self):
        for salario, bono in [(0, 0), (2800, -5000), (float("nan"), 0)]:
            with self.subTest(salario=salario), self.assertRaises(ValueError):
                Administrativo("12345678", "A", "B", salario, bono)

    def test_proyectos(self):
        with self.assertRaises(ValueError):
            Investigador("12345678", "A", "B", 0)

    def test_rectangulo_contrato(self):
        comprobar_rectangulo(Rectangulo(2, 2))

    def test_cuadrado_incompatible_falla_como_se_espera(self):
        with self.assertRaises(AssertionError):
            comprobar_rectangulo(CuadradoIncompatible(2))

    def test_figuras_corregidas(self):
        self.assertEqual([f.area() for f in [Rectangulo(4, 5), Cuadrado(5)]], [20, 25])

    def test_hotel_polimorfismo(self):
        hs = [HabitacionSimple(101), HabitacionDoble(102), Suite(103)]
        self.assertEqual(cotizar_habitaciones(hs), [100, 150, 200])
        self.assertTrue(all(h.precio_noche() > 0 for h in hs))


if __name__ == "__main__":
    unittest.main()
