#pragma once
#include "hotel_base.hpp"
#include "result.hpp"
class ErrorHotel : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
    virtual ~ErrorHotel() = default;
};
class ErrorDatosHotel : public ErrorHotel {
  public:
    using ErrorHotel::ErrorHotel;
};
class ErrorEstadoReserva : public ErrorHotel {
  public:
    using ErrorHotel::ErrorHotel;
};
class ErrorDisponibilidad : public ErrorHotel {
  public:
    using ErrorHotel::ErrorHotel;
};
