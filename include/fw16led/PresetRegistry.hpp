#pragma once

#include "Preset.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace fw16led
{
  /**
   * @brief Registry for presets.
   *
   * Allows registering presets with a unique id and a creator function,
   * and provides functionality to create presets dynamically.
   */
  class PresetRegistry
  {
  public:
    /**
     * @brief Type alias for the preset creator function.
     */
    using PresetCreator = std::function<std::unique_ptr<Preset>()>;

    PresetRegistry() {};

    /**
     * @brief Register a preset with a unique id, its creator, and a display name.
     * @param id Unique identifier for the preset.
     * @param creator Function that creates an instance of the preset.
     * @param displayName Display name for the preset.
     */
    void registerPreset(const std::string& id, const std::string& displayName, PresetCreator creator, std::vector<PresetOptionConfig> options)
    {
      creators_[id] = creator;
      displayNames_[id] = displayName;
      options_[id] = options;
    }

    /**
     * @brief Get the display name of a preset based on its unique id.
     * @param id Unique identifier of the preset.
     * @return Display name of the preset, or an empty string if not found.
     */
    std::string getDisplayName(const std::string& id) const
    {
      if (displayNames_.find(id) != displayNames_.end())
      {
        return displayNames_.at(id);
      }
      return "?";
    }

    std::vector<PresetOptionConfig>& getOptions(const std::string& id)
    {
      if (options_.find(id) != options_.end())
      {
        return options_.at(id);
      }
      return empty;
    }

    /**
     * @brief Create a preset based on its unique id.
     * @param id Unique identifier of the preset.
     * @return A unique_ptr to the created preset, or nullptr if not found.
     */
    std::unique_ptr<Preset> createPreset(const std::string& id)
    {
      if (creators_.find(id) != creators_.end())
      {
        return creators_[id]();
      }
      return nullptr;
    }

    /**
     * @brief Get a list of all registered preset ids.
     * @return Vector containing all registered preset ids.
     */
    std::vector<std::string> getRegisteredPresetIds() const
    {
      std::vector<std::string> ids;
      for (const auto& entry : creators_)
      {
        ids.push_back(entry.first);
      }
      return ids;
    }

  private:
    std::unordered_map<std::string, PresetCreator> creators_;                  /**< Map of preset ids to their creator functions. */
    std::unordered_map<std::string, std::string> displayNames_;                /**< Map of preset ids to their display names. */
    std::unordered_map<std::string, std::vector<PresetOptionConfig>> options_; /**< Map of preset ids to their options. */
    std::vector<PresetOptionConfig> empty;                                     /**< Empty vector to return if no options are found. */
  };

} // namespace fw16led
