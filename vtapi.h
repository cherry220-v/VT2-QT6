#ifndef VtAPI_H
#define VtAPI_H

#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QFileSystemModel>
#include <QToolBar>
#include <QAction>
#include <QDockWidget>
#include <QStatusBar>
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QDialog>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLocale>
#include <QString>
#include <QTimer>
#include <QObject>
#include <QVariantMap>

class VtAPI {
public:
    // Constructor for VtAPI
    VtAPI(QApplication *app);
    class Window;
    class View;

    void version();
    void arch();
    void platform();

    void setTimeout(std::function<void()> function, int delay);

    QVariantMap loadSettings();
    void saveSettings();

    QString defineLocale();
    QString packagesPath();

    class Window {
    public:
        Window(VtAPI* api, QList<View*> views = {}, View* activeView = nullptr, QMainWindow* mw = nullptr);

        void newFile();
        void openFiles(const QStringList& files);
        void saveFile(bool dlg = false);
        QWidget* getActiveView() const;
        const QList<QWidget*>& views() const;
        View* activeView();
        void addView(QWidget* view);
        void delView(QWidget* view);
        void focus(QWidget* view);
        void registerCommandClass(const QJsonObject& data);
        void registerCommand(const QJsonObject& data);
        void runCommand(const QJsonObject& command);
        void addToolBar(const QList<QAction*>& items);
        QAction* getCommand(const QString& name);
        QString getTheme() const;
        void setTheme(const QString& theme);
        QString getLog() const;
        void setLogMsg(const QString& msg);
        QFileSystemModel* setTreeWidgetDir(const QString& dir);
        void setTab(int index);
        void updateMenu(QMenu* menu, const QJsonObject& data);
        void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dock);
        void showDialog(QWidget* content, int flags = 0, int width = 320, int height = 240);
        bool isDockWidget(Qt::DockWidgetArea area);
        void statusMessage(const QString& text, int timeout = 0);

    private:
        VtAPI* __api;
        QMainWindow* __mw;
        QList<View*> __views;
        View* __activeView;
        QFileSystemModel* model;
        QDialog* dialog;
    };

public:
    void addWindow(Window* window);
    Window* activeWindow();
    QList<Window*> windows() const;

private:
    QApplication* __app;
    QList<Window*> __windows;
    Window* __activeWindow;
};

#endif // VtAPI_H
