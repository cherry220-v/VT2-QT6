#include "vtapi.h"
#include <QJsonArray>

VtAPI::VtAPI(QApplication *app) {
    this->__app = app;
    this->__windows = QList<Window*>();
    this->__activeWindow = nullptr;
}

// Window

VtAPI::Window::Window(VtAPI* api, QList<VtAPI::View*> views, View* activeView, QMainWindow* mw) {
    this->__api = api;
    this->__mw = mw;
    this->__views = views;
    this->__activeView = activeView;

    // this->__api->addWindow(this);
}

VtAPI::View* VtAPI::Window::Window::activeView() {
    return this->__activeView;
}


void VtAPI::Window::newFile() {
    this->__mw->addTab();
}

void VtAPI::Window::openFiles(const QStringList& files) {
    QJsonObject command;
    command["command"] = "OpenFileCommand";
    QJsonArray filesArray;
    for (const QString &file : files) {
        filesArray.append(file);
    }
    QJsonObject kwargs;
    kwargs["f"] = filesArray;
    command["kwargs"] = kwargs;
    this->runCommand(command);
}

void VtAPI::Window::saveFile(bool dlg) {
    QJsonObject command;
    command["command"] = "SaveFileCommand";
    QJsonObject kwargs;
    kwargs["dlg"] = dlg;
    command["kwargs"] = kwargs;
    this->runCommand(command);
}

void VtAPI::Window::runCommand(const QJsonObject& command) {
    this->__mw->pl->executeCommand(command);
}



void VtAPI::addWindow(VtAPI::Window* window) {
    this->__windows.append(dynamic_cast<Window*>(window));
}
