#pragma once

#include "./ui/MainWindow.hpp"
#include <QApplication>
#include <QSystemTrayIcon>

namespace fw16led
{
  class Application : public QApplication
  {
    Q_OBJECT
  public:
    Application(int& argc, char** argv);
    void setupTrayIcon();

  private slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

  private:
    ui::MainWindow* mainWindow;
    QSystemTrayIcon* trayIcon;
  };
} // namespace fw16led