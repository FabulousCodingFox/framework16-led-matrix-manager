#pragma once

#include "fw16led/Preset.hpp"
#include "fw16led/PresetRegistry.hpp"
#include <QTimer>

namespace fw16led::presets
{
  class Text : public Preset
  {
  public:
    Text();
    virtual ~Text() = default;
    void init(std::shared_ptr<ledmatrix::LedMatrix> panel) override;
    void exit() override;
    std::vector<PresetOptionConfig> getOptions() const override;
    static void registerPreset(std::shared_ptr<PresetRegistry> registry);

  private:
    std::shared_ptr<ledmatrix::LedMatrix> panel;
    QTimer* timer;
  };

} // namespace fw16led::presets
