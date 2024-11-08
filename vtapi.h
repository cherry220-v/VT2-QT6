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
#include <QObject>
#include <QDialog>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLocale>
#include <QString>
#include <QTimer>
#include <QObject>
#include <QVariantMap>

class MainWindow;

class VtAPI {
public:
    VtAPI(QApplication *app);
    class Window;
    class View;
    class Signals;
    class Dialogs;
    class Plugin;
    class Widgets;

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
        Window(VtAPI* api, QList<View*> views = {}, View* activeView = nullptr, MainWindow* mw = nullptr);

        void newFile();
        void openFiles(const QStringList& files);
        void saveFile(bool dlg = false);
        View* activeView() const;
        const QList<View*>& views() const;
        const Signals* wsignals() const;
        View* activeView();
        void addView(View* view);
        void delView(View* view);
        void focus(View* view);
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
        friend class View;
        VtAPI *__api;
        MainWindow* __mw;
    private:
        QList<View*> __views;
        Signals *__signals;
        View *__activeView;
        QFileSystemModel* model;
        QDialog* dialog;
    };

    class View {
        public:
            View(VtAPI* api, Window* window = nullptr, QWidget* qwclass = nullptr);

            bool operator==(const View& other) const {
                return (this->id == other.id);
            }

            QString getTitle() const;
            void setTitle(const QString& text);
            QString getText() const;
            QString getHtml() const;
            void setText(const QString& text);
            QString getFile() const;
            void setFile(const QString& file);
            bool getCanSave() const;
            void setCanSave(bool canSave);
            bool getCanEdit() const;
            void setReadOnly(bool readOnly);
            QString getEncoding() const;
            void setEncoding(const QString& enc);
            bool getSaved() const;
            void setSaved(bool saved);

            QString id;
            Window *window();
            int tabIndex();
            void update();
            void close();
        private:
            VtAPI *__api;
            Window *__window;
            QWidget *__tab;
            QTabWidget *__tabWidget;


    };

public:
    void addWindow(Window* window);
    void setWindow(Window* window);
    Window* activeWindow();
    QList<Window*> windows() const;

private:
    QApplication* __app;
    QList<Window*> __windows;
    Window* __activeWindow;
};
class VtAPI::Signals : public QObject {
    Q_OBJECT
public:
    explicit Signals(MainWindow *mw);
signals:
    void tabClosed();
    void tabCreated();
    void tabChanged();
    void textChanged();
    void windowClosed();
    void windowStarted();
    void logWrited(const QString&);

    void treeWidgetClicked(const QModelIndex&);
    void treeWidgetDoubleClicked(const QModelIndex&);
    void treeWidgetActivated();
private:
    MainWindow* mw;
};

#endif // VtAPI_H
