#include "pluginmanager.h"
#include "mainwindow.h"
#include "vtapi.h"

PluginManager::PluginManager(MainWindow* mainWindow)
    : m_mainWindow(mainWindow) {
}

void PluginManager::loadPlugins() {
    QString pluginsPath = m_mainWindow->pluginsDir;

    QDir dir(pluginsPath);
    if (!dir.exists()) {
        qWarning() << "Plugins directory does not exist!";
        return;
    }

    QStringList pluginFiles = dir.entryList(QStringList() << "*.plugin", QDir::Files);
    for (const QString& pluginFile : pluginFiles) {
        qDebug() << "Loading plugin:" << pluginFile;
    }

    qDebug() << "All plugins loaded!";
}

void PluginManager::executeCommand(const QJsonObject& command) {

}
