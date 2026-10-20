#include "comun.hpp"
#include <chrono>
#include <fstream>
#include <filesystem>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
using Reloj = std::chrono::steady_clock;
inline double ms(Reloj::time_point inicio) {
    return std::chrono::duration<double, std::milli>(Reloj::now() - inicio).count();
}
template <class Crear, class Buscar>
void medir(std::ofstream &salida, int n, const std::string &nombre, Crear crear, Buscar buscar) {
    std::vector<double> construcciones, busquedas;
    int aciertos = 0;
    for (int repeticion = 0; repeticion < 3; ++repeticion) {
        auto inicio = Reloj::now();
        auto contenedor = crear();
        construcciones.push_back(ms(inicio));
        inicio = Reloj::now();
        int total = 0;
        for (int i = 0; i < 200; ++i) {
            int q = (i * 7919 + 206) % (2 * n);
            if (buscar(contenedor, q)) {
                ++total;
            }
        }
        busquedas.push_back(ms(inicio));
        aciertos = total;
    }
    std::sort(construcciones.begin(), construcciones.end());
    std::sort(busquedas.begin(), busquedas.end());
    salida << "Cpp," << n << ',' << nombre << ",200,3," << construcciones[1] << ',' << busquedas[1]
           << ',' << aciertos << '\n';
}
int main() {
    std::filesystem::create_directories("evidencias");
    std::ofstream salida("evidencias/benchmark_cpp.csv");
    if (!salida) {
        return 1;
    }
    salida << "lenguaje,n,estructura,consultas,repeticiones,construccion_ms,busqueda_lote_ms,"
              "aciertos\n";
    for (int n : {1000, 10000, 100000, 1000000}) {
        auto vector = [n] {
            std::vector<int> v(static_cast<std::size_t>(n));
            std::iota(v.begin(), v.end(), 0);
            return v;
        };
        medir(salida, n, "vector", vector,
              [](const auto &c, int q) { return std::find(c.begin(), c.end(), q) != c.end(); });
        medir(
            salida, n, "unordered_map",
            [n] {
                std::unordered_map<int, int> c;
                for (int i = 0; i < n; ++i) {
                    c.emplace(i, i);
                }
                return c;
            },
            [](const auto &c, int q) { return c.find(q) != c.end(); });
        medir(
            salida, n, "unordered_set",
            [n] {
                std::unordered_set<int> c;
                for (int i = 0; i < n; ++i) {
                    c.emplace(i);
                }
                return c;
            },
            [](const auto &c, int q) { return c.find(q) != c.end(); });
        medir(salida, n, "binary_search", vector,
              [](const auto &c, int q) { return std::binary_search(c.begin(), c.end(), q); });
    }
    std::cout << "Benchmark C++ guardado con tiempos medidos y construccion separada.\n";
}
