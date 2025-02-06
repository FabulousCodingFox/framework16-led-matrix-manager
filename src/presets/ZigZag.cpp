#include "ZigZag.hpp"
#include "fw16led/PresetOption.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace fw16led::presets
{
  constexpr auto ID = "zigzag";
  constexpr auto DISPLAY_NAME = "ZigZag";
  const auto SETTINGS = std::vector<PresetOptionConfig>{
      PresetOptionConfig{
        type : PresetOptionType::Checkbox,
        key : "scroll",
        label : "Scroll",
        defaultBool : false,
      },
  };

  ZigZag::ZigZag()
    : Preset(ID, DISPLAY_NAME)
  {
  }

  void ZigZag::init(std::shared_ptr<ledmatrix::LedMatrix> panel)
  {
    this->panel = panel;
    panel->pattern_zigzag();

    auto scroll = getOptionValue<bool>("scroll");
    panel->animate(scroll.value());
  }

  void ZigZag::exit()
  {
  }

  std::vector<PresetOptionConfig> ZigZag::getOptions() const
  {
    return SETTINGS;
  }

  void ZigZag::registerPreset(std::shared_ptr<PresetRegistry> registry)
  {
    registry->registerPreset(ID, DISPLAY_NAME, []()
                             { return std::make_unique<fw16led::presets::ZigZag>(); }, SETTINGS);
  }
} // namespace fw16led::presets
