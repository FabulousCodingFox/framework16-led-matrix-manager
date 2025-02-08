#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace fw16led
{
  /**
   * @brief Enumeration of option types for preset configuration.
   */
  enum class PresetOptionType : u_int8_t
  {
    NumberRange = 0, /**< Option that represents a numerical range (e.g., brightness, framerate). */
    Text = 1,        /**< Option that represents a text input. */
    Dropdown = 2,    /**< Option that allows selection from predefined values. */
    Checkbox = 3     /**< Option that represents a boolean value. */
  };

  struct DropdownOption
  {
    int key;
    std::string value;
  };

  /**
   * @brief Structure to hold configuration details for a preset option.
   */
  struct PresetOptionConfig
  {
    PresetOptionType type; /**< Type of the configuration option. */
    std::string key;       /**< Unique key identifier for the option (e.g., "brightness"). */
    std::string label;     /**< Display label for the option (e.g., "Brightness"). */

    // For NumberRange:
    double minValue = 0.0;      /**< Minimum value for numerical range options. */
    double maxValue = 0.0;      /**< Maximum value for numerical range options. */
    double defaultNumber = 0.0; /**< Default number for numerical range options. */
    bool isInteger = false;     /**< Flag to indicate if the number is an integer. */

    // For Dropdown:
    std::vector<DropdownOption> dropdownOptions; /**< List of options for dropdown type. */
    int defaultDropdown = 0;                     /**< Default index for dropdown options. */

    // For Text:
    std::string defaultText; /**< Default text for text input options. */

    // For Checkbox
    bool defaultBool = false;
  };

  using PresetOptionValue = std::variant<double, std::string, bool, int>;

} // namespace fw16led
