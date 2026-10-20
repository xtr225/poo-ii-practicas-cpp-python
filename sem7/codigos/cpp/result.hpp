#pragma once
#include "comun.hpp"
#include <variant>
struct FalloResult {
    std::string mensaje;
};
template <class T> class Result {
    std::variant<T, FalloResult> dato_;
    Result(std::in_place_index_t<0>, T valor) : dato_(std::in_place_index<0>, std::move(valor)) {}
    Result(std::in_place_index_t<1>, std::string error)
        : dato_(std::in_place_index<1>, FalloResult{std::move(error)}) {}

  public:
    static Result ok(T valor) {
        return Result(std::in_place_index<0>, std::move(valor));
    }
    static Result fallo(std::string mensaje) {
        textoValido(mensaje);
        return Result(std::in_place_index<1>, std::move(mensaje));
    }
    bool esExito() const noexcept {
        return dato_.index() == 0;
    }
    const T &valor() const {
        if (!esExito()) {
            throw std::logic_error("Result fallido");
        }
        return std::get<0>(dato_);
    }
    const std::string &error() const {
        if (esExito()) {
            throw std::logic_error("Un exito no tiene error");
        }
        return std::get<1>(dato_).mensaje;
    }
    template <class F> auto map(F funcion) const -> decltype(funcion(std::declval<const T &>())) {
        using Resultado = decltype(funcion(std::declval<const T &>()));
        if (!esExito()) {
            return Resultado::fallo(error());
        }
        return funcion(valor());
    }
};
