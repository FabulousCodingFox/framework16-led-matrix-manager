#include "./presets/Gradient.hpp"
#include "./presets/Off.hpp"
#include "./presets/Text.hpp"
#include "./presets/ZigZag.hpp"
#include "Application.hpp"
#include "fw16led/PresetRegistry.hpp"
#include "fw16led/global.hpp"
#include "fw16led/managers/usb.hpp"
#include "spdlog/spdlog.h"
#include <iostream>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> logger_default;
std::shared_ptr<fw16led::managers::UsbManager> usb_manager;
std::shared_ptr<fw16led::PresetRegistry> preset_registry;
std::shared_ptr<QSettings> settings;

void init_loggers()
{
  try
  {
    // Create Console Sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    // Create and register loggers
    logger_default = std::make_shared<spdlog::logger>("logger_default", console_sink);
    logger_default->set_level(spdlog::level::trace);
    logger_default->set_pattern("[%Y-%m-%d %H:%M:%S] [%s:%!:%#] %^[%l]%$ %v");
    spdlog::register_logger(logger_default);

    LOG_DEBUG("Loggers initialized successfully.");
  }
  catch (const spdlog::spdlog_ex& ex)
  {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    exit(1);
  }
}

void init_presets()
{
  fw16led::presets::Off::registerPreset(preset_registry);
  fw16led::presets::ZigZag::registerPreset(preset_registry);
  fw16led::presets::Gradient::registerPreset(preset_registry);
  fw16led::presets::Text::registerPreset(preset_registry);
}

/**
 * @brief Entry point of the application.
 * @return int Exit status code.
 */
int main(int argc, char* argv[])
{
  init_loggers();

  settings = std::make_shared<QSettings>("framework16-led-matrix-manager", "framework16-led-matrix-manager");

  preset_registry = std::make_shared<fw16led::PresetRegistry>();
  init_presets();
  for (const auto& id : preset_registry->getRegisteredPresetIds())
    LOG_INFO("Found Preset '{}'", id);

  usb_manager = std::make_shared<fw16led::managers::UsbManager>();

  fw16led::Application app(argc, argv);

  return app.exec();
}
