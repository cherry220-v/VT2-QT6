#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QStatusBar>
#include <QMenuBar>
#include <QTreeView>
#include <QWidget>
#include <QTranslator>
#include <QSplitter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QCloseEvent>
#include <QMimeData>

#include "pluginmanager.h"
#include "vtapi.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc, char *argv[], QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    friend class PluginManager;

    QString locale;
    QString appPath;
    QString packageDirs;
    QString themesDir;
    QString pluginsDir;
    QString uiDir;
    QString cacheDir;
    QString menuFile;
    QString logStdout;
    bool remindOnClose;

    QJsonObject settData;
    QFile settFile;

private:
    Ui::MainWindow *ui;

    QHBoxLayout *horizontalLayout;
    QWidget *centralWidget;
    QLabel *encodingLabel;
    QStringList arguments;
    QWidget *console;
    QTranslator *translator;
    QTreeView *treeView;
    QTabWidget *tabWidget;
    QStatusBar *statusbar;
    QSplitter *treeSplitter;
    QMenuBar *menubar;

    PluginManager *pl;
    VtAPI *api;

    void addTab();
    QString defineLocale();
    void translate();
    void settings();

    void windowInit();
    void restoreWState();
    void saveWState();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
