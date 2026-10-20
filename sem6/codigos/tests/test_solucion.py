import unittest
from dataclasses import dataclass
from python.solucion import crear_repositorio, crear_hotel
from python.repositorio import Repositorio, ColaAtencion, PlanillaIterador, ventana_deslizante


class ContratosSemana6(unittest.TestCase):
    def test_guardar_y_buscar(self):
        self.assertEqual(crear_repositorio().buscar(1001).nombre, "Juan")

    def test_guardar_actualiza(self):
        r = Repositorio(lambda x: x[0])
        r.guardar((1, "A"))
        r.guardar((1, "B"))
        self.assertEqual((len(r), r.buscar(1)), (1, (1, "B")))

    def test_insertar_rechaza_duplicado(self):
        r = Repositorio(lambda x: x)
        r.insertar(1)
        with self.assertRaises(ValueError):
            r.insertar(1)

    def test_eliminar_valor_falso(self):
        r = Repositorio(lambda x: x)
        r.guardar(0)
        self.assertTrue(r.eliminar(0))
        self.assertFalse(r.eliminar(0))

    def test_resultado_none_guardado(self):
        r = Repositorio(lambda x: 1)
        r.guardar(None)
        self.assertTrue(r.buscar_resultado(1).es_exito)

    def test_busqueda_ausente(self):
        self.assertFalse(crear_repositorio().buscar_resultado(9999).es_exito)

    def test_consultas_dominio(self):
        r = crear_repositorio()
        self.assertEqual(r.contar(lambda e: e.ppa >= 14 and e.escuela == "Ing. Sistemas"), 2)
        self.assertEqual(
            [e.codigo for e in r.ordenar(lambda e: e.ppa, True)], [1002, 1001, 1004, 1003]
        )

    def test_estadisticas(self):
        self.assertEqual(crear_repositorio().estadisticas("ppa")["promedio"], 14.125)

    def test_repositorio_vacio(self):
        self.assertEqual(Repositorio(lambda e: e).estadisticas("x")["n"], 0)

    def test_copia_lista_no_modifica_contenedor(self):
        r = crear_repositorio()
        r.todos().clear()
        self.assertEqual(len(r), 4)

    def test_fifo(self):
        c = ColaAtencion()
        c.encolar(0)
        c.encolar(1)
        self.assertEqual([c.atender(), c.atender(), c.atender()], [0, 1, None])

    def test_iterador_reiniciable(self):
        p = PlanillaIterador([1, 3, 2], key=lambda x: x)
        self.assertEqual(list(p), [3, 2, 1])
        self.assertEqual(list(p), [3, 2, 1])

    def test_ventanas(self):
        self.assertEqual(
            list(ventana_deslizante([12, 15, 18, 14, 16], 3)),
            [[12, 15, 18], [15, 18, 14], [18, 14, 16]],
        )

    def test_ventanas_invalidas(self):
        for k in [-1, 0, 4, True]:
            with self.subTest(k=k), self.assertRaises(ValueError):
                list(ventana_deslizante([1, 2, 3], k))

    def test_consultas_hotel(self):
        r = crear_hotel()
        self.assertEqual(r.contar(lambda h: h.plazas_libres >= 2), 2)
        self.assertEqual(r.acumular(0, lambda s, h: s + h.precio_noche()) / len(r), 142.5)


if __name__ == "__main__":
    unittest.main()
