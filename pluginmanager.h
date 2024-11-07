#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QString>
#include <QDir>
#include <QDebug>

class MainWindow;

class PluginManager {
public:
    explicit PluginManager(MainWindow* mainWindow);

    void loadPlugins();

private:
    MainWindow* m_mainWindow;
};

#endif // PLUGINMANAGER_H
