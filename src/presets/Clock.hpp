#pragma once

#include "fw16led/Preset.hpp"
#include "fw16led/PresetRegistry.hpp"
#include <QTimer>

namespace fw16led::presets
{
  class Clock : public Preset
  {
  public:
    Clock();
    virtual ~Clock() = default;
    void init(std::shared_ptr<ledmatrix::LedMatrix> panel) override;
    void exit() override;
    std::vector<PresetOptionConfig> getOptions() const override;
    static void registerPreset(std::shared_ptr<PresetRegistry> registry);

  private:
    std::shared_ptr<ledmatrix::LedMatrix> panel;
    QTimer* timer;
    void render();
  };

} // namespace fw16led::presets
