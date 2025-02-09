#include "Text.hpp"
#include "fw16led/PresetOption.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace fw16led::presets
{
  constexpr auto ID = "text";
  constexpr auto DISPLAY_NAME = "Text";
  const auto SETTINGS = std::vector<PresetOptionConfig>{
      PresetOptionConfig{
          .type = PresetOptionType::Text,
          .key = "text",
          .label = "Text",
          .defaultText = "LOTUS"},
  };

  Text::Text()
    : Preset(ID, DISPLAY_NAME)
  {
  }

  void Text::init(std::shared_ptr<ledmatrix::LedMatrix> panel)
  {
    this->panel = panel;

    auto text = getOptionValue<std::string>("text");
    if (text.has_value())
    {
      panel->pattern_text(text.value());
    }

    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [this]()
                     { this->panel->keep_awake(); });
    timer->start(1000);
  }

  void Text::exit()
  {
    delete timer;
  }

  std::vector<PresetOptionConfig> Text::getOptions() const
  {
    return SETTINGS;
  }

  void Text::registerPreset(std::shared_ptr<PresetRegistry> registry)
  {
    registry->registerPreset(ID, DISPLAY_NAME, []()
                             { return std::make_unique<fw16led::presets::Text>(); }, SETTINGS);
  }
} // namespace fw16led::presets
