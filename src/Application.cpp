#include "Application.hpp"

#include "fw16led/global.hpp"
#include <QAction>
#include <QIcon>
#include <QMainWindow>
#include <QMenu>

namespace fw16led
{
  Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
  {
    mainWindow = new ui::MainWindow();
    setupTrayIcon();
    LOG_INFO("Started QT application");
  }

  void Application::setupTrayIcon()
  {
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/icon.png"));

    QMenu* trayMenu = new QMenu();
    QAction* openAction = new QAction("Open Window");
    QAction* exitAction = new QAction("Exit");

    connect(openAction, &QAction::triggered, mainWindow, &QMainWindow::show);
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    trayMenu->addAction(openAction);
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &Application::onTrayIconActivated);

    mainWindow->trayIcon = trayIcon;
    trayIcon->show();
  }

  void Application::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
  {
    if (reason == QSystemTrayIcon::Trigger)
    {
      mainWindow->show();
    }
  }
} // namespace fw16led