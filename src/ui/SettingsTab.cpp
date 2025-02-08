#include "SettingsTab.hpp"
#include "fw16led/PresetRegistry.hpp"
#include "fw16led/global.hpp"
#include "fw16led/managers/usb.hpp"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QOverload>
#include <QSpinBox>

namespace fw16led::ui
{
  void clearLayout(QLayout* layout)
  {
    if (layout == nullptr || layout->isEmpty() || layout->count() == 0)
      return;

    while (QLayoutItem* item = layout->takeAt(0))
    {
      if (QWidget* widget = item->widget())
      {
        widget->setParent(nullptr);
        widget->deleteLater();
      }
      else if (QLayout* childLayout = item->layout())
      {
        clearLayout(childLayout);
      }
      delete item;
    }
  }

  SettingsTab::SettingsTab(uint8_t panelId)
    : panelId(panelId)
  {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    QLabel* presetLabel = new QLabel("Preset: ", this);
    presetComboBox = new QComboBox(this);
    connect(presetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingsTab::onPresetChanged);
    for (const auto preset : preset_registry->getRegisteredPresetIds())
    {
      QString presetName = QString::fromStdString(preset_registry->getDisplayName(preset));
      QString presetId = QString::fromStdString(preset);
      presetComboBox->addItem(presetName, presetId);
    }
    QHBoxLayout* presetLayout = new QHBoxLayout();
    presetLayout->addWidget(presetLabel);
    presetLayout->addWidget(presetComboBox);
    mainLayout->addLayout(presetLayout);

    QHBoxLayout* brightnessLayout = new QHBoxLayout();
    QLabel* brightnessLabel = new QLabel("Brightness: ");
    brightnessValueLabel = new QLabel("");
    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(0, 255);
    connect(brightnessSlider, &QSlider::valueChanged, this, [this](int value)
            { brightnessValueLabel->setText(QString::number(value)); });
    brightnessLayout->addWidget(brightnessLabel);
    brightnessLayout->addWidget(brightnessSlider);
    brightnessLayout->addWidget(brightnessValueLabel);
    mainLayout->addLayout(brightnessLayout);

    QFrame* hLine = new QFrame();
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(hLine);

    dynamicSettingsLayout = new QVBoxLayout();
    mainLayout->addLayout(dynamicSettingsLayout);

    mainLayout->addStretch();

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    applyButton = new QPushButton("Apply", this);
    connect(applyButton, &QPushButton::clicked, this, &SettingsTab::apply);
    buttonLayout->addWidget(applyButton);

    cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsTab::reset);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    reset();
  }

  void SettingsTab::reset()
  {
    QString savedKey = settings->value(QString("panel_%1_preset").arg(panelId), "off").toString();
    int index = presetComboBox->findData(savedKey);
    if (index != -1)
    {
      presetComboBox->setCurrentIndex(index);
    }
    updateDynamicSettings(savedKey);

    brightnessSlider->setValue(settings->value(QString("panel_%1_brightness").arg(panelId), 150).toInt());
    brightnessValueLabel->setText(QString::number(brightnessSlider->value()));
  }

  void SettingsTab::apply()
  {
    QString presetKey = presetComboBox->currentData().toString();
    settings->setValue(QString("panel_%1_preset").arg(panelId), presetKey);
    settings->setValue(QString("panel_%1_brightness").arg(panelId), brightnessSlider->value());

    // Save dynamic settings
    for (int i = 0; i < dynamicSettingsLayout->count(); i++)
    {
      QLayoutItem* item = dynamicSettingsLayout->itemAt(i);
      if (item->widget() == nullptr)
        continue;

      QWidget* widget = item->widget();
      if (widget->property("controlsSetting").isValid())
      {
        QString controlsSetting = widget->property("controlsSetting").toString();
        if (QCheckBox* checkbox = qobject_cast<QCheckBox*>(widget))
        {
          settings->setValue(controlsSetting, checkbox->isChecked());
        }
        else if (QLineEdit* textEdit = qobject_cast<QLineEdit*>(widget))
        {
          settings->setValue(controlsSetting, textEdit->text());
        }
        else if (QComboBox* dropdown = qobject_cast<QComboBox*>(widget))
        {
          settings->setValue(controlsSetting, dropdown->currentText());
        }
        else if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget))
        {
          settings->setValue(controlsSetting, spinBox->value());
        }
        else if (QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(widget))
        {
          settings->setValue(controlsSetting, doubleSpinBox->value());
        }
      }
    }

    usb_manager->applyConfig(panelId);
  }

  void SettingsTab::onPresetChanged(int index)
  {
    QString presetKey = presetComboBox->currentData().toString();
    LOG_TRACE("Preset changed to: {}", presetKey.toStdString());
    updateDynamicSettings(presetKey);
  }

  void SettingsTab::updateDynamicSettings(const QString& presetKey)
  {
    if (dynamicSettingsLayout == nullptr)
      return;

    LOG_TRACE("Updating dynamic settings for preset: {}", presetKey.toStdString());

    // Delete all existing dynamic settings
    clearLayout(dynamicSettingsLayout);

    // Get the preset options
    auto options = preset_registry->getOptions(presetKey.toStdString());

    for (const auto& option : options)
    {
      QLabel* label = new QLabel(QString::fromStdString(option.label));
      dynamicSettingsLayout->addWidget(label);

      QString controlsSetting = QString("panel_%1_preset_%2_%3").arg(panelId).arg(presetKey).arg(QString::fromStdString(option.key));

      switch (option.type)
      {
      case PresetOptionType::Checkbox:
      {
        QCheckBox* checkbox = new QCheckBox();
        checkbox->setProperty("controlsSetting", controlsSetting);
        checkbox->setChecked(settings->value(controlsSetting, option.defaultBool).toBool());
        dynamicSettingsLayout->addWidget(checkbox);
        break;
      }
      case PresetOptionType::Text:
      {
        QLineEdit* textEdit = new QLineEdit();
        textEdit->setProperty("controlsSetting", controlsSetting);
        textEdit->setText(settings->value(controlsSetting, QString::fromStdString(option.defaultText)).toString());
        dynamicSettingsLayout->addWidget(textEdit);
        break;
      }
      case PresetOptionType::Dropdown:
      {
        QComboBox* dropdown = new QComboBox();
        dropdown->setProperty("controlsSetting", controlsSetting);
        for (const auto& dropdownOption : option.dropdownOptions)
        {
          dropdown->addItem(QString::fromStdString(dropdownOption));
        }
        QString selectedOption = settings->value(controlsSetting, QString::fromStdString(option.dropdownOptions[0])).toString();
        int index = dropdown->findText(selectedOption);
        if (index != -1)
        {
          dropdown->setCurrentIndex(index);
        }
        dynamicSettingsLayout->addWidget(dropdown);
        break;
      }
      case PresetOptionType::NumberRange:
      {
        if (option.isInteger)
        {
          QSpinBox* spinBox = new QSpinBox();
          spinBox->setProperty("controlsSetting", controlsSetting);
          spinBox->setRange(static_cast<int>(option.minValue), static_cast<int>(option.maxValue));
          spinBox->setValue(settings->value(controlsSetting, static_cast<int>(option.defaultNumber)).toInt());
          dynamicSettingsLayout->addWidget(spinBox);
        }
        else
        {
          QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox();
          doubleSpinBox->setProperty("controlsSetting", controlsSetting);
          doubleSpinBox->setRange(option.minValue, option.maxValue);
          doubleSpinBox->setValue(settings->value(controlsSetting, option.defaultNumber).toDouble());
          dynamicSettingsLayout->addWidget(doubleSpinBox);
        }
        break;
      }
      }

      LOG_TRACE("Updated dynamic settings for preset: {}", presetKey.toStdString());
    }
  }
} // namespace fw16led::ui