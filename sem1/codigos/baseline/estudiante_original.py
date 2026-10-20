# Actividad 1: identificar los problemas de diseño del ejemplo original.
class est:
    def __init__(self, n, a, p):
        self.n = n
        self.a = a
        self.p = p
        self.h = []

    def add(self, c, n):
        self.h.append([c, n])

    def ppa(self):
        if len(self.h) == 0:
            return 0
        t = 0
        for i in self.h:
            t = t + i[1]
        return t / len(self.h)

    def pr(self):
        print(self.n, self.a, self.p, self.ppa())
        for i in self.h:
            print(i)


e1 = est("Juan Mamani", 20210500, "Ing. Sistemas")
e1.add("POO II", 16)
e1.add("BD I", 14)
e1.pr()
