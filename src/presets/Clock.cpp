#include "Clock.hpp"
#include "fw16led/PresetOption.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fw16led::presets
{
  constexpr auto ID = "clock";
  constexpr auto DISPLAY_NAME = "Clock";
  const auto SETTINGS = std::vector<PresetOptionConfig>{
      PresetOptionConfig{
          .type = PresetOptionType::Dropdown,
          .key = "format",
          .label = "Format",
          .dropdownOptions = {
              DropdownOption(0, "12h"),
              DropdownOption(1, "24h")},
          .defaultDropdown = 1},
  };

  Clock::Clock()
    : Preset(ID, DISPLAY_NAME)
  {
  }

  void Clock::render()
  {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm local_time;
#if defined(_MSC_VER)
    localtime_s(&local_time, &now_time_t);
#else
    localtime_r(&now_time_t, &local_time);
#endif

    auto format = getOptionValue<int>("format");
    if (format == 0)
    {
      // Format the time as HH:MM AM/PM
      std::ostringstream time_stream;
      time_stream << std::put_time(&local_time, "%I:%M");
      std::string current_time = time_stream.str();
      panel->pattern_text(current_time);
    }
    else
    {
      // Format the time as HH:MM
      std::ostringstream time_stream;
      time_stream << std::put_time(&local_time, "%H:%M");
      std::string current_time = time_stream.str();
      panel->pattern_text(current_time);
    }
  }

  void Clock::init(std::shared_ptr<ledmatrix::LedMatrix> panel)
  {
    this->panel = panel;

    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [this]()
                     { this->render(); });
    timer->start(1000);

    render();
  }

  void Clock::exit()
  {
    delete timer;
  }

  std::vector<PresetOptionConfig> Clock::getOptions() const
  {
    return SETTINGS;
  }

  void Clock::registerPreset(std::shared_ptr<PresetRegistry> registry)
  {
    registry->registerPreset(ID, DISPLAY_NAME, []()
                             { return std::make_unique<fw16led::presets::Clock>(); }, SETTINGS);
  }
} // namespace fw16led::presets
