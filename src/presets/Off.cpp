#include "Off.hpp"
#include "fw16led/PresetOption.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace fw16led::presets
{
  constexpr auto ID = "off";
  constexpr auto DISPLAY_NAME = "Off";
  const auto SETTINGS = std::vector<PresetOptionConfig>{};

  Off::Off()
    : Preset(ID, DISPLAY_NAME)
  {
  }

  void Off::init(std::shared_ptr<ledmatrix::LedMatrix> panel)
  {
    panel->pattern_empty_matrix();
  }

  void Off::exit()
  {
  }

  std::vector<PresetOptionConfig> Off::getOptions() const
  {
    return SETTINGS;
  }

  void Off::registerPreset(std::shared_ptr<PresetRegistry> registry)
  {
    registry->registerPreset(ID, DISPLAY_NAME, []()
                             { return std::make_unique<fw16led::presets::Off>(); }, SETTINGS);
  }
} // namespace fw16led::presets
