#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionMenuRun_triggered()
{
    engine = Engine();
    QTextEdit *textEditor = findChild<QTextEdit*>("textEditor", Qt::FindChildOption::FindChildrenRecursively);
    QTextEdit *textResults = findChild<QTextEdit*>("textResults", Qt::FindChildOption::FindChildrenRecursively);
    if (!textEditor) return;
    if (!textResults) return;
    if(textEditor->toPlainText().size() > 0) {
        if (engine.New()) {
            QString codeString = textEditor->toPlainText();
            QByteArray code8bit = codeString.toLocal8Bit();
            char *code = code8bit.data();
            const char *result = engine.Run(code);
            textResults->append(result);
            return;
        }
    }
    textResults->append("Error\n");
}
