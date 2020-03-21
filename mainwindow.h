#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qt>
#include <QTextEdit>
#include <QMainWindow>
#include "engine.h"
#include "keywordHighlight.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionMenuRun_triggered();
    void on_actionMenuNew_triggered();
    void on_actionMenuClear_triggered();

private:
    void on_textEditor_textChanged();
    QString highlightCode(QString code);
    Ui::MainWindow *ui;
    Engine engine;

    char stringsHtmlColor[8] = "#F2DF17";
    char numbersHtmlColor[8] = "#792692";
    char commentsHtmlColor[8] = "#75715E";
    char keywordLuaHtmlColor[8] = "#F92672";
    char keywordStormworksColor[8] = "#66D9EF";
};
#endif // MAINWINDOW_H
