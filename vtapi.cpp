#include "vtapi.h"
#include "mainwindow.h"
#include <QJsonArray>

VtAPI::VtAPI(QApplication *app) {
    this->__app = app;
    this->__windows = QList<Window*>();
    this->__activeWindow = nullptr;
}

// Window

VtAPI::Window::Window(VtAPI* api, QList<VtAPI::View*> views, View* activeView, MainWindow* mw) {
    this->__api = api;
    this->__mw = mw;
    this->__views = views;
    this->__activeView = activeView;
    this->__signals = new VtAPI::Signals(this->__mw);

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

const QList<VtAPI::View*>& VtAPI::Window::views() const {
    return this->__views;
}

const VtAPI::Signals* VtAPI::Window::Window::wsignals() const {
    return this->__signals;
}

void VtAPI::Window::addView(VtAPI::View* view) {
    this->__views.append(view);
}

void VtAPI::Window::delView(VtAPI::View* view) {
    this->__views.removeAll(view);
}

void VtAPI::Window::focus(VtAPI::View* view) {
    for (View* v : this->views()) {
        if (v == view) {
            // this->__mw->tabWidget->setCurrentIndex(view->tabIndex());
            this->__activeView = v;
            break;
        }
    }
}






// View

VtAPI::View::View(VtAPI* api, Window* window, QWidget* qwclass) {
    this->__api = api;
    this->__window = window;
    this->__tab = qwclass;
    this->__tabWidget = this->__window->__mw->tabWidget;
    this->id = this->__tab->objectName().split("-").last();
}

int VtAPI::View::tabIndex() {
    return this->__tabWidget->indexOf(this->__tab);
}

void VtAPI::View::update() {
    VtAPI::View* view = new VtAPI::View(this->__api, this->__window, this->__tab);
    view->id = __tab->objectName().split("-").last();
    this->window()->focus(view);
}

void VtAPI::View::close() {
    this->__tabWidget.closeTab(this->tabIndex());
}

VtAPI::Window *VtAPI::View::window() {
    return this->__window;
}

QString VtAPI::View::getTitle() const {
    return this->__tabWidget->tabText(this->__tabWidget->indexOf(this->__tab));
}

void VtAPI::View::setTitle(const QString& text) {
    this->__tabWidget->setTabText(this->__tabWidget->indexOf(this->__tab), text);
}

QString VtAPI::View::getText() const {
    return this->__tab->findChild<QTextEdit*>("textEdit")->toPlainText();
}

QString VtAPI::View::getHtml() const {
    return this->__tab->findChild<QTextEdit*>("textEdit")->toHtml();
}

void VtAPI::View::setText(const QString& text) {
    this->__tab->findChild<QTextEdit*>("textEdit")->safeSetText(text);
}

QString VtAPI::View::getFile() const {
    return this->__tab->property("file").toString();
}

void VtAPI::View::setFile(const QString& file) {
    this->__tab->setProperty("file", file);
}

bool VtAPI::View::getCanSave() const {
    return this->__tab->property("canSave").toBool();
}

void VtAPI::View::setCanSave(bool canSave) {
    this->__tab->setProperty("canSave", canSave);
}

bool VtAPI::View::getCanEdit() const {
    return this->__tab->property("canEdit").toBool();
}

void VtAPI::View::setReadOnly(bool readOnly) {
    this->__tab->setProperty("canEdit", !readOnly);
    this->__tab->findChild<QTextEdit*>("textEdit")->setReadOnly(readOnly);
    this->__tab->findChild<QTextEdit*>("textEdit")->setDisabled(readOnly);
}

QString VtAPI::View::getEncoding() const {
    return this->__tab->property("encoding").toString();
}

void VtAPI::View::setEncoding(const QString& enc) {
    this->__tab->setProperty("encoding", enc);
}

bool VtAPI::View::getSaved() const {
    return this->__tabWidget->isSaved(this->__tab);
}

void VtAPI::View::setSaved(bool saved) {
    this->__tabWidget->tabBar()->setTabSaved(this->__tab, saved);
}

// Signals

VtAPI::Signals::Signals(MainWindow *mw) : QObject(nullptr), mw(mw) {
}






VtAPI::Window* VtAPI::activeWindow() {
    return this->__activeWindow;
}

void VtAPI::setWindow(VtAPI::Window* window) {
    this->__activeWindow = window;
}

void VtAPI::addWindow(VtAPI::Window* window) {
    this->__windows.append(dynamic_cast<Window*>(window));
}

QList<VtAPI::Window*> VtAPI::windows() const {
    return this->__windows;
}
