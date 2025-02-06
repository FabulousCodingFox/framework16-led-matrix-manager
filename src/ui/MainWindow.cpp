#include "MainWindow.hpp"

#include "SettingsTab.hpp"
#include "fw16led/global.hpp"
#include "fw16led/managers/usb.hpp"
#include <QCloseEvent>
#include <QTabWidget>
#include <QWidget>

namespace fw16led::ui
{
  MainWindow::MainWindow()
  {
    setWindowTitle("Framework 16 LED Matrix Manager");
    setWindowIcon(QIcon(":/icons/icon.png"));
    setIconSize(QSize(64, 64));
    resize(800, 600);

    QTabWidget* tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    for (auto panel : usb_manager->get_ledpanels())
    {
      uint8_t id = panel->getId();
      tabWidget->addTab(new SettingsTab(id), QString("Panel %1").arg(id));
    }

    // Ensure tabs fill the window
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tabWidget->setTabPosition(QTabWidget::North);
  }

  void MainWindow::closeEvent(QCloseEvent* event)
  {
    if (trayIcon->isVisible())
    {
      hide();
      event->ignore();
    }
  }
} // namespace fw16led::ui