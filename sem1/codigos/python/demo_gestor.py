from .auditoria import AuditoriaMemoria
from .reportes import GeneradorCSV
from .reportes_json import GeneradorJSON
from .repositorio import RepositorioEstudiantes
from .servicio import ServicioAcademico


def main():
    servicio = ServicioAcademico(RepositorioEstudiantes(), AuditoriaMemoria())
    servicio.registrar("E001", "Ana")
    servicio.agregar_nota("E001", 10)
    servicio.agregar_nota("E001", 11)
    print(servicio.reporte(GeneradorCSV()))
    print(servicio.reporte(GeneradorJSON()))


if __name__ == "__main__":
    main()
