"""El destino de auditoría se elige al construir el servicio."""
from pathlib import Path


class AuditoriaMemoria:
    def __init__(self):
        self._eventos: list[str] = []

    def registrar(self, evento: str) -> None:
        self._eventos.append(evento)

    @property
    def eventos(self) -> tuple[str, ...]:
        return tuple(self._eventos)


class AuditoriaArchivo:
    def __init__(self, ruta: str | Path):
        self._ruta = Path(ruta)

    def registrar(self, evento: str) -> None:
        with self._ruta.open("a", encoding="utf-8") as archivo:
            archivo.write(evento + "\n")
