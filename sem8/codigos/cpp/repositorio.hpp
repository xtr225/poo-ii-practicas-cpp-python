#pragma once
#include "comun.hpp"
#include "result.hpp"
#include <deque>
#include <optional>
#include <unordered_map>
template <class T, class ID> class Repositorio {
    std::function<ID(const T &)> fnId_;
    std::unordered_map<ID, T> datos_;

  public:
    explicit Repositorio(std::function<ID(const T &)> fn) : fnId_(std::move(fn)) {
        if (!fnId_) {
            throw std::invalid_argument("Extractor requerido");
        }
    }
    void guardar(T entidad) {
        auto id = fnId_(entidad);
        datos_.insert_or_assign(id, std::move(entidad));
    }
    void insertar(T entidad) {
        if (existe(fnId_(entidad))) {
            throw std::invalid_argument("ID duplicado");
        }
        guardar(std::move(entidad));
    }
    std::optional<T> buscar(const ID &id) const {
        auto it = datos_.find(id);
        if (it == datos_.end()) {
            return std::nullopt;
        }
        return it->second;
    }
    Result<T> buscarResultado(const ID &id) const {
        auto r = buscar(id);
        return r ? Result<T>::ok(*r) : Result<T>::fallo("Recurso no encontrado");
    }
    bool eliminar(const ID &id) {
        return datos_.erase(id) > 0;
    }
    bool existe(const ID &id) const {
        return datos_.find(id) != datos_.end();
    }
    std::size_t tamano() const noexcept {
        return datos_.size();
    }
    std::vector<T> todos() const {
        std::vector<T> r;
        for (const auto &par : datos_) {
            r.push_back(par.second);
        }
        return r;
    }
    template <class F> std::vector<T> filtrar(F pred) const {
        std::vector<T> r;
        for (const auto &par : datos_) {
            if (pred(par.second)) {
                r.push_back(par.second);
            }
        }
        return r;
    }
    template <class F> std::vector<T> ordenar(F menor) const {
        auto r = todos();
        std::sort(r.begin(), r.end(), menor);
        return r;
    }
    template <class A, class F> A acumular(A inicial, F fn) const {
        for (const auto &par : datos_) {
            inicial = fn(inicial, par.second);
        }
        return inicial;
    }
    template <class F> std::size_t contar(F pred) const {
        return filtrar(pred).size();
    }
    template <class F> std::optional<T> primero(F pred) const {
        for (const auto &par : datos_) {
            if (pred(par.second)) {
                return par.second;
            }
        }
        return std::nullopt;
    }
    auto begin() const {
        return datos_.cbegin();
    }
    auto end() const {
        return datos_.cend();
    }
};
template <class T> class ColaAtencion {
    std::deque<T> cola_;

  public:
    void encolar(T valor) {
        cola_.push_back(std::move(valor));
    }
    std::optional<T> atender() {
        if (cola_.empty()) {
            return std::nullopt;
        }
        T valor = std::move(cola_.front());
        cola_.pop_front();
        return valor;
    }
    std::size_t tamano() const noexcept {
        return cola_.size();
    }
};
template <class T> class PlanillaIterador {
    std::vector<T> ordenados_;

  public:
    template <class F>
    PlanillaIterador(std::vector<T> datos, F menor) : ordenados_(std::move(datos)) {
        std::sort(ordenados_.begin(), ordenados_.end(), menor);
    }
    auto begin() const {
        return ordenados_.cbegin();
    }
    auto end() const {
        return ordenados_.cend();
    }
};
template <class T> std::vector<std::vector<T>> ventanas(const std::vector<T> &datos, int k) {
    if (k <= 0 || static_cast<std::size_t>(k) > datos.size()) {
        throw std::invalid_argument("Ventana invalida");
    }
    std::vector<std::vector<T>> salida;
    for (std::size_t i = 0; i + static_cast<std::size_t>(k) <= datos.size(); ++i) {
        salida.emplace_back(datos.begin() + i, datos.begin() + i + k);
    }
    return salida;
}
