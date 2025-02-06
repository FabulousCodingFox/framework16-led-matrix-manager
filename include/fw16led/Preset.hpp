#pragma once

#include "PresetOption.hpp"
#include "ledmatrix/ledmatrix.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace fw16led
{

  /**
   * @brief Abstract base class for presets.
   *
   * Provides lifecycle methods and a mechanism to store and access dynamic configuration options.
   */
  class Preset
  {
  public:
    Preset(const std::string& id, const std::string& displayName)
      : id_(id)
      , displayName_(displayName)
    {
    }
    virtual ~Preset() = default;

    virtual void init(std::shared_ptr<ledmatrix::LedMatrix> panel) = 0;
    virtual void exit() = 0;

    const std::string& getId() const { return id_; }
    const std::string& getDisplayName() const { return displayName_; }

    virtual std::vector<PresetOptionConfig> getOptions() const
    {
      std::vector<PresetOptionConfig> options;
      return options;
    }

    /**
     * @brief Set the value for a given preset option.
     * @param key The option's unique key.
     * @param value The new value for the option.
     */
    void setOptionValue(const std::string& key, PresetOptionValue value)
    {
      configValues_[key] = value;
    }

    /**
     * @brief Get the current value of a preset option.
     * @param key The option's unique key.
     * @return An optional containing the value if set, or std::nullopt otherwise.
     */
    std::optional<PresetOptionValue> getOptionValue(const std::string& key) const
    {
      auto it = configValues_.find(key);
      if (it != configValues_.end())
      {
        return it->second;
      }
      for (const auto& option : getOptions())
      {
        if (option.key == key)
        {
          if (option.type == PresetOptionType::NumberRange)
          {
            return option.defaultNumber;
          }
          else if (option.type == PresetOptionType::Text)
          {
            return option.defaultText;
          }
          else if (option.type == PresetOptionType::Dropdown)
          {
            return option.dropdownOptions[0];
          }
        }
      }
      return std::nullopt;
    }

    template <typename T>
    std::optional<T> getOptionValue(const std::string& key) const
    {
      auto it = configValues_.find(key);
      if (it != configValues_.end())
      {
        return std::get<T>(it->second);
      }
      for (const auto& option : getOptions())
      {
        if (option.key == key)
        {
          if constexpr (std::is_same_v<T, double> && option.type == PresetOptionType::NumberRange)
          {
            return option.defaultNumber;
          }
          else if constexpr (std::is_same_v<T, std::string> && option.type == PresetOptionType::Text)
          {
            return option.defaultText;
          }
          else if constexpr (std::is_same_v<T, std::string> && option.type == PresetOptionType::Dropdown)
          {
            return option.dropdownOptions[0];
          }
        }
      }
      return std::nullopt;
    }

  private:
    std::string id_;
    std::string displayName_;
    std::unordered_map<std::string, PresetOptionValue> configValues_;
  };

} // namespace fw16led
