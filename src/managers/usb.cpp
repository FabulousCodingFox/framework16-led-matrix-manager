#include "fw16led/managers/usb.hpp"
#include <vector>

namespace fw16led::managers
{

  UsbManager::UsbManager()
  {
    LOG_DEBUG("Initializing libusb");
    int r = libusb_init(&libusb_ctx);
    if (r < 0)
    {
      SPDLOG_CRITICAL("Failed to initialize libusb: {}", r);
      return;
    }

    // Get the list of USB devices
    LOG_DEBUG("Listing USB devices");
    libusb_device** dev_list = nullptr;
    ssize_t cnt = libusb_get_device_list(libusb_ctx, &dev_list);
    if (cnt < 0)
    {
      SPDLOG_CRITICAL("Failed to get device list: {}", cnt);
      libusb_exit(libusb_ctx);
      return;
    }

    for (ssize_t i = 0; i < cnt; i++)
    {
      libusb_device* device = dev_list[i];
      libusb_device_descriptor desc;

      // Retrieve the device descriptor
      if (libusb_get_device_descriptor(device, &desc) == 0)
      {
        // Check if it matches VID=0x32AC and PID=0x0020
        if (desc.idVendor == ledmatrix::VID && desc.idProduct == ledmatrix::PID)
        {
          LOG_DEBUG("Found Framework LED Matrix device");

          // Attempt to open this device
          libusb_device_handle* handle = nullptr;
          r = libusb_open(device, &handle);
          if (r == 0 && handle != nullptr)
          {
            LOG_DEBUG("-> Successfully opened device.");

            r = libusb_set_configuration(handle, 1);
            if (r != LIBUSB_SUCCESS && r != LIBUSB_ERROR_BUSY)
            {
              LOG_ERROR("-> Failed to set configuration: {}", libusb_strerror((libusb_error) r));
              libusb_close(handle);
              continue;
            }

#ifdef __linux__
            // On Linux, if a kernel driver is attached, detach it.
            if (libusb_kernel_driver_active(handle, 1) == 1)
            {
              r = libusb_detach_kernel_driver(handle, 1);
              if (r != LIBUSB_SUCCESS)
              {
                LOG_ERROR("-> Could not detach kernel driver: {}", libusb_strerror((libusb_error) r));
                libusb_close(handle);
                continue;
              }
            }

            LOG_DEBUG("-> Successfully detached kernel driver");
#endif

            // Claim the interface
            int interfaceNum = 1;
            r = libusb_claim_interface(handle, interfaceNum);
            if (r != LIBUSB_SUCCESS)
            {
              LOG_ERROR("-> Could not claim interface 1: {}", libusb_strerror((libusb_error) r));
              libusb_close(handle);
              continue;
            }

            LOG_DEBUG("-> Successfully claimed interface 1");

            // Store the handle
            ledpanels.push_back(std::make_shared<LedPanel>(std::make_shared<ledmatrix::LedMatrix>(handle)));
          }
          else
          {
            LOG_ERROR("-> Failed to open device: {}", r);
          }
        }
      }
      else
      {
        LOG_ERROR("Failed to get device descriptor for device index {}", i);
      }
    }

    libusb_free_device_list(dev_list, 1);
  }

  UsbManager::~UsbManager()
  {
    SPDLOG_DEBUG("Freeing usb resources");
    ledpanels.clear();

    SPDLOG_DEBUG("Exiting libusb");
    libusb_exit(libusb_ctx);
  };

  void UsbManager::applyConfig(uint8_t panelId)
  {
    for (auto panel : ledpanels)
    {
      if (panel->getId() == panelId)
      {
        panel->applyConfig();
        return;
      }
    }
  }
} // namespace fw16led::managers