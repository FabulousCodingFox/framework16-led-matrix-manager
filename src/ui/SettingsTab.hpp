#pragma once

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <cstdint>

namespace fw16led::ui
{
  class SettingsTab : public QWidget
  {
    Q_OBJECT
  public:
    SettingsTab(uint8_t panelId);
    void reset();

  private:
    void apply();
    void onPresetChanged(int index);
    void updateDynamicSettings(const QString& presetKey);

  private:
    uint8_t panelId;
    QComboBox* presetComboBox;
    QPushButton* applyButton;
    QPushButton* cancelButton;
    QVBoxLayout* dynamicSettingsLayout = nullptr;
    QLabel* brightnessValueLabel;
    QSlider* brightnessSlider;
  };
} // namespace fw16led::ui