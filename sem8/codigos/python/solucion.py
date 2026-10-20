"""Demostración EVI1 reproducible; toda notificación es una simulación en memoria."""

from .hotel_integrado import crear_sistema
from .errores_hotel import ErrorDatosHotel


def demo():
    s = crear_sistema()
    precios = ",".join(
        f"{h.precio_noche():.2f}" for h in s.habitaciones.ordenar(lambda h: h.numero)
    )
    r = s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03", True).valor
    conflicto = s.crear_reserva("R2", "H1", 102, "2026-11-02", "2026-11-04")
    error = ""
    try:
        s.crear_reserva("R3", "H1", 101, "2026-11-03", "2026-11-01")
    except ErrorDatosHotel:
        error = "ErrorDatosHotel"
    disponibles = ",".join(str(h.numero) for h in s.disponibles("2026-11-01", "2026-11-03"))
    ordenadas = ",".join(str(h.numero) for h in s.habitaciones.ordenar(lambda h: h.precio_noche()))
    s.anular_reserva("R1", True)
    return [
        f"PRECIOS={precios}",
        f"RESERVA={r.codigo}",
        f"TOTAL={r.total:.2f}",
        f"CONFLICTO={conflicto.error}",
        f'BUSQUEDA={s.reservas.buscar_resultado("NO").error}',
        f"ERROR_DOMINIO={error}",
        f"DISPONIBLES={disponibles}",
        f"ORDENADAS={ordenadas}",
        f"ANULADA={r.estado.name}",
        f'DISPONIBLES_TRAS_ANULAR={len(s.disponibles("2026-11-01","2026-11-03"))}',
    ]
