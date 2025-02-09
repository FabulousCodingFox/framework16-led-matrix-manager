#include "fw16led/LedPanel.hpp"
#include "fw16led/PresetRegistry.hpp"
#include "fw16led/global.hpp"

namespace fw16led
{
  static uint8_t nextId = 0;

  LedPanel::LedPanel(std::shared_ptr<ledmatrix::LedMatrix> ledMatrix)
    : id(++nextId)
    , ledMatrix(ledMatrix)
    , currentPreset(nullptr)
  {
    LOG_INFO("LedPanel created with id: {}", id);
    applyConfig();
  }

  LedPanel::~LedPanel()
  {
  }

  void LedPanel::applyConfig()
  {
    LOG_INFO("Applying config to LedPanel with id: {}", id);

    if (currentPreset)
    {
      currentPreset->exit();
      currentPreset = nullptr;
    }

    auto brightness = settings->value(QString("panel_%1_brightness").arg(id), 150).toInt();
    ledMatrix->brightness(brightness);
    ledMatrix->animate(false);

    auto newPresetName = settings->value(QString("panel_%1_preset").arg(id), "off").toString();
    auto newPresetNameStdString = newPresetName.toStdString();
    currentPreset = preset_registry->createPreset(newPresetNameStdString);
    if (currentPreset)
    {
      // Apply settings
      for (auto option : preset_registry->getOptions(newPresetNameStdString))
      {
        auto optionName = QString("panel_%1_preset_%2_%3").arg(id).arg(newPresetName).arg(QString::fromStdString(option.key));
        switch (option.type)
        {
        case PresetOptionType::Checkbox:
          currentPreset->setOptionValue(option.key, settings->value(optionName, option.defaultBool).toBool());
          break;
        case PresetOptionType::NumberRange:
          currentPreset->setOptionValue(option.key, settings->value(optionName, option.defaultNumber).toDouble());
          break;
        case PresetOptionType::Text:
          currentPreset->setOptionValue(option.key, settings->value(optionName, QString::fromStdString(option.defaultText)).toString().toStdString());
          break;
        case PresetOptionType::Dropdown:
          currentPreset->setOptionValue(option.key, settings->value(optionName, option.defaultDropdown).toInt());
          break;
        }
      }

      currentPreset->init(ledMatrix);
    }
  }
} // namespace fw16led