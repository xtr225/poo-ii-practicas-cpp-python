"""Parte II, página 10: se recuperó solamente la indentación."""


class GestorAcademico:
    def __init__(self):
        self.estudiantes = []

    def registrar(self, codigo, nombre):
        self.estudiantes.append({"codigo": codigo, "nombre": nombre, "notas": []})

    def agregar_nota(self, codigo, nota):
        for e in self.estudiantes:
            if e["codigo"] == codigo:
                e["notas"].append(nota)
                with open("auditoria.txt", "a", encoding="utf-8") as f:
                    f.write(f"nota:{codigo}:{nota}\n")

    def reporte(self):
        salida = []
        for e in self.estudiantes:
            promedio = sum(e["notas"]) / len(e["notas"]) if e["notas"] else 0
            estado = "APROBADO" if promedio >= 10.5 else "DESAPROBADO"
            salida.append(f'{e["codigo"]};{e["nombre"]};{promedio:.2f};{estado}')
        with open("reporte.csv", "w", encoding="utf-8") as f:
            f.write("\n".join(salida))
        print("Reporte generado")


    def reporte_json(self):
        import json
        salida = []
        for estudiante in self.estudiantes:
            notas = estudiante["notas"]
            promedio = sum(notas) / len(notas) if notas else 0
            salida.append({
                "codigo": estudiante["codigo"],
                "nombre": estudiante["nombre"],
                "promedio": promedio,
                "estado": "APROBADO" if promedio >= 10.5 else "DESAPROBADO",
            })
        return json.dumps(salida, ensure_ascii=False)
