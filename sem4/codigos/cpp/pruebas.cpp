#include <cstdio>
#include "solucion.hpp"
#include <type_traits>
class ExportadorMock : public Exportador {
  public:
    mutable int llamadas = 0;
    std::string extension() const override {
        return ".mock";
    }
    void exportar(const std::string &, const std::string &) const override {
        ++llamadas;
    }
};
std::vector<std::unique_ptr<INotificador>> canales() {
    std::vector<std::unique_ptr<INotificador>> ns;
    ns.push_back(std::make_unique<NotificadorEmail>("local"));
    ns.push_back(std::make_unique<NotificadorSMS>("local"));
    return ns;
}
int main() {
    Pruebas t;
    t.caso("interfaz abstracta", [] { exigir(std::is_abstract<INotificador>::value); });
    t.caso("dos canales", [] {
        SistemaAlertas s(canales());
        exigir(s.alertar("d", "mensaje") == 2);
    });
    t.caso("indisponible", [] {
        auto ns = canales();
        static_cast<NotificadorEmail *>(ns[0].get())->setActivo(false);
        SistemaAlertas s(std::move(ns));
        exigir(s.alertar("d", "mensaje") == 1);
    });
    t.caso("sms limites", [] {
        NotificadorSMS n("local");
        exigir(n.enviar("d", std::string(160, 'x')));
        debeFallar([&] { n.enviar("d", std::string(161, 'x')); });
    });
    t.caso("errores aislados", [] {
        SistemaAlertas s(canales());
        exigir(s.alertar("d", std::string(161, 'x')) == 1 && s.errores.size() == 1);
    });
    t.caso("vacio", [] { debeFallar([] { SistemaAlertas s({}); }); });
    t.caso("mock", [] {
        auto mock = std::make_unique<ExportadorMock>();
        auto *vista = mock.get();
        GeneradorReporte g(std::move(mock));
        exigir(g.generar("datos", "prueba") == "prueba.mock");
        exigir(vista->llamadas == 1);
    });
    t.caso("exportacion y firma", [] {
        GeneradorReporte g(std::make_unique<ExportadorCSV>(), std::make_unique<FirmadorDigital>());
        exigir(g.generar("n\n45\n", "reporte_prueba_exportacion") == "reporte_prueba_exportacion.csv.signed");
        g.cambiarExportador(std::make_unique<ExportadorJSON>());
        exigir(g.generar("{\"n\":45}", "reporte_prueba_exportacion") == "reporte_prueba_exportacion.json.signed");
        for (const auto *extension : {".csv", ".csv.signed", ".json", ".json.signed"}) {
            std::remove((std::string("reporte_prueba_exportacion") + extension).c_str());
        }
    });
    t.caso("inyeccion hotel", [] {
        ServicioCotizacion s(std::make_unique<TarifaEstandar>());
        exigir(s.cotizar(HabitacionDoble(102), 2) == 300);
    });
    t.caso("noches invalidas", [] {
        ServicioCotizacion s(std::make_unique<TarifaEstandar>());
        debeFallar([&] { s.cotizar(HabitacionDoble(102), 0); });
    });
    return t.terminar();
}
