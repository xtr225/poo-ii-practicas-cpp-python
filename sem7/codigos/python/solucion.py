from .estados import Matricula, EstadoMatricula, MaquinaEstados
from .hotel_integrado import crear_sistema


def demo():
    m = Matricula()
    m.validar_pago()
    m.suspender("deuda")
    m.regularizar()
    m.anular()
    s = crear_sistema()
    r = s.crear_reserva("R1", "H1", 102, "2026-11-01", "2026-11-03").valor
    r.ingresar()
    r.finalizar()
    return [
        f"MATRICULA={m.estado.name}",
        f"HISTORIAL_MATRICULA={len(m.historial)}",
        f"RESERVA={r.estado.name}",
        f"HISTORIAL_RESERVA={len(r.historial)}",
    ]
