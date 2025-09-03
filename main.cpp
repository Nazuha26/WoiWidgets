#include <QtWidgets>

#include "WoiCustomWindow.h"
#include "core/Icons.h"
#include "core/WoiThemeEngine.h"
#include "core/style/WoiStyle.h"

QMainWindow* buildMainWindow();
QMenuBar* buildMenuBar();



int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle(new WoiStyle("Fusion"));

    WoiThemeEngine::applyDarkTheme();

    WoiCustomWindow window(buildMainWindow());
    window.resize(800, 480);
    window.setWindowTitle("Window");
    window.setMenuBar(buildMenuBar());

    QLineEdit* le = new QLineEdit();
    le->setPlaceholderText("Smart search...");

    window.addWidget2titleBar(le, 1);

    window.show();
    return QApplication::exec();
}



QMainWindow* buildMainWindow()
{
    const QIcon testIcon = QIcon(Icons::path(Icons::TestIcon));

    QMainWindow* win = new QMainWindow();
    win->setWindowTitle("WoiWidgets Showcase — v1.0.0");

    /* ===== TOOLBAR ===== */
    QToolBar* tb = win->addToolBar("Main");
    tb->setIconSize(QSize(16, 16));

    const QAction* aNew = tb->addAction("New");
    const QAction* aOpen = tb->addAction(testIcon, "Open...");
    const QAction* aSave = tb->addAction("Save");

    tb->addSeparator();

    auto* searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("Search...");
    searchEdit->setClearButtonEnabled(true);
    tb->addWidget(searchEdit);

    auto* searchBtn = new QPushButton("OK");
    tb->addWidget(searchBtn);

    /* ===== STATUS BAR ===== */
    auto* sb = win->statusBar();
    sb->showMessage("WoiWidgets v1.0.0 © Nazuha26 2025");

    /* ===== CENTRAL (tabs) ===== */
    auto* tabs = new QTabWidget;
    win->setCentralWidget(tabs);

    // ---- Tab 1: Inputs ----
    {
        auto* page = new QWidget;
        auto* form = new QFormLayout(page);

        auto* le = new QLineEdit;
        le->setPlaceholderText("Your text");

        auto* te = new QTextEdit;
        for (int i = 0; i < 16; i++)
            te->append("Text " + QString::number(i));
        te->setPlaceholderText("Rich text...");

        auto* cbx = new QComboBox;
        cbx->addItems({ "Item A", "Item B", "Item C" });

        auto* check = new QCheckBox("Enable option");
        auto* radio1 = new QRadioButton("Choice 1");
        auto* radio2 = new QRadioButton("Choice 2");

        auto* radios = new QWidget;
        auto* hv = new QHBoxLayout(radios);
        hv->setContentsMargins(0, 0, 0, 0);
        hv->addWidget(radio1);
        hv->addWidget(radio2);
        hv->addStretch();

        auto* spin = new QSpinBox;
        spin->setRange(0, 100);

        auto* slider = new QSlider(Qt::Horizontal);
        slider->setRange(0, 100);
        slider->setValue(40);

        auto* prg = new QProgressBar;
        prg->setRange(0, 100);
        prg->setValue(60);

        auto* btnCalc = new QPushButton("Calculate");
        btnCalc->setEnabled(false);

        auto* btnDefault = new QPushButton("Default btn");
        btnDefault->setIcon(testIcon);
        btnDefault->setDefault(true);
        btnDefault->setToolTip("Just some text to demonstrate a\ncustom tooltip");

        form->addRow("Line edit:", le);
        form->addRow("Text edit:", te);
        form->addRow("Combo:", cbx);
        form->addRow("", check);
        form->addRow("Radio:", radios);
        form->addRow("Spin:", spin);
        form->addRow("Slider:", slider);
        form->addRow("Progress:", prg);
        form->addRow(btnDefault, btnCalc);

        tabs->addTab(page, "Inputs");
    }

    // ---- Tab 2: Calendar ----
    {
        auto* page = new QWidget;
        auto* layout = new QVBoxLayout(page);

        auto* btnShowCalendar = new QPushButton("Show Calendar");
        layout->addWidget(btnShowCalendar);

        QObject::connect(btnShowCalendar, &QPushButton::clicked, [win, btnShowCalendar] {
            // создаём всплывающий календарь
            auto* popup = new QCalendarWidget(win);
            popup->setGridVisible(true);
            popup->setWindowFlags(Qt::Popup);
            popup->setAttribute(Qt::WA_DeleteOnClose);

            const QPoint pos = btnShowCalendar->mapToGlobal(QPoint(0, btnShowCalendar->height()));
            popup->move(pos);

            popup->show();

            QObject::connect(popup, &QCalendarWidget::activated, popup, &QCalendarWidget::close);
        });

        tabs->addTab(page, QIcon(), "Calendar");
    }

    // ---- Tab 3+: Middle pages ----
    {
        auto* page = new QWidget;
        auto* layout = new QVBoxLayout(page);
        layout->addWidget(new QLabel("Middle page 2"));
        tabs->addTab(page, "Middle 2");
    }

    {
        auto* page = new QWidget;
        auto* layout = new QVBoxLayout(page);
        layout->addWidget(new QLabel("Middle page 3"));
        tabs->addTab(page, testIcon, "Middle 3");
        tabs->setTabsClosable(true);
    }

    // ---- Tab 4: Views (splitter) ----
    {
        auto* page = new QWidget;
        auto* h = new QHBoxLayout(page);

        auto* list = new QListWidget;
        list->addItems({ "Alpha", "Beta", "Gamma", "Delta", "Epsilon" });

        auto* table = new QTableWidget(5, 3);
        table->setHorizontalHeaderLabels({ "Col 1", "Col 2", "Col 3" });
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c < 3; ++c)
                table->setItem(
                    r, c, new QTableWidgetItem(QString("R%1C%2").arg(QString::number(r)).arg(QString::number(c))));
        table->horizontalHeader()->setStretchLastSection(true);

        auto* splitter = new QSplitter;
        splitter->addWidget(list);
        splitter->addWidget(table);
        splitter->setStretchFactor(1, 1);

        h->addWidget(splitter);
        tabs->addTab(page, "Views");
    }

    /* ===== DOCKS ===== */
    auto* leftDock = new QDockWidget("Navigation", win);
    leftDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    auto* navList = new QListWidget;
    navList->addItems({ "Home", "Projects", "Settings", "About" });
    leftDock->setWidget(navList);
    win->addDockWidget(Qt::LeftDockWidgetArea, leftDock);

    auto* rightDock = new QDockWidget("Log", win);
    auto* log = new QTextEdit;
    log->setReadOnly(true);
    log->setPlaceholderText("Application log...");
    rightDock->setWidget(log);
    win->addDockWidget(Qt::RightDockWidgetArea, rightDock);

    /* ===== CONNECTIONS ===== */
    auto showMsg = [sb](const QString& s) { sb->showMessage(s, 2000); };
    QObject::connect(aNew, &QAction::triggered, [=] { showMsg("New"); });
    QObject::connect(aOpen, &QAction::triggered, [=] { showMsg("Open"); });
    QObject::connect(aSave, &QAction::triggered, [=] { showMsg("Save"); });

    return win;
}



QMenuBar* buildMenuBar()
{
    const QIcon testIcon = QIcon(Icons::path(Icons::TestIcon));
    QMenuBar* menuBar = new QMenuBar();

    /* ===== FILE ===== */
    auto* fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction(testIcon, "New Project")->setShortcut(QKeySequence::New);
    fileMenu->addAction("New File...")->setShortcut(QKeySequence("Ctrl+Shift+N"));
    fileMenu->addSeparator();
    fileMenu->addAction(testIcon, "Open...")->setShortcut(QKeySequence::Open);
    fileMenu->addAction("Open Recent")->setDisabled(true);
    fileMenu->addSeparator();
    fileMenu->addAction("Save")->setShortcut(QKeySequence::Save);
    fileMenu->addAction("Save As...")->setShortcut(QKeySequence("Ctrl+Shift+S"));
    fileMenu->addAction("Save All")->setDisabled(true);
    fileMenu->addSeparator();

    auto* exportMenu = fileMenu->addMenu("Export");
    exportMenu->addAction("As PDF...")->setDisabled(true);
    exportMenu->addAction("As HTML...")->setDisabled(true);
    exportMenu->addAction("As Image...");
    fileMenu->addSeparator();
    fileMenu->addAction("Print...")->setShortcut(QKeySequence::Print);
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("Exit");
    exitAction->setShortcut(QKeySequence::Quit);


    /* ===== EDIT ===== */
    auto* editMenu = menuBar->addMenu("&Edit");
    editMenu->addAction("Undo")->setShortcut(QKeySequence::Undo);
    editMenu->addAction("Redo")->setShortcut(QKeySequence::Redo);
    editMenu->addSeparator();
    QAction* cutAction = editMenu->addAction("Cut");
    cutAction->setDisabled(true);
    cutAction->setShortcut(QKeySequence::Cut);
    editMenu->addAction("Copy")->setShortcut(QKeySequence::Copy);
    editMenu->addAction("Paste")->setShortcut(QKeySequence::Paste);
    editMenu->addAction("Delete")->setShortcut(QKeySequence::Delete);
    editMenu->addSeparator();
    editMenu->addAction("Find...")->setShortcut(QKeySequence::Find);
    editMenu->addAction("Replace...")->setShortcut(QKeySequence("Ctrl+H"));
    editMenu->addSeparator();

    auto* prefMenu = editMenu->addMenu("Preferences");
    prefMenu->addAction("General");
    prefMenu->addAction("Appearance");
    prefMenu->addAction("Shortcuts");

    /* ======== VIEW ======== */
    auto* viewMenu = menuBar->addMenu("&View");

    auto* themeMenu = viewMenu->addMenu("Themes");
    QActionGroup* themeGroup = new QActionGroup(menuBar);
    themeGroup->setExclusive(true);

    QAction* lightAct     = themeMenu->addAction("Light");
    lightAct->setIcon(testIcon);
    QAction* darkAct      = themeMenu->addAction("Dark");
    QAction* contrastAct  = themeMenu->addAction("High Contrast");
    contrastAct->setDisabled(true);

    lightAct->setCheckable(true);
    darkAct->setCheckable(true);
    contrastAct->setCheckable(true);

    themeGroup->addAction(lightAct);
    themeGroup->addAction(darkAct);
    themeGroup->addAction(contrastAct);

    darkAct->setChecked(true);      // by default

    viewMenu->addSeparator();

    QAction* toolbarAct = viewMenu->addAction("Show Toolbar");
    toolbarAct->setCheckable(true);
    toolbarAct->setChecked(true);

    QAction* statusbarAct = viewMenu->addAction("Show Status Bar");
    statusbarAct->setCheckable(true);
    statusbarAct->setChecked(true);

    /* ===== TOOLS ===== */
    auto* toolsMenu = menuBar->addMenu("&Tools");
    toolsMenu->addAction("Options...");
    toolsMenu->addAction("Extensions");
    toolsMenu->addSeparator();
    toolsMenu->addAction("Check for Updates");
    toolsMenu->addAction("Run Script...");

    /* ===== WINDOW ===== */
    auto* windowMenu = menuBar->addMenu("&Window");
    windowMenu->addAction("Minimize");
    windowMenu->addAction("Zoom");
    windowMenu->addSeparator();
    windowMenu->addAction("Next Window")->setShortcut(QKeySequence("Ctrl+Tab"));
    windowMenu->addAction("Previous Window")->setShortcut(QKeySequence("Ctrl+Shift+Tab"));

    /* ===== HELP ===== */
    auto* helpMenu = menuBar->addMenu("&Help");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Tutorials");
    helpMenu->addSeparator();
    helpMenu->addAction("Check for Updates");
    helpMenu->addAction(testIcon, "About...");

    return menuBar;
}
