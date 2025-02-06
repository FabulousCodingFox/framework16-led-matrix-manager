#pragma once

#include "fw16led/LedPanel.hpp"
#include <memory>
#include <vector>

namespace fw16led::managers
{
  class UsbManager
  {
  public:
    UsbManager();
    ~UsbManager();

    auto get_ledpanels() -> std::vector<std::shared_ptr<LedPanel>>
    {
      return ledpanels;
    }

    void applyConfig(uint8_t panelId);

  private:
    std::vector<std::shared_ptr<LedPanel>> ledpanels;
    libusb_context* libusb_ctx;
  };
} // namespace fw16led::managers