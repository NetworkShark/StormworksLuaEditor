#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qt>
#include <QTextEdit>
#include <QMainWindow>
#include "engine.h"

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
    const QString keywordsList = "(break|do|else|elseif|end|for|function|if|in|local|repeat|return|then|until|while|or|and|not)";
    void on_textEditor_textChanged();
    QString colorWord(QString code, QString words, QString color);
    Ui::MainWindow *ui;
    Engine engine;
};
#endif // MAINWINDOW_H
