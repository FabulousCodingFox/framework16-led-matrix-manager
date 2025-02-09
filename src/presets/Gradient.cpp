#include "Gradient.hpp"
#include "fw16led/PresetOption.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace fw16led::presets
{
  constexpr auto ID = "gradient";
  constexpr auto DISPLAY_NAME = "Gradient";
  const auto SETTINGS = std::vector<PresetOptionConfig>{
      PresetOptionConfig{
          .type = PresetOptionType::Dropdown,
          .key = "type",
          .label = "Type",
          .dropdownOptions = {
              DropdownOption(0, "Simple"),
              DropdownOption(1, "Double")},
          .defaultDropdown = 0},
  };

  Gradient::Gradient()
    : Preset(ID, DISPLAY_NAME)
  {
  }

  void Gradient::init(std::shared_ptr<ledmatrix::LedMatrix> panel)
  {
    this->panel = panel;

    auto type = getOptionValue<int>("type");
    if (type == 0)
    {
      panel->pattern_gradient();
    }
    else
    {
      panel->pattern_double_gradient();
    }

    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [this]()
                     { this->panel->keep_awake(); });
    timer->start(1000);
  }

  void Gradient::exit()
  {
    delete timer;
  }

  std::vector<PresetOptionConfig> Gradient::getOptions() const
  {
    return SETTINGS;
  }

  void Gradient::registerPreset(std::shared_ptr<PresetRegistry> registry)
  {
    registry->registerPreset(ID, DISPLAY_NAME, []()
                             { return std::make_unique<fw16led::presets::Gradient>(); }, SETTINGS);
  }
} // namespace fw16led::presets
