"""Construcción y búsqueda medidas por separado; consultas iguales y reproducibles."""

import bisect
import csv
import statistics
import time
from pathlib import Path


def medir(funcion, repeticiones=3):
    muestras = []
    for _ in range(repeticiones):
        inicio = time.perf_counter()
        valor = funcion()
        muestras.append((time.perf_counter() - inicio) * 1000)
    return statistics.median(muestras), valor


def benchmark():
    filas = []
    for n in [1000, 10000, 100000, 1000000]:
        consultas = [(i * 7919 + 206) % (2 * n) for i in range(200)]
        for nombre, crear in [
            ("list", lambda: list(range(n))),
            ("dict", lambda: dict.fromkeys(range(n))),
            ("set", lambda: set(range(n))),
            ("bisect", lambda: list(range(n))),
        ]:
            construccion, contenedor = medir(crear)
            if nombre == "bisect":

                def buscar():
                    total = 0
                    for q in consultas:
                        posicion = bisect.bisect_left(contenedor, q)
                        total += posicion < len(contenedor) and contenedor[posicion] == q
                    return total

            else:

                def buscar():
                    return sum(q in contenedor for q in consultas)

            busqueda, aciertos = medir(buscar)
            filas.append(
                {
                    "lenguaje": "Python",
                    "n": n,
                    "estructura": nombre,
                    "consultas": 200,
                    "repeticiones": 3,
                    "construccion_ms": construccion,
                    "busqueda_lote_ms": busqueda,
                    "aciertos": aciertos,
                }
            )
    salida = Path(__file__).resolve().parents[1] / "evidencias/benchmark_python.csv"
    salida.parent.mkdir(parents=True, exist_ok=True)
    with salida.open("w", newline="", encoding="utf-8") as archivo:
        escritor = csv.DictWriter(archivo, fieldnames=filas[0].keys())
        escritor.writeheader()
        escritor.writerows(filas)
    print("Benchmark Python guardado con tiempos medidos y construccion separada.")


if __name__ == "__main__":
    benchmark()
