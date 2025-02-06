#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>

namespace fw16led::ui
{
  class MainWindow : public QMainWindow
  {
    Q_OBJECT
  public:
    MainWindow();

  protected:
    void closeEvent(QCloseEvent* event) override;

  public:
    QSystemTrayIcon* trayIcon;
  };
} // namespace fw16led::ui