"""Consultas académicas y adaptación hotelera a partir del código disponible."""

from dataclasses import dataclass
from .repositorio import Repositorio, ColaAtencion, PlanillaIterador, ventana_deslizante
from .hotel_herencia import HabitacionSimple, HabitacionDoble, Suite


@dataclass(frozen=True)
class Estudiante:
    codigo: int
    nombre: str
    ppa: float
    escuela: str


def crear_repositorio():
    r = Repositorio(lambda e: e.codigo)
    for e in [
        Estudiante(1001, "Juan", 15.2, "Ing. Sistemas"),
        Estudiante(1002, "Maria", 18.5, "Ing. Sistemas"),
        Estudiante(1003, "Pedro", 9.8, "Ciencias Comp."),
        Estudiante(1004, "Ana", 13, "Ing. Sistemas"),
    ]:
        r.guardar(e)
    return r


def crear_hotel():
    r = Repositorio(lambda h: h.numero)
    for h in [HabitacionSimple(101), HabitacionDoble(102), Suite(103), HabitacionSimple(104, 120)]:
        r.guardar(h)
    return r


def demo():
    r = crear_repositorio()
    hotel = crear_hotel()
    c = ColaAtencion()
    c.encolar("Ana")
    c.encolar("Juan")
    ranking = r.ordenar(lambda e: e.ppa, reverse=True)
    return [
        f"DESTACADOS={r.contar(lambda e:e.ppa>=14 and e.escuela=='Ing. Sistemas')}",
        "RANKING=" + ",".join(str(e.codigo) for e in ranking),
        f"PROMEDIO={r.estadisticas('ppa')['promedio']:.3f}",
        f"HABITACIONES_DOS_PLAZAS={hotel.contar(lambda h:h.plazas_libres>=2)}",
        "RANKING_HOTEL="
        + ",".join(str(h.numero) for h in hotel.ordenar(lambda h: h.precio_noche(), reverse=True)),
        f"PROMEDIO_HOTEL={hotel.acumular(0,lambda s,h:s+h.precio_noche())/len(hotel):.2f}",
        f"FIFO={c.atender()}",
        f"VENTANAS={len(list(ventana_deslizante([12,15,18,14,16],3)))}",
    ]
