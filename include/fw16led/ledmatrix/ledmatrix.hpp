#pragma once

#include "fw16led/global.hpp"
#include <cstdint>
#include <libusb.h>
#include <memory>
#include <vector>

namespace fw16led::ledmatrix
{
  enum class Command : uint8_t
  {
    Brightness = 0x00,
    Pattern = 0x01,
    BootloaderReset = 0x02,
    Sleep = 0x03,
    Animate = 0x04,
    Panic = 0x05,
    Draw = 0x06,
    StageGreyCol = 0x07,
    DrawGreyColBuffer = 0x08,
    SetText = 0x09,
    StartGame = 0x10,
    GameControl = 0x11,
    GameStatus = 0x12,
    SetColor = 0x13,
    DisplayOn = 0x14,
    InvertScreen = 0x15,
    SetPixelColumn = 0x16,
    FlushFramebuffer = 0x17,
    ClearRam = 0x18,
    ScreenSaver = 0x19,
    SetFps = 0x1A,
    SetPowerMode = 0x1B,
    PwmFreq = 0x1E,
    DebugMode = 0x1F,
    Version = 0x20,
  };

  enum class IntegratedPattern : uint8_t
  {
    Percentage = 0x00,
    Gradient = 0x01,
    DoubleGradient = 0x02,
    DisplayLotus = 0x03,
    ZigZag = 0x04,
    FullBrightness = 0x05,
    DisplayPanic = 0x06,
    DisplayLotus2 = 0x07,
  };

  constexpr unsigned int VID = 0x32AC;
  constexpr unsigned int PID = 0x0020;

  constexpr int WIDTH = 9;
  constexpr int HEIGHT = 34;
  constexpr int PIXELS = WIDTH * HEIGHT;

  class LedMatrix
  {
  private:
    std::unique_ptr<libusb_device_handle, decltype(&libusb_close)> device;

  public:
    LedMatrix(libusb_device_handle* device)
      : device(device, libusb_close)
    {
    }

    ~LedMatrix();

    void send_command(Command command, const std::vector<uint8_t>& parameters = {});
    auto send_command_with_response(Command command, const std::vector<uint8_t>& parameters = {}) -> std::vector<uint8_t>;

    void animate(bool animate = true)
    {
      LOG_TRACE("Setting integrated animate to {}", animate);
      this->send_command(Command::Animate, {static_cast<uint8_t>(animate ? 0x01 : 0x00)});
    }

    auto get_animate() -> bool
    {
      LOG_TRACE("Getting current animation status");
      auto res = this->send_command_with_response(Command::Animate);
      return !res.empty() && res[0] == 0x01;
    }

    void pattern_full_brightness()
    {
      LOG_TRACE("Setting integrated pattern to full brightness");
      this->send_command(Command::Pattern, {static_cast<uint8_t>(IntegratedPattern::FullBrightness)});
    }

    void pattern_gradient()
    {
      LOG_TRACE("Setting integrated pattern to gradient");
      this->send_command(Command::Pattern, {static_cast<uint8_t>(IntegratedPattern::Gradient)});
    }

    void pattern_double_gradient()
    {
      LOG_TRACE("Setting integrated pattern to double gradient");
      this->send_command(Command::Pattern, {static_cast<uint8_t>(IntegratedPattern::DoubleGradient)});
    }

    void pattern_lotus()
    {
      LOG_TRACE("Setting integrated pattern to lotus");
      this->send_command(Command::Pattern, {static_cast<uint8_t>(IntegratedPattern::DisplayLotus)});
    }

    void pattern_zigzag()
    {
      LOG_TRACE("Setting integrated pattern to zigzag");
      this->send_command(Command::Pattern, {static_cast<uint8_t>(IntegratedPattern::ZigZag)});
    }

    void pattern_panic()
    {
      LOG_TRACE("Setting integrated pattern to panic");
      this->send_command(Command::Pattern, {static_cast<uint8_t>(IntegratedPattern::DisplayPanic)});
    }

    void pattern_lotus2()
    {
      LOG_TRACE("Setting integrated pattern to lotus2");
      this->send_command(Command::Pattern, {static_cast<uint8_t>(IntegratedPattern::DisplayLotus2)});
    }

    void pattern_percentage(uint8_t value)
    {
      LOG_TRACE("Setting integrated pattern to percentage with value {}", value);
      if (value > 100)
      {
        spdlog::error("Value must be between 0 and 100");
        return;
      }
      this->send_command(Command::Pattern, {static_cast<uint8_t>(IntegratedPattern::Percentage), value});
    }

    void pattern_empty_matrix()
    {
      LOG_TRACE("Setting pattern to empty matrix");
      std::vector<bool> matrix(PIXELS, false);
      this->pattern_matrix(matrix);
    }

    void pattern_text(const std::string& text);

    void pattern_symbols(std::vector<std::string>& parts);

    void pattern_count(int value);

    auto get_pwm_freq() -> int;

    void pattern_matrix(std::vector<bool>& matrix);

    void pattern_equalizer(std::vector<uint8_t>& values);

    void brightness(uint8_t value)
    {
      LOG_TRACE("Setting global brightness to {}", value);
      this->send_command(Command::Brightness, {value});
    }

    auto get_brightness() -> uint8_t
    {
      LOG_TRACE("Getting current global brightness");
      auto res = this->send_command_with_response(Command::Brightness);
      return !res.empty() ? res[0] : 0;
    }
  };
} // namespace fw16led::ledmatrix
