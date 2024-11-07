#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <filesystem>
#include <QUuid>
#include <QLocale>
#include <QDir>
#include <QTranslator>
#include <QCoreApplication>
#include <QString>
#include <QSysInfo>
#include <QDebug>
#include <QProcessEnvironment>

MainWindow::MainWindow(int argc, char *argv[], QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < argc; ++i) {
        arguments.append(QString(argv[i]));
    }

    QString locale = defineLocale();

    resize(800, 600);
    setObjectName("MainWindow");

    std::filesystem::path appPath = std::filesystem::path(arguments[0].toStdString()).parent_path();

    QWidget *centralwidget = new QWidget(this);
    centralwidget->setObjectName("centralwidget");
    setCentralWidget(centralwidget);

    translator = new QTranslator(this);

    horizontalLayout = new QHBoxLayout(centralwidget);
    horizontalLayout->setObjectName("horizontalLayout");

    treeView = new QTreeView(centralwidget);
    treeView->setMinimumWidth(150);
    treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
    treeView->setMaximumWidth(300);
    treeView->setObjectName("treeWidget");

    treeSplitter = new QSplitter(Qt::Orientation::Horizontal);
    horizontalLayout->addWidget(treeSplitter);

    tabWidget = new QTabWidget(centralwidget);
    treeSplitter->addWidget(treeView);
    treeSplitter->addWidget(tabWidget);

    menubar = new QMenuBar(this);
    menubar->setGeometry(QRect(0, 0, 800, 21));
    menubar->setObjectName("menuBar");

    this->setMenuBar(menubar);

    encodingLabel = new QLabel("UTF-8");
    encodingLabel->setObjectName("encodingLabel");

    statusbar = new QStatusBar(this);
    statusbar->setObjectName("statusbar");
    statusbar->addPermanentWidget(encodingLabel);

    centralwidget->setLayout(horizontalLayout);
    this->setStatusBar(statusbar);

    addTab();
    settings();

    QApplication *app = static_cast<QApplication *>(QCoreApplication::instance());
    VtAPI *api = new VtAPI(app);
    this->windowInit();

    PluginManager *pl = new PluginManager(this);
    pl->loadPlugins();

    this->restoreWState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTab() {
    QString file = "";
    bool canSave = true;
    bool canEdit = true;
    QString encoding = "utf-8";
    QWidget *tab = new QWidget();
    tab->setObjectName(QString("tab-%1").arg(QUuid::createUuid().toString()));
    tab->setProperty("file", file);
    tab->setProperty("canSave", canSave);
    tab->setProperty("canEdit", canEdit);
    tab->setProperty("encoding", encoding);

    QVBoxLayout *verticalLayout = new QVBoxLayout(tab);
    verticalLayout->setObjectName("verticalLayout");

    QFrame *frame = new QFrame(tab);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    frame->setObjectName("tabFrame");
    verticalLayout->addWidget(frame);

    QTextEdit *textEdit = new QTextEdit(tab);
    textEdit->setReadOnly(false);
    textEdit->setObjectName("textEdit");
    verticalLayout->addWidget(textEdit);

    tabWidget->addTab(tab, "Untitled");

    tabWidget->setCurrentIndex(tabWidget->count()-1);

    // // (Опционально) Обработка создания вкладки, например, сигнал
    // emit tabCreated();
}

QString MainWindow::defineLocale() {
    return QLocale::system().name().split('_')[0];
}

void MainWindow::translate() {
    QDir dir(QCoreApplication::applicationDirPath());

    QStringList filters;
    filters << "*.vt-locale";
    QFileInfoList localeFiles = dir.entryInfoList(filters, QDir::Files);
    for (const QFileInfo& fileInfo : localeFiles) {
        QString localeFilePath = fileInfo.absoluteFilePath();
        if (fileInfo.suffix() == "vt-locale") {
            if (fileInfo.fileName().startsWith(locale)) {
                if (translator->load(localeFilePath)) {
                    QCoreApplication::installTranslator(translator);
                }
            }
        }
    }
}

void MainWindow::settings() {
    QFile settingsFile(QCoreApplication::applicationDirPath() + "Main.settings");

    if (!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&settingsFile);
    QString settingsData = in.readAll();
    settingsFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(settingsData.toUtf8());
    if (doc.isNull()) {
        return;
    }

    QJsonObject settings = doc.object();

    QString appName = settings.value("appName").toString();
    double apiVersion = settings.value("apiVersion").toDouble();
    QJsonObject packageDirs = settings.value("packageDirs").toObject();
    QString menu = settings.value("menu").toString();
    bool logStdout = settings.value("logStdout").toBool();
    QString locale = settings.value("locale").toString();

    QString platform = QSysInfo::productType();
    QString packageDir = packageDirs.value(platform).toString();

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    packageDir.replace("%USERPROFILE%", env.value("USERPROFILE"));
    packageDir.replace("%HOME%", env.value("HOME"));

    qDebug() << "Package Directory:" << packageDir;

    QDir dir(packageDir);
    if (!dir.exists()) {
        dir.mkpath(packageDir);
    }

    QString themesDir = packageDir + "/Themes";
    QString pluginsDir = packageDir + "/Plugins";
    QString uiDir = packageDir + "/Ui";
    QString cacheDir = packageDir + "/cache";

    QDir(themesDir).mkpath(themesDir);
    QDir(pluginsDir).mkpath(pluginsDir);
    QDir(uiDir).mkpath(uiDir);
    QDir(cacheDir).mkpath(cacheDir);

    qInfo() << "App Name:" << appName;
    qInfo() << "API Version:" << apiVersion;
    qInfo() << "Locale:" << locale;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    for (const QUrl &url : event->mimeData()->urls()) {
        url.toLocalFile();
    }
    // openFile = self.api.getCommand("OpenFileCommand")
    // if openFile:
    //     self.api.activeWindow.runCommand({"command": "OpenFileCommand", "kwargs": {"f": files}})
    // else:
    //     QtWidgets.QMessageBox.warning(self.MainWindow, self.MainWindow.appName + " - Warning", f"Open file function not found. Check your Open&Save plugin at {os.path.join(self.pluginsDir, 'Open&Save')}")
}

void MainWindow::closeEvent(QCloseEvent *event) {
    this->saveWState();
    // api->activeWindow->wsignals->windowClosed->emit();
    event->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int keyCode = event->key();
    Qt::KeyboardModifiers modifiers = event->modifiers();

    QString keyText = event->text();

    if (keyText.isEmpty()) {
        return;
    }

    QString modifierString;
    if (modifiers & Qt::ControlModifier) {
        modifierString += "Ctrl+";
    }
    if (modifiers & Qt::ShiftModifier) {
        modifierString += "Shift+";
    }
    if (modifiers & Qt::AltModifier) {
        modifierString += "Alt+";
    }

    if (keyCode >= Qt::Key_A && keyCode <= Qt::Key_Z) {
        keyText = QString(QChar('A' + keyCode - Qt::Key_A));
    } else if (keyCode >= Qt::Key_0 && keyCode <= Qt::Key_9) {
        keyText = QString(QChar('0' + keyCode - Qt::Key_0));
    } else if (keyCode == Qt::Key_Space) {
        keyText = "Space";
    } else if (keyCode == Qt::Key_Return) {
        keyText = "Return";
    } else if (keyCode == Qt::Key_Escape) {
        keyText = "Esc";
    } else if (keyCode == Qt::Key_Backspace) {
        keyText = "Backspace";
    } else if (keyCode == Qt::Key_Tab) {
        keyText = "Tab";
    }

    QString actionName = modifierString + keyText;

    // QAction* action = findAction(actionName);
    // if (action) {
    //     action->trigger();  // Запускаем действие
    // }
}

void MainWindow::windowInit() {

}

void MainWindow::restoreWState() {

}

void MainWindow::saveWState() {

}

