#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QString>
#include <QDir>
#include <QDebug>
#include <QJsonObject>

class VtAPI;
class MainWindow;

class PluginManager {
public:
    explicit PluginManager(MainWindow* mainWindow);

    void loadPlugins();

private:
    friend class VtAPI;
    friend class MainWindow;

    void executeCommand(const QJsonObject& command);

private:
    MainWindow* m_mainWindow;
};

#endif // PLUGINMANAGER_H
