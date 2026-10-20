"""Actividad 2: mismo caso y mensajes que cpp/main.cpp."""
from .dominio import Estudiante


def main():
    estudiante = Estudiante("Juan Carlos Mamani Quispe", 20210500, "Ing. Sistemas")
    estudiante.agregar_nota("POO II", 16)
    estudiante.agregar_nota("Base de Datos I", 14)
    print(estudiante)
    print(f"PPA: {estudiante.calcular_ppa():.2f}")
    try:
        estudiante.agregar_nota("Redes", 25)
    except ValueError as error:
        print(f"Error capturado: {error}")


if __name__ == "__main__":
    main()
