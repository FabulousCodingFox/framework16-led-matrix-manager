#include "fw16led/ledmatrix/ledmatrix.hpp"
#include "fw16led/ledmatrix/font.hpp"
#include <codecvt>

namespace fw16led::ledmatrix
{
  inline constexpr std::array<uint8_t, 2> FWK_MAGIG = {0x32, 0xAC};
  inline constexpr uint8_t RESPONSE_SIZE = 32;
  inline constexpr uint8_t ENDPOINT_OUT = 0x01;
  inline constexpr uint8_t ENDPOINT_IN = 0x82;
  inline constexpr int TRANSFER_TIMEOUT_MS = 100;

  LedMatrix::~LedMatrix()
  {
    // Reset the device
    if (int r = libusb_reset_device(device.get()); r != LIBUSB_SUCCESS)
    {
      LOG_WARN("Could not reset device: {}", libusb_strerror(static_cast<libusb_error>(r)));
    }

    // Release the interface
    if (int r = libusb_release_interface(device.get(), 1); r != LIBUSB_SUCCESS)
    {
      LOG_WARN("Could not release interface: {}", libusb_strerror(static_cast<libusb_error>(r)));
    }

#ifdef __linux__
    // Re-attach kernel driver
    if (int r = libusb_attach_kernel_driver(device.get(), 1); r != LIBUSB_SUCCESS)
    {
      LOG_WARN("Could not attach kernel driver: {}", libusb_strerror(static_cast<libusb_error>(r)));
    }
#endif
  }

  void LedMatrix::send_command(Command command, const std::vector<uint8_t>& parameters)
  {
    // Build the outgoing data packet
    std::vector<uint8_t> outData;
    outData.reserve(FWK_MAGIG.size() + 1 + parameters.size());
    outData.insert(outData.end(), FWK_MAGIG.begin(), FWK_MAGIG.end());
    outData.push_back(static_cast<uint8_t>(command));
    outData.insert(outData.end(), parameters.begin(), parameters.end());

    LOG_TRACE("Sending command {} with {} parameters", static_cast<int>(command), parameters.size());

    // Send the data via bulk OUT transfer
    int actual_length = 0;
    int ret = libusb_bulk_transfer(device.get(), ENDPOINT_OUT, outData.data(), static_cast<int>(outData.size()), &actual_length, TRANSFER_TIMEOUT_MS);
    if (ret != LIBUSB_SUCCESS)
    {
      LOG_WARN("Bulk OUT transfer failed: {}", libusb_strerror(static_cast<libusb_error>(ret)));
    }

    if (actual_length != static_cast<int>(outData.size()))
    {
      LOG_WARN("Bulk OUT transfer size does not match. Expected {}; Got {}", outData.size(), actual_length);
    }
  }

  auto LedMatrix::send_command_with_response(Command command, const std::vector<uint8_t>& parameters) -> std::vector<uint8_t>
  {
    // Build the outgoing data packet
    std::vector<uint8_t> outData;
    outData.reserve(FWK_MAGIG.size() + 1 + parameters.size());
    outData.insert(outData.end(), FWK_MAGIG.begin(), FWK_MAGIG.end());
    outData.push_back(static_cast<uint8_t>(command));
    outData.insert(outData.end(), parameters.begin(), parameters.end());

    LOG_TRACE("Sending command {} with {} parameters", static_cast<int>(command), parameters.size());

    // Send the data via bulk OUT transfer
    int actual_length = 0;
    int ret = libusb_bulk_transfer(device.get(), ENDPOINT_OUT, outData.data(), static_cast<int>(outData.size()), &actual_length, TRANSFER_TIMEOUT_MS);
    if (ret != LIBUSB_SUCCESS)
    {
      LOG_WARN("Bulk OUT transfer failed: {}", libusb_strerror(static_cast<libusb_error>(ret)));
      return {};
    }

    if (actual_length != static_cast<int>(outData.size()))
    {
      LOG_WARN("Bulk OUT transfer size does not match. Expected {}; Got {}", outData.size(), actual_length);
    }

    std::vector<uint8_t> inData(RESPONSE_SIZE, 0);

    ret = libusb_bulk_transfer(device.get(), ENDPOINT_IN, inData.data(), RESPONSE_SIZE, &actual_length, TRANSFER_TIMEOUT_MS);

    if (ret != LIBUSB_SUCCESS)
    {
      LOG_WARN("Bulk IN transfer failed: {}", libusb_strerror(static_cast<libusb_error>(ret)));
      return {};
    }

    // Shrink the buffer to the actual number of bytes read
    inData.resize(actual_length);
    return inData;
  }

  auto splitUTF8(const std::string& input) -> std::vector<std::string>
  {
    std::vector<std::string> result;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    try
    {
      // Convert UTF-8 string to a wide string (char32_t for full Unicode (Needed for vowel mutations))
      std::u32string wideStr = converter.from_bytes(input);

      for (auto ch : wideStr)
      {
        result.push_back(converter.to_bytes(ch));
      }
    }
    catch (const std::range_error& e)
    {
      LOG_ERROR("Error processing string: {}", e.what());
    }

    return result;
  }

  void LedMatrix::pattern_text(const std::string& text)
  {
    LOG_TRACE("Setting text to {}", text);
    auto parts = splitUTF8(text);
    this->pattern_symbols(parts);
  }

  void LedMatrix::pattern_symbols(std::vector<std::string>& parts)
  {
    std::vector<std::reference_wrapper<const std::array<bool, FONT_PIXELS>>> font_items;

    for (size_t i = 0; i < std::min(parts.size(), size_t(5)); ++i)
    {
      font_items.push_back(std::cref(get_char(parts[i])));
    }

    std::vector<uint8_t> vals(39, 0x00);

    for (size_t digit_i = 0; digit_i < font_items.size(); ++digit_i)
    {
      const auto& digit_pixels = font_items[digit_i].get();
      size_t offset = digit_i * 7;

      for (size_t pixel_x = 0; pixel_x < FONT_WIDTH; ++pixel_x)
      {
        for (size_t pixel_y = 0; pixel_y < FONT_HEIGHT; ++pixel_y)
        {
          bool pixel_value = digit_pixels[pixel_x + pixel_y * FONT_WIDTH];
          size_t i = (2 + pixel_x) + (9 * (pixel_y + offset));

          if (pixel_value)
          {
            vals[i / 8] |= (1 << (i % 8));
          }
        }
      }
    }

    this->send_command(Command::Draw, vals);
  }

  void LedMatrix::pattern_count(int value)
  {
    spdlog::trace("Setting pattern to count ", value);
    if (value > PIXELS || value < 0)
    {
      spdlog::error("Value must be between 0 and {}", PIXELS);
      return;
    }

    std::vector<uint8_t> vals(39, 0x00);

    for (int byte = 0; byte < value / CHAR_BIT; ++byte)
    {
      vals[byte] = 0xFF;
    }
    for (int i = 0; i < value % CHAR_BIT; ++i)
    {
      vals[value / 8] += 1 << i;
    }

    this->send_command(Command::Draw, vals);
  }

  auto LedMatrix::get_pwm_freq() -> int
  {
    auto res = this->send_command_with_response(Command::PwmFreq);
    if (res.empty())
      return -1;

    switch (res[0])
    {
    case 0:
      return 29000;
    case 1:
      return 3600;
    case 2:
      return 1800;
    case 3:
      return 900;
    default:
      return -1;
    }
  }

  void LedMatrix::pattern_matrix(std::vector<bool>& matrix)
  {
    LOG_TRACE("Setting pattern to matrix with {} values", matrix.size());
    std::vector<uint8_t> vals(39, 0x00);

    for (int x = 0; x < WIDTH; ++x)
    {
      for (int y = 0; y < HEIGHT; ++y)
      {
        int i = x + y * WIDTH;
        if (matrix[i])
          vals[i / 8] |= (1 << (i % 8));
      }
    }

    this->send_command(Command::Draw, vals);
  }

  void LedMatrix::pattern_equalizer(std::vector<uint8_t>& values)
  {
    LOG_TRACE("Setting pattern to equalizer with {} values", values.size());
    std::vector<bool> matrix(PIXELS, false);
    for (int col = 0; col < std::min(values.size(), static_cast<std::size_t>(9)); ++col)
    {
      int val = values[col];

      int row = HEIGHT / 2;
      int above = val / 2;
      int below = val - above;

      for (int i = 0; i < above; i++)
      {
        matrix[col + (row + i) * WIDTH] = true;
      }

      for (int i = 0; i < below; i++)
      {
        matrix[col + (row - 1 - i) * WIDTH] = true;
      }
    }
    this->pattern_matrix(matrix);
  }
} // namespace fw16led::ledmatrix