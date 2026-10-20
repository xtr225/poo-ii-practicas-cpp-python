"""Jerarquía académica con contexto y causas preservadas."""


class ErrorAcademico(Exception):
    def __init__(self, mensaje, contexto=None):
        super().__init__(mensaje)
        self.contexto = dict(contexto or {})

    def __str__(self):
        return super().__str__() + f" | contexto={self.contexto}"


class ErrorValidacion(ErrorAcademico):
    pass


class ErrorCalificacion(ErrorValidacion):
    def __init__(self, valor):
        super().__init__("Calificacion fuera de 0 a 20", {"valor": valor})
        self.valor_invalido = valor


class ErrorCapacidad(ErrorAcademico):
    def __init__(self, actual, maximo, entidad):
        super().__init__(
            "Capacidad invalida", {"actual": actual, "maximo": maximo, "entidad": entidad}
        )


class ErrorRecursoNoEncontrado(ErrorAcademico):
    def __init__(self, tipo, identificador):
        super().__init__("Recurso no encontrado", {"tipo": tipo, "id": identificador})


class ErrorMatriculaDuplicada(ErrorValidacion):
    def __init__(self, codigo):
        super().__init__("Matricula duplicada", {"codigo_estudiante": str(codigo)})
        self.codigo_estudiante = str(codigo)


def validar_matricula(codigo, matriculados):
    if codigo in matriculados:
        raise ErrorMatriculaDuplicada(codigo)


def cargar_nota_desde_archivo(ruta, codigo):
    try:
        with open(ruta, encoding="utf-8") as archivo:
            return float(archivo.read())
    except OSError as causa:
        raise ErrorRecursoNoEncontrado("Nota", codigo) from causa
    except ValueError as causa:
        raise ErrorValidacion("Contenido no numerico", {"ruta": str(ruta)}) from causa
