#pragma once

#include "fw16led/Preset.hpp"
#include "fw16led/PresetRegistry.hpp"

namespace fw16led::presets
{
  class ZigZag : public Preset
  {
  public:
    ZigZag();
    virtual ~ZigZag() = default;
    void init(std::shared_ptr<ledmatrix::LedMatrix> panel) override;
    void exit() override;
    std::vector<PresetOptionConfig> getOptions() const override;
    static void registerPreset(std::shared_ptr<PresetRegistry> registry);

  private:
    std::shared_ptr<ledmatrix::LedMatrix> panel;
  };

} // namespace fw16led::presets
