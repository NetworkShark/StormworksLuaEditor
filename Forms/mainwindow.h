#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qt>
#include <QTextEdit>
#include <QMainWindow>
#include <QTimer>
#include <QSettings>

#include <map>

#include "engine.h"
#include "structs.h"
#include "optionsdialog.h"
#include "highlighter.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Settings* globalSettings = nullptr);
    ~MainWindow();
    Settings* globalSettings = nullptr;

private slots:
    void on_actionMenuExit_triggered();
    void on_actionMenuOptions_triggered();
    void on_actionMenuRun_triggered();
    void on_actionMenuNew_triggered();
    void on_actionMenuClear_triggered();

private:
    void on_textEditor_textChanged();
    QString highlightCode(QString code);
    Ui::MainWindow* ui;
    QTimer* timerhighlightCode;
    Engine* engine = nullptr;
    int oldCursorPosition = 0;
    Highlighter *highlighter;

};

#endif // MAINWINDOW_H
