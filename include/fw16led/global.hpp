#pragma once

#include <QSettings>
#include <spdlog/spdlog.h>

// Forward declaration of UsbManager
namespace fw16led::managers
{
  class UsbManager;
}

// Forward declaration of PresetRegistry
namespace fw16led
{
  class PresetRegistry;
}

extern std::shared_ptr<spdlog::logger> logger_default;
extern std::shared_ptr<fw16led::managers::UsbManager> usb_manager;
extern std::shared_ptr<fw16led::PresetRegistry> preset_registry;
extern std::shared_ptr<QSettings> settings;

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(logger_default, __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(logger_default, __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(logger_default, __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(logger_default, __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(logger_default, __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(logger_default, __VA_ARGS__)
