#pragma once

#include "fw16led/Preset.hpp"
#include "fw16led/ledmatrix/ledmatrix.hpp"
#include <cstdint>
#include <memory>

namespace fw16led
{
  class LedPanel
  {
  public:
    LedPanel(std::shared_ptr<ledmatrix::LedMatrix> ledMatrix);
    ~LedPanel();

    void applyConfig();

    inline uint8_t getId() const { return id; }

  private:
    uint8_t id;
    std::shared_ptr<Preset> currentPreset = nullptr;
    std::shared_ptr<ledmatrix::LedMatrix> ledMatrix;
  };
} // namespace fw16led